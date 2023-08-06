#include "pch.h"
#include "CRazorBeakMiniAlertState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniAlertState::CRazorBeakMiniAlertState()
	: m_iAggroAnimNum(1)
{
}

CRazorBeakMiniAlertState::~CRazorBeakMiniAlertState()
{

}
void CRazorBeakMiniAlertState::Enter()
{
	m_iAggroAnimNum = 1;
	wstring basename = L"RazorBeak_Mini\\Idle_Aggro";
	basename += std::to_wstring(m_iAggroAnimNum);
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	CJungleAlertState::Enter();
}

void CRazorBeakMiniAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 3) m_iAggroAnimNum = 1;

		wstring basename = L"RazorBeak_Mini\\Idle_Aggro";
		basename += std::to_wstring(m_iAggroAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	}
	CJungleAlertState::tick();
}

void CRazorBeakMiniAlertState::Exit()
{
	CJungleAlertState::Exit();
}