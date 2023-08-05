#pragma once
#include "CJungleReturnState.h"
class CKrugReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CKrugReturnState();
    ~CKrugReturnState();
};

