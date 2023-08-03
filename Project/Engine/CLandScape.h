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

    Vec2                    m_vBrushScale;      // 브러쉬 크기(전체 지형대비 크기 비율값)
    Ptr<CTexture>           m_pBrushTex;        // 브러쉬용 텍스쳐

    Ptr<CRaycastShader>     m_pCSRaycast;       // 픽킹 쉐이더
    CStructuredBuffer*      m_pCrossBuffer;	    // 마우스 피킹되는 지점 정보 받는 버퍼

    Ptr<CHeightMapShader>   m_pCSHeightMap;     // 높이맵 쉐이더
    Ptr<CTexture>           m_pHeightMap;       // 높이맵 텍스쳐

    Ptr<CWeightMapShader>   m_pCSWeightMap;     // 가중치 쉐이더
    CStructuredBuffer*      m_pWeightMapBuffer; // 가중치 저장 버퍼
    UINT					m_iWeightWidth;		// 가중치 버퍼 가로세로 행렬 수
    UINT					m_iWeightHeight;	// 가중치 버퍼 가로세로 행렬 수
    UINT                    m_iWeightIdx;		// 증가 시킬 가중치 부위

    LANDSCAPE_MOD           m_eMod; 	        // 지형 툴모드에서 상태값

    Ptr<CTexture>           m_pTileArrTex;      // 타일 배열 텍스쳐

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

