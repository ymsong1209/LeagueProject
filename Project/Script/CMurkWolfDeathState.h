#pragma once
#include "CJungleDeathState.h"
class CMurkWolfDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CMurkWolfDeathState();
    ~CMurkWolfDeathState();
};

