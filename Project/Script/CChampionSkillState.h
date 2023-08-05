#pragma once
#include "CUnitState.h"
class CChampionSkillState :
    public CUnitState
{
public:
    CChampionSkillState();
    ~CChampionSkillState();

protected:
    CGameObject* m_UserObj;
    CGameObject* m_TargetObj;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;

public:
    void SetUserObj(CGameObject* _obj) { m_UserObj = _obj; }
    void SetTargetObj(CGameObject* _obj) { m_TargetObj = _obj; }
};

