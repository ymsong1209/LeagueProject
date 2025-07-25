#include "pch.h"
#include "CKrugMiniAlertState.h"
#include <Engine/CAnim3D.h>
#include "CKrugMiniScript.h"
CKrugMiniAlertState::CKrugMiniAlertState()
	: m_iAggroAnimNum(1)
{
}

CKrugMiniAlertState::~CKrugMiniAlertState()
{

}
void CKrugMiniAlertState::Enter()
{
	m_iAggroAnimNum = 1;
	wstring basename = L"Krug_Mini\\krug_idle_aggro";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	CJungleAlertState::Enter();
}

void CKrugMiniAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 4) m_iAggroAnimNum = 1;

		wstring basename = L"Krug_Mini\\krug_idle_aggro";
		if (m_iAggroAnimNum != 1) {
			basename += std::to_wstring(m_iAggroAnimNum);
		}

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	}
	CJungleAlertState::tick();
}

void CKrugMiniAlertState::Exit()
{
	CJungleAlertState::Exit();
}
