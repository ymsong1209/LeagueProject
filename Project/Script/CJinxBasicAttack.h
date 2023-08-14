#pragma once
#include "CBasicAttack.h"
class CJinxBasicAttack :
    public CBasicAttack
{
public:
    CJinxBasicAttack();
    ~CJinxBasicAttack();

private:

public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel);
};

