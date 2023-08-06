#include "pch.h"
#include "CEditorObjMgr.h"

#include "CGameObjectEx.h"
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CKeyMgr.h>

#include <Engine/CCamera.h>

#include <Script\CCameraMoveScript.h>
#include <Script\CInGameCameraScript.h>


CEditorObjMgr::CEditorObjMgr()
	: m_DebugShape{}
	, m_DebugBounding(nullptr)
{

}

CEditorObjMgr::~CEditorObjMgr()
{
	Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Array(m_DebugShape);

	delete m_DebugBounding;
}


void CEditorObjMgr::init()
{
	// 디버그 쉐이프 생성
	m_DebugShape[(UINT)SHAPE_TYPE::RECT] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"),0);

	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"),0);

	m_DebugShape[(UINT)SHAPE_TYPE::CUBE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"),0);

	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"),0);

	m_DebugShape[(UINT)SHAPE_TYPE::ICECREAM] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::ICECREAM]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::ICECREAM]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::ICECREAM]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"IceCreamMesh"));
	m_DebugShape[(UINT)SHAPE_TYPE::ICECREAM]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug")); // 임시
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"),0);

	// EditorObject 생성
	CGameObjectEx* pEditorCamObj = new CGameObjectEx;
	pEditorCamObj->AddComponent(new CTransform);
	pEditorCamObj->AddComponent(new CCamera);
	pEditorCamObj->AddComponent(new CCameraMoveScript);
	//pEditorCamObj->AddComponent(new CInGameCameraScript);

	pEditorCamObj->Camera()->SetLayerMaskAll(true);
	pEditorCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pEditorCamObj->Camera()->SetFar(50000.f);
	pEditorCamObj->Transform()->SetRelativePos(Vec3(0, 100, 0));
	m_vecEditorObj.push_back(pEditorCamObj);
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCamObj->Camera());

	// Editor BoundingObject 생성
	m_DebugBounding = new CGameObjectEx;
	m_DebugBounding->AddComponent(new CTransform);
	m_DebugBounding->AddComponent(new CMeshRender);

	m_DebugBounding->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	m_DebugBounding->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugBoundingMtrl"),0);
}



void CEditorObjMgr::progress()
{
	// DebugShape 정보 가져오기
	vector<tDebugShapeInfo>& vecInfo = CRenderMgr::GetInst()->GetDebugShapeInfo();
	m_DebugShapeInfo.insert(m_DebugShapeInfo.end(), vecInfo.begin(), vecInfo.end());
	vecInfo.clear();

	// Bounding DebugShape 정보 가져오기
	vector<tDebugBoundingInfo>& vecBoundingInfo = CRenderMgr::GetInst()->GetDebugBoundingInfo();
	m_vecBoundingInfo.insert(m_vecBoundingInfo.end(), vecBoundingInfo.begin(), vecBoundingInfo.end());
	vecBoundingInfo.clear();


	tick();

	render();
}


void CEditorObjMgr::tick()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}

