#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CHeightMapShader.h"

class CLandScape :
    public CRenderComponent
{
private:
    UINT                    m_iFaceX;
    UINT                    m_iFaceZ;

    Ptr<CTexture>           m_HeightMap;

    Vec2                    m_vBrushScale;      // 브러쉬 크기(전체 지형대비 크기 비율값)
    Ptr<CTexture>           m_pBrushTex;        // 브러쉬용 텍스쳐

    Ptr<CHeightMapShader>   m_pCSHeightMap;     // 높이맵 쉐이더
    Ptr<CTexture>           m_pHeightMap;       // 높이맵 텍스쳐

public:
    void SetFace(UINT _iFaceX, UINT _iFaceZ);
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap; }

    virtual void finaltick() override;
    virtual void render() override;

private:
    void init();
    void CreateMesh();
    void CreateComputeShader();
    void CreateTexture();

    CLONE(CLandScape);
public:
    CLandScape();
    ~CLandScape();
};

