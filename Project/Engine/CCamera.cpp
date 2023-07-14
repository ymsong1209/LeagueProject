#include "pch.h"
#include "CCamera.h"


#include "CDevice.h"
#include "CRenderMgr.h"
#include "CTransform.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"
#include "CMaterial.h"
#include "CGraphicsShader.h"

#include "CMRT.h"
#include "CLight3D.h"
 
#include "CResMgr.h"
#include "CKeyMgr.h"

#include "CCollider2D.h"
#include "CCollider3D.h"





CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Frustum(this)
	, m_fWidth(0.f)
	, m_fAspectRatio(1.f)
	, m_fScale(1.f)
	, m_fFar(50000.f)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_iLayerMask(0)
	, m_iCamIdx(-1)
	, m_bShowFrustumDebug(false)
{
	SetName(L"Camera");

	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_fWidth = vRenderResol.x;
	m_fAspectRatio = vRenderResol.x / vRenderResol.y;
}

CCamera::CCamera(const CCamera& _Other)
	: CComponent(_Other)
	, m_Frustum(this)
	, m_fWidth(_Other.m_fWidth)
	, m_fAspectRatio(_Other.m_fAspectRatio)
	, m_fScale(_Other.m_fScale)
	, m_fFar(50000.f)
	, m_ProjType(_Other.m_ProjType)
	, m_iLayerMask(_Other.m_iLayerMask)
	, m_iCamIdx(-1)
	, m_bShowFrustumDebug(_Other.m_bShowFrustumDebug)
{
}

CCamera::~CCamera()
{	
}

void CCamera::begin()
{
	if (-1 != m_iCamIdx)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_iCamIdx);
	}
}

void CCamera::finaltick()
{
	CalcViewMat();

	CalcProjMat();

	m_Frustum.finaltick();

	// 마우스방향 직선 계산
	CalRay();

	CollideRay();

}

void CCamera::CalRay()
{
	// 마우스 방향을 향하는 Ray 구하기
	// SwapChain 타겟의 ViewPort 정보
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

	//  현재 마우스 좌표
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// 직선은 카메라의 좌표를 반드시 지난다.
	m_ray.vStart = Transform()->GetWorldPos();

	// view space 에서의 방향
	m_ray.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
	m_ray.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
	m_ray.vDir.z = 1.f;

	// world space 에서의 방향
	m_ray.vDir = XMVector3TransformNormal(m_ray.vDir, m_matViewInv);
	m_ray.vDir.Normalize();
}

