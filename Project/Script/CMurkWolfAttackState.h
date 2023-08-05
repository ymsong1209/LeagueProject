#pragma once
#include "CJungleAttackState.h"
class CMurkWolfAttackState :
    public CJungleAttackState
{
private:
    int         m_iAttackAnimNum;
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CMurkWolfAttackState();
    ~CMurkWolfAttackState();
};

