#include "pch.h"
#include "CMalphiteIdleState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

#include "CSendServerEventMgr.h"
#include "CUnitScript.h"
CMalphiteIdleState::CMalphiteIdleState()
	:m_iIdleAnimNum(1)
{
	SetName(L"Idle");
}

CMalphiteIdleState::~CMalphiteIdleState()
{
}


void CMalphiteIdleState::Enter()
{
	m_iIdleAnimNum = 1;

	wstring baseName = L"Malphite\\Idle";
	baseName += std::to_wstring(m_iIdleAnimNum);
	GetOwner()->Animator3D()->PlayOnce(baseName, true, 0.5f);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, baseName, false, false, true, 0.5f);

	CChampionIdleState::Enter();
}

void CMalphiteIdleState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iIdleAnimNum;
		wstring baseName = L"Malphite\\Idle";
		baseName += std::to_wstring(m_iIdleAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(baseName);

		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, baseName, false, false, false, 0.f);
	}
}


void CMalphiteIdleState::Exit()
{
	m_iIdleAnimNum = 1;

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CChampionIdleState::Exit();
}

void CMalphiteIdleState::HandleEvent(CGameEvent& event)
{
	CChampionIdleState::HandleEvent(event);
}
