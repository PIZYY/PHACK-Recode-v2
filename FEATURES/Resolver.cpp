#include "../MISC/includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CClientEntityList.h"
#include "../UTILS/render.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../SDK/AnimLayer.h"
#include "../UTILS/qangle.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Resolver.h"
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
float lerp_time()
{
	int ud_rate = INTERFACES::cvar->FindVar("cl_updaterate")->GetFloat();
	SDK::ConVar *min_ud_rate = INTERFACES::cvar->FindVar("sv_minupdaterate");
	SDK::ConVar *max_ud_rate = INTERFACES::cvar->FindVar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();
	float ratio = INTERFACES::cvar->FindVar("cl_interp_ratio")->GetFloat();
	if (ratio == 0)
		ratio = 1.0f;
	float lerp = INTERFACES::cvar->FindVar("cl_interp")->GetFloat();
	SDK::ConVar *c_min_ratio = INTERFACES::cvar->FindVar("sv_client_min_interp_ratio");
	SDK::ConVar *c_max_ratio = INTERFACES::cvar->FindVar("sv_client_max_interp_ratio");
	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());
	return max(lerp, (ratio / ud_rate));
}
void CResolver::HandleBackUpResolve(SDK::CBaseEntity* pEnt) {

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (pEnt->GetTeam() == local_player->GetTeam())
		return;

	const auto player_animation_state = pEnt->GetAnimState();

	if (!player_animation_state)
		return;

	float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

	auto v48 = 0.f;

	if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
	{
		v48 = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 1.0f);
	}
	else
	{
		v48 = 0.0f;
	}

	float v49 = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * v48;

	float flYawModifier = v49 + 1.0;

	if (player_animation_state->m_fDuckAmount > 0.0)
	{
		float v53 = 0.0f;

		if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
		{
			v53 = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
		}
		else
		{
			v53 = 0.0f;
		}
	}

	float flMaxYawModifier = player_animation_state->pad10[516] * flYawModifier;
	float flMinYawModifier = player_animation_state->pad10[512] * flYawModifier;

	float newFeetYaw = 0.f;

	auto eyeYaw = player_animation_state->m_flEyeYaw;

	auto lbyYaw = player_animation_state->m_flGoalFeetYaw;

	float eye_feet_delta = fabs(eyeYaw - lbyYaw);

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
	{
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
	}

	float v136 = fmod(newFeetYaw, 360.0);

	if (v136 > 180.0)
	{
		v136 = v136 - 360.0;
	}

	if (v136 < 180.0)
	{
		v136 = v136 + 360.0;
	}

	player_animation_state->m_flGoalFeetYaw = v136;
}
void CResolver::AnimationFix(SDK::CBaseEntity* pEnt)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (pEnt == local_player) {
		pEnt->ClientAnimations(true);
		auto player_animation_state = pEnt->GetAnimState();
		player_animation_state->m_flLeanAmount = 20;
		player_animation_state->m_flCurrentTorsoYaw += 15;
		pEnt->UpdateClientSideAnimation();
		pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));
		pEnt->ClientAnimations(false);
	}
	else {
		auto player_index = pEnt->GetIndex() - 1;

		pEnt->ClientAnimations(true);

		auto old_curtime = INTERFACES::Globals->curtime;
		auto old_frametime = INTERFACES::Globals->frametime;

		INTERFACES::Globals->curtime = pEnt->GetSimTime();
		INTERFACES::Globals->frametime = INTERFACES::Globals->interval_per_tick;

		auto player_animation_state = pEnt->GetAnimState();
		auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
		if (player_animation_state != nullptr && player_model_time != nullptr)
			if (*player_model_time == INTERFACES::Globals->framecount)
				* player_model_time = INTERFACES::Globals->framecount - 1;


		pEnt->UpdateClientSideAnimation();

		INTERFACES::Globals->curtime = old_curtime;
		INTERFACES::Globals->frametime = old_frametime;

		//pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));

		pEnt->ClientAnimations(false);
	}

}
void CResolver::max_eye_ang_delta()
{
	resolver_info_t2
		&current = m_current,
		&previous = m_previous;

	float delta = previous.EyeAng - current.EyeAng;
	if (current.Desync >= abs(delta) && current.lastMaxDSC < abs(delta) && abs(delta) > 4.0)
		current.lastMaxDSC = delta;
	if (current.Desync < current.lastMaxDSC)
		current.lastMaxDSC = current.Desync;
}
bool IsMovingOnInAir(SDK::CBaseEntity* entity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return !(entity->GetFlags() & FL_ONGROUND);
}
bool is_slow_walking(SDK::CBaseEntity * entity) {
	/*float velocity_2D[64], old_velocity_2D[64];

	if (entity->GetVelocity().Length2D() != velocity_2D[entity->GetIndex()] && entity->GetVelocity().Length2D() != NULL) {
		old_velocity_2D[entity->GetIndex()] = velocity_2D[entity->GetIndex()];
		velocity_2D[entity->GetIndex()] = entity->GetVelocity().Length2D();
	}

	if (velocity_2D[entity->GetIndex()] > 0.1) {
		int tick_counter[64];

		if (velocity_2D[entity->GetIndex()] == old_velocity_2D[entity->GetIndex()])
			++tick_counter[entity->GetIndex()];
		else
			tick_counter[entity->GetIndex()] = 0;

		while (tick_counter[entity->GetIndex()] > (1 / INTERFACES::Globals->interval_per_tick) * fabsf(0.1f))// should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
			return true;

	}
	return false;*/
	if (IsMovingOnInAir(entity))
		return false;
	float large = 0;
	float velocity_2D[64], old_velocity_2D[64];
	if (entity->GetVecVelocity().Length2D() != velocity_2D[entity->GetIndex()] && entity->GetVecVelocity().Length2D() != NULL) {
		old_velocity_2D[entity->GetIndex()] = velocity_2D[entity->GetIndex()];
		velocity_2D[entity->GetIndex()] = entity->GetVecVelocity().Length2D();
	}
	if (large == 0)return false;
	Vector velocity = entity->GetVecVelocity();
	Vector direction = entity->GetEyeAngles();

	float speed = velocity.Length();
	direction.y = entity->GetEyeAngles().y - direction.y;
	//method 1
	if (velocity_2D[entity->GetIndex()] > 1) {
		int tick_counter[64];
		if (velocity_2D[entity->GetIndex()] == old_velocity_2D[entity->GetIndex()])
			tick_counter[entity->GetIndex()] += 1;
		else
			tick_counter[entity->GetIndex()] = 0;

		while (tick_counter[entity->GetIndex()] > (1 / INTERFACES::Globals->interval_per_tick) * fabsf(0.1f))//should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
			return true;
	}
	//method 2
	//removed
	//method 3  / resource heavy but does most of the work
	//removed
	//method 4
	if (velocity.Length2D() == entity->m_flMaxspeed() / 3)//ima keep that in because its basically public
		return true;
	//method 5
	if (speed < entity->m_flMaxspeed() / 3 && velocity.Length2D() != 0)//same here
		return true;

	return false;
}


