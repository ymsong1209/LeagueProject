#pragma once
#include "CComponent.h"
#include "CFrustum.h"

class CGameObject;
class CLevel;
struct ColliderStruct;

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

    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D;	    // Deferred
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F;	    // Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj;		    // Single Object

   
    vector<CGameObject*>    m_vecDecal;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;

    vector<CGameObject*>    m_vecDynamicShadow;     // 동적 그림자 물체
   

    vector<CGameObject*>    m_vecContour;

    float                   m_LayMinDistance;   // 오브젝트가 여러개 겹쳐있을때 마우스 클릭하는 것을 대비해서 오브젝트들중에 깊이가(길이) 가장 작은
                                                //오브젝트의 길이값을 기억해두고 그 오브젝트를 최종 선택오브젝트로 세팅 

    bool                    m_bViewGizmoBounding; //기즈모 클릭범위(바운딩콜리전) 를 보여줘야하는경우 true, 안보여줘도 되는경우 false

    int        m_isGizmoEditMode; // 0: 디폴트 모드 (기즈모x) 1: 에디트 모드 (기즈모o)  : 모드가 추가될수도 있으므로, bool대신 int로함

    float       m_fFov; //fov값

   


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

    void SetIsGizmoEditMode(int _Mode) { m_isGizmoEditMode = _Mode; }
    int GetIsGizmoEditMode() { return m_isGizmoEditMode; }

    bool OutlineCheck(CGameObject* _Obj);

    float GetCameraFov() { return m_fFov; }
    void SetCameraFov(float _Fov) { m_fFov = _Fov; }

public:
    void SortObject();
    void SortShadowObject();
    void render();
    void render_depthmap();

public:
    virtual void begin() override;
    virtual void finaltick() override;

protected:
    // 마우스 방향으로 광선 연산
    void CalRay();  
    // Rect 충돌과 Cube충돌 진행
    void CollideRay(); 
    //전장의 안개용; Ray를 쏘는 Object랑 현재 Object 사이에 box있는지 판별
    bool CheckRayCollideBox(CGameObject* _CurObject); 


public:
    IntersectResult IsCollidingBtwRayRect(tRay& _ray, CGameObject* _Object);
    IntersectResult IsCollidingBtwRayCube(tRay& _ray, CGameObject* _Object);
    IntersectResult IntersectsLay(Vec3* _vertices, tRay _ray);

    IntersectResult IntersecrRayFog(Vec3 _Vertices0, Vec3 _Vertices1, Vec3 _Vertices2, tRay _Ray);


    bool IsCollidingBtwRayWall(Vec2& RayObjPos, Vec2& _CollideObjPos, float& _Raidus, float& _RayObjRadius, ColliderStruct& _ColliderData);

private:
    void clear();
    void render_deferred();

    void render_Outline();
    void render_DefaultContourPaint();
    void render_ContourPaint();
    void render_contour();

    void render_decal();

    void render_merge();

    //아직 함수 미완성
    void render_forward();
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

