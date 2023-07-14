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

	// ���콺���� ���� ���
	CalRay();

	CollideRay();

}

void CCamera::CalRay()
{
	// ���콺 ������ ���ϴ� Ray ���ϱ�
	// SwapChain Ÿ���� ViewPort ����
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

	//  ���� ���콺 ��ǥ
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// ������ ī�޶��� ��ǥ�� �ݵ�� ������.
	m_ray.vStart = Transform()->GetWorldPos();

	// view space ������ ����
	m_ray.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
	m_ray.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
	m_ray.vDir.z = 1.f;

	// world space ������ ����
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

			// ���� �浹���� ���ٸ� EndOverlap �ĺ����� �־�� �� (Ray - Rect�� ���ϴ� ����)
			if (Objects[j]->Collider2D())
			{
				if (Objects[j]->Collider2D()->IsCollidedFromRay() == true)
				{
					// �浹���̿��µ� �̹������ӿ����� �浹������ �Ǿ������� End Ray Overlap�� ȣ������
					if (result.bResult == false)
					{
						Objects[j]->Collider2D()->SetCollidedFromRay(false);
						Objects[j]->Collider2D()->EndRayOverlap();
					}

					// �̰��� ��� �浹��������. ������ ���� ������ �ִ� ��ü�� �ƴ϶�� End Ray Overlap�� ȣ���ؾ� �ϱ⿡
					// �ĺ����� �־��ش�.
					else
					{

						TempEndOverlapRect.push_back(Objects[j]);
					}
				}

				//if (result.vCrossPoint != Vec3(0.f, 0.f, 0.f))
				if (result.bResult == true)
				{
					// �ִ� �Ÿ��� �ִ� Object���� Ȯ���ؾ� �Ѵ�.
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
					// �浹���̿��µ� �̹������ӿ����� �浹������ �Ǿ������� End Ray Overlap�� ȣ������
					if (CubeResult.bResult == false)
					{
						Objects[j]->Collider3D()->SetCollidedFromRay(false);
						Objects[j]->Collider3D()->EndRayOverlap();
					}

					// �̰��� ��� �浹��������. ������ ���� ������ �ִ� ��ü�� �ƴ϶�� End Ray Overlap�� ȣ���ؾ� �ϱ⿡
					// �ĺ����� �־��ش�.
					else
					{

						TempEndOverlapCube.push_back(Objects[j]);
					}
				}

				//if (result.vCrossPoint != Vec3(0.f, 0.f, 0.f))
				if (CubeResult.bResult == true)
				{
					// �ִ� �Ÿ��� �ִ� Object���� Ȯ���ؾ� �Ѵ�.
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
		// ������� ������ ���� �浹���̰� ���� ����� ���ϳ��� Rect Collider�� ���� Object��
		if (FinalRectObject->Collider2D()->IsCollidedFromRay())
		{
			FinalRectObject->Collider2D()->OnRayOverlap();
		}

		// Begin Ray Overlap�� �ؾ��Ѵ�.
		else
		{
			FinalRectObject->Collider2D()->SetCollidedFromRay(true);
			FinalRectObject->Collider2D()->BeginRayOverlap();
		}

		// TemObject���߿��� FinalObject�� �ƴ� Object���� ��� EndRayOverlap�� ȣ���ؾ��Ѵ�.
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
		// ������� ������ ���� �浹���̰� ���� ����� ���ϳ��� Cube Collider�� ���� Object��
		if (FinalCubeObject->Collider3D()->IsCollidedFromRay())
		{
			FinalCubeObject->Collider3D()->OnRayOverlap();
		}

		// Begin Ray Overlap�� �ؾ��Ѵ�.
		else
		{
			FinalCubeObject->Collider3D()->SetCollidedFromRay(true);
			FinalCubeObject->Collider3D()->BeginRayOverlap();
		}

		// TemObject���߿��� FinalObject�� �ƴ� Object���� ��� EndRayOverlap�� ȣ���ؾ��Ѵ�.
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
	// View ��� ���
	// ==============
	m_matView = XMMatrixIdentity();

	// ī�޶� ��ǥ�� �������� �̵�
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// ī�޶� �ٶ󺸴� ������ Z ��� �����ϰ� ����� ȸ�� ����� ����
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
	// ���� ��� ���
	// =============
	m_matProj = XMMatrixIdentity();
	
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// ���� ����
		float fHeight = m_fWidth / m_fAspectRatio;
		m_matProj = XMMatrixOrthographicLH(m_fWidth * (1.f / m_fScale), fHeight * (1.f / m_fScale), 1.f, m_fFar);
	}
	else
	{	
		// ���� ����
		m_matProj = XMMatrixPerspectiveFovLH(XM_PI / 2.f, m_fAspectRatio, 1.f, m_fFar);
	}

	// ������� �����
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
	// ���� ������ �з����� ����
	clear();

	// ���� ���� �����ͼ� �з�
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� ����ũ Ȯ��
		if (m_iLayerMask & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			m_LayMinDistance = GetFar();//����� �׽�Ʈ :����ĳ��Ʈ �Ҷ� �����ִ� ������Ʈ üũ�ϴµ� �����ִ� ������Ʈ�� ī�޶��� �Ÿ��� ���� ���� ���� ����ϴ� ����.
			//�̴� ������Ʈ �˻��Ҷ����� �ѹ��� �ʱ�ȭ�� ���־����.


			for (size_t j = 0; j < vecObject.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObject[j]->GetRenderComponent();

				// ������ ����� ���� ������Ʈ�� ����
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
						// Bounding Debug Shape �׸���
						tDebugBoundingInfo info = {};
						info.vWorldPos = vWorldPos;
						info.fBounding = pRenderCom->GetBounding();
						info.fMaxTime = 0.f;
						CRenderMgr::GetInst()->AddDebugBoundingInfo(info);
					}

					if (false == m_Frustum.FrustumCheckBySphere(vWorldPos, pRenderCom->GetBounding()))
						continue;
				}

				GizmoClickCheck(vecObject[j], pCurLevel); //����� Ŭ�� üũ

				// ���̴� �����ο� ���� �з�
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
	// ��� ������Ʈ
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProj = m_matProj;

	// =====================================
	// ���̴� �����ο� ���� ���������� �׸���
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
	// ���� ī�޶��� View, Proj ����
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
	// ���࿡ Collider2D�� ���ų� Rect����� �ƴ� ��� return
	if (_Object->Collider2D() == nullptr || _Object->Collider2D()->GetColliderShape() != COLLIDER2D_TYPE::RECT)
		return IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };

	Matrix ColliderWorldMat = _Object->Collider2D()->GetColliderWorldMat();

	// Local Rect�� 3���� Pos�� ���ؼ� World Pos ����� ���ش�.
	// �� 4������ �ʿ���� 3���� ������ ��
	// �Ʒ��� �������� ������ �ǹ̰� ����
	// IntersectsLay�Լ����� Ư�� ������ ���������� �ùٸ��� ����ϵ��� ��
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
	// ���࿡ Collider3D�� ���ų� Cube����� �ƴ� ��� return
	if (_Object->Collider3D() == nullptr || _Object->Collider3D()->GetCollider3DType() != COLLIDER3D_TYPE::CUBE)
		return IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };

	Matrix ColliderWorldMat = _Object->Collider3D()->GetColliderWorldMat();

	// ���� IsCollidingBtwRayRect�Լ��� ���� �˰������� ���
	// �� Cube�� ���� 6���̱� ������ 6�� ����� ��� �ϴ� ����
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
		//DataTex�� �ڽ��� LayerNum�� ����ִ´�.
		if (m_vecDeferred[i]->GetLayerIndex() != -1) {
			//layer�� 0~31������ ��Ʈ������ ���� ��ĭ�� �ű�
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

	if (_CurLevel->GetState() != LEVEL_STATE::PLAY) // �÷��� ��忡���� ����� �۵����� ����
	{
		if (KEY_TAP(KEY::RBTN)) // Ŭ���Ǿ�����츸
		{
			if (RayIntersectsSphere(vWorldPos, _CheckTargetObj->Transform()->GetGizmoBounding()) && !_CheckTargetObj->Transform()->GetNogizmoObj())  //������Ʈ ��ü �ݸ��� - ���� Ŭ�� üũ
			{
				// ���� ���̰� ��������� ������Ʈ�� �����ϰԵ� SetGizMoTargetObj �Լ���ü�� ������ ȣ�������. ������Ʈ ������ŭ �ݺ����� ���� �ᱹ �������� ���� ���� ���̸� ���� ������Ʈ�� ����� Ÿ��  ������Ʈ�� ���õǾ����� ����
				if (!_CheckTargetObj->IsDead())
				{
					CRenderMgr::GetInst()->SetGizmoObjectChanged(true);
					CRenderMgr::GetInst()->SetGizMoTargetObj(_CheckTargetObj); //���� if������ �������ͼ�Ʈ ���Ǿ� (���� ��ü���� ���콺�� ����) ���°� ���콺�� �����ٸ�, �װ��� ����� ������Ʈ�� �����ص�
				}

			}
		}

		if (b_ViewGizmoBounding && !_CheckTargetObj->Transform()->GetNogizmoObj()) //����� Ŭ�������� �����޶�� ��û�ߴٸ�(Ʈ������ui����) �����ֱ�
		{
			// Bounding Debug Shape �׸���
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
	// ������ �������� ���� �߽ɱ����� ���͸� ����մϴ�.
	Vec3 m = ray.vStart - _SphereTrans;

	// ������ ����� m ������ ������ ����մϴ�.
	float b = m.x * ray.vDir.x + m.y * ray.vDir.y + m.z * ray.vDir.z;

	// m ������ ������ ���� �������� ������ ���� ����մϴ�.
	float c = (m.x * m.x + m.y * m.y + m.z * m.z) - _SphereRadius * _SphereRadius;

	// ������ ������ ���� �ܺο� �ְ� (c > 0), ���̰� ���� ���ϰ� ���� ������ (b > 0) false�� ��ȯ�մϴ�.
	if (c > 0.0f && b > 0.0f) return false;

	// �Ǻ����� ����մϴ�. �Ǻ����� �����̸� ���̰� ���� ��ġ�� ���� �ǹ��մϴ�.
	float discr = b * b - c;

	// �Ǻ����� �����̸� ���̰� ���� ��ģ ���̹Ƿ� false�� ��ȯ�մϴ�.
	if (discr < 0.0f) return false;

	// ���� ���̰� ���� �����ϴ� ���� Ȯ�εǾ����Ƿ�, �������� ���� ���� t ���� ����մϴ�.
	float t = -b - sqrt(discr);

	// ���� t�� �����̸� ���̰� �� ���ο��� ���۵� ���̹Ƿ� t�� 0���� �����մϴ�.
	if (t < 0.0f) t = 0.0f;

	// ���̿� ������ ���̰��� ����մϴ�.
	float depth = XMVectorGetX(XMVector3Length(ray.vDir) * t);

	if (m_LayMinDistance > depth) //���� ���̰����� �� �۴ٸ� ���տ��ִ� ������Ʈ
	{
		m_LayMinDistance = depth;  //���� ���Ľ� ������ ������Ʈ
		return true;
	}
	else
		return false;

	// �� �������� ���̰� ���� �����ϴ� ���� Ȯ�εǾ����Ƿ� true�� ��ȯ�մϴ�.

}