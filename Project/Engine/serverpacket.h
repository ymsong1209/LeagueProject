#pragma once
#include<Windows.h>
#include "global.h"


// unsigned __int16�� Uint16�� �����ϴ�.

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
	wstring animName;			// ������ �ִϸ��̼� �̸�
	unsigned __int16  targetId; // �ִϸ��̼��� ����� ������Ʈ�� id
	bool    bRepeat;			// �ݺ� ����
	bool    blend;				// ���� ��뿩��
	float   blendTime;			
};

enum class SkillType
{
	BASIC_ATTACK,		// ��� ������ ��Ÿ (�̴Ͼ�/���۸��� �⺻���� ����)
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




// ������, ���� �����Բ��� ��ü
enum class FactionType
{
	BLUE = 0,
	RED = 1,
	NONE = 2, // �񼱰�����(��θ� ������. ����x)
	END = 3,
};

