#pragma once


enum class FactionType
{
	BLUE = 0,
	RED = 1,
};

struct PlayerMove
{
	enum PlayerState
	{
		IDLE = 0,
		MOVE = 1,
	};

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

	PlayerState state;
	MoveDir moveDir;
	Pos pos;
};
