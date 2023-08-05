#pragma once
#include "CJungleDeathState.h"
class CKrugMiniDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CKrugMiniDeathState();
    ~CKrugMiniDeathState();
};

