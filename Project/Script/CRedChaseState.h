#pragma once
#include "CJungleChaseState.h"
class CRedChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRedChaseState();
    ~CRedChaseState();
};
