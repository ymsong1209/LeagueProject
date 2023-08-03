#pragma once
#include "CJungleAttackState.h"
class CKrugMiniAttackState :
    public CJungleAttackState
{
private:
    int         m_iAttackAnimNum;
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CKrugMiniAttackState();
    ~CKrugMiniAttackState();
};

