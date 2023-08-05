#pragma once
#include "CJungleMonsterScript.h"
class CRazorBeakScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CRazorBeakScript();
    ~CRazorBeakScript();
    CLONE(CRazorBeakScript);
};

