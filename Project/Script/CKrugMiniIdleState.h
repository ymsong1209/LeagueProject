#pragma once
#include "CJungleIdleState.h"
class CKrugMiniIdleState :
    public CJungleIdleState
{
private:
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CKrugMiniIdleState();
    ~CKrugMiniIdleState();
};

