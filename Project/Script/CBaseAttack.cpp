#include "pch.h"
#include "CBaseAttack.h"
#include <Engine/CPrefab.h>
#include <Engine/ptr.h>
#include "CUnitScript.h"
#include "CChampionScript.h"

CBaseAttack::CBaseAttack()
{
	m_strSkillName = L"BaseAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;
}

CBaseAttack::~CBaseAttack()
{
}

void CBaseAttack::tick()
{
	// ��Ÿ�� ��� �ʿ� ����
}

bool CBaseAttack::Use()
{
	// ��Ÿ ����ü ����

	if (m_vecSkillObj.size() == 0)
		return false;

	m_vecSkillObj[0]->Instantiate();

	return true;
}

void CBaseAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript)
{
	// ��Ÿ ����ü�� ���� �浹�� �� �Լ��� ���� ��ũ��Ʈ �־ ȣ���� ����

	float Damage = 0;

	// �������� ����, �⺻ ���ݷ� � ���� ������ ���
	CChampionScript* ChampScript = dynamic_cast<CChampionScript*>(_UserScript);
	if (ChampScript != nullptr)
	{
		float BaseDamage = 0.f;
		float AttackPow = ChampScript->GetAttackPower();

		// �����Դϴ�
		Damage = AttackPow;
	}

	// ���������� Ÿ���� ���¸�ŭ�� ���� �� ���� �ݿ��� ������ ���
	float DefencePow = _TargetScript->GetDefencePower();
	
	Damage -= DefencePow;

	float minDam = 10.f;
	if (Damage < minDam)
	{
		// ������ �ּҰ�
		Damage = minDam;
	}

	_TargetScript->SetCurHPVar(-Damage);
}
