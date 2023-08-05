#pragma once
#include "CJungleMonsterScript.h"
class CRedScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CRedScript();
    ~CRedScript();
    CLONE(CRedScript);
};

