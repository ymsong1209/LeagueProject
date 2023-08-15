#pragma once
#include <Engine\CScript.h>
#include "CUIScript.h"


class CCharacterUIScript :
    public CUIScript
{
private:
    CGameObject* CharacterImage;
    CGameObject* Skill_Q_Image;
    CGameObject* Skill_W_Image;
    CGameObject* Skill_E_Image;
    CGameObject* Skill_R_Image;
    CGameObject* Skill_PASSIVE_Image;

    SkillNum SkillNumber;

    CGameObject* Spell_D;
    CGameObject* Spell_F;

    SUMMONERS_SPELL Spell_1;
    SUMMONERS_SPELL Spell_2;

    CGameObject* HPBar;
    CGameObject* MPBar;
    CGameObject* EXPBar;
    CGameObject* WadingTotem;

    int         m_iGold; // °¡Áö°í ÀÖ´Â µ·.(°ñµå ÆùÆ®UI)
    int         m_iPlayerLevel;
    int         m_iPrevChampLevel;

    bool        m_bAnimating;
    float       m_fTimer;

    float animationDuration;
    float animationDistance;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void SkillUILoad();
    void SpellUILoad();
    void BarUILoad();

    void LevelUpFont();
public:
    void UISetting();

    CLONE(CCharacterUIScript);

public:
    CCharacterUIScript();
    ~CCharacterUIScript();
};

