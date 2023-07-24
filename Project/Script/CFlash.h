#pragma once
#include "CSkill.h"
class CFlash :
    public CSkill
{
public:
    CFlash();
    ~CFlash();

private:

public:
    virtual void tick();
    virtual bool Use();
};

