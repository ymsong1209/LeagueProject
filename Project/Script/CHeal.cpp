#include "pch.h"
#include "CHeal.h"

#include "CChampionScript.h"

CHeal::CHeal()
{
    m_fCoolDown = 120.f;
    m_iLevel = 1;
}

CHeal::~CHeal()
{
}

void CHeal::tick()
{
    CSkill::tick();
}

bool CHeal::Use()
{
    // 스킬 쿨타임 초기화
    if (!CSkill::Use())
        return false;

    // 챔피언 스크립트 -> 현재 Hp 추가
    CChampionScript* ChampScript = dynamic_cast<CChampionScript*>(m_OwnerScript);
    if (ChampScript != nullptr)
    {
        ChampScript->SetCurHPVar(200);
    }

    // Heal SFX & VFX 재생

    return true;
}
