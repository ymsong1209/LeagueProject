#pragma once

enum class SERVER_EVENT_TYPE
{
	// Server Packet //
	MOVE_PACKET,    // wParam : GameObject,  lParam : Update Move Packet
};

// Event
struct tServerEvent
{
	SERVER_EVENT_TYPE Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};


enum class FactionType
{
	BLUE = 0,
	RED = 1,
	NONE = 2, // 비선공몬스터(모두를 적대함. 선빵x)
	END = 3,
};

struct ObjectMove
{
	struct MoveDir
	{
		float x;
		float y;
		float z;
	};

	struct Pos
	{
		float x;
		float y;
		float z;
	};

	MoveDir moveDir;
	Pos pos;
};

struct ObjectInfo
{

};