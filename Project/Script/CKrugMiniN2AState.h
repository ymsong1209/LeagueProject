#pragma once
#include "CJungleN2AState.h"
class CKrugMiniN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CKrugMiniN2AState();
    ~CKrugMiniN2AState();
};

