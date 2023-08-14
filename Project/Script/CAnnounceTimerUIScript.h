#pragma once
#include <Engine\CScript.h>


class CAnnounceTimerUIScript :
    public CScript
{
private:
    float m_fCurTimer; //총 hp나 mp (채워야 하는 exp 량)
public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CAnnounceTimerUIScript);
public:
    CAnnounceTimerUIScript();
    ~CAnnounceTimerUIScript();
};

