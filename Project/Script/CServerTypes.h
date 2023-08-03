#pragma once

#include<Windows.h>

// unsigned __int16은 Uint16과 동일하다.

enum class SERVER_EVENT_TYPE
{
	// Server Packet 
	MOVE_PACKET,			// wParam : GameObject,  lParam : ObjectMove Update Move Packet
	ANIM_PACKET,			// wParam : GameObject,  lParam : AnimInfo
	SKILL_HIT_PACKET,		// WParam : uin64 hitId, lParam : SkillInfo
	DESPAWN_PACKET,
	DEAD_PACKET,
	SOUND_PACKET,

	// Script Send Packet
	SEND_ANIM_PACKET,		  // wParam : X,  lParam : AnimInfo
	SKILL_PROJECTILE_PACKET,  // wParam : SkillInfo
	SEND_HIT_PACKET, 	      // wParam : HitInfo, 
	SEND_DESPAWN_PACKET,      // wParam : objId,  lParam : lifeSpanTime
	SEND_DEAD_PACKET,		  //
	SEND_SOUND_PACKET,
};

// Event
struct tServerEvent
{
	SERVER_EVENT_TYPE Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};

struct AnimInfo
{
	wstring animName;		// 변경할 애니메이션 이름
	UINT16  targetId;		// 애니메이션이 변경될 오브젝트의 id
	bool    bRepeat;		// 반복 여부
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

	UINT64      SkillId;    // 스킬 투사체 id
	UINT64      OwnerId;    // 스킬을 사용한 플레이어 id
	UINT64      TargetId;   // 타겟팅일시 맞을 플레이어 id (논타겟일 경우 -1)

	UINT16      SkillLevel; // 스킬레벨
	SkillType   skillType;  // 어떤 스킬인지 모아둔 enum 중 하나

	vec3Server   offsetPos;  // 중점기준 offset

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

// 군중 제어기
enum class CC
{
	CLEAR = 0,
	SLOW = 1 << 0,
	SILENCE = 1 << 1,
	ROOT = 1 << 2,
	STUN = 1 << 3,
	AIRBORNE = 1 << 4,
};

enum class MinionType
{
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

	RAPTORS, // 엄마, 자식들
	WOLF,    // 대장, 부하
	KRUG,    // 돌거북 큰애, 작은 애
	DRAGON,
	BARON,   // 렌더링만

	JUNGLE_RED,  // 붉은 덩굴정령
	JUNGLE_BLUE, // 푸른 파수꾼

	TURRET,
	INHIBITOR,
	NEXUS,

	PROJECTILE,
	EFFECT,

	END,
};
