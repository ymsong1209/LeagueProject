#pragma once
#include "CComponent.h"
#include "CFrustum.h"


class CLevel;

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

    vector<CGameObject*>    m_vecDeferred;
    vector<CGameObject*>    m_vecDecal;
    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecTransparent;    
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;

    vector<CGameObject*>    m_vecDynamicShadow;     // ���� �׸��� ��ü

    float                   m_LayMinDistance;  // ������Ʈ�� ������ ���������� ���콺 Ŭ���ϴ� ���� ����ؼ� ������Ʈ���߿� ���̰�(����) ���� ����
    //������Ʈ�� ���̰��� ����صΰ� �� ������Ʈ�� ���� ���ÿ�����Ʈ�� ����

    bool                    m_bViewGizmoBounding; //����� Ŭ������(�ٿ���ݸ���) �� ��������ϴ°�� true, �Ⱥ����൵ �Ǵ°�� false


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

public:
    void SortObject();
    void SortShadowObject();
    void render();
    void render_depthmap();

public:
    virtual void begin() override;
    virtual void finaltick() override;

protected:
    void CalRay();  // ���콺 �������� ���� ����


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

