#pragma once
#include "CChampionScript.h"

class CMalphiteScript :
    public CChampionScript
{
public:
    CMalphiteScript();
    virtual ~CMalphiteScript();

private:
    float        m_fAttackAnimCoolTime; //������Ʈ 1Ÿ ���� 2Ÿ�� ĥ�ǵ�, 1Ÿ�� ġ�� �����ð��� ������ 2Ÿ ��� 1Ÿ�� ���ƿ�

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

