#pragma once
#include "CJungleReturnState.h"
class CMurkWolfMiniReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfMiniReturnState();
    ~CMurkWolfMiniReturnState();
};

