#pragma once
#include "CJungleMonsterScript.h"
class CBlueScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CBlueScript();
    ~CBlueScript();
    CLONE(CBlueScript);
};

