#include "pch.h"
#include "CInGameCameraScript.h"


#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CMRT.h>



CInGameCameraScript::CInGameCameraScript()
	: CScript((UINT)SCRIPT_TYPE::INGAMECAMERASCRIPT)
	, m_fCamSpeed(500.f)
	, m_iLeftXLimit(100.f)
	, m_iRightXLimit(100.f)
	, m_iUpYLimit(100.f)
	, m_iDownYLimit(100.f)
	
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fCamSpeed, "Cam     Speed");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iLeftXLimit, "Left	 X Limit");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRightXLimit, "Right X Limit");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iUpYLimit, "Left  Y Limit");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iDownYLimit, "Right Y Limit");
}

CInGameCameraScript::~CInGameCameraScript()
{
	// Window Clipping된 것 해제
	ClipCursor(NULL);
}


void CInGameCameraScript::begin()
{
	RECT WindowPos;
	HWND WindowHandle = CEngine::GetInst()->GetInst()->GetMainWnd();
	GetWindowRect(WindowHandle, &WindowPos);

	int topMenuBarHeight = GetSystemMetrics(SM_CYCAPTION);


	Vec2 WindowResolution = CEngine::GetInst()->GetWindowResolution();

	RECT rect;
	long dx = 10;
	rect.left = WindowPos.left + dx;
	rect.top = WindowPos.top + topMenuBarHeight + dx;

	rect.right = WindowPos.left + WindowResolution.x;
	rect.bottom = WindowPos.top + WindowResolution.y + dx;

	ClipCursor(&rect);
}

