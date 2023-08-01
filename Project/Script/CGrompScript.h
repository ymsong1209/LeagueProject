#pragma once
#include "CJungleMonsterScript.h"
class CGrompScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CGrompScript();
    ~CGrompScript();
    CLONE(CGrompScript);
};

