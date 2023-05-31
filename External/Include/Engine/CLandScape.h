#pragma once
#include "CRenderComponent.h"
class CLandScape :
    public CRenderComponent
{
private:
    UINT    m_iFaceX;
    UINT    m_iFaceZ;

public:
    void SetFace(UINT _iFaceX, UINT _iFaceZ);


    virtual void finaltick() override;
    virtual void render() override;

    CLONE(CLandScape);
public:
    CLandScape();
    ~CLandScape();
};

