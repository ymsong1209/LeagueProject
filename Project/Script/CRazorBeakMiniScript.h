#pragma once
#include "CJungleMonsterScript.h"
class CRazorBeakMiniScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CRazorBeakMiniScript();
    ~CRazorBeakMiniScript();
    CLONE(CRazorBeakMiniScript);
};

