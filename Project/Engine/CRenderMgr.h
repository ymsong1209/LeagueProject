#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CTexture.h"

class CCamera;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;
class CFogOfWarShader;
class CFogFilterShader;


//-----------전장의 안개에 전달해줄 구조체---------------------//
struct ColliderStruct{
    Matrix      m_ColliderFinalMat;
    int         m_ColliderType;     //0 : Sphere, 1 : Cube;
    int         padding[3];
};

struct RayStruct {
    Vec3        m_vRayPos;
    int         m_iRayCount;
    float       m_fRayRange;
    int         padding[3];
};

struct RWStruct {
    Vec3        m_vCrossPos;        //Ray랑 사각형이 부딪힌 지점.
    float       m_vCrossRadius;     //Ray랑 교차된 지점의 좌표, 교차가 안되면 최대 반지름이 들어옴
    Vec3        m_vCenterPos;       //Ray의 중앙 지점
    int         m_iRayCount;        //몇번째 Ray인지

    float       m_fMaxRadius;       //Ray의 원본 범위
    float       padding[3];
};
//------------------------------------------------------------//


class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    CMRT*                       m_MRT[(UINT)MRT_TYPE::END];

    vector<CCamera*>            m_vecCam;
    CCamera*                    m_pEditorCam;

    vector<tDebugShapeInfo>     m_vecShapeInfo;
    vector<tDebugBoundingInfo>  m_vecBoundingInfo;

    vector<CLight2D*>           m_vecLight2D;
    CStructuredBuffer*          m_Light2DBuffer;

    vector<CLight3D*>           m_vecLight3D;    
    CStructuredBuffer*          m_Light3DBuffer;

    CGameObject*                m_GizMoTargetObject;  //기즈모가 생겨야할 타겟 오브젝트
    bool                        m_bGizmoObjectChanged;

    Ptr<CTexture>               m_RTCopyTex;

    // 전장의 안개
    Ptr<CFogOfWarShader>        m_FogOfWarShader;   //전장의 안개 ComputeShader
    vector<ColliderStruct>      m_vecWallObject;    //Ray에 충돌될 가능성이 있는 벽 오브젝트
    vector<RayStruct>           m_vecRayObject;     //Ray를 쏘는 object player, 와드 등
    CStructuredBuffer*          m_WallBuffer;
    CStructuredBuffer*          m_RayBuffer;
    CStructuredBuffer*          m_RWBuffer;         // ComputeShader 계산 후 받아올 버퍼 
    int                         m_iRayCount;         // 물체 하나가 쏠 Ray개수
    bool                        m_bIsQClicked;
   
    // 전장의 안개 필터
    Ptr<CFogFilterShader>       m_FogFilterShader;  // 전장의 안개 필터맵 ComputeShader
    
    //CStructuredBuffer*          m_FogFilterMapBuffer; // ComputeShader 계산 후 받아올 버퍼 
    Ptr<CTexture>               m_FogFilterMap; // 결과 안개 필터맵 텍스처

    int                         m_iMaxRWSize;
    float                       m_FogFilterTime;


    void (CRenderMgr::* RENDER_FUNC)(void);

public:
    void init();
    void render();

private:
    void render_clear();
    void render_dynamic_shadowdepth();
    void CalcRayForFog();

    void SetRayCount(int _ray) { m_iRayCount = _ray; }
    int  GetRayCount() { return m_iRayCount; }

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

    void AddDebugBoundingInfo(const tDebugBoundingInfo& _info) { m_vecBoundingInfo.push_back(_info); }
    vector<tDebugBoundingInfo>& GetDebugBoundingInfo() { return m_vecBoundingInfo; }

    CCamera* GetMainCam();      // 상태에 따라 MainCam을 가져온다
    CCamera* GetPlayMainCam();  // PLAY 상태의 MainCam을 가져온다
    CCamera* GetEditorCam() { return m_pEditorCam; }


    const vector<CLight3D*> GetLight3D() { return m_vecLight3D; }


public:
    CGameObject* GetGizMoTargetObj() { return m_GizMoTargetObject; }   //기즈모가 생겨야할 타겟오브젝트 게터, 세터 함수
    void SetGizMoTargetObj(CGameObject* _Object) { m_GizMoTargetObject = _Object; }

    void SetGizmoObjectChanged(bool _State) { m_bGizmoObjectChanged = _State; }
    bool GetGizmoObjectChanged() { return m_bGizmoObjectChanged; }

    void CopyRenderTarget();

private:
    void MRT_Clear();
    void UpdateData();
    void render_play();
    void render_editor();
    void Clear();


};

