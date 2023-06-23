#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CTexture.h"

class CCamera;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    CMRT*                       m_MRT[(UINT)MRT_TYPE::END];

    vector<CCamera*>            m_vecCam;
    CCamera*                    m_pEditorCam;

    vector<tDebugShapeInfo>     m_vecShapeInfo;

    vector<CLight2D*>           m_vecLight2D;
    CStructuredBuffer*          m_Light2DBuffer;

    vector<CLight3D*>           m_vecLight3D;    
    CStructuredBuffer*          m_Light3DBuffer;


    void (CRenderMgr::* RENDER_FUNC)(void);

    Ptr<CTexture>               m_RTCopyTex;

public:
    void init();
    void render();

private:
    void render_clear();
    void render_dynamic_shadowdepth();

public:
    CMRT* GetMRT(MRT_TYPE _type) { return m_MRT[(UINT)_type]; }

    int RegisterCamera(CCamera* _Cam, int _idx);
    void RegisterEditorCamera(CCamera* _Cam) { m_pEditorCam = _Cam; }
    void SetRenderFunc(bool _IsPlay);

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
    UINT RegisterLight3D(CLight3D* _Light3D) { m_vecLight3D.push_back(_Light3D); return (UINT)m_vecLight3D.size() - 1; }

    void ClearCamera() { m_vecCam.clear(); }

    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecShapeInfo.push_back(_info); }
    vector<tDebugShapeInfo>& GetDebugShapeInfo() { return m_vecShapeInfo; }

    CCamera* GetMainCam();      // 상태에 따라 MainCam을 가져온다
    CCamera* GetPlayMainCam();  // PLAY 상태의 MainCam을 가져온다


    const vector<CLight3D*> GetLight3D() { return m_vecLight3D; }

    void CopyRenderTarget();

private:
    void MRT_Clear();
    void UpdateData();
    void render_play();
    void render_editor();
    void Clear();


};

