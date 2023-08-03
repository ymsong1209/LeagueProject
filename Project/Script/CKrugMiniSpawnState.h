#pragma once
#include "CJungleSpawnState.h"
class CKrugMiniSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CKrugMiniSpawnState();
    ~CKrugMiniSpawnState();
};

