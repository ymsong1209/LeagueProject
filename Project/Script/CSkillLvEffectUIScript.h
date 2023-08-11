#pragma once
#include <Engine\CScript.h>

class CSkillLvEffectUIScript :
    public CScript
{
private:
    int                     m_iQLevel;
    int                     m_iWLevel;
    int                     m_iELevel;
    int                     m_iRLevel;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


public:
    CLONE(CSkillLvEffectUIScript);

public:
    CSkillLvEffectUIScript();
    ~CSkillLvEffectUIScript();
};

