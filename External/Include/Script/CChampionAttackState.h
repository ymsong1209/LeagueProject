#pragma once
#include "CUnitState.h"

class CChampionAttackState :
    public CUnitState
{
public:
    CChampionAttackState();
    ~CChampionAttackState();

private:
    int     m_iUserID;
    int     m_iTargetID;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;

public:
    void SetUserID(int _id) { m_iUserID = _id; }
    void SetTargetID(int _id) { m_iTargetID = _id; }
};

