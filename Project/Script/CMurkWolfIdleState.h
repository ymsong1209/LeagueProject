#pragma once
#include "CJungleIdleState.h"
class CMurkWolfIdleState :
    public CJungleIdleState
{
private:
    int         m_iIdleAnimNum;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CMurkWolfIdleState();
    ~CMurkWolfIdleState();
};

