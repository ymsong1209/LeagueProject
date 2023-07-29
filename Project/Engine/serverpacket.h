#pragma once
#include<Windows.h>
#include "global.h"

enum class SERVER_EVENT_TYPE
{
	// Server Packet //
	MOVE_PACKET,    // wParam : GameObject,  lParam : ObjectMove Update Move Packet
	ANIM_PACKET,    // wParam : GameObject,  lParam : AnimInfo
	SKILL_PROJECTILE_PACKET,
	SKILL_HIT_PACKET,

	SEND_ANIM_PACKET,	// wParam : id,  lParam : AnimInfo
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
	wstring animName;
	unsigned __int16  animIdx;
	bool    blend;
	float   blendTime;
};

enum class FactionType
{
	BLUE = 0,
	RED = 1,
	NONE = 2, // 비선공몬스터(모두를 적대함. 선빵x)
	END = 3,
};

