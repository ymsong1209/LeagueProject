#include "pch.h"
#include "CRazorBeakIdleState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakScript.h"
CRazorBeakIdleState::CRazorBeakIdleState()
	:m_iIdleAnimNum(1)
{
}

CRazorBeakIdleState::~CRazorBeakIdleState()
{
}

void CRazorBeakIdleState::Enter()
{
	CJungleIdleState::Enter();
	m_iIdleAnimNum = 1;
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak\\sru_razorbeak_idle_normal1");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak\\sru_razorbeak_idle_normal1", false, false, false, 0.f);
}

void CRazorBeakIdleState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iIdleAnimNum;
		if (m_iIdleAnimNum > 5) m_iIdleAnimNum = 1;
		wstring basestring = L"RazorBeak\\sru_razorbeak_idle_normal";
		basestring += std::to_wstring(m_iIdleAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basestring);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basestring, false, false, false, 0.f);
	}
	CJungleIdleState::tick();
}

void CRazorBeakIdleState::Exit()
{
	m_iIdleAnimNum = 1;
	CJungleIdleState::Exit();
}
