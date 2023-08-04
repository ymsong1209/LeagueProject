#pragma once

enum class GAME_EVENT_TYPE
{
	PLAYER_DEATH,
	PLAYER_RESPAWN,
	PLAYER_STOP,
	PLAYER_MOVE,
	PLAYER_KILL_CHAMPION,
	PLAYER_KILL_MOB,
	PLAYER_BASIC_ATTACK,
	PLAYER_RECALL,
	PLAYER_ABILITY_MODIFY,
	GET_HIT,
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
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_MOVE;
	}

public:

	void Clear() override {}
};

class BasicAttackEvent :
	public CGameEvent
{
public:
	BasicAttackEvent()
		: m_UserObj(nullptr)
		, m_TargetObj(nullptr)
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_BASIC_ATTACK;
	}

private:
	CGameObject*             m_UserObj;
	CGameObject*             m_TargetObj;


public:
	void	SetUserObj(CGameObject* _obj) { m_UserObj = _obj; }
	void	SetTargetObj(CGameObject* _obj) { m_TargetObj = _obj; }

	CGameObject*		GetUserObj() { return m_UserObj; }
	CGameObject*		GetTargetObj() { return m_TargetObj; }

	void Clear() override { m_UserObj = nullptr; m_TargetObj = nullptr; }
};


class CGameObject;
enum class SkillType;

class GetHitEvent :
	public CGameEvent
{
public:
	GetHitEvent()
		: m_iUserObj(nullptr)
		, m_eSkillType{ SkillType::NONE }
	{
		m_eEventType = GAME_EVENT_TYPE::GET_HIT;
	}

private:
	CGameObject*	m_iUserObj;
	CGameObject*	m_iTargetObj;
	SkillType		m_eSkillType;
	int				m_iSkillLevel;


public:
	
	void	SetUserObj(CGameObject* _Obj)	{ m_iUserObj = _Obj; }
	void	SetTargetObj(CGameObject* _Obj) { m_iTargetObj = _Obj; }
	void	SetSkillType(SkillType _type)	{ m_eSkillType = _type; }
	void	SetSkillLevel(int _i) { m_iSkillLevel = _i; }

	CGameObject* GetUserObj() { return m_iUserObj; }
	CGameObject* GetTargetObj() { return m_iTargetObj; }
	SkillType	 GetSkillType() { return m_eSkillType; }
	int			 GetSkillLevel() { return m_iSkillLevel; }

	void Clear() override { m_iUserObj = nullptr; m_iTargetObj = nullptr;  m_eSkillType = SkillType::NONE; m_iSkillLevel = 0; }
};


class PlayerQEvent :
	public CGameEvent
{
public:
	PlayerQEvent()
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_SKILL_Q;
	}
private:
	CGameObject* m_UserObj;
	CGameObject* m_TargetObj;

public:
	void	SetUserObj(CGameObject* _obj) { m_UserObj = _obj; }
	void	SetTargetObj(CGameObject* _obj) { m_TargetObj = _obj; }

	CGameObject* GetUserObj() { return m_UserObj; }
	CGameObject* GetTargetObj() { return m_TargetObj; }

	void Clear() override { m_UserObj = nullptr; m_TargetObj = nullptr; }
};

class PlayerWEvent :
	public CGameEvent
{
public:
	PlayerWEvent()
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_SKILL_W;
	}
private:
	CGameObject* m_UserObj;
	CGameObject* m_TargetObj;

public:
	void	SetUserObj(CGameObject* _obj) { m_UserObj = _obj; }
	void	SetTargetObj(CGameObject* _obj) { m_TargetObj = _obj; }

	CGameObject* GetUserObj() { return m_UserObj; }
	CGameObject* GetTargetObj() { return m_TargetObj; }

	void Clear() override { m_UserObj = nullptr; m_TargetObj = nullptr; }
};

class PlayerEEvent :
	public CGameEvent
{
public:
	PlayerEEvent()
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_SKILL_E;
	}
private:
	CGameObject* m_UserObj;
	CGameObject* m_TargetObj;

public:
	void	SetUserObj(CGameObject* _obj) { m_UserObj = _obj; }
	void	SetTargetObj(CGameObject* _obj) { m_TargetObj = _obj; }

	CGameObject* GetUserObj() { return m_UserObj; }
	CGameObject* GetTargetObj() { return m_TargetObj; }

	void Clear() override { m_UserObj = nullptr; m_TargetObj = nullptr; }
};

class PlayerREvent :
	public CGameEvent
{
public:
	PlayerREvent()
	{
		m_eEventType = GAME_EVENT_TYPE::PLAYER_SKILL_R;
	}
private:
	CGameObject* m_UserObj;
	CGameObject* m_TargetObj;

public:
	void	SetUserObj(CGameObject* _obj) { m_UserObj = _obj; }
	void	SetTargetObj(CGameObject* _obj) { m_TargetObj = _obj; }

	CGameObject* GetUserObj() { return m_UserObj; }
	CGameObject* GetTargetObj() { return m_TargetObj; }

	void Clear() override { m_UserObj = nullptr; m_TargetObj = nullptr; }
};




/*
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
*/



