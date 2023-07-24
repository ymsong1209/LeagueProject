#pragma once

class CSkill
{
public:
	CSkill();
	virtual	~CSkill();

protected:
	wstring					m_strSkillName;	// 스킬명

	float					m_fCoolDown;	// 쿨타임
	float					m_fCurCoolDown;	// 현재 쿨타임

	float					m_fSkillRange;	// 스킬 사거리

	float					m_fCost;        // 사용 자원(마나 등)

	int						m_iLevel;		// 스킬 레벨
	int						m_iMaxLevel;	// 최대 레벨 (QWE : 5, R : 3)

	CScript* m_OwnerScript;	// 챔피언 스크립트

	vector<CGameObject*>	m_vecSkillObj;	// 스킬 사용 시 생성할 Obj(메쉬 등)

public:
	virtual void tick() = 0;		// 쿨타임 계산해줌. 필수!
	virtual bool Use() = 0;			// 각 스킬마다 다른 사용 효과 작성. 필수!

public:
	// 챔피언 스크립트 생성자에서 스킬 생성 후 Owner 등록
	void SetOwnerScript(CScript* _script) { m_OwnerScript = _script; }

	// 스킬 사용가능 여부 (쿨타임만)
	bool	IsReady() { return m_fCurCoolDown <= 0.0f; }


	wstring GetSkillName() { return m_strSkillName; }

	float	GetCurCoolDown() { return m_fCurCoolDown; }
	float	GetCoolDown() { return m_fCoolDown; }
	float	GetSkillRange() { return m_fSkillRange; }
	float	GetCost() { return m_fCost; }

	int		GetLevel() { return m_iLevel; }

};

