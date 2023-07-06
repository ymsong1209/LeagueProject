#pragma once
#include "CEntity.h"


#include "CTexture.h"
#include "ptr.h"

class CMRT :
    public CEntity
{
private:
    Ptr<CTexture>   m_RT[8];
    Vec4            m_Clear[8];

    UINT            m_RTCount;

    Ptr<CTexture>   m_DSTex;

    D3D11_VIEWPORT  m_tViewPort;

public:
    void Create(Ptr<CTexture>(&_RTArr)[8], Vec4(&_Clear)[8], Ptr<CTexture> _DSTex);
    const D3D11_VIEWPORT& GetViewPort() { return m_tViewPort; }
    void OMSet();
    void Clear();

    CLONE_DISABLE(CMRT);
public:
    CMRT();
    ~CMRT();
};
                                                                                                                                                                                                                                                                                    //assortrock
                   