#pragma once
#include "CJungleSpawnState.h"
class CRazorBeakSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CRazorBeakSpawnState();
    ~CRazorBeakSpawnState();
};

