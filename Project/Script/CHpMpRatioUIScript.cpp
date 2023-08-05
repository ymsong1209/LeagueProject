#include "pch.h"
#include "CHpMpRatioUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>


CHpMpRatioUIScript::CHpMpRatioUIScript(BARTYPE _Type)
	:CScript((UINT)SCRIPT_TYPE::HPMPRATIOUISCRIPT)
	,BarType(_Type)
	, m_fTotal(1700.f)
	, m_fCurrent(1700.f)
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
	//매틱마다 현재 체력, 전체체력 가져와야함(전체체력도 변할 수 있음)
	// 바 타입이 mp라면 mp를 가져오고 hp라면 hp를 가져오면 됨.
	//--------------------------------

	//------디버깅용-------------
	if (BarType == BARTYPE::HP)
	{
		if (KEY_PRESSED(KEY::B))
			m_fCurrent += DT * 50.f;

		if (KEY_PRESSED(KEY::V))
			m_fCurrent -= DT * 50.f;
	}

	if (BarType == BARTYPE::MP)
	{
		if (KEY_PRESSED(KEY::G))
			m_fCurrent += DT * 50.f;

		if (KEY_PRESSED(KEY::F))
			m_fCurrent -= DT * 50.f;
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

	//============폰트=============
	if (BarType == BARTYPE::HP)
	{
		wstring CurrentHP = to_wstring((int)m_fCurrent);
		wstring TotalHP = to_wstring((int)m_fTotal);
		wstring slash = L" / ";
		wstring Text = CurrentHP + slash + TotalHP;

		tFont Font3 = {};
		Font3.wInputText = Text; //레벨 폰트
		Font3.fontType = FONT_TYPE::RIX_KOR_L;
		Font3.fFontSize = 13.5;
		Font3.vDisplayPos = Vec2(767, 961);
		Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);
		UICamera->AddText(FONT_DOMAIN::MASK, Font3);
	}

	else if (BarType == BARTYPE::MP)
	{
		wstring CurrentMP = to_wstring((int)m_fCurrent);
		wstring TotalMP = to_wstring((int)m_fTotal);
		wstring slash = L" / ";
		wstring Text = CurrentMP + slash + TotalMP;

		tFont Font3 = {};
		Font3.wInputText = Text; //레벨 폰트
		Font3.fontType = FONT_TYPE::RIX_KOR_L;
		Font3.fFontSize = 13.5;
		Font3.vDisplayPos = Vec2(767, 976);
		Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);
		UICamera->AddText(FONT_DOMAIN::MASK, Font3);
	}
}

void CHpMpRatioUIScript::BeginOverlap(CCollider2D* _Other)
{
}

