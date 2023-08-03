#pragma once
#include "CJungleChaseState.h"
class CMurkWolfChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfChaseState();
    ~CMurkWolfChaseState();
};

