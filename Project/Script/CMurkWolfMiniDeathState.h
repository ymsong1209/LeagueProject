#pragma once
#include "CJungleDeathState.h"
class CMurkWolfMiniDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CMurkWolfMiniDeathState();
    ~CMurkWolfMiniDeathState();
};

