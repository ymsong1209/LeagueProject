#pragma once
#include "CJungleReturnState.h"
class CMurkWolfReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfReturnState();
    ~CMurkWolfReturnState();
};

