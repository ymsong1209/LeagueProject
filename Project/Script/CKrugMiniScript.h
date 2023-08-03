#pragma once
#include "CJungleMonsterScript.h"
class CKrugMiniScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CKrugMiniScript();
    ~CKrugMiniScript();
    CLONE(CKrugMiniScript);
};

