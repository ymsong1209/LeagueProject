#pragma once


class CUnitScript;
class CPrefab;
class CProjectileScript;
#include <Engine\ptr.h>

class CSkill
{
public:
	CSkill();
	virtual	~CSkill();

protected:
	wstring					m_strSkillName;	// ��ų��

	float					m_fCoolDown;	// ��Ÿ��
	float					m_fCurCoolDown;	// ���� ��Ÿ��

	CGameObject*			m_UserObj;		// ����� 
	CGameObject*			m_TargetObj;	// Ÿ�� (��Ÿ���Ͻ� -1)

	SkillType				m_eSkillType;	// ��ų Ÿ��

	float					m_fSkillRange;	// ��ų ��Ÿ�
	//float					m_fDamage;		// ��ų �⺻ ������
	float					m_fCost;        // ��� �ڿ�(���� ��)

	int						m_iLevel;		// ��ų ����
	int						m_iMaxLevel;	// �ִ� ���� (QWE : 5, R : 3)

	CUnitScript*			m_OwnerScript;	// è�Ǿ� ��ũ��Ʈ
	CProjectileScript*		m_ProjectileScript;

	vector<Ptr<CPrefab>>	m_vecSkillObj;			// ��ų ��� �� ������ ����ü ��
	int						m_iProjectileCount;		// ����ü ����


	// ======== ��Ʈ�� / �����̻� ���� ========== 
	bool					m_bIsDot;		// �����Ͽ� ������/�����̻��� �Ŵ��� ����
	float					m_fDotTime;		// ���ӽ�ų �ð�
	int						m_iTickCount;	// ���ӽð� ���� ����� ������ �������� �� ��
	float					m_fDotDamage;	// ��Ʈ ������
	CC						m_eCC;			// ���ӽ�ų CC��

public:
	virtual void tick() = 0;		// ��Ÿ�� �������. �ʼ�!
	virtual bool Use() = 0;			// �� ��ų���� �ٸ� ��� ȿ�� �ۼ�. �ʼ�!
	virtual vector<CGameObject*>& GetProjectile();

	virtual	void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel) {};	// ����ü ���� �¾��� �� ȣ��Ǵ� �Լ�

	Vec3 GetMousePos();
	Vec3 GetMouseDir();

public:
	// è�Ǿ� ��ũ��Ʈ �����ڿ��� ��ų ���� �� Owner ���
	void SetOwnerScript(CUnitScript* _script) { m_OwnerScript = _script; }

	// ��ų ��밡�� ���� (��Ÿ�Ӹ�)
	bool	IsReady() { return m_fCurCoolDown <= 0.0f; }

	wstring GetSkillName() { return m_strSkillName; }

	SkillType GetSkillType() { return m_eSkillType; }

	CGameObject*		GetUserObj() { return m_UserObj; }
	CGameObject*		GetTargetObj() { return m_TargetObj; }

	void SetUserObj(CGameObject*	_obj) { m_UserObj = _obj; }
	void SetTargetObj(CGameObject* _obj) { m_TargetObj = _obj; }

	float	GetCurCoolDown() { return m_fCurCoolDown; }
	float	GetCoolDown() { return m_fCoolDown; }
	float	GetSkillRange() { return m_fSkillRange; }
	float	GetCost() { return m_fCost; }

	int		GetLevel() { return m_iLevel; }

	void	SetProjectileObj(Ptr<CPrefab> _prefab) { m_vecSkillObj.push_back(_prefab); }

	int		GetProjectileCount() { return m_iProjectileCount; }

};

