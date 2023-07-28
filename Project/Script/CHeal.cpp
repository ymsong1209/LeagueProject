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
    // ��ų ��Ÿ�� �ʱ�ȭ
    if (!CSkill::Use())
        return false;

    // è�Ǿ� ��ũ��Ʈ -> ���� Hp �߰�
    CChampionScript* ChampScript = dynamic_cast<CChampionScript*>(m_OwnerScript);
    if (ChampScript != nullptr)
    {
        ChampScript->SetCurHPVar(200);
    }

    // Heal SFX & VFX ���

    return true;
}
