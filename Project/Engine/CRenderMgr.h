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


//-----------������ �Ȱ��� �������� ����ü---------------------//
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
    Vec3        m_vCrossPos;        //Ray�� �簢���� �ε��� ����.
    float       m_vCrossRadius;     //Ray�� ������ ������ ��ǥ, ������ �ȵǸ� �ִ� �������� ����
    Vec3        m_vCenterPos;       //Ray�� �߾� ����
    int         m_iRayCount;        //���° Ray����

    float       m_fMaxRadius;       //Ray�� ���� ����
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

    CGameObject*                m_GizMoTargetObject;  //����� ���ܾ��� Ÿ�� ������Ʈ
    bool                        m_bGizmoObjectChanged;

    Ptr<CTexture>               m_RTCopyTex;

    // ������ �Ȱ�
    Ptr<CFogOfWarShader>        m_FogOfWarShader;   //������ �Ȱ� ComputeShader
    vector<ColliderStruct>      m_vecWallObject;    //Ray�� �浹�� ���ɼ��� �ִ� �� ������Ʈ
    vector<RayStruct>           m_vecRayObject;     //Ray�� ��� object player, �͵� ��
    CStructuredBuffer*          m_WallBuffer;
    CStructuredBuffer*          m_RayBuffer;
    CStructuredBuffer*          m_RWBuffer;         // ComputeShader ��� �� �޾ƿ� ���� 
    int                         m_iRayCount;         // ��ü �ϳ��� �� Ray����
    bool                        m_bIsQClicked;
   
    // ������ �Ȱ� ����
    Ptr<CFogFilterShader>       m_FogFilterShader;  // ������ �Ȱ� ���͸� ComputeShader
    
    //CStructuredBuffer*          m_FogFilterMapBuffer; // ComputeShader ��� �� �޾ƿ� ���� 
    Ptr<CTexture>               m_FogFilterMap; // ��� �Ȱ� ���͸� �ؽ�ó

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

    CCamera* GetMainCam();      // ���¿� ���� MainCam�� �����´�
    CCamera* GetPlayMainCam();  // PLAY ������ MainCam�� �����´�
    CCamera* GetEditorCam() { return m_pEditorCam; }


    const vector<CLight3D*> GetLight3D() { return m_vecLight3D; }


public:
    CGameObject* GetGizMoTargetObj() { return m_GizMoTargetObject; }   //����� ���ܾ��� Ÿ�ٿ�����Ʈ ����, ���� �Լ�
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

