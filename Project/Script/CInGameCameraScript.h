#pragma once
#include <Engine\CScript.h>

class CInGameCameraScript :
    public CScript
{
private:
    float       m_fCamSpeed;
    UINT        m_iLeftXLimit;
    UINT        m_iRightXLimit;
    UINT        m_iUpYLimit;
    UINT        m_iDownYLimit;

public:
    virtual void tick() override;


    CLONE(CInGameCameraScript);

public:
    CInGameCameraScript();
    ~CInGameCameraScript();
};



 

