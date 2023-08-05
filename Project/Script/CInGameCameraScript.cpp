#include "pch.h"
#include "CInGameCameraScript.h"


#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>



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


	// 화면 안에 들어왔는지 먼저 판별
	if (0 <= MousePos.x && MousePos.x <= WindowResolution.x && 0 <= MousePos.y && MousePos.y <= WindowResolution.y)
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

	CCamera* TargetCamera = CRenderMgr::GetInst()->GetMainCam();
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

