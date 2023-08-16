#pragma once
#include "CUnitState.h"

class CChampionAttackState :
    public CUnitState
{
public:
    CChampionAttackState();
    ~CChampionAttackState();

private:
    CGameObject*     m_UserObj;
    CGameObject*     m_TargetObj;

    Vec3             m_EnterRot;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;

public:
    void SetUserObj(CGameObject* _obj)  { m_UserObj = _obj; }
    void SetTargetObj(CGameObject* _obj) { m_TargetObj = _obj; }
};

