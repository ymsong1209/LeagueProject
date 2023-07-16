#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "components.h"

#include "CResMgr.h"
#include "CMRT.h"

#include "CFogOfWarShader.h"


CRenderMgr::CRenderMgr()
    : m_Light2DBuffer(nullptr)
    , RENDER_FUNC(nullptr)
    , m_pEditorCam(nullptr)
    , m_WallBuffer(nullptr)
    , m_RayBuffer(nullptr)
    , m_RWBuffer(nullptr)
{
    Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
    m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
                                                    , (UINT)vResolution.x, (UINT)vResolution.y
                                                    , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
                                                    , D3D11_USAGE_DEFAULT);

    CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl")->SetTexParam(TEX_0, m_RTCopyTex);

    CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl")->SetTexParam(TEX_0, m_RTCopyTex);

    m_WallBuffer = new CStructuredBuffer;
    m_RayBuffer = new CStructuredBuffer;
    m_FogOfWarShader = (CFogOfWarShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"FogOfWarShader").Get();
}

CRenderMgr::~CRenderMgr()
{
    if (nullptr != m_Light2DBuffer)
        delete m_Light2DBuffer;

    if (nullptr != m_Light3DBuffer)
        delete m_Light3DBuffer;

    if (nullptr != m_WallBuffer)
        delete m_WallBuffer;

    if (nullptr != m_RayBuffer)
        delete m_RayBuffer;

    if (nullptr != m_RWBuffer)
        delete m_RWBuffer;

    Safe_Del_Array(m_MRT);
}

void CRenderMgr::render_clear()
{
    MRT_Clear();

    // Texture Register Clear
    for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
    {
        CTexture::Clear(i);
    }    
}


void CRenderMgr::render()
{
    // ������ ����
    render_clear();

    // ���� �� ���� ������ ������Ʈ �� ���ε�
    UpdateData();


    // ����� ���õ� ���� ����
    m_bGizmoObjectChanged = false;

    // ���� �Լ� ȣ��
    (this->*RENDER_FUNC)();
    
    // ���� ����
    Clear();
}




void CRenderMgr::render_play()
{    
    // Directional ���� �������� Shadow������ ���� DepthMap ����
    render_dynamic_shadowdepth();
    // ComputeShader�� Wall, Ray ���� ����, ����
    CalcRayForFog();

    // ī�޶� ���� ������
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        if (nullptr == m_vecCam[i])
            continue;

        m_vecCam[i]->SortObject();
        m_vecCam[i]->render();
    }
}

void CRenderMgr::render_editor()
{   
    // Directional ���� �������� Shadow������ ���� DepthMap ����
    render_dynamic_shadowdepth();
    // ComputeShader�� Wall, Ray ���� ����, ����
    CalcRayForFog();
   
    m_pEditorCam->SortObject();
    m_pEditorCam->render();    

    //Camera���� frustum�� ������.
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        if (m_vecCam[i]->GetShowDebug()) {
            DrawDebugFrustum(m_vecCam[i]->Transform()->GetWorldPos()
                , Vec2(m_vecCam[i]->Transform()->GetRelativeScale().x, m_vecCam[i]->Transform()->GetRelativeScale().y)
                , Vec4(0.f, 1.f, 0.f, 1.f)
                , Vec3(m_vecCam[i]->Transform()->GetRelativeRot())
                , 0.f);
        }
    }
}

void CRenderMgr::render_dynamic_shadowdepth()
{
    m_MRT[(UINT)MRT_TYPE::SHADOW]->OMSet();

    for (size_t i = 0; i < m_vecLight3D.size(); ++i)
    {
        if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_vecLight3D[i]->GetLightInfo().LightType)
            m_vecLight3D[i]->render_depthmap();
    }
}

