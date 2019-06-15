#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	class CBaseWeapon;
}
class AnimationLayer2
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void* m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038
struct CBasePlayerAnimState2
{
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	SDK::CBaseEntity* pBaseEntity; //0x60
	SDK::CBaseAnimState* pActiveWeapon; //0x64
	SDK::CBaseAnimState* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float m_velocity; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
};
struct resolver_info_t2 {
	float EyeAng;
	SDK::CBaseEntity* Entity;
	float Desync;
	float lastMaxDSC;
	float LBYT;
	float RealAng;
	float FakeAng;
};
struct Info 
{
	Info() {}

	SDK::CAnimationLayer backup_layer, prev_layer;
	Vector last_lby, inverse, inverse_right, inverse_left, lby, back, left, right, backtrack, wideright, wideleft, forwards;
	float stored_simtime, last_move_time, pre_anim_lby;
	int last_ammo;
	bool breaking_lby, reset_state, could_be_faking;
	std::vector<float> unresolved_yaw, lby_deltas;

	bool lby_changed;
	bool could_be_slowmo;
	bool is_moving;
	bool is_jumping;
	bool is_crouching;
	bool lby_updated;
	bool using_fake_angles;
	float last_moving_lby;
	float stored_lby;
	float next_lby_update_time;
	int stored_missed;
};
enum class ResolverModes : int
{
	NONE,
	FREESTANDING,
	BACKWARDS,
	EDGE,
	MOVE_STAND_DELTA,
	FORCE_LAST_MOVING_LBY,
	FORCE_FREESTANDING,
	FORCE_BACKWARDS,
	BRUTFORCE_ALL_DISABLED, //todo
	BRUTFORCE,
	FORCE_MOVE_STAND_DELTA,
	FORCE_LBY,
	MOVING,
	LBY_BREAK,
	SPINBOT,
	AIR_FREESTANDING,
	AIR_BRUTFORCE,
	FAKEWALK_FREESTANDING,
	FAKEWALK_BRUTFORCE,
	PREDICT_FREESTANDING,
	PREDICT_BRUTFORCE,
	MAX
};

enum class ResolverDetections : int
{
	NONE,
	FAKEWALKING,
	AIR,
	MOVING,
	STANDING
};

enum class BacktrackPriorities : int
{
	none,
	good_prediction,
	normal_prediction,
	lby_prediction,
	lby_update,
	moving
};

