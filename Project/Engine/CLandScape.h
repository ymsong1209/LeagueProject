#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"

class CLandScape :
    public CRenderComponent
{
private:
    UINT                    m_iFaceX;
    UINT                    m_iFaceZ;

    Vec2                    m_vBrushScale;      // �귯�� ũ��(��ü ������� ũ�� ������)
    Ptr<CTexture>           m_pBrushTex;        // �귯���� �ؽ���

    Ptr<CRaycastShader>     m_pCSRaycast;       // ��ŷ ���̴�
    CStructuredBuffer*      m_pCrossBuffer;	    // ���콺 ��ŷ�Ǵ� ���� ���� �޴� ����

    Ptr<CHeightMapShader>   m_pCSHeightMap;     // ���̸� ���̴�
    Ptr<CTexture>           m_pHeightMap;       // ���̸� �ؽ���

    Ptr<CWeightMapShader>   m_pCSWeightMap;     // ����ġ ���̴�
    CStructuredBuffer*      m_pWeightMapBuffer; // ����ġ ���� ����
    UINT					m_iWeightWidth;		// ����ġ ���� ���μ��� ��� ��
    UINT					m_iWeightHeight;	// ����ġ ���� ���μ��� ��� ��
    UINT                    m_iWeightIdx;		// ���� ��ų ����ġ ����

    LANDSCAPE_MOD           m_eMod; 	        // ���� ����忡�� ���°�

    Ptr<CTexture>           m_pTileArrTex;      // Ÿ�� �迭 �ؽ���

    bool                    m_bIsClicking;
    RS_TYPE                 m_rstype;

public:

    virtual void finaltick() override;
    virtual void render() override;
    virtual void render_depthmap() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

public:
    const UINT& GetFaceX() { return m_iFaceX; }
    const UINT& GetFaceZ() { return m_iFaceZ; }
    const Vec2& GetBrushScale() { return m_vBrushScale; }
    Ptr<CTexture>           GetBrushTex() { return m_pBrushTex; }
    const UINT& GetWeightWidth() { return m_iWeightWidth; }
    const UINT& GetWeightHeight() { return m_iWeightHeight; }
    const UINT& GetWeightIdx() { return m_iWeightIdx; }
    const RS_TYPE& GetRSType() { return m_rstype; }
    Ptr<CTexture>           GetHeightMap() { return m_pHeightMap; }
    const LANDSCAPE_MOD& GetLandScapeMod() { return m_eMod; }

    void SetFace(UINT _iFaceX, UINT _iFaceZ);
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_pHeightMap = _HeightMap; }
    void SetBrushScale(float scale) { m_vBrushScale = Vec2(scale, scale); }
    void SetBrushTex(Ptr<CTexture> tex) { m_pBrushTex = tex; }
    void SetRSType(RS_TYPE rs) { m_rstype = rs; }
    void SetWeightIdx(int num) { m_iWeightIdx = (UINT)num; }
    void SetLandScapeMod(LANDSCAPE_MOD _mod);

private:
    void init();
    void CreateMesh();
    void CreateComputeShader();
    void CreateTexture();

    void Raycasting();
    void CopyFromLoadedTexture(wstring _FilePath);

    CLONE(CLandScape);
public:
    CLandScape();
    ~CLandScape();
};

