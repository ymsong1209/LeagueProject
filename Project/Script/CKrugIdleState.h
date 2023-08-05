#pragma once
#include "CJungleIdleState.h"
class CKrugIdleState :
    public CJungleIdleState
{
private:
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CKrugIdleState();
    ~CKrugIdleState();
};

