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
            m_bSkillUse = true; // 스킬 사용 시작
            m_felapsedTime = 0.0f; // 스킬 사용 시간 초기화
        }
    }

    if (!m_bSkillUse)
        m_dCoolDown = 0.0f; // 쿨타임 없음, 0 설정

    if (m_bSkillUse)
    {
        m_felapsedTime += DT; // 스킬 사용 시간 누적
        if (m_felapsedTime >= m_fcooldownTime)
        {
            m_dCoolDown = 0.0f;  // 쿨타임 종료, 0 설정
            m_bSkillUse = false; // 다음 사용을 위해 초기화
        }
        else 
            m_dCoolDown = 1.0f - (m_felapsedTime / m_fcooldownTime);// 쿨타임 중, 쿨타임의 진행 비율을 계산하여 설정
    }

    MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &m_dCoolDown);
}


