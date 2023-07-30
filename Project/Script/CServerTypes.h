#pragma once

#include<Windows.h>

// unsigned __int16�� Uint16�� �����ϴ�.

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
	wstring animName;		// ������ �ִϸ��̼� �̸�
	UINT16  targetId;		// �ִϸ��̼��� ����� ������Ʈ�� id
	bool    bRepeat;		// �ݺ� ����
	bool    blend;			// ���� ��뿩��
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
	MELEE,      // �ٰŸ�
	RANGED,     // ���Ÿ�
	SEIGE,      // ����
	SUPER,      // ����
};

// ���� �����
enum CC
{
	CLEAR = 0,
	SLOW = 1 << 0,
	SILENCE = 1 << 1,
	ROOT = 1 << 2,
	STUN = 1 << 3,
	AIRBORNE = 1 << 4,
};

// =====���� �̳��� ���濹��============
// ������, �����Բ��� ��ü���̶� ���ܵ�
// ====================================
enum class FactionType
{
	BLUE = 0,
	RED = 1,
	NONE = 2, // �񼱰�����(��θ� ������. ����x)
	END = 3,
};

enum class CC_TYPE
{
	NONE,
	STUN, // ����
	SLOW, // ��ȭ
	SILENCE, // ħ��
	SNARE, // �ӹ�
	BLEED, // ����
	AIRBORNE, // ���
};

enum class LaneType {
	NONE,
	TOP,
	MID,
	BOTTOM,
	END,
};