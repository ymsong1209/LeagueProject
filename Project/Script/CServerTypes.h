#pragma once

#include<Windows.h>

// unsigned __int16�� Uint16�� �����ϴ�.

enum class SERVER_EVENT_TYPE
{
	// Server Packet 
	MOVE_PACKET,		// wParam : GameObject,		   lParam : ObjectMove (Update Move Packet)
	ANIM_PACKET,		// wParam : GameObject,		   lParam : AnimInfo
	SKILL_HIT_PACKET,	// WParam : uin64 hitId,	   lParam : SkillInfo
	DESPAWN_PACKET,		// wParam : uint64 despawnId,  lParam : float lifespan
	KDA_CS_PACKET,		// wParam : KDACSInfo
	SOUND_PACKET,
	TIME_PACKET,

	// Script Send Packet
	SEND_ANIM_PACKET,		  // wParam : X,  lParam : AnimInfo
	SKILL_PROJECTILE_PACKET,  // wParam : SkillInfo
	SEND_HIT_PACKET, 	      // wParam : HitInfo, 
	SEND_DESPAWN_PACKET,      // wParam : objId,  lParam : lifeSpanTime
	SEND_KDA_CS_PACKET,		  // wParam : KDACSInfo
	SEND_SOUND_PACKET,
};

// Event
struct tServerEvent
{
	SERVER_EVENT_TYPE Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
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


enum class ChampionType
{
	NONE,
	BLITZCRANK,
	JINX,
	AMUMU,
	MALPHITE,
	END,
};

extern const wchar_t* ChampionType_WSTR[(UINT)ChampionType::END];

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

// �ൿ ����
enum RESTRAINT
{
	CAN_MOVE = 1 << 0,
	CAN_ATTACK = 1 << 1,
	CAN_USE_SKILL = 1 << 2,

	DEFAULT = CAN_MOVE | CAN_ATTACK | CAN_USE_SKILL,
	BLOCK = 0,
};

enum class MinionType
{
	NONE,
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

	SOUTH_GROMP,
	SOUTH_MURKWOLF,
	SOUTH_MURKWOLF_MINI_L,
	SOUTH_MURKWOLF_MINI_R,
	SOUTH_RAZORBEAK,
	SOUTH_RAZORBEAK_MINI_1,
	SOUTH_RAZORBEAK_MINI_2,
	SOUTH_RAZORBEAK_MINI_3,
	SOUTH_RAZORBEAK_MINI_4,
	SOUTH_RAZORBEAK_MINI_5,
	SOUTH_KRUG,
	SOUTH_KRUG_MINI,
	SOUTH_RED,
	SOUTH_BLUE,

	NORTH_GROMP,
	NORTH_MURKWOLF,
	NORTH_MURKWOLF_MINI_L,
	NORTH_MURKWOLF_MINI_R,
	NORTH_RAZORBEAK,
	NORTH_RAZORBEAK_MINI_1,
	NORTH_RAZORBEAK_MINI_2,
	NORTH_RAZORBEAK_MINI_3,
	NORTH_RAZORBEAK_MINI_4,
	NORTH_RAZORBEAK_MINI_5,
	NORTH_KRUG,
	NORTH_KRUG_MINI,
	NORTH_RED,
	NORTH_BLUE,

	DRAGON,
	BARON,   // ��������

	TURRET,
	INHIBITOR,
	NEXUS,

	PROJECTILE,
	EFFECT,

	END,
};

struct AnimInfo
{
	wstring animName;		// ������ �ִϸ��̼� �̸�
	UINT16  targetId;		// �ִϸ��̼��� ����� ������Ʈ�� id
	bool    bRepeat;		// �ִϸ��̼� �ݺ� ����
	bool    bRepeatBlend;   // ���� �ݺ� ���� (PlayRepeat������ ����ϴ� �Լ�)
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
	MALPHITE_Q,
	MALPHITE_W,
	MALPHITE_E,
	MALPHITE_R,
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

	UINT64      SkillId;		// ��ų ����ü id
	UINT64      OwnerId;		// ��ų�� ����� �÷��̾� id
	UINT64      TargetId;		// Ÿ�����Ͻ� ���� �÷��̾� id (��Ÿ���� ��� -1)

	UINT16      SkillLevel;		// ��ų����
	SkillType   skillType;		// � ��ų���� ��Ƶ� enum �� �ϳ�

	vec3Server   offsetPos;		// �������� offset
	int        projectileCount = 1; // ����ü�� �����

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

struct KDACSInfo
{
	UINT64      killerId;
	UINT64      victimId;
	UnitType   deadObjUnitType;
};

// ��ȯ�� �ֹ�(�ӽ�)
enum class SUMMONERS_SPELL
{
	FLASH,      // ����
	HEAL,       // ȸ��
	GHOST,      // ��üȭ
	IGNITE,     // ��ȭ
	SMITE,      // ��Ÿ
	EXHAUST,    // Ż��
	CLEANSE,    // ��ȭ
	END,
};

extern const wchar_t* SUMMONERS_SPELL_WSTR[(UINT)SUMMONERS_SPELL::END];

