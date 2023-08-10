#include "pch.h"
#include "CMalphiteW.h"

#include "CMalphiteScript.h"
#include "CTimedEffect.h"

CMalphiteW::CMalphiteW()
{
	m_strSkillName = L"GroundSlam";
	m_fCoolDown = 5.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;

}

CMalphiteW::~CMalphiteW()
{
}

void CMalphiteW::tick()
{
	CSkill::tick();
}

bool CMalphiteW::Use()
{
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	CMalphiteScript* chamscript = dynamic_cast<CMalphiteScript*>(m_OwnerScript);
	chamscript->SetWActive(true);
	

	// ��Ÿ�� �ʱ�ȭ
	m_fCurCoolDown = m_fCoolDown;

	return true;
}