void CEditorObjMgr::render()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->render();
	}

	// DebugShape Render
	CGameObjectEx* pShapeObj = nullptr;

	vector<tDebugShapeInfo>::iterator iter = m_DebugShapeInfo.begin();
	for (; iter != m_DebugShapeInfo.end();)
	{
		switch (iter->eShape)
		{
		case SHAPE_TYPE::RECT:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::RECT];
			break;
		case SHAPE_TYPE::CIRCLE:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE];
			break;
		case SHAPE_TYPE::CUBE:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CUBE];
			break;
		case SHAPE_TYPE::SPHERE:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::SPHERE];
			break;
		case SHAPE_TYPE::ICECREAM:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::ICECREAM];
			break;
		case SHAPE_TYPE::FRUSTUM:
		{
			CreateFrustumDebugMesh();
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM];
			break;
		}
		}

		if (iter->matWorld != XMMatrixIdentity())
		{
			pShapeObj->Transform()->SetWorldMat(iter->matWorld);
		}
		else
		{
			pShapeObj->Transform()->SetRelativePos(iter->vWorldPos); 
			pShapeObj->Transform()->SetRelativeScale(iter->vWorldScale);
			pShapeObj->Transform()->SetRelativeRot(iter->vWorldRotation);
			pShapeObj->finaltick();
		}
		
		pShapeObj->GetRenderComponent()->GetMaterial(0)->SetScalarParam(VEC4_0, &iter->vColor);
		// EditorObj때 메인캠의 WVP가 필요해서 MAT_0에 VP를 넣어둠.
		Matrix VP = CRenderMgr::GetInst()->GetMainCam()->GetViewMat() * CRenderMgr::GetInst()->GetMainCam()->GetProjMat();
		pShapeObj->GetRenderComponent()->GetMaterial(0)->SetScalarParam(MAT_0, &VP);
		pShapeObj->render();

		iter->fCurTime += EditorDT;
		if (iter->fMaxTime <= iter->fCurTime)
		{
			iter = m_DebugShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}


	// DebugBounding render
	CGameObjectEx* pBoundingObj = nullptr;

	vector<tDebugBoundingInfo>::iterator Boundingiter = m_vecBoundingInfo.begin();
	for (; Boundingiter != m_vecBoundingInfo.end();)
	{
		pBoundingObj = m_DebugBounding;
		pBoundingObj->Transform()->SetRelativePos(Boundingiter->vWorldPos);
		pBoundingObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
		pBoundingObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
		pBoundingObj->finaltick();

		pBoundingObj->GetRenderComponent()->GetMaterial(0)->SetScalarParam(FLOAT_0, &Boundingiter->fBounding);

		pBoundingObj->render();

		Boundingiter->fCurTime += EditorDT;
		if (Boundingiter->fMaxTime <= Boundingiter->fCurTime)
		{
			Boundingiter = m_vecBoundingInfo.erase(Boundingiter);
		}
		else
		{
			++Boundingiter;
		}
	}
}



void CEditorObjMgr::CreateFrustumDebugMesh()
{
	Vtx v;
	vector<Vtx> vecVtx;

	CCamera* pMainCam = CRenderMgr::GetInst()->GetPlayMainCam();

	Vec3	arrProj[8];
	arrProj[0] = Vec3(-1.f, 1.f, 0.f);
	arrProj[1] = Vec3(1.f, 1.f, 0.f);
	arrProj[2] = Vec3(1.f, -1.f, 0.f);
	arrProj[3] = Vec3(-1.f, -1.f, 0.f);
	arrProj[4] = Vec3(-1.f, 1.f, 1.f);
	arrProj[5] = Vec3(1.f, 1.f, 1.f);
	arrProj[6] = Vec3(1.f, -1.f, 1.f);
	arrProj[7] = Vec3(-1.f, -1.f, 1.f);


	// 절두체 로컬 정점들
	Matrix matInv = pMainCam->GetProjMatInv() * pMainCam->GetViewMatInv() * pMainCam->Transform()->GetWorldMatInv();
	for (int i = 0; i < 8; ++i)
	{
		v.vPos = XMVector3TransformCoord(arrProj[i], matInv);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);
	}

	vector<UINT> vecIdx;
	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	vecIdx.push_back(3); vecIdx.push_back(0); vecIdx.push_back(4);
	vecIdx.push_back(5); vecIdx.push_back(6); vecIdx.push_back(7);
	vecIdx.push_back(4); vecIdx.push_back(7); vecIdx.push_back(3);
	vecIdx.push_back(2); vecIdx.push_back(6); vecIdx.push_back(5);
	vecIdx.push_back(1);

	Ptr<CMesh> pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());

	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->MeshRender()->SetMesh(pMesh);
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	vecVtx.clear();
	vecIdx.clear();
}