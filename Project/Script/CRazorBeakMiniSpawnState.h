#pragma once
#include "CJungleSpawnState.h"
class CRazorBeakMiniSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CRazorBeakMiniSpawnState();
    ~CRazorBeakMiniSpawnState();
};

