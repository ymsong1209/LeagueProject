#pragma once
#include <Engine\CScript.h>

class CMinionHPRatioScript :
    public CScript
{
private:
    float m_fRatio;
    float m_fCurHP;
    float m_fTotalHP;
    int MyNum;
    Vec2 OffsetBarPos;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void SetMyNum(int _Num) { _Num = MyNum; }
public:
    CLONE(CMinionHPRatioScript);

public:
    CMinionHPRatioScript();
    ~CMinionHPRatioScript();
};

