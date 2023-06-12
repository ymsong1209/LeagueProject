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

    Vec2                    m_vBrushScale;      // �귯�� ũ��(��ü ������� ũ�� ������)
    Ptr<CTexture>           m_pBrushTex;        // �귯���� �ؽ���

    Ptr<CHeightMapShader>   m_pCSHeightMap;     // ���̸� ���̴�
    Ptr<CTexture>           m_pHeightMap;       // ���̸� �ؽ���

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