void CCamera::CollideRay()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();

	CGameObject* FinalRectObject = nullptr;
	CGameObject* FinalCubeObject = nullptr;

	float fRectResult = FBXSDK_FLOAT_MAX;
	float fCubeResult = FBXSDK_FLOAT_MAX;

	vector<CGameObject*> TempEndOverlapRect;
	vector<CGameObject*> TempEndOverlapCube;


	for (int i = 0; i < MAX_LAYER; ++i)
	{
		vector<CGameObject*> Objects = CurLevel->GetLayer(i)->GetObjects();


		for (int j = 0; j < Objects.size(); ++j)
		{
			IntersectResult result = IsCollidingBtwRayRect(m_ray, Objects[j]);
			IntersectResult CubeResult = IsCollidingBtwRayCube(m_ray, Objects[j]);

			// 만약 충돌중이 였다면 EndOverlap 후보군에 넣어야 함 (Ray - Rect를 말하는 것임)
			if (Objects[j]->Collider2D())
			{
				if (Objects[j]->Collider2D()->IsCollidedFromRay() == true)
				{
					// 충돌중이였는데 이번프레임에서는 충돌해제가 되었음으로 End Ray Overlap을 호출해줌
					if (result.bResult == false)
					{
						Objects[j]->Collider2D()->SetCollidedFromRay(false);
						Objects[j]->Collider2D()->EndRayOverlap();
					}

					// 이경우는 계속 충돌중이지다. 하지만 가장 가까이 있는 물체가 아니라면 End Ray Overlap을 호출해야 하기에
					// 후보군에 넣어준다.
					else
					{

						TempEndOverlapRect.push_back(Objects[j]);
					}
				}

				//if (result.vCrossPoint != Vec3(0.f, 0.f, 0.f))
				if (result.bResult == true)
				{
					// 최단 거리에 있는 Object인지 확인해야 한다.
					if (result.fResult < fRectResult)
					{
						fRectResult = result.fResult;
						FinalRectObject = Objects[j];
					}
				}
			}

			else if (Objects[j]->Collider3D())
			{
				if (Objects[j]->Collider3D()->IsCollidedFromRay() == true)
				{
					// 충돌중이였는데 이번프레임에서는 충돌해제가 되었음으로 End Ray Overlap을 호출해줌
					if (CubeResult.bResult == false)
					{
						Objects[j]->Collider3D()->SetCollidedFromRay(false);
						Objects[j]->Collider3D()->EndRayOverlap();
					}

					// 이경우는 계속 충돌중이지다. 하지만 가장 가까이 있는 물체가 아니라면 End Ray Overlap을 호출해야 하기에
					// 후보군에 넣어준다.
					else
					{

						TempEndOverlapCube.push_back(Objects[j]);
					}
				}

				//if (result.vCrossPoint != Vec3(0.f, 0.f, 0.f))
				if (CubeResult.bResult == true)
				{
					// 최단 거리에 있는 Object인지 확인해야 한다.
					if (CubeResult.fResult < fCubeResult)
					{
						fCubeResult = CubeResult.fResult;
						FinalCubeObject = Objects[j];
					}
				}
			}

		}
	}

	if (FinalRectObject != nullptr)
	{
		// 여기까지 왔으면 현재 충돌중이고 가장 가까운 단하나의 Rect Collider를 지닌 Object임
		if (FinalRectObject->Collider2D()->IsCollidedFromRay())
		{
			FinalRectObject->Collider2D()->OnRayOverlap();
		}

		// Begin Ray Overlap을 해야한다.
		else
		{
			FinalRectObject->Collider2D()->SetCollidedFromRay(true);
			FinalRectObject->Collider2D()->BeginRayOverlap();
		}

		// TemObject들중에서 FinalObject가 아닌 Object들은 모두 EndRayOverlap을 호출해야한다.
		for (int i = 0; i < TempEndOverlapRect.size(); ++i)
		{
			if (TempEndOverlapRect[i] != FinalRectObject)
			{
				TempEndOverlapRect[i]->Collider2D()->SetCollidedFromRay(false);
				TempEndOverlapRect[i]->Collider2D()->EndRayOverlap();
			}

		}
	}

	if (FinalCubeObject != nullptr)
	{
		// 여기까지 왔으면 현재 충돌중이고 가장 가까운 단하나의 Cube Collider를 지닌 Object임
		if (FinalCubeObject->Collider3D()->IsCollidedFromRay())
		{
			FinalCubeObject->Collider3D()->OnRayOverlap();
		}

		// Begin Ray Overlap을 해야한다.
		else
		{
			FinalCubeObject->Collider3D()->SetCollidedFromRay(true);
			FinalCubeObject->Collider3D()->BeginRayOverlap();
		}

		// TemObject들중에서 FinalObject가 아닌 Object들은 모두 EndRayOverlap을 호출해야한다.
		for (int i = 0; i < TempEndOverlapCube.size(); ++i)
		{
			if (TempEndOverlapCube[i] != FinalCubeObject)
			{
				TempEndOverlapCube[i]->Collider3D()->SetCollidedFromRay(false);
				TempEndOverlapCube[i]->Collider3D()->EndRayOverlap();
			}

		}
	}

	TempEndOverlapRect.clear();
	TempEndOverlapCube.clear();
}

void CCamera::CalcViewMat()
{
	// ==============
	// View 행렬 계산
	// ==============
	m_matView = XMMatrixIdentity();

	// 카메라 좌표를 원점으로 이동
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 카메라가 바라보는 방향을 Z 축과 평행하게 만드는 회전 행렬을 적용
	Matrix matViewRot = XMMatrixIdentity();

	Vec3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13 = vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23 = vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33 = vF.z;

	m_matView = matViewTrans * matViewRot;
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
}

void CCamera::CalcProjMat()
{
	// =============
	// 투영 행렬 계산
	// =============
	m_matProj = XMMatrixIdentity();
	
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교 투영
		float fHeight = m_fWidth / m_fAspectRatio;
		m_matProj = XMMatrixOrthographicLH(m_fWidth * (1.f / m_fScale), fHeight * (1.f / m_fScale), 1.f, m_fFar);
	}
	else
	{	
		// 원근 투영
		m_matProj = XMMatrixPerspectiveFovLH(XM_PI / 2.f, m_fAspectRatio, 1.f, m_fFar);
	}

	// 투영행렬 역행렬
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);


}



