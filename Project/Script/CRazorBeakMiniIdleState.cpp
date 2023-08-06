#include "pch.h"
#include "CRazorBeakMiniIdleState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniIdleState::CRazorBeakMiniIdleState()
	:m_iIdleAnimNum(1)
{
}

CRazorBeakMiniIdleState::~CRazorBeakMiniIdleState()
{
}

void CRazorBeakMiniIdleState::Enter()
{
	m_iIdleAnimNum = 1;
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak_Mini\\Idle_Normal1");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak_Mini\\Idle_Normal1", false, false, false, 0.f);
}

void CRazorBeakMiniIdleState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iIdleAnimNum;
		if (m_iIdleAnimNum > 3) m_iIdleAnimNum = 1;
		wstring basestring = L"RazorBeak_Mini\\Idle_Normal";
		basestring += std::to_wstring(m_iIdleAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basestring);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basestring, false, false, false, 0.f);
	}
	CJungleIdleState::tick();
}

void CRazorBeakMiniIdleState::Exit()
{
	m_iIdleAnimNum = 1;
	CJungleIdleState::Exit();
}