void CInGameCameraScript::tick()
{
	

	// 지금 tick이 돌아버리면 작업하기 불편하기 때문에 tick()을 돌리지 않는다

	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 WindowResolution = CEngine::GetInst()->GetWindowResolution();

	Vec3 RightDir = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 UpDir = GetOwner()->Transform()->GetRelativeDir(DIR_TYPE::UP);

	float XMoveAmount = 0;
	float YMoveAmount = 0;

	Vec3 RightMoveAmount = Vec3(0.f, 0.f, 0.f);
	Vec3 UpMoveAmount = Vec3(0.f, 0.f, 0.f);

	CCamera* TargetCamera = CRenderMgr::GetInst()->GetMainCam();
	Vec3 CamPos = TargetCamera->Transform()->GetRelativePos();

	Matrix CamWorld = TargetCamera->Transform()->GetWorldMat();

	//원래는 Camera의 정중앙에 Listener를 배치하고 싶었지만, 왠지는 모르겠지만 마우스 포인터를 따라감
	//임시로 챔피언을 기준으로 소리가 나게 설정함
	CGameObject* LoLMap = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
	if (LoLMap != nullptr) {
		//tRay ray = CalcRay();
		tRay ray;
		ray.vDir = TargetCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		ray.vStart = TargetCamera->Transform()->GetWorldPos();
		IntersectResult result = TargetCamera->IsCollidingBtwRayRect(ray, LoLMap);
		Vec3 EndPos = result.vCrossPoint;
		if (result.bResult) {
			CamPos = Vec3(EndPos.x, EndPos.y, EndPos.z);
		}
		else {
			CamPos = TargetCamera->Transform()->GetRelativePos();
			CamPos.y = 0.f;
		}

	}
	else {
		CamPos.y = 0.f;
	}
	
	// 임시로 짜둠. 챔피언이 있으면 listener는 챔피언 기준으로 하게 됨
	if (m_pTargetObject) {
		CamPos = m_pTargetObject->Transform()->GetWorldPos();
	}


	Vec3 CamForward = TargetCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 CamUp = TargetCamera->Transform()->GetWorldDir(DIR_TYPE::UP);
	CSound::UpdateListenerAttributes(CamPos, CamForward, CamUp);


	// 스페이스를 누르면 케릭터의 중심으로 돌아가도록 한다
	if (CKeyMgr::GetInst()->GetKeyState(KEY::SPACE) == KEY_STATE::TAP || CKeyMgr::GetInst()->GetKeyState(KEY::SPACE) == KEY_STATE::PRESSED)
	{
		Vec3 TargetObjPos = m_pTargetObject->Transform()->GetRelativePos();

		TargetCamera->Transform()->SetRelativePos(TargetObjPos.x, TargetCamera->Transform()->GetRelativePos().y, TargetObjPos.z - WindowResolution.y / 4.f);
	}


	// 화면 안에 들어왔는지 먼저 판별 (화면 안에 마우스가 들어와야지 마우스로 카메라 이동이 가능하니까)
	else if (0 <= MousePos.x && MousePos.x <= WindowResolution.x && 0 <= MousePos.y && MousePos.y <= WindowResolution.y)
	{
		if (MousePos.x <= m_iLeftXLimit)
		{
			if (MousePos.y <= m_iUpYLimit)
			{
				RightMoveAmount -= (m_fCamSpeed * RightDir * EditorDT * cos(XM_PI / 4.f));
				UpMoveAmount += (m_fCamSpeed * UpDir * EditorDT * cos(XM_PI / 4.f));
			}

			else if (MousePos.y >= WindowResolution.y - m_iDownYLimit)
			{

				RightMoveAmount -= (m_fCamSpeed * RightDir * EditorDT * cos(XM_PI / 4.f));
				UpMoveAmount -= (m_fCamSpeed * UpDir * EditorDT * cos(XM_PI / 4.f));
			}
			else
			{
				RightMoveAmount -= m_fCamSpeed * RightDir * EditorDT;
			}
		}


		else if (MousePos.x >= WindowResolution.x - m_iRightXLimit)
		{
			if (MousePos.y <= m_iUpYLimit)
			{
				RightMoveAmount += (m_fCamSpeed * RightDir * EditorDT * cos(XM_PI / 4.f));
				UpMoveAmount += (m_fCamSpeed * UpDir * EditorDT * cos(XM_PI / 4.f));
			}

			else if (MousePos.y >= WindowResolution.y - m_iDownYLimit)
			{
				RightMoveAmount += (m_fCamSpeed * RightDir * EditorDT * cos(XM_PI / 4.f));
				UpMoveAmount -= (m_fCamSpeed * UpDir * EditorDT * cos(XM_PI / 4.f));
			}

			else
			{
				RightMoveAmount += m_fCamSpeed * RightDir * EditorDT;
			}

		}

		else if (MousePos.y <= m_iUpYLimit)
			UpMoveAmount += (m_fCamSpeed * UpDir * EditorDT);

		else if (MousePos.y >= WindowResolution.y - m_iDownYLimit)
			UpMoveAmount -= (m_fCamSpeed * UpDir * EditorDT);
	}

	
	Vec3 PreviousPos =  TargetCamera->Transform()->GetRelativePos();
	TargetCamera->Transform()->SetRelativePos(PreviousPos.x + RightMoveAmount.x + UpMoveAmount.x,
											 526.f,
											  PreviousPos.z + RightMoveAmount.z + UpMoveAmount.z);

	CGameObject* DirLight = nullptr;

	DirLight = CLevelMgr::GetInst()->FindObjectByName(L"Directional Light");

	if (DirLight != nullptr)
	{
		DirLight->Transform()->SetRelativePos(PreviousPos.x + RightMoveAmount.x + UpMoveAmount.x,
			526.f,
			PreviousPos.z + RightMoveAmount.z + UpMoveAmount.z);
	}

}

tRay CInGameCameraScript::CalcRay()
{
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	Matrix m_matProj = MainCam->GetProjMat();
	Matrix m_matViewInv = MainCam->GetViewMatInv();
	PROJ_TYPE m_ProjType = MainCam->GetProjType();
	float m_fScale = MainCam->GetScale();

	// SwapChain 타겟의 ViewPort 정보
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

	
	RECT rect;
	GetWindowRect(CEngine::GetInst()->GetMainWnd(), &rect);

	int windowCenterX = (rect.left + rect.right) / 2;
	int windowCenterY = (rect.top + rect.bottom) / 2;

	// 카메라의 중심 좌표
	Vec2 vCenterPos = Vec2(windowCenterX, windowCenterY);


	tRay m_ray;


	if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
	{
		// 직선은 카메라의 좌표를 반드시 지난다.
		m_ray.vStart = MainCam->Transform()->GetWorldPos();

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