float network_latency() {
	SDK::INetChannelInfo *nci = INTERFACES::Engine->GetNetChannelInfo();
	if (nci)
		return nci->GetAvgLatency(FLOW_INCOMING);

	return 0.0f;
}

int GetChokedTicksNetworked(SDK::CBaseEntity * entity) {
	float flSimulationTime = entity->GetSimTime();
	float flSimDiff = INTERFACES::Globals->curtime - flSimulationTime;
	float latency = network_latency();
	return (int)(0.5f + (float)(max(0.0f, flSimDiff - latency)) / INTERFACES::Globals->interval_per_tick);
}


void CResolver::c_data(SDK::CBaseEntity* entity) {
	bool IsMoving = entity->GetVelocity().Length2D() > 0.1;
	bool SlowMo = entity->GetVelocity().Length2D() < 40;
	bool IsDucking = entity->GetFlags() & FL_DUCKING;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto lowerbody = entity->GetLowerBodyYaw();

	auto animationstate = entity->GetAnimState();
	auto feet_yaw = animationstate->m_flCurrentFeetYaw;
	auto body_max_rotation = animationstate->pad10[516];

	for (int i = 1; i < INTERFACES::Globals->maxclients; i++)
	{
		auto p_entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (p_entity && !p_entity->GetIsDormant())
		{

			if (feet_yaw <= 58)
			{
				if (-58 > feet_yaw)
					lowerbody = body_max_rotation + p_entity->GetLowerBodyYaw();
				//resolve_type[entity->GetIndex()] = 3; //desync
			}
			else
			{
				lowerbody = body_max_rotation - p_entity->GetLowerBodyYaw();
			}
			if (p_entity->GetAnimOverlays()->m_flPlaybackRate > 0.1)
			{
				for (int resolve_delta = 58.f; resolve_delta < -58.f; resolve_delta = resolve_delta - 20.f)
				{
					lowerbody = resolve_delta;
				//	GLOBAL::resolver_mode = "desync (brute)";
				}
			}
		}
	}
}
float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}

