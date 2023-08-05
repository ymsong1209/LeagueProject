#include "pch.h"
#include "CExpRatioUIScript.h"
#include <Engine\define.h>
#include <Engine\CKeyMgr.h>

void CExpRatioUIScript::begin()
{
}

void CExpRatioUIScript::tick()
{
	//------������-------------
	if (KEY_PRESSED(KEY::P))
		m_fCurrentExp += DT * 50.f;

	if (KEY_PRESSED(KEY::O))
		m_fCurrentExp -= DT * 50.f;
	//--------------------------------
	
	m_fCurrentExpRatio = m_fCurrentExp / m_fLevelTotalExp;

	if (m_fCurrentExp >= m_fLevelTotalExp)
		m_fCurrentExp = m_fLevelTotalExp;
	if (m_fCurrentExp <= 0.f)
		m_fCurrentExp = 0.f;

	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &m_fCurrentExpRatio);
}

void CExpRatioUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CExpRatioUIScript::CExpRatioUIScript()
	:CScript((UINT)SCRIPT_TYPE::EXPRATIOUISCRIPT)
	, m_fLevelTotalExp(100.f)
	, m_fCurrentExp(100.f)
{

}

CExpRatioUIScript::~CExpRatioUIScript()
{
}
