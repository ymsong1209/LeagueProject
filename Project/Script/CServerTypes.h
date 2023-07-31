#pragma once

#include<Windows.h>

// unsigned __int16�� Uint16�� �����ϴ�.

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
	wstring animName;		// ������ �ִϸ��̼� �̸�
	UINT16  targetId;		// �ִϸ��̼��� ����� ������Ʈ�� id
	bool    bRepeat;		// �ݺ� ����
	bool    blend;			// ���� ��뿩��
	float   blendTime;
};

enum class SkillType
{
	NONE,
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

struct SkillInfo
{
	UINT64    OwnerId;    // ��ų�� ����� �÷��̾� id
	UINT64    TargetId;   // Ÿ�����Ͻ� ���� �÷��̾� id (��Ÿ���� ��� -1)
	UINT16    SkillLevel; // ��ų����
	SkillType skillType;  // � ��ų���� ��Ƶ� enum �� �ϳ�
};

struct HitInfo
{
	UINT64		 hitObjId;        // ������
	UINT64		 useObjId;   // ���
	int			 SkillLevel;    // ��ų����
	SkillType	 skillType;     // ��ųŸ��
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

// ���� �����
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
	MELEE,      // �ٰŸ�
	RANGED,     // ���Ÿ�
	SEIGE,      // ����
	SUPER,      // ����
};