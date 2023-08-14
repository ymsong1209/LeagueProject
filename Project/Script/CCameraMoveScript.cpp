#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CSound.h>
#include <Engine/CResMgr.h>
#include <Engine/CMRT.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_fCamSpeed(100.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
	// Editor Camera에서의 Play 상태에서의 Tick을 주지 않기 위함 
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY &&
		CRenderMgr::GetInst()->GetMainCam() != this->GetOwner()->Camera()) {
		return;
	}
	
	//에디터 상에서(play모드가아닐때) 에디터 카메라 이동하면 메인카메라도 같이 움직여버리는 현상때문에 아래 코드도 추가합니다.
	//나중에 카메라 스크립트를 에디터랑, 메인카메라 따로 만들어야할것같아요! 인게임 카메라랑 저희가 편집할때 카메라랑 많이달라서..
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() != LEVEL_STATE::PLAY &&
		CRenderMgr::GetInst()->GetMainCam() != this->GetOwner()->Camera()) {
		return;
	}


	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	Vec3 CamPos = GetOwner()->Transform()->GetRelativePos();


	Matrix CamWorld = MainCam->Transform()->GetWorldMat();

	//EditorCamera의 Position에 스폰됩니다.
	CGameObject* LoLMap = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
	if (LoLMap != nullptr) {
		tRay ray = CalcRay();
		IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, LoLMap);
		Vec3 EndPos = result.vCrossPoint;
		if (result.bResult) {
			CamPos = Vec4(EndPos.x, EndPos.y, EndPos.z, 1.f);
		}
		else {
			CamPos = GetOwner()->Transform()->GetRelativePos();
			CamPos.y = 0.f;
		}

	}
	else {
		CamPos.y = 0.f;
	}


	
	Vec3 CamForward = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 CamUp = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::UP);
	CSound::UpdateListenerAttributes(CamPos, CamForward, CamUp);
	

	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		Camera2DMove();
	else
		Camera3DMove();
}

void CCameraMoveScript::Camera2DMove()
{
	// 키 입력에 따른 카메라 이동
	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fCamSpeed;
	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 5.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos.z += EditorDT * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.z -= EditorDT * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= EditorDT * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += EditorDT * fSpeed;
	}

	Transform()->SetRelativePos(vPos);
}

void CCameraMoveScript::Camera3DMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = m_fCamSpeed;

	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
	{
		if (KEY_PRESSED(KEY::LSHIFT))
			fSpeed *= 5.f;

		if (KEY_PRESSED(KEY::W))
		{
			vPos.z += EditorDT * fSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos.z -= EditorDT  * fSpeed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos.x -= EditorDT * fSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos.x += EditorDT * fSpeed;
		}
	}

	else
	{
		if (KEY_PRESSED(KEY::LSHIFT))
			fSpeed *= 5.f;

		if (KEY_PRESSED(KEY::W))
		{
			vPos += EditorDT * vFront * fSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos -= EditorDT * vFront * fSpeed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos -= EditorDT * vRight * fSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos += EditorDT * vRight * fSpeed;
		}
	}

	if (KEY_PRESSED(KEY::MBTN))
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += EditorDT * vMouseDir.x * 5.f;
		vRot.x -= EditorDT * vMouseDir.y * 5.f;
	}


	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}

tRay CCameraMoveScript::CalcRay()
{
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	Matrix m_matProj = MainCam->GetProjMat();
	Matrix m_matViewInv = MainCam->GetViewMatInv();
	PROJ_TYPE m_ProjType = MainCam->GetProjType();
	float m_fScale = MainCam->GetScale();

	// SwapChain 타겟의 ViewPort 정보
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

	// 카메라의 중심 좌표
	Vec2 vCenterPos = Vec2(tVP.Width / 2.0f, tVP.Height / 2.0f);
	
	tRay m_ray;
	

	if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
	{
		// 직선은 카메라의 좌표를 반드시 지난다.
		m_ray.vStart = Transform()->GetWorldPos();

		// view space 에서의 방향
		m_ray.vDir.x = ((((vCenterPos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
		m_ray.vDir.y = (-(((vCenterPos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
		m_ray.vDir.z = 1.f;

		// world space 에서의 방향
		m_ray.vDir = XMVector3TransformNormal(m_ray.vDir, m_matViewInv);
		m_ray.vDir.Normalize();
	}

	else if (m_ProjType == PROJ_TYPE::ORTHOGRAPHIC)
	{

		// Add앞에 -를 붙여야 하는 이유는 DirectX의 y축과 화면에서의 y축의 증가 방향이
		// 반대이기 때문이다.
		m_ray.vStart = Transform()->GetWorldPos();
		m_ray.vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	}

	return m_ray;
}
