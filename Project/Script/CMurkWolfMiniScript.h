#pragma once
#include "CJungleMonsterScript.h"
class CMurkWolfMiniScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CMurkWolfMiniScript();
    ~CMurkWolfMiniScript();
    CLONE(CMurkWolfMiniScript);
};