void CCamera::SetLayerMask(int _iLayer, bool _Visible)
{
	if (_Visible)
	{
		m_iLayerMask |= 1 << _iLayer;
	}
	else
	{
		m_iLayerMask &= ~(1 << _iLayer);
	}
}

void CCamera::SetLayerMaskAll(bool _Visible)
{
	if (_Visible)
		m_iLayerMask = 0xffffffff;
	else
		m_iLayerMask = 0;
}

void CCamera::SetCameraIndex(int _idx)
{
	m_iCamIdx = _idx;
	CRenderMgr::GetInst()->RegisterCamera(this, m_iCamIdx);
}

void CCamera::SortObject()
{
	// 이전 프레임 분류정보 제거
	clear();

	// 현재 레벨 가져와서 분류
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 레이어 마스크 확인
		if (m_iLayerMask & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			m_LayMinDistance = GetFar();//기즈모 테스트 :레이캐스트 할때 겹쳐있는 오브젝트 체크하는데 겹쳐있는 오브젝트중 카메라의 거리가 가장 작은 값을 기록하는 변수.
			//이는 오브젝트 검사할때마다 한번씩 초기화를 해주어야함.


			for (size_t j = 0; j < vecObject.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObject[j]->GetRenderComponent();

				// 렌더링 기능이 없는 오브젝트는 제외
				if (nullptr == pRenderCom
					|| nullptr == pRenderCom->GetMaterial(0))
					continue;
				if (nullptr != pRenderCom->GetMaterial(0)
					&& nullptr == pRenderCom->GetMaterial(0)->GetShader())
					continue;

				// FrustumCheck
				if (pRenderCom->IsUseFrustumCheck())
				{
					Vec3 vWorldPos = vecObject[j]->Transform()->GetWorldPos();

					if (pRenderCom->IsShowDebugBound())
					{
						// Bounding Debug Shape 그리기
						tDebugBoundingInfo info = {};
						info.vWorldPos = vWorldPos;
						info.fBounding = pRenderCom->GetBounding();
						info.fMaxTime = 0.f;
						CRenderMgr::GetInst()->AddDebugBoundingInfo(info);
					}

					if (false == m_Frustum.FrustumCheckBySphere(vWorldPos, pRenderCom->GetBounding()))
						continue;
				}

				GizmoClickCheck(vecObject[j], pCurLevel); //기즈모 클릭 체크

				// 쉐이더 도메인에 따른 분류
				SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(0)->GetShader()->GetDomain();
				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_DEFERRED:
					m_vecDeferred.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_DECAL:
					m_vecDecal.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecOpaque.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
					m_vecPost.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_UI:
					m_vecUI.push_back(vecObject[j]);
					break;				
				}
			}
		}
	}
}

void CCamera::SortShadowObject()
{
	m_vecDynamicShadow.clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();

			if (pRenderCom && pRenderCom->IsDynamicShadow())
			{
				m_vecDynamicShadow.push_back(vecObj[j]);
			}
		}
	}
}

void CCamera::render()
{
	// 행렬 업데이트
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProj = m_matProj;

	// =====================================
	// 쉐이더 도메인에 따라서 순차적으로 그리기
	// =====================================
	// Deferred Object
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->OMSet();
	render_deferred();

	// Decal Render
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DECAL)->OMSet();
	render_decal();

	// Lighting
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->OMSet();
	
	const vector<CLight3D*> vecLight3D = CRenderMgr::GetInst()->GetLight3D();
	for (size_t i = 0; i < vecLight3D.size(); ++i)
	{
		vecLight3D[i]->render();
	}

	// (Deferred + Light) Merge
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	render_merge();


	// Forward Object	
	render_opaque();
	render_mask();
	render_transparent();

	// PostProcess
	render_postprocess();

	// UI
	render_ui();


 

	
}

void CCamera::render_depthmap()
{
	// 광원 카메라의 View, Proj 세팅
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProj = m_matProj;

	for (size_t i = 0; i < m_vecDynamicShadow.size(); ++i)
	{
		m_vecDynamicShadow[i]->GetRenderComponent()->render_depthmap();
	}
}


