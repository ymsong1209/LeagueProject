#pragma once
#include "CBasicAttack.h"
class CJinxRocketBasicAttack :
    public CBasicAttack
{
public:
    CJinxRocketBasicAttack();
    ~CJinxRocketBasicAttack();

private:

public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel);
};
