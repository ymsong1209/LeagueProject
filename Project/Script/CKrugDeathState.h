#pragma once
#include "CJungleDeathState.h"
class CKrugDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CKrugDeathState();
    ~CKrugDeathState();
};

