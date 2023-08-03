#include "pch.h"
#include "CCamera.h"


#include "CDevice.h"
#include "CRenderMgr.h"
#include "CTransform.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CRenderComponent.h"
#include "CMaterial.h"
#include "CGraphicsShader.h"

#include "CMRT.h"
#include "CLight3D.h"

#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CRenderMgr.h"

#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CEngine.h"

#include "CStructuredBuffer.h"
#include "CInstancingBuffer.h"




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
	, m_bViewGizmoBounding(false)
	, m_isGizmoEditMode(1)
	, m_fFov(30.f)
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
	, m_fFar(_Other.m_fFar)
	, m_ProjType(_Other.m_ProjType)
	, m_iLayerMask(_Other.m_iLayerMask)
	, m_iCamIdx(-1)
	, m_bShowFrustumDebug(_Other.m_bShowFrustumDebug)
	, m_bViewGizmoBounding(_Other.m_bViewGizmoBounding)
	, m_fFov(_Other.m_fFov)
{
	SetName(L"Camera");
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


	// Play상태일때 아래의 내용들은 Main Camera에서만 수행하도록 함.
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY &&
		CRenderMgr::GetInst()->GetPlayMainCam() != this)
		return;

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

	if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
	{
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

	else if (m_ProjType == PROJ_TYPE::ORTHOGRAPHIC)
	{
		Vec2 CamPos = CEngine::GetInst()->GetWindowResolution();
		CamPos.x = CamPos.x / 2.f;
		CamPos.y = CamPos.y / 2.f;

		Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos() - CamPos;

		Vec3 Addx = MousePos.x * Transform()->GetWorldDir(DIR_TYPE::RIGHT) * (1.f / m_fScale);
		Vec3 Addy = MousePos.y * Transform()->GetWorldDir(DIR_TYPE::UP) * (1.f / m_fScale);

		// Add앞에 -를 붙여야 하는 이유는 DirectX의 y축과 화면에서의 y축의 증가 방향이
		// 반대이기 때문이다.
		m_ray.vStart = Transform()->GetWorldPos() + Addx - Addy;
		m_ray.vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	}
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

bool CCamera::CheckRayCollideBox(CGameObject* _object)
{
	vector<CGameObject*> RayVec = CRenderMgr::GetInst()->GetRayObjectVec();
	vector<ColliderStruct> WallVec = CRenderMgr::GetInst()->GetWallObjectVec();

	bool result = false;
	bool CollidedByWall = false;

	for (int i = 0; i < RayVec.size(); ++i) {
		CGameObject* RayShootingObject = RayVec[i];


		float RayRadius = RayShootingObject->Transform()->GetRayRange();


		Vec2 RayObjPos = Vec2{ RayVec[i]->Transform()->GetRelativePos().x, RayVec[i]->Transform()->GetRelativePos().z };
		Vec2 CollideObjPos = Vec2{ _object->Transform()->GetRelativePos().x, _object->Transform()->GetRelativePos().z };

		//Vec3 RayObjPos = RayVec[i]->Transform()->GetRelativePos();
		//Vec3 CollideObjPos = _object->Transform()->GetRelativePos();

		float DistBtwRayObj = Vec2::Distance(RayObjPos, CollideObjPos);

		// 사거리 내에 object가 안들어오면 continue;
		if (DistBtwRayObj > RayRadius)
		{
			continue;

		}


		// 사거리 내에 들어오면 rayobject->CurObject로 ray-box 충돌 검사를 함.
		for (int j = 0; j < WallVec.size(); ++j)
		{
			if (IsCollidingBtwRayWall(RayObjPos, CollideObjPos, RayRadius, DistBtwRayObj, WallVec[j]))
			{
				CollidedByWall = true;
				break;
			}

		}

		if (CollidedByWall == true)
		{
			CollidedByWall = false;
			continue;
		}


		// 사이에 아무것도 없을 경우
		result = true;
		break;

	}

	return result;

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
		m_matProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fFov), m_fAspectRatio, 1.f, m_fFar);
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
					Vec3 vWorldPos;
					if (pRenderCom->GetBoundingBoxOffsetUse()) {
						vWorldPos = pRenderCom->GetBoundingBoxOffset();
					}
					else {
						vWorldPos = vecObject[j]->Transform()->GetWorldPos();
					}

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

				if (m_isGizmoEditMode == 1) //에디트 모드일때만 클릭체크
					GizmoClickCheck(vecObject[j], pCurLevel); //기즈모 클릭 체크

				//아웃라인 출력 오브젝트 테스트
				if (vecObject[j]->Transform()->GetUseMouseOutline() == true)
				{
					if (OutlineCheck(vecObject[j]))
						m_vecContour.push_back(vecObject[j]);
				}


				CollideRay();

				//Ray를 쏘는 오브젝트랑 CurObject사이에 벽이 있음 or RayObject의 시야 범위 내에 CurObject가 안들어옴

				if (vecObject[j]->GetRenderComponent() != nullptr && vecObject[j]->GetRenderComponent()->IsUsingRaySightCulling() && vecObject[j]->Transform()->GetIsShootingRay() != true)
				{
					if (false == CheckRayCollideBox(vecObject[j])) {
						continue;
					}
				}
				// 메테리얼 개수만큼 반복
				UINT iMtrlCount = pRenderCom->GetMtrlCount();

				for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
				{
					// 재질이 없거나, 재질의 쉐이더가 설정이 안된 경우
					if (nullptr == pRenderCom->GetMaterial(iMtrl)
						|| nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
					{
						continue;
					}

					// 쉐이더 도메인에 따른 분류
					SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(iMtrl)->GetShader()->GetDomain();
					Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();

					switch (eDomain)
					{
					case SHADER_DOMAIN::DOMAIN_DEFERRED:
					case SHADER_DOMAIN::DOMAIN_OPAQUE:
					case SHADER_DOMAIN::DOMAIN_MASK:
					{
						// Shader 의 POV 에 따라서 인스턴싱 그룹을 분류한다.
						map<ULONG64, vector<tInstObj>>* pMap = NULL;
						Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial(iMtrl);

						if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
						{
							pMap = &m_mapInstGroup_D;
						}
						else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE
							|| pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASK)
						{
							pMap = &m_mapInstGroup_F;
						}
						else
						{
							assert(nullptr);
							continue;
						}

						uInstID uID = {};
						uID.llID = pRenderCom->GetInstID(iMtrl);

						// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
						if (0 == uID.llID)
							continue;

						map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
						if (iter == pMap->end())
						{
							pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObject[j], iMtrl }}));
						}
						else
						{
							iter->second.push_back(tInstObj{ vecObject[j], iMtrl });
						}
					}
					break;
					case SHADER_DOMAIN::DOMAIN_UI_OPAQUE:
						m_vecUIOpaque.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_UI_MASK:
						m_vecUIMask.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_UI_TRANSPARENT:
						m_vecUITransparent.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_DECAL:
						m_vecDecal.push_back(vecObject[j]);
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

