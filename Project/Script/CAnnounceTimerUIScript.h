#pragma once
#include <Engine\CScript.h>


class CAnnounceTimerUIScript :
    public CScript
{
private:
    float m_fCurTimer; //총 hp나 mp (채워야 하는 exp 량)
    float m_fAlpha; // 알파 값 초기화
    float m_fTimer; // 시간 계산을 위한 타이머 변수
    float m_fDuration; // 알파값이 0에서 1로 변하는 데 걸리는 시간 (0.8초)

public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CAnnounceTimerUIScript);
public:
    CAnnounceTimerUIScript();
    ~CAnnounceTimerUIScript();
};

