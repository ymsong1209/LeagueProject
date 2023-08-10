#pragma once
#include "CSkill.h"

class CMalphiteW :
    public CSkill
{
public:
    CMalphiteW();
    ~CMalphiteW();

public:
    virtual void tick() override;
    virtual bool Use() override;
};

