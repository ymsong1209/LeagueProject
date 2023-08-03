#pragma once
#include "CJungleMonsterScript.h"
class CMurkWolfScript :
    public CJungleMonsterScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CMurkWolfScript();
    ~CMurkWolfScript();
    CLONE(CMurkWolfScript);
};

