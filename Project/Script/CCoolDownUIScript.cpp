#include "pch.h"
#include "CCoolDownUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include "CUnitScript.h"
#include "CSkill.h"
#include "CJinxScript.h"

CCoolDownUIScript::CCoolDownUIScript(COOL_DOWN_TYPE _SkillType)
	:CScript((UINT)SCRIPT_TYPE::COOLDOWNUISCRIPT)
	, m_fCooldownTime(3.f)
	, m_bSkillUse(false)
	, m_fCoolRatio(0.f)
	, m_SkillType(_SkillType)
	, m_ChampType(ChampionType::END)
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
	m_PlayerUnitScript = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>();
	m_SkillQ = m_PlayerUnitScript->GetSkill(1);
	m_SkillW = m_PlayerUnitScript->GetSkill(2);
	m_SkillE = m_PlayerUnitScript->GetSkill(3);
	m_SkillR = m_PlayerUnitScript->GetSkill(4);
}

void CCoolDownUIScript::tick()
{
	if (m_SkillType == COOL_DOWN_TYPE::Q && m_SkillQ)
	{
		m_fCooldownTime = m_SkillQ->GetCoolDown();
		m_fCurCoolTime = m_SkillQ->GetCurCoolDown();

		if (m_ChampType == ChampionType::JINX)
		{
			CJinxScript* JinxScript = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CJinxScript>();

			//=====================디버깅용=====================
			JinxWeaponMode JinxMode2 = JinxScript->GetWeaponMode();
			if (KEY_TAP(KEY::Q) && JinxMode2 == JinxWeaponMode::MINIGUN)
				JinxScript->SetWeaponMode(JinxWeaponMode::ROCKET_LAUNCHER);
			else if(KEY_TAP(KEY::Q) && JinxMode2 == JinxWeaponMode::ROCKET_LAUNCHER)
				JinxScript->SetWeaponMode(JinxWeaponMode::MINIGUN);
			//==================================================

			JinxWeaponMode JinxMode = JinxScript->GetWeaponMode();

			if(JinxMode == JinxWeaponMode::MINIGUN)
				MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\Jinx\\JINX_Q.dds"));
			if (JinxMode == JinxWeaponMode::ROCKET_LAUNCHER)
				MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\Jinx\\jinx_q1.dds"));
		}
	}

	if (m_SkillType == COOL_DOWN_TYPE::W && m_SkillW)
	{
		m_fCooldownTime = m_SkillW->GetCoolDown();
		m_fCurCoolTime = m_SkillW->GetCurCoolDown();
	}

	if (m_SkillType == COOL_DOWN_TYPE::E && m_SkillE)
	{
		m_fCooldownTime = m_SkillE->GetCoolDown();
		m_fCurCoolTime = m_SkillE->GetCurCoolDown();
	}

	if (m_SkillType == COOL_DOWN_TYPE::R && m_SkillR)
	{
		m_fCooldownTime = m_SkillR->GetCoolDown();
		m_fCurCoolTime = m_SkillR->GetCurCoolDown();
	}

	if (m_fCurCoolTime <= 0)
	{
		m_fCoolRatio = 0.f;  // 쿨타임 종료, 0 설정
		m_bSkillUse = false;
	}
	else
	{
		m_fCoolRatio = m_fCurCoolTime / m_fCooldownTime;// 쿨타임 중, 쿨타임의 진행 비율을 계산하여 설정
		m_bSkillUse = true;
	}

	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &m_fCoolRatio);

	//=============폰트출력===============
	if (UICamera && m_SkillType != COOL_DOWN_TYPE::PASSIVE && m_bSkillUse)
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


