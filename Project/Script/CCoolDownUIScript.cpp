#include "pch.h"
#include "CCoolDownUIScript.h"

CCoolDownUIScript::CCoolDownUIScript()
	:CScript((UINT)SCRIPT_TYPE::COOLDOWNUISCRIPT)
	, m_fcooldownTime(3.f)
	, m_felapsedTime(0.f)
	, m_bSkillUse(false)
	, m_dCoolDown(0.f)
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
    if (KEY_TAP(KEY::F7))
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
}


