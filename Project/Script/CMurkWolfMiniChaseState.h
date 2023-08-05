#pragma once
#include "CJungleChaseState.h"
class CMurkWolfMiniChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfMiniChaseState();
    ~CMurkWolfMiniChaseState();
};

