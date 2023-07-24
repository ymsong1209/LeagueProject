#pragma once

enum class GAME_EVENT_TYPE
{
	PLAYER_DEATH,
	PLAYER_RESPAWN,
	PLAYER_STOP,
	PLAYER_MOVE,
	PLAYER_KILL_CHAMPION,
	PLAYER_KILL_MOB,
	PLAYER_ATTACK,
	PLAYER_RECALL,
	PLAYER_ABILITY_MODIFY,
	PLAYER_SKILL_Q,
	PLAYER_SKILL_W,
	PLAYER_SKILL_E,
	PLAYER_SKILL_R,
	END,
};

class CGameEvent
{
public:
	virtual ~CGameEvent() = default;

public:
	GAME_EVENT_TYPE	m_eEventType;

	virtual GAME_EVENT_TYPE GetType() { return m_eEventType; };
	virtual void Clear() = 0;
};

class StopEvent :
	public CGameEvent
{
public:
	StopEvent()
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_STOP;
	}
public:
	virtual void Clear() {};
};

class DeathEvent :
	public CGameEvent
{
public:
	DeathEvent()
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_DEATH;
	}
public:
	virtual void Clear() {};
};

class RespawnEvent :
	public CGameEvent
{
public:
	RespawnEvent()
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_RESPAWN;
	}
public:
	virtual void Clear() {};
};


class MoveEvent :
	public CGameEvent
{
public:
	MoveEvent()
		:m_iPlayerID(-1)
		, m_vTargetPos(Vec3(0, 0, 0))
		, m_fFaceRot(0)
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_MOVE;
	}

private:
	int             m_iPlayerID;
	Vec3            m_vTargetPos;
	float			m_fFaceRot;

public:
	void	SetPlayerID(int _i) { m_iPlayerID = _i; }
	void	SetTargetPos(Vec3 _v) { m_vTargetPos = _v; }
	void	SetFaceRot(float _f) { m_fFaceRot = _f; }

	int		GetPlayerID() { return m_iPlayerID; }
	Vec3	GetTargetPos() { return m_vTargetPos; }
	float	GetFaceRot() { return m_fFaceRot; }

	void Clear() override { m_iPlayerID = -1; m_vTargetPos = {}; m_fFaceRot = 0; }
};

enum Ability
{
	MaxHP = 1 < 0,
	MaxMana = 1 < 1,
	Attack = 1 < 2,
	Deffence = 1 < 3,
	AttackSpeed = 1 < 4,
	AttackRange = 1 < 5,
	MoveSpeed = 1 < 6,
};

class AbilityModifyEvent :
	public CGameEvent
{
public:
	AbilityModifyEvent()
		:m_Ability((Ability)0)
		, m_fTime(0.f)
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_ABILITY_MODIFY;
	}

private:
	int     m_iPlayerID;	// 대상
	Ability	m_Ability;		// 증감시킬 능력치
	float	m_fTime;		// 증감시킬 시간

public:
	void	SetPlayerID(int _i) { m_iPlayerID = _i; }
	void	SetAbility(Ability _a) { m_Ability = _a; }
	void	SetTime(float _f) { m_fTime = _f; }

	int		GetPlayerID() { return m_iPlayerID; }
	Ability GetAbility() { return m_Ability; }
	float	GetTime() { return m_fTime; }

	void Clear() override { m_iPlayerID = -1; m_Ability = (Ability)0; m_fTime = 0.f; }
};


