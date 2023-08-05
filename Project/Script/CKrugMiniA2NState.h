#pragma once
#include "CJungleA2NState.h"
class CKrugMiniA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CKrugMiniA2NState();
    ~CKrugMiniA2NState();
};

