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

private:
    CGameObject* m_pTargetObject;


public:
    virtual void begin() override;
    virtual void tick() override;

    void SetTargetObject(CGameObject* _Object) { m_pTargetObject = _Object; }
    CGameObject* GetTargetObject() { return m_pTargetObject; }

    tRay CalcRay();


    CLONE(CInGameCameraScript);

public:
    CInGameCameraScript();
    ~CInGameCameraScript();
};



 

