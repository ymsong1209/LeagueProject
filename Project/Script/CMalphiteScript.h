#pragma once
#include "CChampionScript.h"

class CMalphiteScript :
    public CChampionScript
{
public:
    CMalphiteScript();
    virtual ~CMalphiteScript();

private:
    float        m_fAttackAnimCoolTime; //말파이트 1타 이후 2타를 칠건데, 1타를 치고 일정시간이 지나면 2타 대신 1타로 돌아옴

    bool         m_bWActive;
    float        m_fWActiveTime;

    
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void GetInput() override;

public:

    float       GetAttackAnimCoolTime() { return m_fAttackAnimCoolTime; }
    void        SetAttackAnimCoolTime(float _time) { m_fAttackAnimCoolTime = _time; }

    bool        GetWActive() { return m_bWActive; }
    void        SetWActive(bool _active) { m_bWActive = _active; }
};

