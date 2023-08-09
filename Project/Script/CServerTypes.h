#pragma once

#include<Windows.h>

// unsigned __int16은 Uint16과 동일하다.

enum class SERVER_EVENT_TYPE
{
	// Server Packet 
	MOVE_PACKET,		// wParam : GameObject,		   lParam : ObjectMove (Update Move Packet)
	ANIM_PACKET,		// wParam : GameObject,		   lParam : AnimInfo
	SKILL_HIT_PACKET,	// WParam : uin64 hitId,	   lParam : SkillInfo
	DESPAWN_PACKET,		// wParam : uint64 despawnId,  lParam : float lifespan
	KDA_CS_PACKET,		// wParam : KDACSInfo
	SOUND_PACKET,
	TIME_PACKET,

	// Script Send Packet
	SEND_ANIM_PACKET,		  // wParam : X,  lParam : AnimInfo
	SKILL_PROJECTILE_PACKET,  // wParam : SkillInfo
	SEND_HIT_PACKET, 	      // wParam : HitInfo, 
	SEND_DESPAWN_PACKET,      // wParam : objId,  lParam : lifeSpanTime
	SEND_KDA_CS_PACKET,		  // wParam : KDACSInfo
	SEND_SOUND_PACKET,
};

// Event
struct tServerEvent
{
	SERVER_EVENT_TYPE Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};


enum class Faction
{
	NONE,
	RED,
	BLUE,
	END,
};

enum class Lane
{
	NONE,
	TOP,
	MID,
	BOTTOM,
	END,
};


enum class ChampionType
{
	NONE,
	BLITZCRANK,
	JINX,
	AMUMU,
	MALPHITE,
	END,
};

extern const wchar_t* ChampionType_WSTR[(UINT)ChampionType::END];

// 군중 제어기
enum CC
{
	CLEAR = 0,
	SLOW = 1 << 0,
	SILENCE = 1 << 1,
	ROOT = 1 << 2,
	STUN = 1 << 3,
	AIRBORNE = 1 << 4,
};

// 행동 제약
enum RESTRAINT
{
	CAN_MOVE = 1 << 0,
	CAN_ATTACK = 1 << 1,
	CAN_USE_SKILL = 1 << 2,

	DEFAULT = CAN_MOVE | CAN_ATTACK | CAN_USE_SKILL,
	BLOCK = 0,
};

enum class MinionType
{
	NONE,
	MELEE,      // 근거리
	RANGED,     // 원거리
	SEIGE,      // 대포
	SUPER,      // 슈퍼
};

// LoL 게임 내에서 플레이어가 상호작용할 수 있는 모든 오브젝트
enum class UnitType
{
	CHAMPION,

	MELEE_MINION,
	RANGED_MINION,
	SIEGE_MINION,
	SUPER_MINION,

	SOUTH_GROMP,
	SOUTH_MURKWOLF,
	SOUTH_MURKWOLF_MINI_L,
	SOUTH_MURKWOLF_MINI_R,
	SOUTH_RAZORBEAK,
	SOUTH_RAZORBEAK_MINI_1,
	SOUTH_RAZORBEAK_MINI_2,
	SOUTH_RAZORBEAK_MINI_3,
	SOUTH_RAZORBEAK_MINI_4,
	SOUTH_RAZORBEAK_MINI_5,
	SOUTH_KRUG,
	SOUTH_KRUG_MINI,
	SOUTH_RED,
	SOUTH_BLUE,

	NORTH_GROMP,
	NORTH_MURKWOLF,
	NORTH_MURKWOLF_MINI_L,
	NORTH_MURKWOLF_MINI_R,
	NORTH_RAZORBEAK,
	NORTH_RAZORBEAK_MINI_1,
	NORTH_RAZORBEAK_MINI_2,
	NORTH_RAZORBEAK_MINI_3,
	NORTH_RAZORBEAK_MINI_4,
	NORTH_RAZORBEAK_MINI_5,
	NORTH_KRUG,
	NORTH_KRUG_MINI,
	NORTH_RED,
	NORTH_BLUE,

	DRAGON,
	BARON,   // 렌더링만

	TURRET,
	INHIBITOR,
	NEXUS,

	PROJECTILE,
	EFFECT,

	END,
};

struct AnimInfo
{
	wstring animName;		// 변경할 애니메이션 이름
	UINT16  targetId;		// 애니메이션이 변경될 오브젝트의 id
	bool    bRepeat;		// 애니메이션 반복 여부
	bool    bRepeatBlend;   // 블렌드 반복 여부 (PlayRepeat에서만 사용하는 함수)
	bool    blend;			// 블렌드 사용여부
	float   blendTime;
};

enum class DimensionType
{
	TwoD,
	ThreeD,
};

enum class SkillType
{
	NONE,
	BASIC_ATTACK,		// 모든 종류의 평타 (미니언/정글몹의 기본공격 포함)
	JINX_Q,
	JINX_W,
	JINX_E,
	JINX_R,
	DARIUS_Q,
	DARIUS_W,
	DARIUS_E,
	DARIUS_R,
	MALPHITE_Q,
	MALPHITE_W,
	MALPHITE_E,
	MALPHITE_R,
};

struct SkillInfo
{
public:
	struct vec3Server
	{
		float x;
		float y;
		float z;
	};

	UINT64      SkillId;		// 스킬 투사체 id
	UINT64      OwnerId;		// 스킬을 사용한 플레이어 id
	UINT64      TargetId;		// 타겟팅일시 맞을 플레이어 id (논타겟일 경우 -1)

	UINT16      SkillLevel;		// 스킬레벨
	SkillType   skillType;		// 어떤 스킬인지 모아둔 enum 중 하나

	vec3Server   offsetPos;		// 중점기준 offset
	int        projectileCount = 1; // 투사체가 몇개인지

	// TargetId == -1일 경우 사용
	bool		UseMousePos;    // mousePos에 생겨야하는 스킬
	vec3Server  MousePos;

	bool		UseMouseDir;    // 이 방향으로 생겨야 하는 스킬
	vec3Server  MouseDir;
};

struct HitInfo
{
	UINT64       skillObjId;     // 스킬 투사체 
	UINT64		 hitObjId;       // 맞은애
	UINT64		 useObjId;       // 쏜애
	int			 SkillLevel;     // 스킬레벨
	SkillType	 skillType;      // 스킬타입
};


struct SoundInfo
{
	struct vec3Server
	{
		float x;
		float y;
		float z;
	};

	wstring			soundName;
	DimensionType   dimensionType;
	Faction         faction;
	int             iRoopCount;
	float           fVolume;
	bool            bOverlap;
	float           fRange;
	vec3Server      soundPos;
};

struct KDACSInfo
{
	UINT64      killerId;
	UINT64      victimId;
	UnitType   deadObjUnitType;
};

// 소환사 주문(임시)
enum class SUMMONERS_SPELL
{
	FLASH,      // 점멸
	HEAL,       // 회복
	GHOST,      // 유체화
	IGNITE,     // 점화
	SMITE,      // 강타
	EXHAUST,    // 탈진
	CLEANSE,    // 정화
	END,
};

extern const wchar_t* SUMMONERS_SPELL_WSTR[(UINT)SUMMONERS_SPELL::END];