void CResolver::fix_animsss(ClientFrameStage_t stage) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || !INTERFACES::Engine->IsInGame())
		return;
	static bool  wasDormant[65];
	for (int i = 1; i < INTERFACES::Engine->GetMaxClients(); ++i)
	{
		SDK::CBaseEntity* pPlayerEntity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive())
			continue;
		if (pPlayerEntity->GetIsDormant())
		{
			wasDormant[i] = true;
			continue;
		}
		if (stage == FRAME_RENDER_START)
		{
		//	HandleHits(pPlayerEntity);
			AnimationFix(pPlayerEntity);

		}
		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			HandleBackUpResolve(pPlayerEntity);
		}
		if (stage == FRAME_NET_UPDATE_END && pPlayerEntity != local_player)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				* reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
		}

		wasDormant[i] = false;
	}
}
void CResolver::resolve3(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	int i = entity->GetIndex();
	GLOBAL::resolver_mode = "none";
	static float MoveReal[65], FakeWalkandslowwalkingfix[65];
	auto animation_state = entity->GetAnimState();
	auto animstate = uintptr_t(local_player->GetAnimState());
	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));
	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));
	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;
	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}
	unk3 = *(float*)(animstate + 0x334) * unk2;
	/*float duckammount = animation_state->m_fDuckAmount;
	float speedfraction = max(0, min(animation_state->m_flFeetSpeedForwardsOrSideWays, 2));
	float speedfactor = max(0, min(1, animation_state->m_flFeetSpeedUnknownForwardOrSideways));
	float unk1 = (animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999 * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0)
	{
	unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}
	unk3 = *(float *)(animation_state + 0x334) * unk2;*/

	auto feet_yaw = animation_state->m_flCurrentFeetYaw;
	float body_yaw = 58.f;//animation_state->m_flCurrentTorsoYaw;
	auto move_yaw = 29.f;
	auto goal_feet_yaw = animation_state->m_flGoalFeetYaw;
	auto shit = body_yaw - feet_yaw;
	auto shitv2 = body_yaw + feet_yaw;

	auto poses = entity->GetPoseParamaters();
	float feet_yaw_rate = animation_state->m_flFeetYawRate;
	float fff = animation_state->m_flFeetSpeedForwardsOrSideWays;
	float forwardorsideways = animation_state->m_flFeetSpeedUnknownForwardOrSideways;
	float feet_cucle = animation_state->m_flFeetCycle;
	float headheighanimation = animation_state->m_flHeadHeightOrOffsetFromHittingGroundAnimation;
	float new_body_yaw = animation_state->m_flCurrentTorsoYaw;
	auto body_max_rotation = animation_state->pad10[516];
	auto normalized_eye_abs_yaw_diff = fmod((animation_state->m_flEyeYaw - feet_yaw), 360.0);
	auto body_min_rotation = animation_state->pad10[512];
	if (shots_missed[entity->GetIndex()] > 2) {
		switch (shots_missed[entity->GetIndex()] % 4) {
	case 0: entity->GetEyeAnglesPointer()->y = entity->GetEyeAnglesPointer()->y + 45; break;
	case 1: entity->GetEyeAnglesPointer()->y = entity->GetEyeAnglesPointer()->y - 45; break;
	case 2: entity->GetEyeAnglesPointer()->y = entity->GetEyeAnglesPointer()->y - 30; break;
	case 3: entity->GetEyeAnglesPointer()->y = entity->GetEyeAnglesPointer()->y + 30; break;
		}
	}
	if (entity->GetVelocity().Length2D() < 32.f) // проверка на мувы(upd:нужно переписать)(UDP:уже переписал)
	{
		if (move_yaw)
		{
			animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + move_yaw + feet_yaw * 29.f && feet_yaw + feet_yaw_rate / 58.f;
		}
		else
		{
			if (feet_yaw && move_yaw)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + feet_yaw + feet_yaw_rate * -29.f && goal_feet_yaw + feet_yaw / 29.f;
			}
		}
	}
	else
	{
		if (entity->GetVelocity().Length2D() > 0 && entity->GetFlags() & FL_ONGROUND) // проверка на стенды
		{
			if (normalized_eye_abs_yaw_diff > 0 || normalized_eye_abs_yaw_diff == 0)
			{
				body_min_rotation / feet_yaw / 58.f;
			}
			else
			{
				body_max_rotation / feet_yaw / -58.f;
			}
			if (new_body_yaw == 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - body_yaw * -58.f + goal_feet_yaw + feet_yaw_rate + feet_yaw / 58.f;
			}
			else if (new_body_yaw >= -46.f && new_body_yaw == body_yaw)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - new_body_yaw / 46.f || 58.f && goal_feet_yaw - feet_yaw * 58.f;
			}
			else if (new_body_yaw <= 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - body_yaw * 58.f + feet_yaw / -58.f && goal_feet_yaw * 58.f;
			}
			else if (new_body_yaw == 58.f && new_body_yaw <= 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - goal_feet_yaw / 58.f + feet_yaw * -58.f && new_body_yaw * 58.f - body_yaw / -58.f;
			}
			else if (new_body_yaw >= -58.f && body_yaw == 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - new_body_yaw * 58.f - feet_yaw * -58.f && goal_feet_yaw - 58.f && feet_yaw / -58.f;
			}
		}
		if (is_slow_walking(entity))// дальше проверка на слоуволк(такая же ебливая как и я) p.s L I S T O N
		{
			GLOBAL::resolver_mode = "slowwalk";
			if (normalized_eye_abs_yaw_diff > 0 || normalized_eye_abs_yaw_diff == 0)
			{
				body_min_rotation / move_yaw / -29.f;
			}
			else
			{
				body_max_rotation / move_yaw / 29.f;
			}
			if (goal_feet_yaw <= -29.f && feet_yaw >= -29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - move_yaw / 29.f + feet_yaw - goal_feet_yaw * 29.f;
			}
			else if (feet_yaw >= 29.f && feet_yaw_rate <= 29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + move_yaw + 29.f - feet_yaw + feet_yaw_rate / 29.f;
			}
			else if (goal_feet_yaw >= -29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - move_yaw / 29.f + feet_yaw_rate - feet_cucle + 29.f && goal_feet_yaw * 29.f;
			}
			resolver->enemy_fakewalk = true;
		}
		else
			resolver->enemy_fakewalk = false;
	}
}
bool CResolver::AntiFreestanding(SDK::CBaseEntity* entity, float& yaw) {
	player_resolve_record player_resolve_records[64];

	const auto freestanding_record = player_resolve_records[entity->GetIndex()].anti_freestanding_record;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return false;

	if (freestanding_record.left_damage >= 20 && freestanding_record.right_damage >= 20)
		return false;

	const float at_target_yaw = UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y;
	if (freestanding_record.left_damage <= 0 && freestanding_record.right_damage <= 0) {
		//resolve_type[entity->GetIndex()] = 4;
		if (freestanding_record.right_fraction < freestanding_record.left_fraction)
			yaw = at_target_yaw + 125.f;
		else
			yaw = at_target_yaw - 73.f;
	}
	else {
		if (freestanding_record.left_damage > freestanding_record.right_damage)
			yaw = at_target_yaw + 130.f;
		else
			yaw = at_target_yaw - 49.f;
	}

	return true;
}
void CResolver::testing_shit(SDK::CBaseEntity* entity) {

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	int i = entity->GetIndex();
	GLOBAL::resolver_mode = "none";
	static float MoveReal[65], FakeWalkandslowwalkingfix[65];
	auto animation_state = entity->GetAnimState();
	auto animstate = uintptr_t(local_player->GetAnimState());
	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));
	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));
	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;
	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}
	unk3 = *(float*)(animstate + 0x334) * unk2;
	/*float duckammount = animation_state->m_fDuckAmount;
	float speedfraction = max(0, min(animation_state->m_flFeetSpeedForwardsOrSideWays, 2));
	float speedfactor = max(0, min(1, animation_state->m_flFeetSpeedUnknownForwardOrSideways));
	float unk1 = (animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999 * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0)
	{
	unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}
	unk3 = *(float *)(animation_state + 0x334) * unk2;*/

	auto feet_yaw = animation_state->m_flCurrentFeetYaw;
	float body_yaw = 58.f;//animation_state->m_flCurrentTorsoYaw;
	auto move_yaw = 29.f;
	auto goal_feet_yaw = animation_state->m_flGoalFeetYaw;
	auto shit = body_yaw - feet_yaw;
	auto shitv2 = body_yaw + feet_yaw;

	auto poses = entity->GetPoseParamaters();
	float feet_yaw_rate = animation_state->m_flFeetYawRate;
	float fff = animation_state->m_flFeetSpeedForwardsOrSideWays;
	float forwardorsideways = animation_state->m_flFeetSpeedUnknownForwardOrSideways;
	float feet_cucle = animation_state->m_flFeetCycle;
	float headheighanimation = animation_state->m_flHeadHeightOrOffsetFromHittingGroundAnimation;
	float new_body_yaw = animation_state->m_flCurrentTorsoYaw;
	auto body_max_rotation = animation_state->pad10[516];
	auto normalized_eye_abs_yaw_diff = fmod((animation_state->m_flEyeYaw - feet_yaw), 360.0);
	auto body_min_rotation = animation_state->pad10[512];
	if (entity->GetVelocity().Length2D() < 32.f) // проверка на мувы(upd:нужно переписать)(UDP:уже переписал)
	{
		if (move_yaw)
		{
			animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + move_yaw + feet_yaw * 29.f && feet_yaw + feet_yaw_rate / 58.f;
		}
		else
		{
			if (feet_yaw && move_yaw)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + feet_yaw + feet_yaw_rate * -29.f && goal_feet_yaw + feet_yaw / 29.f;
			}
		}
	}
	else
	{
		if (entity->GetVelocity().Length2D() > 0 && entity->GetFlags() & FL_ONGROUND) // проверка на стенды
		{
			if (normalized_eye_abs_yaw_diff > 0 || normalized_eye_abs_yaw_diff == 0)
			{
				body_min_rotation / feet_yaw / 58.f;
			}
			else
			{
				body_max_rotation / feet_yaw / -58.f;
			}
			if (new_body_yaw == 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - body_yaw * -58.f + goal_feet_yaw + feet_yaw_rate + feet_yaw / 58.f;
			}
			else if (new_body_yaw >= -46.f && new_body_yaw == body_yaw)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - new_body_yaw / 46.f || 58.f && goal_feet_yaw - feet_yaw * 58.f;
			}
			else if (new_body_yaw <= 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - body_yaw * 58.f + feet_yaw / -58.f && goal_feet_yaw * 58.f;
			}
			else if (new_body_yaw == 58.f && new_body_yaw <= 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - goal_feet_yaw / 58.f + feet_yaw * -58.f && new_body_yaw * 58.f - body_yaw / -58.f;
			}
			else if (new_body_yaw >= -58.f && body_yaw == 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - new_body_yaw * 58.f - feet_yaw * -58.f && goal_feet_yaw - 58.f && feet_yaw / -58.f;
			}
		}
		if (is_slow_walking(entity))// дальше проверка на слоуволк(такая же ебливая как и я) p.s L I S T O N
		{
			GLOBAL::resolver_mode = "slowwalk";
			if (normalized_eye_abs_yaw_diff > 0 || normalized_eye_abs_yaw_diff == 0)
			{
				body_min_rotation / move_yaw / -29.f;
			}
			else
			{
				body_max_rotation / move_yaw / 29.f;
			}
			if (goal_feet_yaw <= -29.f && feet_yaw >= -29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - move_yaw / 29.f + feet_yaw - goal_feet_yaw * 29.f;
			}
			else if (feet_yaw >= 29.f && feet_yaw_rate <= 29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + move_yaw + 29.f - feet_yaw + feet_yaw_rate / 29.f;
			}
			else if (goal_feet_yaw >= -29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - move_yaw / 29.f + feet_yaw_rate - feet_cucle + 29.f && goal_feet_yaw * 29.f;
			}
			resolver->enemy_fakewalk = true;
		}
		else
			resolver->enemy_fakewalk = false;
	}
	bool IsMoving = entity->GetVelocity().Length2D() > 0.1;
	bool SlowMo = entity->GetVelocity().Length2D() < 40;
	bool IsDucking = entity->GetFlags() & FL_DUCKING;

	auto lowerbody = entity->GetLowerBodyYaw();

	auto animationstate = entity->GetAnimState();

	for (int i = 1; i < INTERFACES::Globals->maxclients; i++)
	{
		auto p_entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (p_entity && !p_entity->GetIsDormant())
		{

			if (feet_yaw <= 58)
			{
				if (-58 > feet_yaw)
					lowerbody = body_max_rotation + p_entity->GetLowerBodyYaw();
				//resolve_type[entity->GetIndex()] = 3; //desync
			}
			else
			{
				lowerbody = body_max_rotation - p_entity->GetLowerBodyYaw();
			}
			if (p_entity->GetAnimOverlays()->m_flPlaybackRate > 0.1)
			{
				for (int resolve_delta = 58.f; resolve_delta < -58.f; resolve_delta = resolve_delta - 20.f)
				{
					lowerbody = resolve_delta;
					//	GLOBAL::resolver_mode = "desync (brute)";
				}
			}
		}
	}
}
void CResolver::c_resolver_code(SDK::CBaseEntity * entity) {
	/// a copy of the previous record for comparisons and shit
	const auto previous_record = player_resolve_records[entity->GetIndex()];
	auto& record = player_resolve_records[entity->GetIndex()];
	auto& resolve_record = player_resolve_records[entity->GetIndex()];


	record.resolved_angles = record.networked_angles;
	record.velocity = entity->GetVelocity();
	record.origin = entity->GetVecOrigin();
	record.lower_body_yaw = entity->GetLowerBodyYaw();
	record.is_dormant = entity->GetIsDormant();
	record.resolve_type = 0;

	record.is_balance_adjust_triggered = false, record.is_balance_adjust_playing = false;
	if (AntiFreestanding(entity, resolve_record.resolved_angles.y)) {
		resolve_record.resolve_type |= RESOLVE_TYPE_ANTI_FREESTANDING;
		//GLOBAL::resolver_mode = "desync (ANTIFRESTND)";
	}
	max_eye_ang_delta();

}
void CResolver::g_desync_correction(SDK::CBaseEntity* pPlayer)
{
	/*
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!pPlayer) return;
	if (!local_player) return;

	bool is_local_player = pPlayer == local_player;
	bool is_teammate = local_player->GetTeam() == pPlayer->GetTeam() && !is_local_player;
	if (is_local_player) return;
	if (is_teammate) return;
	if (pPlayer->GetHealth() <= 0) return;
	if (local_player->GetHealth() <= 0) return;

	float old_yaw[64] = { NULL };
	float current_yaw[64] = { NULL };
	float resolved_yaw[64] = { 0.f };

	float y_delta[64] = { 0.f };
	c_data(pPlayer);
	//c_resolver_code(pPlayer);*/
}

CResolver* resolver = new CResolver();



