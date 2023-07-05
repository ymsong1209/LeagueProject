#include "pch.h"
#include "CLight3D.h"

#include "CCamera.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CResMgr.h"


CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightInfo{}
{
	SetLightType(LIGHT_TYPE::POINT);

	m_pLightCam = new CGameObject;
	m_pLightCam->AddComponent(new CTransform);
	m_pLightCam->AddComponent(new CCamera);
}

CLight3D::CLight3D(const CLight3D& _other)
	: CComponent(_other)
	, m_LightInfo(_other.m_LightInfo)
	, m_iLightIdx(-1)
	, m_pLightCam(nullptr)
{
	m_pLightCam = _other.m_pLightCam->Clone();
}


CLight3D::~CLight3D()
{
	if (nullptr != m_pLightCam)
		delete m_pLightCam;
}


void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_LightInfo.LightType = (UINT)_Type;

	if (LIGHT_TYPE::DIRECTIONAL == _Type)
	{
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_LightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl"); 

		m_pLightCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pLightCam->Camera()->SetWidth(4096.f);
		m_pLightCam->Camera()->SetAspectRatio(1.f);
		m_pLightCam->Camera()->SetFar(100000.f);
	}

	else if (LIGHT_TYPE::POINT == _Type)
	{
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_LightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl"); 
	}

	else
	{
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		//m_LightMtrl = 
	}
}

void CLight3D::SetLightDirection(Vec3 _vDir)
{
	m_LightInfo.vWorldDir = _vDir;
	m_LightInfo.vWorldDir.Normalize();

	Vec3 vFront = m_LightInfo.vWorldDir;
	Vec3 vUp = Vec3(0.f, 1.f, 0.f);
	Vec3 vRight = XMVector3Cross(vUp, vFront);
	vRight.Normalize();
	vUp = XMVector3Cross(vFront, vRight);
	vUp.Normalize();

	Matrix matRot = XMMatrixIdentity();
	matRot._11 = vRight.x;	matRot._12 = vRight.y;	matRot._13 = vRight.z;
	matRot._21 = vUp.x;		matRot._22 = vUp.y;		matRot._23 = vUp.z;
	matRot._31 = vFront.x;	matRot._32 = vFront.y;	matRot._33 = vFront.z;

	Vec3 vRot = DecomposeRotMat(matRot);

	// 광원이 가리키는 방향이 Transform 의 Front 가 되도록 회전시켜준다.
	Transform()->SetRelativeRot(vRot);
}


void CLight3D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Transform()->SetRelativeScale(Vec3(m_LightInfo.Radius * 2.f, m_LightInfo.Radius * 2.f, m_LightInfo.Radius * 2.f));

	// 광원의 카메라도 광원과 동일한 Transform 이 되도록 업데이트 한다.
	m_pLightCam->Transform()->SetRelativePos(Transform()->GetWorldPos());
	m_pLightCam->Transform()->SetRelativeRot(DecomposeRotMat(Transform()->GetWorldRotMat()));
	m_pLightCam->finaltick_module();


	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);
}

void CLight3D::render()
{
	if (nullptr == m_LightMtrl || nullptr == m_VolumeMesh)
	{
		//assert(nullptr);
		return;
	}

	Transform()->UpdateData();

	m_LightMtrl->SetScalarParam(INT_0, &m_iLightIdx);
	m_LightMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	m_LightMtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));
	

	// 방향성 광원인 경우 그림자 처리를 위해서 광원카메라로 투영시킬 수 있게 View * Proj 행렬을 전달
	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		Matrix matLightVP = m_pLightCam->Camera()->GetViewMat() * m_pLightCam->Camera()->GetProjMat();
		m_LightMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, &matLightVP);
		m_LightMtrl->SetTexParam(TEX_PARAM::TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"DepthMapTex"));
	}
	
	
	m_LightMtrl->UpdateData();

	m_VolumeMesh->render(0);
}

void CLight3D::render_depthmap()
{
	m_pLightCam->Camera()->SortShadowObject();
	m_pLightCam->Camera()->render_depthmap();
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _File);
	SaveResRef(m_VolumeMesh.Get(), _File);
	SaveResRef(m_LightMtrl.Get(), _File);
	fwrite(&m_iLightIdx, sizeof(UINT), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo, sizeof(tLightInfo), 1, _File);
	LoadResRef(m_VolumeMesh, _File);
	LoadResRef(m_LightMtrl, _File);
	fread(&m_iLightIdx, sizeof(UINT), 1, _File);
}