struct GlobalResolverDataStruct
{
	bool Resolved = false;
	bool ForceBaim = false;
	int Shots = 0;
	int ResolverState = 0; // bad, resolved, lby update, moving
	ResolverModes mode = ResolverModes::NONE;
	ResolverDetections detection = ResolverDetections::NONE;
	//Shots counter
	//int ShotsAtMoveStandDelta = 0;
	int FakeWalkShots = 0;
	bool InFakelag = false;
	bool Fake = true;
	bool Moving = false;
	bool InAir = false;
	int ShotsAtMode[(int)ResolverModes::MAX];
	int OverallShotsAtMode[(int)ResolverModes::MAX];
	int OverallShots = 0;
	//float ResolvedYaw = 0.f;
	BacktrackPriorities BacktrackPriority = BacktrackPriorities::none;
	bool BreakingLC = false; //g_Saver.LCbroken = (Globals::LocalPlayer->m_vecOrigin() - g_Saver.FakelagData.pos).LengthSqr() > 4096.f;
	bool CanuseLbyPrediction = false;
	float NextPredictedLbyBreak = 0.f;

};
struct CurrentStandAADataStruct
{
	bool UsedEdge = false;
	int LastEdgeShots = 0;
	int FreestandShots = 0;
};
struct SavedResolverData
{
	bool WasDormantBetweenMoving = true;
	float LastMovingLby = 0.f;
	float LastCycle = 0.f;
	bool Flip = false;
	float LastSimtime = 0.f;
	float LastResolvedYaw = 0.f;
	bool LastResolved = true;
	bool WasLastMoving = false;
	float LastStandingLby = 0.f;
	float MoveStandDelta = 0.f;
	bool CanUseMoveStandDelta = false;
	bool WasFakeWalking = false;
	//CanUseLbyPrediction
	float LastLby = 0.f;
	float LegitTestLastSimtime = 0.f;
	float lastLbyUpdateTime = 0.f;
	bool UsingAA = false;
	float LastWasUsingAA = 0.f;
	Vector LastPos = Vector(0, 0, 0);
	float FakeYaw = 0.f;
	float LastVel = 0.f;
};
class CResolver
{
	
public:
	resolver_info_t2 m_current;
	resolver_info_t2 m_previous;
	bool enemy_fake;
	bool enemy_fakewalk;
	float resolved_yaw;
	bool UseFreestandAngle[65];
	float FreestandAngle[65];
	Info player_info[65];
	void max_eye_ang_delta();
	void g_desync_correction(SDK::CBaseEntity* entity);
	void c_data(SDK::CBaseEntity* entity);
	void fix_animsss(ClientFrameStage_t stage);
	void c_resolver_code(SDK::CBaseEntity * entity);
	void HandleBackUpResolve(SDK::CBaseEntity* pEnt);
	void AnimationFix(SDK::CBaseEntity* pEnt);
	void OnCreateMove();
	void FrameStage(ClientFrameStage_t stage);
	void resolve3(SDK::CBaseEntity* entity);
	bool AntiFreestanding(SDK::CBaseEntity* entity, float& yaw);
	void testing_shit(SDK::CBaseEntity* entity);
	static const unsigned int RESOLVE_TYPE_NUM = 8;
	static const unsigned short RESOLVE_TYPE_OVERRIDE = 0b00000001,
		RESOLVE_TYPE_NO_FAKE = 0b00000010,
		RESOLVE_TYPE_LBY = 0b00000100,
		RESOLVE_TYPE_LBY_UPDATE = 0b00001000,
		RESOLVE_TYPE_PREDICTED_LBY_UPDATE = 0b00010000,
		RESOLVE_TYPE_LAST_MOVING_LBY = 0b00100000,
		RESOLVE_TYPE_NOT_BREAKING_LBY = 0b01000000,
		RESOLVE_TYPE_BRUTEFORCE = 0b10000000,
		RESOLVE_TYPE_LAST_MOVING_LBY_DELTA = 0b100000000,
		RESOLVE_TYPE_ANTI_FREESTANDING = 0b1000000000;

	class player_resolve_record
	{
	public:
		player_resolve_record()
		{
			resolve_type = 0;
			shots_missed_moving_lby = 0;
			shots_missed_moving_lby_delta = 0;

			last_balance_adjust_trigger_time = 0.f;
			last_moving_lby_delta = 0.f;
			last_time_moving = 0.f;
			last_time_down_pitch = 0.f;
			next_predicted_lby_update = 0.f;

			has_fake = false;
			is_dormant = false, is_last_moving_lby_delta_valid = false;
			is_last_moving_lby_valid = false, is_fakewalking = false;
			is_balance_adjust_triggered = false, is_balance_adjust_playing = false;
			did_lby_flick = false, did_predicted_lby_flick = false;

			for (int i = 0; i < RESOLVE_TYPE_NUM; i++)
			{
				shots_hit[i] = 0;
				shots_fired[i] = 0;
			}
		}

	public:
		struct AntiFreestandingRecord
		{
			int right_damage = 0, left_damage = 0;
			float right_fraction = 0.f, left_fraction = 0.f;
		};

	public:
		SDK::CAnimationLayer anim_layers[15];
		AntiFreestandingRecord anti_freestanding_record;
		static const unsigned int RESOLVE_TYPE_NUM = 8;

		Vector resolved_angles, networked_angles;
		Vector velocity, origin;

		int shots_hit[RESOLVE_TYPE_NUM], shots_fired[RESOLVE_TYPE_NUM];
		int shots_missed_moving_lby, shots_missed_moving_lby_delta;
		unsigned short resolve_type;

		float lower_body_yaw;
		float last_moving_lby;
		float last_moving_lby_delta;
		float last_balance_adjust_trigger_time;
		float last_time_moving;
		float last_time_down_pitch;
		float next_predicted_lby_update;

		bool is_dormant;
		bool is_last_moving_lby_valid;
		bool is_fakewalking;
		bool is_last_moving_lby_delta_valid;
		bool is_balance_adjust_triggered, is_balance_adjust_playing;
		bool did_lby_flick, did_predicted_lby_flick;
		bool has_fake;
	};

private:
	player_resolve_record player_resolve_records[64];

};

extern CResolver* resolver;