IntersectResult CCamera::IsCollidingBtwRayRect(tRay& _ray, CGameObject* _Object)
{
	// 만약에 Collider2D가 없거나 Rect모양이 아닌 경우 return
	if (_Object->Collider2D() == nullptr || _Object->Collider2D()->GetColliderShape() != COLLIDER2D_TYPE::RECT)
		return IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };

	Matrix ColliderWorldMat = _Object->Collider2D()->GetColliderWorldMat();

	// Local Rect의 3개의 Pos에 대해서 World Pos 계산을 해준다.
	// 점 4개까지 필요없고 3개만 있으면 됨
	// 아래의 정점들의 순서는 의미가 있음
	// IntersectsLay함수에서 특정 정점의 순서에서만 올바르게 계산하도록 함
	Vec3 arrLocal[3] =
	{
		Vec3(-0.5f, -0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, 0.5f, 0.f),
	};

	for (int i = 0; i < 3; ++i)
		arrLocal[i] = Vector4::Transform(Vec4(arrLocal[i], 1.f), ColliderWorldMat);
 

	return IntersectsLay(arrLocal, m_ray);
}

IntersectResult CCamera::IsCollidingBtwRayCube(tRay& _ray, CGameObject* _Object)
{
	// 만약에 Collider3D가 없거나 Cube모양이 아닌 경우 return
	if (_Object->Collider3D() == nullptr || _Object->Collider3D()->GetCollider3DType() != COLLIDER3D_TYPE::CUBE)
		return IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };

	Matrix ColliderWorldMat = _Object->Collider3D()->GetColliderWorldMat();

	// 위의 IsCollidingBtwRayRect함수와 같은 알고리즘으로 계산
	// 단 Cube는 면이 6개이기 때문에 6번 계산해 줘야 하는 것임
	Vec3 arrLocal[6][3] =
	{
		{Vec3(-0.5f, 0.5f, -0.5f),  Vec3(0.5f, 0.5f, -0.5f),  Vec3(-0.5f, 0.5f, 0.5f)},	 // 윗면
		{Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.5f, -0.5f, -0.5f), Vec3(-0.5f, -0.5f, 0.5f)}, // 밑면
		{Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.5f, -0.5f, -0.5f), Vec3(-0.5f, 0.5f, -0.5f)}, // 앞면
		{Vec3(-0.5f, -0.5f, 0.5f),  Vec3(0.5f, -0.5f, 0.5f),  Vec3(-0.5f, 0.5f, 0.5f)},  // 뒷면
		{Vec3(-0.5f, 0.5f, -0.5f),  Vec3(-0.5f, -0.5f, -0.5f),Vec3(-0.5f, 0.5f, 0.5f)},  // 왼쪽면
		{Vec3(0.5f, 0.5f, -0.5f),   Vec3(0.5f, -0.5f, -0.5f), Vec3(0.5f, 0.5f, 0.5f)},   // 오른쪽면
	};

	for (int i = 0; i < 6; ++i)
		for (int j = 0; j < 3; ++j)
			arrLocal[i][j] = Vec4::Transform(Vec4(arrLocal[i][j], 1.f), ColliderWorldMat);

	IntersectResult Final = IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };
	IntersectResult Temp;

	for (int i = 0; i < 6; ++i)
	{
		Temp = IntersectsLay(arrLocal[i], m_ray);

		if (Temp.bResult == true)
		{
			if (Final.bResult == false)
			{
				Final = Temp;
			}
			else
			{
				if (Temp.fResult < Final.fResult)
					Final = Temp;
			}
		}
	}

	return Final;
}

