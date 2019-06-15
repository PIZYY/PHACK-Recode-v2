#pragma once

#include "../UTILS/offsets.h"
#include "../UTILS/interfaces.h"
#include "../SDK/ModelInfo.h"
#include "../UTILS/qangle.h"
//#include "../SDK/CBaseWeapon.h"
#include "../SDK/CClientEntityList.h"
#include "../UTILS/NetvarHookManager.h"

#include "AnimLayer.h"
#include "RecvData.h"
enum DataUpdateType_txd
{
	DATA_UPDATE_CREATEDxd = 0,
	DATA_UPDATE_DATATABLE_CHANGEDxd,
};
namespace SDK
{
	class CBaseAnimState;
	class Collideable;
	typedef unsigned long CBaseHandle;
	struct studiohdr_t;
	struct model_t;

	/*enum ItemDefinitionIndex : int
	{
		WEAPON_DEAGLE = 1,
		WEAPON_ELITE = 2,
		WEAPON_FIVESEVEN = 3,
		WEAPON_GLOCK = 4,
		WEAPON_AK47 = 7,
		WEAPON_AUG = 8,
		WEAPON_AWP = 9,
		WEAPON_FAMAS = 10,
		WEAPON_G3SG1 = 11,
		WEAPON_GALIL = 13,
		WEAPON_M249 = 14,
		WEAPON_M4A4 = 16,
		WEAPON_MAC10 = 17,
		WEAPON_P90 = 19,
		WEAPON_UMP45 = 24,
		WEAPON_XM1014 = 25,
		WEAPON_BIZON = 26,
		WEAPON_MAG7 = 27,
		WEAPON_NEGEV = 28,
		WEAPON_SAWEDOFF = 29,
		WEAPON_TEC9 = 30,
		WEAPON_ZEUS = 31,
		WEAPON_P2000 = 32,
		WEAPON_MP7 = 33,
		WEAPON_MP9 = 34,
		WEAPON_NOVA = 35,
		WEAPON_P250 = 36,
		WEAPON_SCAR20 = 38,
		WEAPON_SG553,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_SG556 = 39,
		WEAPON_SSG08 = 40,
		WEAPON_KNIFE = 42,
		WEAPON_FLASHBANG = 43,
		WEAPON_HEGRENADE = 44,
		WEAPON_SMOKEGRENADE = 45,
		WEAPON_MOLOTOV = 46,
		WEAPON_DECOY = 47,
		WEAPON_INC = 48,
		WEAPON_C4 = 49,
		WEAPON_KNIFE_T = 59,
		WEAPON_M4A1S = 60,
		WEAPON_USPS = 61,
		WEAPON_CZ75 = 63,
		WEAPON_REVOLVER = 64,
		CMolotovGrenade = 110,
		WEAPON_CC4 = 127,
		CSmokeGrenade = 152,
		WEAPON_CPlantedC4 = 126,
		WEAPON_KNIFE_BAYONET = 500,
		WEAPON_KNIFE_FLIP = 505,
		WEAPON_KNIFE_GUT = 506,
		WEAPON_KNIFE_KARAMBIT = 507,
		WEAPON_KNIFE_M9_BAYONET = 508,
		WEAPON_KNIFE_TACTICAL = 509,
		WEAPON_KNIFE_FALCHION = 512,
		WEAPON_KNIFE_BOWIE = 514,
		WEAPON_KNIFE_BUTTERFLY = 515,
		WEAPON_KNIFE_PUSH = 516,
		WEAPON_KNIFE_URSUS = 519,
		WEAPON_KNIFE_STILETTO = 522,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_WIDOWMAKER

	};*/

enum ItemDefinitionIndex
{
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALIL = 13,
	WEAPON_M249,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_ZEUS,
	WEAPON_P2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG553,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INC,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1S,
	WEAPON_USPS,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035,
	WEAPON_SG556 = 5039,
	WEAPON_KNIFE_BOWIE,
		/**/
		CMolotovGrenade = 110,
		WEAPON_CPlantedC4 = 126,
		WEAPON_CC4 = 127,
		CSmokeGrenade = 152

	};

