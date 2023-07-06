#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{
private:
    int      m_iCameraMask;
    int      m_iCameraIdx;

public:
    virtual int render_update() override;

public:
    CameraUI();
    ~CameraUI();
};