IntersectResult CCamera::IntersectsLay(Vec3* _vertices, tRay _ray)
{
	IntersectResult result;
	result.vCrossPoint = Vec3(0.f, 0.f, 0.f);
	result.bResult = false;

	Vec3 edge[2] = { Vec3(), Vec3() };
	edge[0] = _vertices[1] - _vertices[0];
	edge[1] = _vertices[2] - _vertices[0];

	Vec3 normal = (edge[0].Cross(edge[1])).Normalize();
	float b = normal.Dot(_ray.vDir);
	 

	Vec3 w0 = _ray.vStart - _vertices[0];
	float a = -(normal.Dot(w0));
	float t = a / b;

	result.fResult = t;

	Vec3 p = _ray.vStart + t * _ray.vDir;

	result.vCrossPoint = p;

	float uu, uv, vv, wu, wv, inverseD;
	uu = edge[0].Dot(edge[0]);
	uv = edge[0].Dot(edge[1]);
	vv = edge[1].Dot(edge[1]);

	Vec3 w = p - _vertices[0];
	wu = w.Dot(edge[0]);
	wv = w.Dot(edge[1]);
	
	inverseD = uv * uv - uu * vv;
	inverseD = 1.0f / inverseD;

	float u = (uv * wv - vv * wu) * inverseD;
	if (u < 0.0f || u > 1.0f)
	{
		result.vCrossPoint = Vec3();
		result.fResult = 0.0f;
		result.bResult = false;
		return result;
	}

	float v = (uv * wu - uu * wv) * inverseD;
	if (v < 0.0f || v > 1.0f)
	{
		result.vCrossPoint = Vec3();
		result.fResult = 0.0f;
		result.bResult = false;
		return result;
	}

	result.bResult = true;
	return result;
}

void CCamera::clear()
{
	m_vecDeferred.clear();
	m_vecDecal.clear();
	m_vecOpaque.clear();
	m_vecMask.clear();
	m_vecTransparent.clear();
	m_vecPost.clear();
	m_vecUI.clear();
}

void CCamera::render_deferred()
{
	for (size_t i = 0; i < m_vecDeferred.size(); ++i)
	{
		//DataTex에 자신의 LayerNum을 집어넣는다.
		if (m_vecDeferred[i]->GetLayerIndex() != -1) {
			//layer는 0~31이지만 비트연산을 위해 한칸씩 옮김
			//layernum = 1~32
			float layernum = m_vecDeferred[i]->GetLayerIndex() + 1;
			m_vecDeferred[i]->GetRenderComponent()->GetMaterial(0)->SetScalarParam(FLOAT_0, &layernum);
		}
		m_vecDeferred[i]->render();
	}
}

void CCamera::render_decal()
{
	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->render();
	}
}

void CCamera::render_merge()
{
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"LightMergeMtrl");

	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex"));
	pMtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex"));
	pMtrl->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"SpecularTargetTex"));
	pMtrl->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"EmissiveTargetTex"));
	pMtrl->UpdateData();

	pMesh->render(0);
}

void CCamera::render_opaque()
{
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->render();
	}
}

void CCamera::render_mask()
{
	for (size_t i = 0; i < m_vecMask.size(); ++i)
	{
		m_vecMask[i]->render();
	}
}

void CCamera::render_transparent()
{
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPost.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPost[i]->render();
	}
}

void CCamera::render_ui()
{
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->render();
	}
}

void CCamera::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fAspectRatio, sizeof(float), 1, _File);
	fwrite(&m_fScale, sizeof(float), 1, _File);
	fwrite(&m_ProjType, sizeof(UINT), 1, _File);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	fwrite(&m_iCamIdx, sizeof(int), 1, _File);
	fwrite(&m_fFar, sizeof(float), 1, _File);
	fwrite(&m_bShowFrustumDebug, sizeof(bool), 1, _File);
}

void CCamera::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_ProjType, sizeof(UINT), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
	fread(&m_fFar, sizeof(float), 1, _File);
	fread(&m_bShowFrustumDebug, sizeof(bool), 1, _File);
	SetCameraIndex(m_iCamIdx);
}

void CCamera::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	_objValue.AddMember("fAspectRatio", m_fAspectRatio, allocator);
	_objValue.AddMember("fScale", m_fScale, allocator);
	_objValue.AddMember("ProjType", (UINT)m_ProjType, allocator);
	_objValue.AddMember("iLayerMask", m_iLayerMask, allocator);
	_objValue.AddMember("iCamIdx", m_iCamIdx, allocator);
	_objValue.AddMember("fFar", m_fFar, allocator);
	_objValue.AddMember("bShowFrustumDebug", m_bShowFrustumDebug, allocator);

}

void CCamera::LoadFromLevelJsonFile(const Value& _componentValue)
{
	m_fAspectRatio = _componentValue["fAspectRatio"].GetFloat();
	m_fScale = _componentValue["fScale"].GetFloat();
	m_ProjType = (PROJ_TYPE)_componentValue["ProjType"].GetUint();
	m_iLayerMask = _componentValue["iLayerMask"].GetUint();
	m_iCamIdx = _componentValue["iCamIdx"].GetInt();
	m_fFar = _componentValue["fFar"].GetFloat();
	m_bShowFrustumDebug = _componentValue["bShowFrustumDebug"].GetBool();

	SetCameraIndex(m_iCamIdx);
}



