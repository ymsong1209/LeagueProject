#pragma once
#include<Windows.h>
#include "global.h"


// unsigned __int16은 Uint16과 동일하다.

enum class SERVER_EVENT_TYPE
{
	// Server Packet //
	MOVE_PACKET,			// wParam : GameObject,  lParam : ObjectMove Update Move Packet
	ANIM_PACKET,			// wParam : GameObject,  lParam : AnimInfo
	SKILL_PROJECTILE_PACKET,
	SKILL_HIT_PACKET,

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
	wstring animName;			// 변경할 애니메이션 이름
	unsigned __int16  targetId; // 애니메이션이 변경될 오브젝트의 id
	bool    bRepeat;			// 반복 여부
	bool    blend;				// 블렌드 사용여부
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
	unsigned __int64 OwnerId;
	unsigned __int64 TargetId;
	unsigned __int16 SkillLevel;
	SkillType skillType;
};




// 다혜꺼, 추후 하은님껄로 교체
enum class FactionType
{
	BLUE = 0,
	RED = 1,
	NONE = 2, // 비선공몬스터(모두를 적대함. 선빵x)
	END = 3,
};

