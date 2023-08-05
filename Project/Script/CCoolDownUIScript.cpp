#include "pch.h"
#include "CCoolDownUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>

CCoolDownUIScript::CCoolDownUIScript(COOL_DOWN_TYPE _SkillType)
	:CScript((UINT)SCRIPT_TYPE::COOLDOWNUISCRIPT)
	, m_fcooldownTime(3.f)
	, m_felapsedTime(0.f)
	, m_bSkillUse(false)
	, m_dCoolDown(0.f)
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
	if (KEY_TAP(KEY::F7) && m_SkillType == COOL_DOWN_TYPE::Q)
	{
		if (!m_bSkillUse)
		{
			m_bSkillUse = true; // ��ų ��� ����
			m_felapsedTime = 0.0f; // ��ų ��� �ð� �ʱ�ȭ
		}
	}

	if (!m_bSkillUse)
		m_dCoolDown = 0.0f; // ��Ÿ�� ����, 0 ����

	if (m_bSkillUse)
	{
		m_felapsedTime += DT; // ��ų ��� �ð� ����
		if (m_felapsedTime >= m_fcooldownTime)
		{
			m_dCoolDown = 0.0f;  // ��Ÿ�� ����, 0 ����
			m_bSkillUse = false; // ���� ����� ���� �ʱ�ȭ
		}
		else
			m_dCoolDown = 1.0f - (m_felapsedTime / m_fcooldownTime);// ��Ÿ�� ��, ��Ÿ���� ���� ������ ����Ͽ� ����
	}

	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &m_dCoolDown);

	if (m_bSkillUse)
	{
		//=============��Ʈ���===============
		if (UICamera && m_SkillType != COOL_DOWN_TYPE::PASSIVE)
		{
			float Timer = m_fcooldownTime - m_felapsedTime;
			int floor = static_cast<int>(std::floor(fabsf(Timer + 1)));

			tFont Font3 = {};
			Font3.wInputText = to_wstring(floor); //���� ��Ʈ
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


