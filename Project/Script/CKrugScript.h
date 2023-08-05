#pragma once
#include "CJungleMonsterScript.h"
class CKrugScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CKrugScript();
    ~CKrugScript();
    CLONE(CKrugScript);
};