void CCamera::GizmoClickCheck(CGameObject* _CheckTargetObj, CLevel* _CurLevel)
{
	Vec3 vWorldPos = _CheckTargetObj->Transform()->GetWorldPos();
	Vec3 Rot = _CheckTargetObj->Transform()->GetRelativeRot();

	if (_CurLevel->GetState() != LEVEL_STATE::PLAY) // 플레이 모드에서는 기즈모 작동하지 않음
	{
		if (KEY_TAP(KEY::RBTN)) // 클릭되었을경우만
		{
			if (RayIntersectsSphere(vWorldPos, _CheckTargetObj->Transform()->GetGizmoBounding()) && !_CheckTargetObj->Transform()->GetNogizmoObj())  //오브젝트 구체 콜리전 - 레이 클릭 체크
			{
				// 가장 깊이가 작은경우의 오브젝트를 선택하게됨 SetGizMoTargetObj 함수자체는 여러번 호출되지만. 오브젝트 개수만큼 반복문을 통해 결국 마지막엔 가장 작은 깊이를 가진 오브젝트가 기즈모 타겟  오브젝트로 세팅되어있을 것임
				if (!_CheckTargetObj->IsDead())
				{
					CRenderMgr::GetInst()->SetGizmoObjectChanged(true);
					CRenderMgr::GetInst()->SetGizMoTargetObj(_CheckTargetObj); //위의 if문에서 레이인터섹트 스피어 (현재 구체위에 마우스가 있음) 상태고 마우스를 눌렀다면, 그것을 기즈모 오브젝트로 지정해둠
				}

			}
		}

		if (b_ViewGizmoBounding && !_CheckTargetObj->Transform()->GetNogizmoObj()) //기즈모 클릭범위를 보여달라고 요청했다면(트랜스폼ui에서) 보여주기
		{
			// Bounding Debug Shape 그리기
			tDebugBoundingInfo info = {};
			info.vWorldPos = vWorldPos;
			info.fBounding = _CheckTargetObj->Transform()->GetGizmoBounding();
			info.fMaxTime = 0.f;
			CRenderMgr::GetInst()->AddDebugBoundingInfo(info);
		}

	}
}


bool CCamera::RayIntersectsSphere(Vec3 _SphereTrans, float _SphereRadius)
{
	tRay ray = m_ray;
	// 레이의 원점에서 구의 중심까지의 벡터를 계산합니다.
	Vec3 m = ray.vStart - _SphereTrans;

	// 레이의 방향과 m 벡터의 내적을 계산합니다.
	float b = m.x * ray.vDir.x + m.y * ray.vDir.y + m.z * ray.vDir.z;

	// m 벡터의 제곱과 구의 반지름의 제곱의 차를 계산합니다.
	float c = (m.x * m.x + m.y * m.y + m.z * m.z) - _SphereRadius * _SphereRadius;

	// 레이의 원점이 구의 외부에 있고 (c > 0), 레이가 구를 향하고 있지 않으면 (b > 0) false를 반환합니다.
	if (c > 0.0f && b > 0.0f) return false;

	// 판별식을 계산합니다. 판별식이 음수이면 레이가 구를 놓치는 것을 의미합니다.
	float discr = b * b - c;

	// 판별식이 음수이면 레이가 구를 놓친 것이므로 false를 반환합니다.
	if (discr < 0.0f) return false;

	// 이제 레이가 구와 교차하는 것이 확인되었으므로, 교차점의 가장 작은 t 값을 계산합니다.
	float t = -b - sqrt(discr);

	// 만약 t가 음수이면 레이가 구 내부에서 시작된 것이므로 t를 0으로 설정합니다.
	if (t < 0.0f) t = 0.0f;

	// 레이와 교차한 깊이값을 계산합니다.
	float depth = XMVectorGetX(XMVector3Length(ray.vDir) * t);

	if (m_LayMinDistance > depth) //현재 길이값보다 더 작다면 더앞에있는 오브젝트
	{
		m_LayMinDistance = depth;  //현재 디스탠스 값으로 업데이트
		return true;
	}
	else
		return false;

	// 이 시점에서 레이가 구와 교차하는 것이 확인되었으므로 true를 반환합니다.

}