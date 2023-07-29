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

    float                   m_fWidth;       // ���� ���α���
    float                   m_fAspectRatio;
    float                   m_fScale;       // Orthograpic ���� ����ϴ� ī�޶� ����
    float                   m_fFar;

    PROJ_TYPE               m_ProjType;

    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    UINT                    m_iLayerMask;

    int                     m_iCamIdx;  // ī�޶� �켱����

    bool                    m_bShowFrustumDebug;

    tRay                    m_ray;      // ���콺 ������ ���ϴ� ����

    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D;	    // Deferred
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F;	    // Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj;		    // Single Object

   
    vector<CGameObject*>    m_vecDecal;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;

    vector<CGameObject*>    m_vecDynamicShadow;     // ���� �׸��� ��ü
   

    vector<CGameObject*>    m_vecContour;

    float                   m_LayMinDistance;   // ������Ʈ�� ������ ���������� ���콺 Ŭ���ϴ� ���� ����ؼ� ������Ʈ���߿� ���̰�(����) ���� ����
                                                //������Ʈ�� ���̰��� ����صΰ� �� ������Ʈ�� ���� ���ÿ�����Ʈ�� ���� 

    bool                    m_bViewGizmoBounding; //����� Ŭ������(�ٿ���ݸ���) �� ��������ϴ°�� true, �Ⱥ����൵ �Ǵ°�� false

    int        m_isGizmoEditMode; // 0: ����Ʈ ��� (�����x) 1: ����Ʈ ��� (�����o)  : ��尡 �߰��ɼ��� �����Ƿ�, bool��� int����

    float       m_fFov; //fov��

   


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
    //����� �ٿ�� ������ �����ٰ��ΰ�?
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
    // ���콺 �������� ���� ����
    void CalRay();  
    // Rect �浹�� Cube�浹 ����
    void CollideRay(); 
    //������ �Ȱ���; Ray�� ��� Object�� ���� Object ���̿� box�ִ��� �Ǻ�
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

    //���� �Լ� �̿ϼ�
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

