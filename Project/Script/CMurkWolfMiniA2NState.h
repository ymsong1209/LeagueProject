#pragma once
#include "CJungleA2NState.h"
class CMurkWolfMiniA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfMiniA2NState();
    ~CMurkWolfMiniA2NState();
};

