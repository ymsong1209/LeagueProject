#pragma once

class CSkill
{
public:
	CSkill();
	virtual	~CSkill();

protected:
	wstring					m_strSkillName;	// ��ų��

	float					m_fCoolDown;	// ��Ÿ��
	float					m_fCurCoolDown;	// ���� ��Ÿ��

	float					m_fSkillRange;	// ��ų ��Ÿ�

	float					m_fCost;        // ��� �ڿ�(���� ��)

	int						m_iLevel;		// ��ų ����
	int						m_iMaxLevel;	// �ִ� ���� (QWE : 5, R : 3)

	CScript* m_OwnerScript;	// è�Ǿ� ��ũ��Ʈ

	vector<CGameObject*>	m_vecSkillObj;	// ��ų ��� �� ������ Obj(�޽� ��)

public:
	virtual void tick() = 0;		// ��Ÿ�� �������. �ʼ�!
	virtual bool Use() = 0;			// �� ��ų���� �ٸ� ��� ȿ�� �ۼ�. �ʼ�!

public:
	// è�Ǿ� ��ũ��Ʈ �����ڿ��� ��ų ���� �� Owner ���
	void SetOwnerScript(CScript* _script) { m_OwnerScript = _script; }

	// ��ų ��밡�� ���� (��Ÿ�Ӹ�)
	bool	IsReady() { return m_fCurCoolDown <= 0.0f; }


	wstring GetSkillName() { return m_strSkillName; }

	float	GetCurCoolDown() { return m_fCurCoolDown; }
	float	GetCoolDown() { return m_fCoolDown; }
	float	GetSkillRange() { return m_fSkillRange; }
	float	GetCost() { return m_fCost; }

	int		GetLevel() { return m_iLevel; }

};

