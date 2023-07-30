#pragma once

#include<Windows.h>

// unsigned __int16은 Uint16과 동일하다.

enum class SERVER_EVENT_TYPE
{
	// Server Packet 
	MOVE_PACKET,			// wParam : GameObject,  lParam : ObjectMove Update Move Packet
	ANIM_PACKET,			// wParam : GameObject,  lParam : AnimInfo
	SKILL_PROJECTILE_PACKET,
	SKILL_HIT_PACKET,
	

	// Script Send Packet
	SEND_ANIM_PACKET,		// wParam : X,  lParam : AnimInfo
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

struct SkillInfo {
	UINT64    OwnerId;
	UINT64    TargetId;
	UINT16    SkillLevel;
	SkillType skillType;
};

enum class Faction
{
	NONE,
	RED,
	BLUE,
};

enum class Lane
{
	NONE,
	TOP,
	MID,
	BOTTOM,
};

enum class MinionType
{
	MELEE,      // 근거리
	RANGED,     // 원거리
	SEIGE,      // 대포
	SUPER,      // 슈퍼
};

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

// =====오늘 이내로 변경예정============
// 다혜꺼, 하은님껄로 교체중이라 남겨둠
// ====================================
enum class FactionType
{
	BLUE = 0,
	RED = 1,
	NONE = 2, // 비선공몬스터(모두를 적대함. 선빵x)
	END = 3,
};

enum class CC_TYPE
{
	NONE,
	STUN, // 기절
	SLOW, // 둔화
	SILENCE, // 침묵
	SNARE, // 속박
	BLEED, // 출혈
	AIRBORNE, // 에어본
};

enum class LaneType {
	NONE,
	TOP,
	MID,
	BOTTOM,
	END,
};