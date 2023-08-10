#include "pch.h"
#include "CJinxR.h"

CJinxR::CJinxR()
{
	m_strSkillName = L"Super Mega Death Rocket!";
	m_fCoolDown = 10.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;


	// ¡ũ�� R�� ���õ� prefab�� ���� ���
	// ToDo
	// CJinxW.cpp ����

	//m_vecSkillObj.push_back(NewPrefab);
	//
	//// ����ü ��ũ��Ʈ
	//m_iProjectileCount = 1;
	//m_ProjectileScript = new CJinxRScript;
}

CJinxR::~CJinxR()
{
}

void CJinxR::tick()
{
	CSkill::tick();
}

bool CJinxR::Use()
{
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// ToDo


	// ��Ÿ�� �ʱ�ȭ
	m_fCurCoolDown = m_fCoolDown;


	return true;
}

void CJinxR::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// ToDo



	// �ǰ��� ����� KDACS ��Ŷ ����
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
