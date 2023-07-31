#pragma once

#include<Windows.h>

// unsigned __int16은 Uint16과 동일하다.

enum class SERVER_EVENT_TYPE
{
	// Server Packet 
	MOVE_PACKET,			// wParam : GameObject,  lParam : ObjectMove Update Move Packet
	ANIM_PACKET,			// wParam : GameObject,  lParam : AnimInfo
	SKILL_HIT_PACKET,		// WParam : uin64 hitId, lParam : SkillInfo

	// Script Send Packet
	SEND_ANIM_PACKET,		// wParam : X,  lParam : AnimInfo
	SKILL_PROJECTILE_PACKET, // wParam : SkillInfo
	SEND_HIT_PACKET, 	    // wParam : HitInfo, 
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
	UINT64    OwnerId;    // 스킬을 사용한 플레이어 id
	UINT64    TargetId;   // 타겟팅일시 맞을 플레이어 id (논타겟일 경우 -1)
	UINT16    SkillLevel; // 스킬레벨
	SkillType skillType;  // 어떤 스킬인지 모아둔 enum 중 하나
};

struct HitInfo
{
	UINT64		 hitObjId;        // 맞은애
	UINT64		 useObjId;   // 쏜애
	int			 SkillLevel;    // 스킬레벨
	SkillType	 skillType;     // 스킬타입
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