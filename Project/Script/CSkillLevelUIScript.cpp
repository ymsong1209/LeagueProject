#include "pch.h"
#include "CSkillLevelUIScript.h"
#include "CUnitScript.h"

CSkillLevelUIScript::CSkillLevelUIScript()
	:CScript((UINT)SCRIPT_TYPE::SKILLLEVELUISCRIPT)
{
}

CSkillLevelUIScript::~CSkillLevelUIScript()
{
}

void CSkillLevelUIScript::begin()
{

}

void CSkillLevelUIScript::tick()
{
	CUnitScript* pUnitScript = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>();

	if (pUnitScript != nullptr)
	{
		m_iQLevel = pUnitScript->GetSkillLevel(1);
		m_iWLevel = pUnitScript->GetSkillLevel(2);
		m_iELevel = pUnitScript->GetSkillLevel(3);
		m_iRLevel = pUnitScript->GetSkillLevel(4);
	}

	//===============µð¹ö±ë¿ë================
	/*m_iQLevel = 5;
	m_iWLevel = 5;
	m_iELevel = 5;
	m_iRLevel = 5;*/

	/*if (KEY_TAP(KEY::Q))
	{
		++m_iQLevel;
		if (m_iQLevel > 5)
			m_iQLevel = 0;
	}

	if (KEY_TAP(KEY::W))
	{
		++m_iWLevel;
		if (m_iWLevel > 5)
			m_iWLevel = 0;
	}

	if (KEY_TAP(KEY::E))
	{
		++m_iELevel;
		if (m_iELevel > 5)
			m_iELevel = 0;
	}

	if (KEY_TAP(KEY::R))
	{
		++m_iRLevel;
		if (m_iRLevel > 5)
			m_iRLevel = 0;
	}*/
	//=======================================


	MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_iQLevel);
	MeshRender()->GetMaterial(0)->SetScalarParam(INT_1, &m_iWLevel);
	MeshRender()->GetMaterial(0)->SetScalarParam(INT_2, &m_iELevel);
	MeshRender()->GetMaterial(0)->SetScalarParam(INT_3, &m_iRLevel);
}

void CSkillLevelUIScript::BeginOverlap(CCollider2D* _Other)
{
}
