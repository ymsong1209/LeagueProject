#pragma once
#include "CJungleA2NState.h"
class CKrugA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CKrugA2NState();
    ~CKrugA2NState();
};

