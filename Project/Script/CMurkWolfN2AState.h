#pragma once
#include "CJungleN2AState.h"
class CMurkWolfN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfN2AState();
    ~CMurkWolfN2AState();
};

