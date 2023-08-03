#pragma once
#include "CJungleReturnState.h"
class CKrugMiniReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CKrugMiniReturnState();
    ~CKrugMiniReturnState();
};