void CRenderMgr::CalcRayForFog()
{
    m_vecWallObject.clear();
    m_vecRayObject.clear();
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        CLayer* pLayer = pCurLevel->GetLayer(i);
        const vector<CGameObject*>& vecObj = pLayer->GetObjects();

        for (size_t j = 0; j < vecObj.size(); ++j)
        {
            if (vecObj[j]->Collider3D() && vecObj[j]->Collider3D()->IsWall()) {
                ColliderStruct collidebuff;
                collidebuff.m_ColliderFinalMat = vecObj[j]->Collider3D()->GetColliderWorldMat();
                collidebuff.m_ColliderType = (UINT)vecObj[j]->Collider3D()->GetColliderShape();

                m_vecWallObject.push_back(collidebuff);
            }

            if (vecObj[j]->Transform() && vecObj[j]->Transform()->GetIsShootingRay()) {
                RayStruct raybuff;
                raybuff.m_vRayPos = vecObj[j]->Transform()->GetWorldPos();
                raybuff.m_iRayCount = vecObj[j]->Transform()->GetRayCount();
                raybuff.m_fRayRange = vecObj[j]->Transform()->GetRayRange();
                m_vecRayObject.push_back(raybuff);
            }
        }
    }
    UINT WallSize = (UINT)m_vecWallObject.size();
    m_WallBuffer->Create(sizeof(ColliderStruct), WallSize, SB_TYPE::READ_WRITE, false, m_vecWallObject.data());
    UINT RayBuffSize = (UINT)m_vecRayObject.size();
    m_RayBuffer->Create(sizeof(RayStruct), RayBuffSize, SB_TYPE::READ_WRITE, false, m_vecRayObject.data());

    UINT RWCount = 0;
    for (size_t i = 0; i < m_vecRayObject.size(); ++i) {
        RWCount += m_vecRayObject[i].m_iRayCount;
    }

    m_RWBuffer->Create(sizeof(RWStruct), RWCount, SB_TYPE::READ_WRITE, true);

    // ������ �Ȱ� ��� ����
    m_FogOfWarShader->SetWallBuffer(m_WallBuffer);
    m_FogOfWarShader->SetRayBuffer(m_RayBuffer);
    m_FogOfWarShader->SetRWBuffer(m_RWBuffer);

    m_FogOfWarShader->Execute();


}

int CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
    //���� ����, Ȥ�� Clone�� �� Camera�� �ε����� -1��
    if (_idx < 0) return _idx;

    if (m_vecCam.size() <= _idx)
    {
        m_vecCam.resize(_idx + 1);
    }

    m_vecCam[_idx] = _Cam;    
    return _idx;
}

void CRenderMgr::SetRenderFunc(bool _IsPlay)
{
    if(_IsPlay)
        RENDER_FUNC = &CRenderMgr::render_play;
    else
        RENDER_FUNC = &CRenderMgr::render_editor;
}

CCamera* CRenderMgr::GetMainCam()
{
    if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
    {
        if (m_vecCam.empty())
            return nullptr;

        return m_vecCam[0];
    }
    else
    {
        return m_pEditorCam;
    }
}

CCamera* CRenderMgr::GetPlayMainCam()
{
    if (m_vecCam.empty())
        return nullptr;

    return m_vecCam[0];
}


void CRenderMgr::CopyRenderTarget()
{
    Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::MRT_Clear()
{
    for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
    {
        m_MRT[i]->Clear();
    } 
}

void CRenderMgr::UpdateData()
{
    // GlobalData �� ���� �������� ����
    GlobalData.Light2DCount = (UINT)m_vecLight2D.size();
    GlobalData.Light3DCount = (UINT)m_vecLight3D.size();

    // ����ȭ������ ũ�Ⱑ ���ڶ�� �� ũ�� ���� �����.
    if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
    {
        m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight2D.size(), SB_TYPE::READ_ONLY, true);
    }

    // ����ȭ���۷� ���� �����͸� �ű��.
    static vector<tLightInfo> vecLight2DInfo;
    vecLight2DInfo.clear();

    for (size_t i = 0; i < m_vecLight2D.size(); ++i)
    {
        vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
    }

    m_Light2DBuffer->SetData(vecLight2DInfo.data(), sizeof(tLightInfo) * (UINT)vecLight2DInfo.size());
    m_Light2DBuffer->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);


    // ����ȭ������ ũ�Ⱑ ���ڶ�� �� ũ�� ���� �����.
    if (m_Light3DBuffer->GetElementCount() < m_vecLight3D.size())
    {
        m_Light3DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight3D.size(), SB_TYPE::READ_ONLY, true);
    }

    // ����ȭ���۷� ���� �����͸� �ű��.
    static vector<tLightInfo> vecLight3DInfo;
    vecLight3DInfo.clear();

    for (size_t i = 0; i < m_vecLight3D.size(); ++i)
    {
        vecLight3DInfo.push_back(m_vecLight3D[i]->GetLightInfo());
    }

    m_Light3DBuffer->SetData(vecLight3DInfo.data(), sizeof(tLightInfo) * (UINT)vecLight3DInfo.size());
    m_Light3DBuffer->UpdateData(13, PIPELINE_STAGE::PS_PIXEL);



    // ���� ��� ������ ���ε�
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
    pGlobalBuffer->SetData(&GlobalData, sizeof(tGlobal));
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();
}


void CRenderMgr::Clear()
{
    m_vecLight2D.clear();
    m_vecLight3D.clear();
}
