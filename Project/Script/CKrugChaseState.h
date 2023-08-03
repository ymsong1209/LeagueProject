#pragma once
#include "CJungleChaseState.h"
class CKrugChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CKrugChaseState();
    ~CKrugChaseState();
};

