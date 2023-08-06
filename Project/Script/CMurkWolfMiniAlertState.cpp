#include "pch.h"
#include "CMurkWolfMiniAlertState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfMiniScript.h"

CMurkWolfMiniAlertState::CMurkWolfMiniAlertState()
	: m_iAggroAnimNum(1)
{
}

CMurkWolfMiniAlertState::~CMurkWolfMiniAlertState()
{

}
void CMurkWolfMiniAlertState::Enter()
{
	m_iAggroAnimNum = 2;
	wstring basename = L"MurkWolf_Mini\\sru_murkwolfmini_idle_aggro";
	basename += std::to_wstring(m_iAggroAnimNum);

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	CJungleAlertState::Enter();
}

void CMurkWolfMiniAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 3) m_iAggroAnimNum = 2;

		wstring basename = L"MurkWolf_Mini\\sru_murkwolfmini_idle_aggro";
		basename += std::to_wstring(m_iAggroAnimNum);

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	}
	CJungleAlertState::tick();
}

void CMurkWolfMiniAlertState::Exit()
{
	CJungleAlertState::Exit();
}