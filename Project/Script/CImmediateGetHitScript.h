#pragma once
#include <Engine/CScript.h>
class CImmediateGetHitScript :
    public CScript
{
private:
    float    m_fTriggerTime;
    float    m_fCurTime;

public:
    CImmediateGetHitScript();
    ~CImmediateGetHitScript();

public:
    void SetTriggerTime(float _TriggerTime) { m_fTriggerTime = _TriggerTime; }

public:
    CLONE(CImmediateGetHitScript)

    virtual void begin() override;
    virtual void tick() override;

public:

};

 