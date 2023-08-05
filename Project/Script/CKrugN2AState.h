#pragma once
#include "CJungleN2AState.h"
class CKrugN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CKrugN2AState();
    ~CKrugN2AState();
};

