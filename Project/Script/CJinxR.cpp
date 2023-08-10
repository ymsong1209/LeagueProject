#include "pch.h"
#include "CJinxR.h"

CJinxR::CJinxR()
{
	m_strSkillName = L"Super Mega Death Rocket!";
	m_fCoolDown = 10.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;


	// 징크스 R와 관련된 prefab을 만들어서 등록
	// ToDo
	// CJinxW.cpp 참조

	//m_vecSkillObj.push_back(NewPrefab);
	//
	//// 투사체 스크립트
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
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;

	// ToDo


	// 쿨타임 초기화
	m_fCurCoolDown = m_fCoolDown;


	return true;
}

void CJinxR::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// ToDo



	// 피격자 사망시 KDACS 패킷 전송
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
