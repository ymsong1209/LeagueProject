#pragma once
#include "CComponent.h"
#include "CFrustum.h"


class CLevel;

class CCamera :
    public CComponent
{
private:
    CFrustum                m_Frustum;

    float                   m_fWidth;       // 투영 가로길이
    float                   m_fAspectRatio;
    float                   m_fScale;       // Orthograpic 에서 사용하는 카메라 배율
    float                   m_fFar;

    PROJ_TYPE               m_ProjType;

    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    UINT                    m_iLayerMask;

    int                     m_iCamIdx;  // 카메라 우선순위

    bool                    m_bShowFrustumDebug;

    tRay                    m_ray;      // 마우스 방향을 향하는 직선

    vector<CGameObject*>    m_vecDeferred;
    vector<CGameObject*>    m_vecDecal;
    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecTransparent;    
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;

    vector<CGameObject*>    m_vecDynamicShadow;     // 동적 그림자 물체

    float                   m_LayMinDistance;  // 오브젝트가 여러개 겹쳐있을때 마우스 클릭하는 것을 대비해서 오브젝트들중에 깊이가(길이) 가장 작은
    //오브젝트의 길이값을 기억해두고 그 오브젝트를 최종 선택오브젝트로 세팅

    bool                    m_bViewGizmoBounding; //기즈모 클릭범위(바운딩콜리전) 를 보여줘야하는경우 true, 안보여줘도 되는경우 false


public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetScale(float _fScale) { m_fScale = _fScale; }
    float GetScale() { return m_fScale; }

    void SetFar(float _Far) { m_fFar = _Far; }
    float GetFar() { return m_fFar; }

    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetAspectRatio(float _fRatio) { m_fAspectRatio = _fRatio; }

    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll(bool _Visible);
    UINT GetLayerMask() { return m_iLayerMask; }

    void SetCameraIndex(int _idx);
    int  GetCameraIndex() { return m_iCamIdx; }

    void SetShowDebug(bool _debug) { m_bShowFrustumDebug = _debug; }
    bool GetShowDebug() { return m_bShowFrustumDebug; }

    const tRay& GetRay() { return m_ray; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetViewMatInv() { return m_matViewInv; }
    const Matrix& GetProjMat() { return m_matProj; }
    const Matrix& GetProjMatInv() { return m_matProjInv; }

    void SetViewGizmoBounding(bool _IsView) { m_bViewGizmoBounding = _IsView; }
    //기즈모 바운딩 범위를 보여줄것인가?
    bool GetViewGizmoBounding() { return m_bViewGizmoBounding; }
    void GizmoClickCheck(CGameObject* _CheckTargetObj, CLevel* _CurLevel);

    bool RayIntersectsSphere(Vec3 _SphereTrans, float _SphereRadius);

public:
    void SortObject();
    void SortShadowObject();
    void render();
    void render_depthmap();

public:
    virtual void begin() override;
    virtual void finaltick() override;

protected:
    void CalRay();  // 마우스 방향으로 광선 연산


private:
    void clear();
    void render_deferred();
    void render_decal();

    void render_merge();

    void render_opaque();
    void render_mask();
    void render_transparent();
    void render_postprocess();
    void render_ui();

    void CalcViewMat();
    void CalcProjMat();


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CCamera);
public:    
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();
};

