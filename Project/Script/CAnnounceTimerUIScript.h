#pragma once
#include <Engine\CScript.h>


class CAnnounceTimerUIScript :
    public CScript
{
private:
    float m_fCurTimer; //�� hp�� mp (ä���� �ϴ� exp ��)
public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CAnnounceTimerUIScript);
public:
    CAnnounceTimerUIScript();
    ~CAnnounceTimerUIScript();
};