bool CCamera::OutlineCheck(CGameObject* _Obj)
{
	if (_Obj->Collider3D() != nullptr) //콜리전이 있을경우에만 아웃라인 출력가능(마우스와 교차점 체크를 해야하기 때문)
	{
		//윤곽선 출력은 콜리전이 스피어,큐브,렉트일때만 함.(레이랑 충돌체크 가능한 것들만)
		COLLIDER3D_TYPE Type = _Obj->Collider3D()->GetCollider3DType();
		if (Type == COLLIDER3D_TYPE::CUBE)
		{
			IntersectResult Result = IsCollidingBtwRayCube(m_ray, _Obj);
			return Result.bResult;
		}
		else if (Type == COLLIDER3D_TYPE::SPHERE)
		{
			//일단 원의 x축 길이를 반지름으로 생각하고 계산함. 하지만 타원과 레이 충돌은 조금더 복잡함..
			//RayIntersectsSphere는 일반 타원x 일반 구체 기준 충돌체크임
			Vec3 Pos = _Obj->Transform()->GetWorldPos();
			Matrix scaleMatrix = _Obj->Collider3D()->GetColliderScaleMat();
			Vec3 scale = {};
			scale.x = scaleMatrix._11;
			bool IsCollide = RayIntersectsSphere(Pos, scale.x);
			return IsCollide;
		}
	}

	else if (_Obj->Collider2D() != nullptr)
	{
		COLLIDER2D_TYPE Type = _Obj->Collider2D()->GetCollider2DType();
		if (Type == COLLIDER2D_TYPE::RECT)
		{
			IntersectResult Result = IsCollidingBtwRayRect(m_ray, _Obj);
			return Result.bResult;
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

	if (CRenderMgr::GetInst()->GetMainCam() == this)
	{
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
		render_forward();
		render_transparent();

		// Outline :포스트 프로세스 단계에 넣어도 상관은없지만 정리를 위해 따로. 테두리는 깊이 모두 기록된후에 마지막에 진행
		if(m_vecContour.size() > 0) //아웃라인을 출력해야하는 오브젝트가 있을경우에만 시행
			render_Outline();

		// PostProcess
		render_postprocess();
		// UI
		render_ui();
	}
	else//지금 왜 이런 처리가 생겼는지 갸우뚱? 하시는 분들이 있을텐데 그건 나중에 회의때 말씀드리도록 하겠습니다..
	{							   //간단하게 말씀드리면 지금 카메라가 하나 더생성되면 디퍼드 렌더링 관련해서 뭔가 버그가 있는거같아요 (디렉셔널 라이트도 카메라가 추가되면 더 밝게 보이거나 함)
		// UI
		render_uicamera();
	}
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


bool CCamera::IsCollidingBtwRayWall(Vec2& RayObjPos, Vec2& _CollideObjPos, float& _Raidus, float& _RayObjRadius, ColliderStruct& _ColliderData)
{
	// Sphere인지 Box인지 관계없이 벽과 Source Ray 사이의 거리가 반지름보다 길 경우 바로 false 반환
	float WallPositionX = _ColliderData.m_ColliderFinalMat._41;
	float WallPositionZ = _ColliderData.m_ColliderFinalMat._43;


	if (Vec2::Distance(RayObjPos, Vec2(WallPositionX, WallPositionZ)) > _Raidus)
	{
		return false;
	}

	// 거리 안에 있는 경우로 Sphere인지 Box인지에 따라 갈림 0 : Sphere, 1 : Box
	if (_ColliderData.m_ColliderType == 0)
	{
		// 일단 이경우는 로직 작성안해놔ㅏㅆ음 추후 Sphere충돌이 추가되면
		// 작성해야함
		return false;
	}

	else if (_ColliderData.m_ColliderType == 1)
	{
		Vec3 arrLocal[6][3] =
		{
			{Vec3(-0.5f, 0.5f, -0.5f),  Vec3(0.5f, 0.5f, -0.5f),  Vec3(-0.5f, 0.5f, 0.5f)},	 // ����
			{Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.5f, -0.5f, -0.5f), Vec3(-0.5f, -0.5f, 0.5f)}, // �ظ�
			{Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.5f, -0.5f, -0.5f), Vec3(-0.5f, 0.5f, -0.5f)}, // �ո�
			{Vec3(-0.5f, -0.5f, 0.5f),  Vec3(0.5f, -0.5f, 0.5f),  Vec3(-0.5f, 0.5f, 0.5f)},  // �޸�
			{Vec3(-0.5f, 0.5f, -0.5f),  Vec3(-0.5f, -0.5f, -0.5f),Vec3(-0.5f, 0.5f, 0.5f)},  // ���ʸ�
			{Vec3(0.5f, 0.5f, -0.5f),   Vec3(0.5f, -0.5f, -0.5f), Vec3(0.5f, 0.5f, 0.5f)},   // �����ʸ�
		};

		for (int i = 0; i < 6; ++i)
			for (int j = 0; j < 3; ++j)
				arrLocal[i][j] = Vec4::Transform(Vec4(arrLocal[i][j], 1.f), _ColliderData.m_ColliderFinalMat);

		IntersectResult Final = IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };
		IntersectResult Temp;

		tRay ray;
		ray.vStart = Vec3(RayObjPos.x, 10.f, RayObjPos.y);
		ray.vDir = Vec3(_CollideObjPos.x - RayObjPos.x, 0.f, _CollideObjPos.y - RayObjPos.y).Normalize();


		for (int i = 0; i < 6; ++i)
		{
			Temp = IntersecrRayFog(arrLocal[i][0], arrLocal[i][1], arrLocal[i][2], ray);

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

		// bResult가 True인 경우 Wall과 Ray가 충돌한 경우임
		if (Final.bResult == true)
		{
			if (Final.fResult < _RayObjRadius)
				return true;
		}

		else
		{
			// Wall과 충돌하지 않은 경우
			return false;
		}
	}


	return false;

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

IntersectResult CCamera::IntersecrRayFog(Vec3 _Vertices0, Vec3 _Vertices1, Vec3 _Vertices2, tRay _Ray)
{
	IntersectResult result;
	result.vCrossPoint = Vec3(0.f, 0.f, 0.f);
	result.fResult = 0.f;
	result.bResult = false;

	Vec3 edge[2] = { _Vertices1 - _Vertices0, _Vertices2 - _Vertices0 };
	Vec3 normal = (edge[0].Cross(edge[1])).Normalize();




	Vec3 pvec = _Ray.vDir.Cross(edge[1]);
	float det = edge[0].Dot(pvec);

	// 이 값이 작으면 ray와 triangle이 평행
	if (std::abs(det) < 0.000001f)
	{
		return result;
	}

	float invDet = 1.0f / det;
	Vec3 tvec = _Ray.vStart - _Vertices0;

	float u = tvec.Dot(pvec) * invDet;

	if (u < 0.0f || u > 1.0f)
	{
		return result;
	}



	Vec3 qvec = tvec.Cross(edge[0]);
	float v = _Ray.vDir.Dot(qvec) * invDet;

	if (v < 0.0f || v > 1.0f)
	{
		return result;
	}



	float t = edge[1].Dot(qvec) * invDet;

	// 성공적인 교차점을 찾았다면 결과를 저장.
	result.vCrossPoint = _Ray.vStart + _Ray.vDir * t;
	result.fResult = t;
	result.bResult = true;

	if (t < 0)
		result.bResult = false;

	return result;
}



void CCamera::clear()
{
	for (auto& pair : m_mapInstGroup_D)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_F)
		pair.second.clear();
	m_vecDecal.clear();
	m_vecTransparent.clear();
	m_vecContour.clear();
	m_vecPost.clear();
	m_vecUI.clear();

	m_vecUIOpaque.clear();
	m_vecUIMask.clear();
	m_vecUITransparent.clear();
}

void CCamera::render_deferred()
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};


	//같은 재질, Mtrl을 들고 있어도, LayerNum이 다르면 또 분류를 해줘야한다.
	//first : layernum , second : mtrlnum, cgameobject
	map<int, map<int, vector<CGameObject*>>> ObjDividedByLayer;
	for (auto& pair : m_mapInstGroup_D)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		for (UINT i = 0; i < pair.second.size(); ++i) {
			ObjDividedByLayer[pair.second[i].pObj->GetLayerIndex()][pair.second[i].iMtrlIdx].push_back(pair.second[i].pObj);
		}
	}

	for (auto& LayerPair : ObjDividedByLayer) {

		for (auto& MaterialPair : LayerPair.second) {
			CGameObject* pObj = MaterialPair.second[0];
			Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
			Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(MaterialPair.first);

			// Instancing 버퍼 클리어
			CInstancingBuffer::GetInst()->Clear();

			int iRowIdx = 0;
			bool bHasAnim3D = false;
			for (UINT i = 0; i < MaterialPair.second.size(); ++i)
			{
				tInstData.matWorld = MaterialPair.second[i]->Transform()->GetWorldMat();
				tInstData.matWV = tInstData.matWorld * m_matView;
				tInstData.matWVP = tInstData.matWV * m_matProj;

				if (MaterialPair.second[i]->Animator3D())
				{
					MaterialPair.second[i]->Animator3D()->UpdateData();
					tInstData.iRowIdx = iRowIdx++;
					CInstancingBuffer::GetInst()->AddInstancingBoneMat(MaterialPair.second[i]->Animator3D()->GetFinalBoneMat());
					bHasAnim3D = true;
				}
				else
					tInstData.iRowIdx = -1;

				CInstancingBuffer::GetInst()->AddInstancingData(tInstData);

				if (MaterialPair.second[i]->GetLayerIndex() != -1) {
					//layer는 0~31이지만 비트연산을 위해 한칸씩 옮김
					//layernum = 1~32
					float layernum = MaterialPair.second[i]->GetLayerIndex() + 1;
					pMtrl->SetScalarParam(FLOAT_0, &layernum);
				}
			}

			// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
			CInstancingBuffer::GetInst()->SetData();

			if (bHasAnim3D)
			{
				pMtrl->SetAnim3D(true); // Animation Mesh 알리기
				pMtrl->SetBoneCount(pMesh->GetBoneCount());
			}

			pMtrl->UpdateData_Inst();
			pMesh->render_instancing(MaterialPair.first);

			// 정리
			if (bHasAnim3D)
			{
				pMtrl->SetAnim3D(false); // Animation Mesh 알리기
				pMtrl->SetBoneCount(0);
			}
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		//DataTex에 자신의 LayerNum을 집어넣는다.
		if (pair.second[0].pObj->GetLayerIndex() != -1) {
			//layer는 0~31이지만 비트연산을 위해 한칸씩 옮김
			//layernum = 1~32
			float layernum = pair.second[0].pObj->GetLayerIndex() + 1;
			UINT matsize = pair.second[0].pObj->GetRenderComponent()->GetMtrlCount();
			for (UINT j = 0; j < matsize; ++j) {
				pair.second[0].pObj->GetRenderComponent()->GetMaterial(j)->SetScalarParam(FLOAT_0, &layernum);
			}
		}

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
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



void CCamera::render_forward()
{

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};


	//같은 재질, Mtrl을 들고 있어도, LayerNum이 다르면 또 분류를 해줘야한다.
	//first : layernum , second : mtrlnum, cgameobject
	map<int, map<int, vector<CGameObject*>>> ObjDividedByLayer;
	for (auto& pair : m_mapInstGroup_F)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		for (UINT i = 0; i < pair.second.size(); ++i) {
			ObjDividedByLayer[pair.second[i].pObj->GetLayerIndex()][pair.second[i].iMtrlIdx].push_back(pair.second[i].pObj);
		}
	}

	for (auto& LayerPair : ObjDividedByLayer) {

		for (auto& MaterialPair : LayerPair.second) {
			CGameObject* pObj = MaterialPair.second[0];
			Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
			Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(MaterialPair.first);

			// Instancing 버퍼 클리어
			CInstancingBuffer::GetInst()->Clear();

			int iRowIdx = 0;
			bool bHasAnim3D = false;
			for (UINT i = 0; i < MaterialPair.second.size(); ++i)
			{
				tInstData.matWorld = MaterialPair.second[i]->Transform()->GetWorldMat();
				tInstData.matWV = tInstData.matWorld * m_matView;
				tInstData.matWVP = tInstData.matWV * m_matProj;

				if (MaterialPair.second[i]->Animator3D())
				{
					MaterialPair.second[i]->Animator3D()->UpdateData();
					tInstData.iRowIdx = iRowIdx++;
					CInstancingBuffer::GetInst()->AddInstancingBoneMat(MaterialPair.second[i]->Animator3D()->GetFinalBoneMat());
					bHasAnim3D = true;
				}
				else
					tInstData.iRowIdx = -1;

				CInstancingBuffer::GetInst()->AddInstancingData(tInstData);

				if (MaterialPair.second[i]->GetLayerIndex() != -1) {
					//layer는 0~31이지만 비트연산을 위해 한칸씩 옮김
					//layernum = 1~32
					float layernum = MaterialPair.second[i]->GetLayerIndex() + 1;
					pMtrl->SetScalarParam(FLOAT_0, &layernum);
				}
			}

			// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
			CInstancingBuffer::GetInst()->SetData();

			if (bHasAnim3D)
			{
				pMtrl->SetAnim3D(true); // Animation Mesh 알리기
				pMtrl->SetBoneCount(pMesh->GetBoneCount());
			}

			pMtrl->UpdateData_Inst();
			pMesh->render_instancing(MaterialPair.first);

			// 정리
			if (bHasAnim3D)
			{
				pMtrl->SetAnim3D(false); // Animation Mesh 알리기
				pMtrl->SetBoneCount(0);
			}
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		//DataTex에 자신의 LayerNum을 집어넣는다.
		if (pair.second[0].pObj->GetLayerIndex() != -1) {
			//layer는 0~31이지만 비트연산을 위해 한칸씩 옮김
			//layernum = 1~32
			float layernum = pair.second[0].pObj->GetLayerIndex() + 1;
			UINT matsize = pair.second[0].pObj->GetRenderComponent()->GetMtrlCount();
			for (UINT j = 0; j < matsize; ++j) {
				pair.second[0].pObj->GetRenderComponent()->GetMaterial(j)->SetScalarParam(FLOAT_0, &layernum);
			}
		}

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
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

void CCamera::render_uiopaque()
{
	for (size_t i = 0; i < m_vecUIOpaque.size(); ++i)
	{
		m_vecUIOpaque[i]->render();
	}
}

void CCamera::render_uimask()
{
	for (size_t i = 0; i < m_vecUIMask.size(); ++i)
	{
		m_vecUIMask[i]->render();
	}
}

void CCamera::render_uitransparent()
{
	for (size_t i = 0; i < m_vecUITransparent.size(); ++i)
	{
		m_vecUITransparent[i]->render();
	}
}

void CCamera::render_uicamera()
{
	render_uiopaque();
	render_uimask();
	render_uitransparent();
}

void CCamera::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fAspectRatio, sizeof(float), 1, _File);
	fwrite(&m_fWidth, sizeof(float), 1, _File);
	fwrite(&m_fScale, sizeof(float), 1, _File);
	fwrite(&m_ProjType, sizeof(UINT), 1, _File);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	fwrite(&m_iCamIdx, sizeof(int), 1, _File);
	fwrite(&m_fFar, sizeof(float), 1, _File);
	fwrite(&m_bShowFrustumDebug, sizeof(bool), 1, _File);
	fwrite(&m_bViewGizmoBounding, sizeof(bool), 1, _File);
	fwrite(&m_fFov, sizeof(float), 1, _File);
}

void CCamera::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fWidth, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_ProjType, sizeof(UINT), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
	fread(&m_fFar, sizeof(float), 1, _File);
	fread(&m_bShowFrustumDebug, sizeof(bool), 1, _File);
	fread(&m_bViewGizmoBounding, sizeof(bool), 1, _File);
	fread(&m_fFov, sizeof(float), 1, _File);

	SetCameraIndex(m_iCamIdx);
}

