#include "pch.h"
#include "CHpMpRatioUIScript.h"


CHpMpRatioUIScript::CHpMpRatioUIScript(BARTYPE _Type)
	:CScript((UINT)SCRIPT_TYPE::HPMPRATIOUISCRIPT)
	,BarType(_Type)
	, m_fTotal(100.f)
	, m_fCurrent(100.f)
{
}

CHpMpRatioUIScript::CHpMpRatioUIScript()
	:CScript((UINT)SCRIPT_TYPE::HPMPRATIOUISCRIPT)
{
}


CHpMpRatioUIScript::~CHpMpRatioUIScript()
{
}


void CHpMpRatioUIScript::begin()
{

}

void CHpMpRatioUIScript::tick()
{
	//��ƽ���� ���� ü��, ��üü�� �����;���(��üü�µ� ���� �� ����)
	// �� Ÿ���� mp��� mp�� �������� hp��� hp�� �������� ��.
	//--------------------------------

	//------������-------------
	if (BarType == BARTYPE::HP)
	{
		if (KEY_PRESSED(KEY::B))
			m_fTotal += DT * 50.f;

		if (KEY_PRESSED(KEY::V))
			m_fTotal -= DT * 50.f;
	}

	if (BarType == BARTYPE::MP)
	{
		if (KEY_PRESSED(KEY::G))
			m_fTotal += DT * 50.f;

		if (KEY_PRESSED(KEY::F))
			m_fTotal -= DT * 50.f;
	}
	//--------------------------------

	//CurrentExpRatio = CurrentExp / LevelTotalExp;

	//if (CurrentExp >= LevelTotalExp)
	//	CurrentExp = LevelTotalExp;
	//if (CurrentExp <= 0.f)
	//	CurrentExp = 0.f;

	m_fCurrentRatio = m_fCurrent / m_fTotal;
	if (m_fCurrent >= m_fTotal)
		m_fCurrent = m_fTotal;

	if (m_fCurrent <= 0.f)
		m_fCurrent = 0.f;

	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &m_fCurrentRatio);
}

void CHpMpRatioUIScript::BeginOverlap(CCollider2D* _Other)
{
}

