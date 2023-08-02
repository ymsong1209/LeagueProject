#pragma once
#include "CUnitState.h"

class CChampionAttackState :
    public CUnitState
{
public:
    CChampionAttackState();
    ~CChampionAttackState();

private:
    CGameObject*     m_iUserObj;
    CGameObject*     m_iTargetObj;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;

public:
    void SetUserObj(CGameObject* _obj)  { m_iUserObj = _obj; }
    void SetTargetObj(CGameObject* _obj) { m_iTargetObj = _obj; }
};