void CCamera::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	_objValue.AddMember("fAspectRatio", m_fAspectRatio, allocator);
	_objValue.AddMember("fWidth", m_fWidth, allocator);
	_objValue.AddMember("fScale", m_fScale, allocator);
	_objValue.AddMember("ProjType", (UINT)m_ProjType, allocator);
	_objValue.AddMember("iLayerMask", m_iLayerMask, allocator);
	_objValue.AddMember("iCamIdx", m_iCamIdx, allocator);
	_objValue.AddMember("fFar", m_fFar, allocator);
	_objValue.AddMember("bShowFrustumDebug", m_bShowFrustumDebug, allocator);
	_objValue.AddMember("bViewGizmoBounding", m_bViewGizmoBounding, allocator);
	_objValue.AddMember("fFov", m_fFov, allocator);
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
	m_bViewGizmoBounding = _componentValue["bViewGizmoBounding"].GetBool();
	m_fFov = _componentValue["fFov"].GetFloat();

	SetCameraIndex(m_iCamIdx);
}



void CCamera::GizmoClickCheck(CGameObject* _CheckTargetObj, CLevel* _CurLevel)
{
	Vec3 vWorldPos = _CheckTargetObj->Transform()->GetWorldPos();
	Vec3 Rot = _CheckTargetObj->Transform()->GetRelativeRot();

	if (_CurLevel->GetState() != LEVEL_STATE::PLAY) // 플레이 모드에서는 기즈모 작동하지 않음
	{
		if (KEY_TAP(KEY::LBTN) && !_CheckTargetObj->Transform()->GetGizmoObjExcept() && !CRenderMgr::GetInst()->GetIsImGuiHovered())
		{
			if (RayIntersectsSphere(vWorldPos, _CheckTargetObj->Transform()->GetGizmoBounding()))  //오브젝트 구체 콜리전 - 레이 클릭 체크
			{
				// 가장 깊이가 작은경우의 오브젝트를 선택하게됨 SetGizMoTargetObj 함수자체는 여러번 호출되지만. 오브젝트 개수만큼 반복문을 통해 결국 마지막엔 가장 작은 깊이를 가진 오브젝트가 기즈모 타겟  오브젝트로 세팅되어있을 것임
				if (!_CheckTargetObj->IsDead())
				{
					CRenderMgr::GetInst()->SetGizmoObjectChanged(true);
					CRenderMgr::GetInst()->SetGizMoTargetObj(_CheckTargetObj); //위의 if문에서 레이인터섹트 스피어 (현재 구체위에 마우스가 있음) 상태고 마우스를 눌렀다면, 그것을 기즈모 오브젝트로 지정해둠
				}
			}
		}

		if (m_bViewGizmoBounding && !_CheckTargetObj->Transform()->GetGizmoObjExcept()) //기즈모 클릭범위를 보여달라고 요청했다면(트랜스폼ui에서) 보여주기
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


void CCamera::render_DefaultContourPaint()
{
	//외곽선 필요한 오브젝트 렌더타겟에 기록
	for (size_t i = 0; i < m_vecContour.size(); ++i)
	{
		CGameObject* Obj = m_vecContour[i];
		UINT MtrlNum = Obj->GetRenderComponent()->GetMtrlCount();
		vector<Ptr<CGraphicsShader>> VecShader;

		for (UINT j = 0; j < MtrlNum; ++j) //렌더타겟 출력위한 셰이더 변경
		{
			Ptr<CGraphicsShader> DefaultShader = Obj->GetRenderComponent()->GetMaterial(j)->GetShader();
			VecShader.push_back(DefaultShader);
			Obj->GetRenderComponent()->GetMaterial(j)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"DefaultObjWriteShader"));
		}
		Obj->render();

		for (UINT z = 0; z < MtrlNum; ++z) //기존셰이더 장착
			Obj->GetRenderComponent()->GetMaterial(z)->SetShader(VecShader[z]);
	}
}


