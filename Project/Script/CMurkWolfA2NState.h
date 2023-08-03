#pragma once
#include "CJungleA2NState.h"
class CMurkWolfA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfA2NState();
    ~CMurkWolfA2NState();
};

