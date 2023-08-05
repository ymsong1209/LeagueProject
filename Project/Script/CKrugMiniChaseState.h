#pragma once
#include "CJungleChaseState.h"
class CKrugMiniChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CKrugMiniChaseState();
    ~CKrugMiniChaseState();
};