void CCamera::render_ContourPaint()
{
	//업스케일 오브젝트 기록
	for (size_t i = 0; i < m_vecContour.size(); ++i)
	{
		UINT MtrlNum = m_vecContour[i]->GetRenderComponent()->GetMtrlCount();
		vector<Ptr<CGraphicsShader>> VecShader;
		CGameObject* Obj = m_vecContour[i];

		for (UINT j = 0; j < MtrlNum; ++j)
		{
			//렌더타겟 기록을 위한 셰이더 변경 (모든 머터리얼에 적용)
			Ptr<CGraphicsShader> DefaultShader = Obj->GetRenderComponent()->GetMaterial(j)->GetShader();
			VecShader.push_back(DefaultShader);
			Obj->GetRenderComponent()->GetMaterial(j)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"ContourPaintShader"));
		}

		CTransform* Transform = Obj->Transform();
		float fThickness = Transform->GetOutlineThickness();
		Vec3 DefaultScale = Transform->GetRelativeScale();

		//두께계산-----
		Vec3 vThickness = Vec3(DefaultScale.x * fThickness, DefaultScale.y * fThickness, DefaultScale.z * fThickness);
		vThickness.y /= 4.f; //y는 조금더 작은값으로 해줘야 자연스러움
		Transform->SetRelativeScale(Vec3(DefaultScale.x + vThickness.x, DefaultScale.y + vThickness.y, DefaultScale.z + vThickness.z));
		Transform->finaltick(); //렌더 전 월드행렬 계산
		//---------------

		Obj->render();

		Transform->SetRelativeScale(DefaultScale);
		Transform->finaltick(); //해줘야함! 바로 다음함수가 기본 오브젝트 기록이기때문에 바로 변경된 스케일 적용

		//기존 셰이더 다시 장착 (모든 머터리얼에 적용함)
		for (UINT z = 0; z < MtrlNum; ++z)
			Obj->GetRenderComponent()->GetMaterial(z)->SetShader(VecShader[z]);
	}
}


