#pragma once
#include "CSkill.h"
class CHeal :
    public CSkill
{
public:
    CHeal();
    ~CHeal();

private:

public:
    virtual void tick();
    virtual bool Use();
};

