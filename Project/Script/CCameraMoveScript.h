#pragma once
#include <Engine\CScript.h>

class CSound;
class CCameraMoveScript :
    public CScript
{
private:
    float       m_fCamSpeed;
public:
    virtual void tick() override;


private:
    void Camera2DMove();
    void Camera3DMove();

    tRay CalcRay();

    void MainCam3DMove();
    void EditCam3DMove();

    CLONE(CCameraMoveScript);
public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