void CCamera::render_contour()
{
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ContourMtrl");
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"DefaultContourTargetTex"));
	pMtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"ContourTargetTex"));

	pMtrl->UpdateData();
	pMesh->render(0);
}

void CCamera::render_Outline()
{
	CMRT* DefferedMrt = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED);
	//디퍼드에 담겨있는 ContourTargetTex,DefaultContourTargetTex를 렌더타겟으로 쓰기전에, 스왑체인에 그려내면서 셰이더 리소스 역할로 되어있던것들을 해제
	UINT RTCount = DefferedMrt->GetRTCount();
	for (UINT i = 0; i < RTCount; ++i)
		DefferedMrt->GetRTAtIndex(i)->Clear();

	//해제한후 렌더타겟으로 지정
	DefferedMrt->OMSet();
	//하나의 텍스쳐에 원래 오브젝트, 업스케일 오브젝트의 값을 같이 기록할수 없음. 이유는 두개의 물체가 겹쳐있는 부분은 나중에 그려지는 쪽의 rgba값으로 초기화됨.
	//결국 하나의 텍스쳐에 업스케일 오브젝트의 값만 있거나, 디폴트 오브젝트의 값만 있어서 나중에 테두리 부분만 추출할수없음.

	//서로 다른 도화지가 있을때 하나의 도화지엔 업스케일 오브젝트, 두번째 도화지엔 디폴트 오브젝트를 그려놓고 render_contour단계에서 값을 비교해 테두리 부분을 추출.
	//처음에 생각했던 방식인 같은렌더타겟을 사용하여 기존에 그려진 픽셀에 +1 하는방법은 그려지는곳을 텍스쳐로써 사용하는것이므로 안됨
	render_ContourPaint();
	render_DefaultContourPaint();

	//mrt를 다시 스왑체인으로 변경하고 테두리 검출,출력 진행
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	render_contour();

	DefferedMrt = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED);
	RTCount = DefferedMrt->GetRTCount();
	for (UINT i = 0; i < RTCount; ++i)
		DefferedMrt->GetRTAtIndex(i)->Clear();  //셰이더 리소스 해제
	 
	DefferedMrt->Clear(); //다른 카메라가 있을 경우를 대비한 디퍼드 mrt클리어(다른 카메라에 영향을 주지 않게 함)
}