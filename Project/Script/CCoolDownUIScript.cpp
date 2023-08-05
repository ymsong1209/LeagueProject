#include "pch.h"
#include "CCoolDownUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include "CUnitScript.h"
#include "CSkill.h"

CCoolDownUIScript::CCoolDownUIScript(COOL_DOWN_TYPE _SkillType)
	:CScript((UINT)SCRIPT_TYPE::COOLDOWNUISCRIPT)
	, m_fCooldownTime(3.f)
	, m_bSkillUse(false)
	, m_fCoolRatio(0.f)
	, m_SkillType(_SkillType)
{
}

CCoolDownUIScript::CCoolDownUIScript()
	:CScript((UINT)SCRIPT_TYPE::COOLDOWNUISCRIPT)
{
}

CCoolDownUIScript::~CCoolDownUIScript()
{

}

void CCoolDownUIScript::begin()
{

}

void CCoolDownUIScript::tick()
{
	if (m_SkillType == COOL_DOWN_TYPE::Q)
	{
		CUnitScript* UnitInfo = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>();
		m_fCooldownTime = UnitInfo->GetSkill(1)->GetCoolDown();
		m_fCurCoolTime = UnitInfo->GetSkill(1)->GetCurCoolDown();
	}

	if (m_SkillType == COOL_DOWN_TYPE::W)
	{

	}

	if (m_SkillType == COOL_DOWN_TYPE::E)
	{

	}

	if (m_SkillType == COOL_DOWN_TYPE::R)
	{

	}
	//===========디버깅용================
	if (KEY_TAP(KEY::F7) && !m_bSkillUse)
	{
		m_bSkillUse = true;
	}

	if (!m_bSkillUse)
	{
		m_fCoolRatio = 0.0f; // 쿨타임 없음, 0 설정
		m_fCurCoolTime = m_fCooldownTime;
	}

	if (m_bSkillUse)
	{
		m_fCurCoolTime -= DT;
		if (m_fCurCoolTime <= 0)
		{
			m_fCoolRatio = 0.0f;  // 쿨타임 종료, 0 설정
			m_bSkillUse = false; // 다음 사용을 위해 초기화
		}
		else
			m_fCoolRatio = m_fCurCoolTime / m_fCooldownTime;// 쿨타임 중, 쿨타임의 진행 비율을 계산하여 설정
	}
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Jinx_E.mtrl"), 0);
	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &m_fCoolRatio);

	if (m_bSkillUse)
	{
		//=============폰트출력===============
		if (UICamera && m_SkillType != COOL_DOWN_TYPE::PASSIVE)
		{
			float Timer = m_fCurCoolTime;
			int floor = static_cast<int>(std::floor(fabsf(Timer + 1)));

			tFont Font3 = {};
			Font3.wInputText = to_wstring(floor); //레벨 폰트
			Font3.fontType = FONT_TYPE::RIX_KOR_L;
			Font3.fFontSize = 16.5;
			Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);

			switch ((UINT)m_SkillType)
			{
			case ((UINT)COOL_DOWN_TYPE::Q):
				Font3.vDisplayPos = Vec2(670, 915);
				break;
			case ((UINT)COOL_DOWN_TYPE::W):
				Font3.vDisplayPos = Vec2(722, 915);
				break;
			case ((UINT)COOL_DOWN_TYPE::E):
				Font3.vDisplayPos = Vec2(773, 915);
				break;
			case ((UINT)COOL_DOWN_TYPE::R):
				Font3.vDisplayPos = Vec2(824, 915);
				break;
			case ((UINT)COOL_DOWN_TYPE::SPELL_D):
			{
				Font3.vDisplayPos = Vec2(872, 914);
				Font3.fFontSize = 10.5f;
			}
				break;
			case ((UINT)COOL_DOWN_TYPE::SPELL_F):
			{
				Font3.vDisplayPos = Vec2(910, 914);
				Font3.fFontSize = 10.5f;
			}
				break;
			}

			UICamera->AddText(FONT_DOMAIN::MASK, Font3);
		}

	}
}