	enum MoveType_t
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_ISOMETRIC,
		MOVETYPE_WALK,
		MOVETYPE_STEP,
		MOVETYPE_FLY,
		MOVETYPE_FLYGRAVITY,
		MOVETYPE_VPHYSICS,
		MOVETYPE_PUSH,
		MOVETYPE_NOCLIP,
		MOVETYPE_LADDER,
		MOVETYPE_OBSERVER,
		MOVETYPE_CUSTOM,
		MOVETYPE_LAST = MOVETYPE_CUSTOM,
		MOVETYPE_MAX_BITS = 4
	};

	class CBaseAnimating
	{
	public:
		std::array<float, 24>* m_flPoseParameter()
		{
			static int offset = 0;
			if (!offset)
				offset = OFFSETS::m_flPoseParameter;
			return (std::array<float, 24>*)((uintptr_t)this + offset);
		}
		model_t* GetModel()
		{
			void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			typedef model_t* (__thiscall* fnGetModel)(void*);

			return VMT::VMTHookManager::GetFunction<fnGetModel>(pRenderable, 8)(pRenderable);
		}
		void SetBoneMatrix(matrix3x4_t* boneMatrix)
		{
			//Offset found in C_BaseAnimating::GetBoneTransform, string search ankle_L and a function below is the right one
			const auto model = this->GetModel();
			if (!model)
				return;

			matrix3x4_t* matrix = *(matrix3x4_t**)((DWORD)this + 9880);
			studiohdr_t *hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return;
			int size = hdr->numbones;
			if (matrix) {
				for (int i = 0; i < size; i++)
					memcpy(matrix + i, boneMatrix + i, sizeof(matrix3x4_t));
			}
		}
		void GetDirectBoneMatrix(matrix3x4_t* boneMatrix)
		{
			const auto model = this->GetModel();
			if (!model)
				return;

			matrix3x4_t* matrix = *(matrix3x4_t**)((DWORD)this + 9880);
			studiohdr_t *hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return;
			int size = hdr->numbones;
			if (matrix) {
				for (int i = 0; i < size; i++)
					memcpy(boneMatrix + i, matrix + i, sizeof(matrix3x4_t));
			}
		}
	};

	class CollisionProperty
	{
	public:
		Vector VecMins()
		{
			return *reinterpret_cast<Vector*>((DWORD)this + 0x8);
		}
		Vector VecMaxs()
		{
			return *reinterpret_cast<Vector*>((DWORD)this + 0x14);
		}
	};
	class CBaseEntity
	{
	public:
		int GetHealth()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iHealth);
		}
		int GetFlags()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_fFlags);
		}
		void SetFlags(int flags)
		{
			*reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_fFlags) = flags;
		}
		int GetTeam()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iTeamNum);
		}
		Vector GetVecVelocity()
		{
			return *(Vector*)((DWORD)this + 0x114);
		}
		
		int GetObserverMode()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iObserverMode);
		}
		int SetObserverMode(int mode)
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iObserverMode) = mode;
		}
		bool GetIsScoped()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bIsScoped);
		}
		Vector GetVelocity()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecVelocity);
		}
		void SetVelocity(Vector velocity)
		{
			*reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecVelocity) = velocity;
		}
		int GetMoney()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iAccount);
		}
		HANDLE GetObserverTargetHandle()
		{
			return *(HANDLE*)((uintptr_t)this + OFFSETS::m_hObserverTarget);
		}
		int GetLifeState()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_lifeState);
		}

		int m_iObserverMode()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_lifeState);
		}

		float GetLowerBodyYaw()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flLowerBodyYawTarget);
		}
		float m_flDuckAmount()
		{
			return *reinterpret_cast<float*>(uintptr_t(this + OFFSETS::m_flDuckAmount));
		}
		float m_flDuckSpeed()
		{
			return *reinterpret_cast<float*>(uintptr_t(this + OFFSETS::m_flDuckSpeed));
		}


		Vector GetVecOrigin()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecOrigin);
		}
		bool IsAlive() 
		{ 
			return this->GetLifeState() == 0; 
		}
		Vector* GetViewPunchAngle()
		{
			return (Vector*)((uintptr_t)this + OFFSETS::m_viewPunchAngle);
		}
		Vector* GetAimPunchAngle()
		{
			return (Vector*)((uintptr_t)this + OFFSETS::m_aimPunchAngle);
		}
		Vector* GetEyeAnglesPointer()
		{
			return reinterpret_cast<Vector*>((DWORD)this + OFFSETS::m_angEyeAngles);
		}

		float getmaxdesync() {

			auto animstate = uintptr_t(this->GetAnimState());

			float rate = 180;
			float duckammount = *(float *)(animstate + 0xA4);
			float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

			float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

			float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
			float unk2 = unk1 + 1.1f;
			float unk3;

			if (duckammount > 0) {

				unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

			}

			unk3 = *(float *)(animstate + 0x334) * unk2;

			return unk3;
		}

		CollisionProperty* GetCollision()
		{
			return reinterpret_cast<CollisionProperty*>((DWORD)this + 0x0318);
		}

		std::array<float, 24>& get_ragdoll_pos()
		{
			return *reinterpret_cast<std::array<float, 24>*>(reinterpret_cast<uintptr_t>(this) + 0x00002970);
		}
		Vector& GetAbsOrigin()
		{
			typedef Vector& (__thiscall* OriginalFn)(void*);
			return ((OriginalFn)VMT::VMTHookManager::GetFunction<OriginalFn>(this, 10))(this);
		}
		int GetTickBase()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_nTickBase);
		}
		bool GetIsDormant()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bDormant);
		}
		void SetLowerBodyYaw(float value)
		{
			*reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flLowerBodyYawTarget) = value;
		}
		float LowerBodyYaw()
		{
			return *reinterpret_cast<float*>((DWORD)this + OFFSETS::m_flLowerBodyYawTarget);
		}
		float GetCycle()
		{
			return *reinterpret_cast<int*>((DWORD)this + OFFSETS::m_flCycle);
		}
		int GetSequence()
		{
			return *reinterpret_cast<int*>((DWORD)this + OFFSETS::m_nSequence);
		}
		int* GetFlags1()
		{
			return (int*)((DWORD)this + OFFSETS::m_fFlags);
		}
		SDK::CAnimationLayer* GetAnimOverlays()
		{
			// to find offset: use 9/12/17 dll
			// sig: 55 8B EC 51 53 8B 5D 08 33 C0
			return *(SDK::CAnimationLayer**)((DWORD)this + 0x2980);
		}
		bool& GetClientSideAnimation2()
		{
			return *reinterpret_cast<bool*>((uintptr_t)this + OFFSETS::m_bClientSideAnimation);
		}
		void CBaseEntity::ClientAnimations(bool value)
		{
			*reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bClientSideAnimation) = value;
		}
		float GetAnimationTime()
		{
			return *reinterpret_cast<float*>((DWORD)this + /*offys.m_flAnimTime*/0x3C);
		}
		CBaseAnimState* GetAnimState()
		{
			return *reinterpret_cast<CBaseAnimState**>(uintptr_t(this) + OFFSETS::animstate);
		}
		CAnimationLayer* GetAnimOverlaysModifiable() 
		{ 
			return (*reinterpret_cast< CAnimationLayer** >(reinterpret_cast< std::uintptr_t >(this) + OFFSETS::animlayer));
		}
		int GetNumAnimOverlays()
		{
			return *(int*)((DWORD)this + 0x298C);
		}
		Collideable* GetCollideable()
		{
			return (Collideable*)((DWORD)this + OFFSETS::m_Collision);
		}
		void GetRenderBounds(Vector& mins, Vector& maxs)
		{
			void* pRenderable = (void*)(this + 0x4);
			typedef void(__thiscall* Fn)(void*, Vector&, Vector&);
			VMT::VMTHookManager::GetFunction<Fn>(pRenderable, 17)(pRenderable, mins, maxs);
		}
		int GetIndex()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + 0x64);
		}
		int GetMoveType()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_MoveType);
		}
		SDK::CAnimationLayer* AnimOverlays()
		{
			return *reinterpret_cast<SDK::CAnimationLayer**>(uintptr_t(this) + 0x2980);
		}
		CAnimationLayer& GetAnimOverlay(int Index)
		{
			return (*(CAnimationLayer**)((DWORD)this + OFFSETS::animlayer))[Index];
		}
		void SetAnimOverlay(int Index, CAnimationLayer layer)
		{
			(*(CAnimationLayer**)((DWORD)this + OFFSETS::animlayer))[Index] = layer;
		}
		Vector GetOrigin()
		{
			return *(Vector*)((uintptr_t)this + OFFSETS::m_vecOrigin);
		}
		int CBaseEntity::GetSequenceActivity(int sequence)
		{
			const auto model = GetModel();
			if (!model)
				return -1;

			const auto hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return -1;

			static auto offset = (DWORD)UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83");
			static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, SDK::studiohdr_t*, int)>(offset);

			return GetSequenceActivity(this, hdr, sequence);
		}
		Vector GetEyeAngles()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_angEyeAngles);
		}
		Vector GetVecMins()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x320);
		}
		Vector GetVecMaxs()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x32C);
		}
		QAngle* EasyEyeAngles()
		{
			return (QAngle*)((uintptr_t)this + OFFSETS::m_angEyeAngles);
		}
		void SetModelIndex(int index)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(this, 75)(this, index);
		}
		IClientNetworkable* CBaseEntity::GetNetworkable()
		{
			return reinterpret_cast<IClientNetworkable*>(reinterpret_cast<uintptr_t>(this) + 0x8);
		}
		void PreDataUpdate(DataUpdateType_txd updateType)
		{
			PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
			typedef void(__thiscall* OriginalFn)(PVOID, int);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
		}
		CBaseHandle * m_hMyWeapons()
		{
			return (CBaseHandle*)((uintptr_t)this + OFFSETS::m_hMyWeapons);
		}
		void SetEyeAngles(Vector angles)
		{
			*reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_angEyeAngles) = angles;
		}
		float GetSimTime()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flSimulationTime);
		}
		float GetOldSimTime() {
			static int m_flSimulationTime = 0x264;
			return *(float*)((DWORD)this + (m_flSimulationTime + 0x4));
		}
		Vector GetViewOffset()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecViewOffset);
		}
		model_t* GetModel()
		{
			void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			typedef model_t* (__thiscall* fnGetModel)(void*);

			return VMT::VMTHookManager::GetFunction<fnGetModel>(pRenderable, 8)(pRenderable);
		}
		matrix3x4_t GetBoneMatrix(int BoneID)
		{
			matrix3x4_t matrix;

			auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + OFFSETS::m_dwBoneMatrix);
			if (offset)
				matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

			return matrix;
		}
		
		
		Vector GetBonePosition(int i)
		{
			VMatrix matrix[128];
			if (this->SetupBones(matrix, 128, BONE_USED_BY_HITBOX, GetTickCount64()))
			{
				return Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]);
			}
			return Vector(0, 0, 0);
		}
		Vector GetEyePosition(void)
		{
			return GetVecOrigin() + *(Vector*)((DWORD)this + OFFSETS::m_vecViewOffset);
		}
		Vector GetPunchAngles()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_aimPunchAngle);
		}
		int GetTeamNum()
		{
			return *reinterpret_cast<int*>((DWORD)this + 0xF4);
		}
		bool GetImmunity()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bGunGameImmunity);
		}
		bool SetupBones(VMatrix *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
		{
			/*void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			if (!pRenderable)
				false;

			typedef bool(__thiscall* Fn)(void*, matrix3x4_t*, int, int, float);
			return VMT::VMTHookManager::GetFunction<Fn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);*/
			__asm
			{
				mov edi, this
				lea ecx, dword ptr ds : [edi + 0x4]
				mov edx, dword ptr ds : [ecx]
				push currentTime
				push boneMask
				push nMaxBones
				push pBoneToWorldOut
				call dword ptr ds : [edx + 0x34]
			}
		}
		void UpdateClientSideAnimation() // UpdateClientAnimation 
		{
			typedef void(__thiscall* original)(void*);
			VMT::VMTHookManager::GetFunction<original>(this, 221)(this);
		}
		
		void SetAbsOrigin(Vector ArgOrigin)
		{
			using Fn = void(__thiscall*)(CBaseEntity*, const Vector &origin);
			static Fn func;

			if (!func)
				func = (Fn)(UTILS::FindPattern("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??"));

			func(this, ArgOrigin);
		}
		void SetOriginz(Vector wantedpos)
		{
			typedef void(__thiscall* SetOriginFn)(void*, const Vector &);
			static SetOriginFn SetOriginze = (SetOriginFn)(UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
			SetOriginze(this, wantedpos);
		}
		Vector& GetAbsAngles()
		{
			typedef Vector& (__thiscall* OriginalFn)(void*);
			return ((OriginalFn)VMT::VMTHookManager::GetFunction<OriginalFn>(this, 11))(this);
		}
		void SetAbsAngles(Vector angles)
		{
			using Fn = void(__thiscall*)(CBaseEntity*, const Vector &angles);
			static Fn func;

			if (!func)
				func = (Fn)(UTILS::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

			func(this, angles);
		}
		bool IsMoving()
		{
			return GetVelocity().Length2D() > 0.1f;
		}
		void SetOrigin(Vector origin)
		{
			using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
			static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");

			SetAbsOrigin(this, origin);
		}
		float* GetPoseParamaters()
		{
			return reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flPoseParameter);
		}
		int DrawModel(int flags, uint8_t alpha)
		{
			void* pRenderable = (void*)(this + 0x4);

			using fn = int(__thiscall*)(void*, int, uint8_t);
			return VMT::VMTHookManager::GetFunction< fn >(pRenderable, 9)(pRenderable, flags, alpha);
		}
		ClientClass* GetClientClass()
		{
			void* Networkable = (void*)(this + 0x8);
			typedef ClientClass*(__thiscall* OriginalFn)(void*);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(Networkable, 2)(Networkable);
		}
		float GetNextAttack()
		{
			return *reinterpret_cast<float*>(uint32_t(this) + OFFSETS::m_flNextAttack);
		}
		float& m_flMaxspeed() {
			/*static unsigned int _m_flMaxspeed = util::find_in_datamap(GetPredDescMap(), "m_flMaxspeed");
			return *(float*)(uintptr_t(this) + _m_flMaxspeed);*/
			return *reinterpret_cast<float*>(uint32_t(this) + OFFSETS::m_flNextAttack);
		}
		int GetActiveWeaponIndex()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_hActiveWeapon) & 0xFFF;
		}
		/*SDK::CBaseWeapon* GetActiveWeaponIndex2()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_hActiveWeapon) & 0xFFF;
		}*/
		int GetArmor()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_ArmorValue);
		}
		bool HasHelmet()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bHasHelmet);
		}
		char* GetArmorName()
		{
			if (GetArmor() > 0)
			{
				if (HasHelmet())
					return "hk";
				else
					return "k";
			}
			else
				return " ";
		}
		Vector* GetEyeAnglesPtr()
		{
			return reinterpret_cast<Vector*>((DWORD)this + OFFSETS::m_angEyeAngles);
		}
		bool HasC4()
		{
			int iBombIndex = *(int*)(*(DWORD*)(OFFSETS::dwPlayerResource) + OFFSETS::m_iPlayerC4);
			if (iBombIndex == this->GetIndex())
				return true;
			else
				return false;
		}
		void SetAngle2(Vector wantedang) {
			typedef void(__thiscall* oSetAngle)(void*, const Vector &);
			static oSetAngle _SetAngle = (oSetAngle)((uintptr_t)UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
			_SetAngle(this, wantedang);
		}
		bool IsVisibleVector(Vector bone);
		bool IsVisible(int bone);
};

class CBaseViewModel : public CModelInfo
{
public:

	inline DWORD GetOwner() {

		return *(PDWORD)((DWORD)this + OFFSETS::m_hOwner);
	}

	inline int GetModelIndex() {

		return *(int*)((DWORD)this + OFFSETS::m_nModelIndex);
	}
};
}
