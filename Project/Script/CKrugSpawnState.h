#pragma once
#include "CJungleSpawnState.h"
class CKrugSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CKrugSpawnState();
    ~CKrugSpawnState();
};

