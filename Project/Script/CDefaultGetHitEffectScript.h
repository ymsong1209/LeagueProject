#pragma once
#include <Engine\CScript.h>

class CDefaultGetHitEffectScript :
    public CScript
{
public:
    CDefaultGetHitEffectScript();
    ~CDefaultGetHitEffectScript();

private:
    //float   m_fDeleteTime;
    float   m_fDeleteScale;
    float   m_fExpandSpeed;
    bool    m_bUseBillBoard;

    bool    m_bIsmaxScale;
public:
    //void SetDeleteTime(float _Time) { m_fDeleteTime = _Time; }
    void SetDeleteScale(float _Scale) { m_fDeleteScale = _Scale; }
    void SetExpandSpeed(float _Speed) { m_fExpandSpeed = _Speed; }
    void SetUseBillBoard(bool _State) { m_bUseBillBoard = _State; }

    CLONE(CDefaultGetHitEffectScript)


    virtual void begin() override;
    virtual void tick() override;

};

