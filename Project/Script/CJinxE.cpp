#include "pch.h"
#include "CJinxE.h"

CJinxE::CJinxE()
{
	m_strSkillName = L"Flame Chompers!";
	m_fCoolDown = 5.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;


	// ¡ũ�� E�� ���õ� prefab�� ���� ���
	// ToDo
	// CJinxW.cpp ����

	//m_vecSkillObj.push_back(NewPrefab);
	//
	//// ����ü ��ũ��Ʈ
	//m_iProjectileCount = 3;
	//m_ProjectileScript = new CJinxWScript;
}

CJinxE::~CJinxE()
{
}

void CJinxE::tick()
{
	CSkill::tick();
}

bool CJinxE::Use()
{
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// ToDo


	// ��Ÿ�� �ʱ�ȭ
	m_fCurCoolDown = m_fCoolDown;


    return true;
}

void CJinxE::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{

	// ToDo



	// �ǰ��� ����� KDACS ��Ŷ ����
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
