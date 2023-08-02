#include "pch.h"
#include "CUnitState.h"
#include <Engine\CFsm.h>
#include <Engine\CRenderMgr.h>
#include "CUnitScript.h"
#include "CSendServerEventMgr.h"

CUnitState::CUnitState()
	: m_Listener(new CUnitScriptEventListener(this))
{
	CGameEventMgr::GetInst()->AddListener(m_Listener);
}

CUnitState::~CUnitState()
{
}

void CUnitState::tick()
{
}

void CUnitState::Enter()
{
	CState::Enter();
}

void CUnitState::Exit()
{
	CState::Exit();
}

Vec3 CUnitState::GetMousePos()
{
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = MainCam->GetRay();

	CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
	IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 MousePos = result.vCrossPoint;	// 마우스 좌표

	return MousePos;
}

Vec3 CUnitState::GetMouseDir()
{
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = MainCam->GetRay();

	CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
	IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;	// 마우스 좌표

	// 두 좌표 사이의 방향을 구함
	Vec3 MyPos = GetOwnerFSM()->Transform()->GetRelativePos();

	Vec3 MouseDir = (TargetPos - MyPos).Normalize();
	return MouseDir;
}
