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
#include "CFogFilterShader.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"


CRenderMgr::CRenderMgr()
    : m_Light2DBuffer(nullptr)
    , RENDER_FUNC(nullptr)
    , m_pEditorCam(nullptr)
    , m_WallBuffer(nullptr)
    , m_RayBuffer(nullptr)
    , m_RWBuffer(nullptr)
    , m_iRayCount(1440)
    , m_bIsQClicked(false)
    , m_iMaxRWSize(0)
    , m_FogFilterTime(0.f)
    , b_IsImGuiHovered(false)
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
    m_RWBuffer = new CStructuredBuffer;
    m_FogOfWarShader = (CFogOfWarShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"FogOfWarShader").Get();
    m_FogFilterShader = (CFogFilterShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"FogFilterShader").Get();
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
    // 렌더링 시작
    render_clear();

    // 광원 및 전역 데이터 업데이트 및 바인딩
    UpdateData();

    // 기즈모 선택된 상태 정리
    m_bGizmoObjectChanged = false;

    // 렌더 함수 호출
    (this->*RENDER_FUNC)();
    
    // 광원 해제
    Clear();
}




void CRenderMgr::render_play()
{    
    // Directional 광원 시점에서 Shadow맵핑을 위한 DepthMap 생성
    render_dynamic_shadowdepth();
    // ComputeShader에  Wall, Ray 정보 전달, 연산
    CalcRayForFog();

    // 카메라 기준 렌더링
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
    // Directional 광원 시점에서 Shadow맵핑을 위한 DepthMap 생성
    render_dynamic_shadowdepth();
    // ComputeShader에 Wall, Ray 정보 전달, 연산
    CalcRayForFog();
   
    m_pEditorCam->SortObject();
    m_pEditorCam->render();    

    // Camera별로 frustum을 보여줌.
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
    m_vecRayStruct.clear();

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
                raybuff.m_iRayCount = m_iRayCount;
                raybuff.m_fRayRange = vecObj[j]->Transform()->GetRayRange();
                m_vecRayObject.push_back(vecObj[j]);
                m_vecRayStruct.push_back(raybuff);
            }
        }
    }
    UINT WallSize = (UINT)m_vecWallObject.size();
    m_WallBuffer->Create(sizeof(ColliderStruct), WallSize, SB_TYPE::READ_ONLY, false, m_vecWallObject.data());

    UINT RayBuffSize = (UINT)m_vecRayStruct.size();
    m_RayBuffer->Create(sizeof(RayStruct), RayBuffSize, SB_TYPE::READ_ONLY, false, m_vecRayStruct.data());

    UINT RWCount = 0;
    for (size_t i = 0; i < m_vecRayStruct.size(); ++i) {
        RWCount += m_iRayCount;
    }

    if (RWCount > m_iMaxRWSize) {
        m_RWBuffer->Create(sizeof(RWStruct), RWCount, SB_TYPE::READ_WRITE, true);
        m_iMaxRWSize = RWCount;
    }
  

   


    if (m_FogFilterTime < 0.01f) {
        m_FogFilterTime += EditorDT;
    }
    else {

        // ������ �Ȱ� ray/wall �浹 ����
        m_FogOfWarShader->SetSourceLightCount((int)RayBuffSize);
        m_FogOfWarShader->SetSourceLightPerRay(m_iRayCount);
        m_FogOfWarShader->SetColliderVecCount((int)WallSize);
        m_FogOfWarShader->SetWallBuffer(m_WallBuffer);
        m_FogOfWarShader->SetRayBuffer(m_RayBuffer);
        m_FogOfWarShader->SetOutputBuffer(m_RWBuffer);

        m_FogOfWarShader->UpdateData();
        m_FogOfWarShader->Execute();


        // 전장의 안개 필터 제작 컴퓨트 쉐이더 -> 추후 0.1초에 한번하도록 변경
        int m_iWidth = 1024;
        int m_itHeight = 1024; // 구조화버퍼 생성 사이즈도 init에서 1024로 해줌

        m_FogFilterShader->SetCalcedFogInfo(m_RWBuffer);
        // m_FogFilterShader->SetFogFilterMap(m_FogFilterMapBuffer, m_iWidth, m_itHeight);
        m_FogFilterShader->SetFogFilterMap(m_FogFilterMap);
        m_FogFilterShader->SetCountObject((int)m_vecRayObject.size()); // 시야 오브젝트의 개수
        m_FogFilterShader->SetCountRayPerObj(m_iRayCount); // 오브젝트가 가지는 레이 개수
        m_FogFilterShader->UpdateData();
        m_FogFilterShader->Execute();
        m_FogFilterTime = 0.f;
    }
  

    // 전장의 안개 테스트용 코드 (왜인지 터짐 흠...)
    /*
    if (CKeyMgr::GetInst()->GetKeyState(KEY::Q) == KEY_STATE::TAP)
    {
        m_bIsQClicked = true;
        UINT bufferSize = m_RWBuffer->GetBufferSize();
        size_t size = sizeof(RWStruct);
        RWStruct* data = new RWStruct[bufferSize / sizeof(RWStruct)];
        m_RWBuffer->GetData((void*)data);

        if (m_bIsQClicked == true)
        {
            CGameObject* TestObj;

            for (int i = 0; i < m_iRayCount * m_vecRayObject.size(); ++i)
            {
                TestObj = new CGameObject;
                TestObj->SetName(L"HI!");
                TestObj->AddComponent(new CMeshRender);
                TestObj->AddComponent(new CTransform);
                TestObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));
                TestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
                TestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"), 0);


                SpawnGameObject(TestObj, data[i].m_vCrossPos, 0);


            }

            m_bIsQClicked = false;
        }

        delete[] data;
    }
    */
      


    // 전장의 안개 디버깅용 코드. 나중에 지워도 됨.

    /*auto a = m_RWBuffer[0];

    UINT bufferSize = m_RWBuffer->GetBufferSize();
    size_t size = sizeof(RWStruct);
    RWStruct* data = new RWStruct[bufferSize / sizeof(RWStruct)];
    m_RWBuffer->GetData((void*)data);

    if (data && bufferSize > 30) {
        RWStruct test = data[0];
        RWStruct test2 = data[1];
        RWStruct test3 = data[2];
        RWStruct test4 = data[3];
        RWStruct test5 = data[4];
        RWStruct test6 = data[5];
        RWStruct test7 = data[6];
        RWStruct test8 = data[7];
        RWStruct test9 = data[8];
        RWStruct test10 = data[9];
        RWStruct test11 = data[10];
        RWStruct test12 = data[11];
        RWStruct test13 = data[12];
        RWStruct test14 = data[13];
        RWStruct test15 = data[14];
        RWStruct test16 = data[15];
        RWStruct test17 = data[16];
        RWStruct test18 = data[17];
        RWStruct test19 = data[18];
        RWStruct test20 = data[19];

        RWStruct test48 = data[47];
        RWStruct test49 = data[48];
        RWStruct test50 = data[49];
        RWStruct test51 = data[50];
        RWStruct test52 = data[51];

       
    }
  

    delete[] data;*/
}

int CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
    // 새로 생성, 혹은 Clone을 한 Camera는 인덱스가 -1임
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

void CRenderMgr::MRT_Clear(MRT_TYPE _Type)
{
    GetMRT(_Type)->Clear();
}

void CRenderMgr::UpdateData()
{
    // GlobalData에 광원 개수정보 세팅
    GlobalData.Light2DCount = (UINT)m_vecLight2D.size();
    GlobalData.Light3DCount = (UINT)m_vecLight3D.size();

    // 구조화버퍼의 크기가 모자라면 더 크게 새로 만든다.
    if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
    {
        m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight2D.size(), SB_TYPE::READ_ONLY, true);
    }

    // 구조화버퍼로 광원 데이터를 옮긴다.
    static vector<tLightInfo> vecLight2DInfo;
    vecLight2DInfo.clear();

    for (size_t i = 0; i < m_vecLight2D.size(); ++i)
    {
        vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
    }

    m_Light2DBuffer->SetData(vecLight2DInfo.data(), sizeof(tLightInfo) * (UINT)vecLight2DInfo.size());
    m_Light2DBuffer->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);


    // 구조화버퍼의 크기가 모자라면 더 크게 새로 만든다.
    if (m_Light3DBuffer->GetElementCount() < m_vecLight3D.size())
    {
        m_Light3DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight3D.size(), SB_TYPE::READ_ONLY, true);
    }

    // 구조화버퍼로 광원 데이터를 옮긴다.
    static vector<tLightInfo> vecLight3DInfo;
    vecLight3DInfo.clear();

    for (size_t i = 0; i < m_vecLight3D.size(); ++i)
    {
        vecLight3DInfo.push_back(m_vecLight3D[i]->GetLightInfo());
    }

    m_Light3DBuffer->SetData(vecLight3DInfo.data(), sizeof(tLightInfo) * (UINT)vecLight3DInfo.size());
    m_Light3DBuffer->UpdateData(13, PIPELINE_STAGE::PS_PIXEL);


    // 전역 상수 데이터 바인딩
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
