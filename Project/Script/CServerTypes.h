#pragma once

#include<Windows.h>

// unsigned __int16�� Uint16�� �����ϴ�.

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
	wstring animName;		// ������ �ִϸ��̼� �̸�
	UINT16  targetId;		// �ִϸ��̼��� ����� ������Ʈ�� id
	bool    bRepeat;		// �ݺ� ����
	bool    blend;			// ���� ��뿩��
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
public:
	struct vec3Server
	{
		float x;
		float y;
		float z;
	};

	UINT64      SkillId;    // ��ų ����ü id
	UINT64      OwnerId;    // ��ų�� ����� �÷��̾� id
	UINT64      TargetId;   // Ÿ�����Ͻ� ���� �÷��̾� id (��Ÿ���� ��� -1)

	UINT16      SkillLevel; // ��ų����
	SkillType   skillType;  // � ��ų���� ��Ƶ� enum �� �ϳ�

	vec3Server   offsetPos;  // �������� offset

	// TargetId == -1�� ��� ���
	bool		UseMousePos;    // mousePos�� ���ܾ��ϴ� ��ų
	vec3Server  MousePos;

	bool		UseMouseDir;    // �� �������� ���ܾ� �ϴ� ��ų
	vec3Server  MouseDir;
};

struct HitInfo
{
	UINT64       skillObjId;     // ��ų ����ü 
	UINT64		 hitObjId;       // ������
	UINT64		 useObjId;       // ���
	int			 SkillLevel;     // ��ų����
	SkillType	 skillType;      // ��ųŸ��
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

// LoL ���� ������ �÷��̾ ��ȣ�ۿ��� �� �ִ� ��� ������Ʈ
enum class UnitType
{
	CHAMPION,

	MELEE_MINION,  
	RANGED_MINION,
	SIEGE_MINION,
	SUPER_MINION,

	RAPTORS, // ����, �ڽĵ�
	WOLF,    // ����, ����
	KRUG,    // ���ź� ū��, ���� ��
	DRAGON,
	BARON,   // ��������

	JUNGLE_RED,  // ���� ��������
	JUNGLE_BLUE, // Ǫ�� �ļ���

	TURRET,
	INHIBITOR,
	NEXUS,

	PROJECTILE,
	EFFECT,

	END,
};
