#pragma once
#include "CJungleN2AState.h"
class CMurkWolfMiniN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfMiniN2AState();
    ~CMurkWolfMiniN2AState();
};

