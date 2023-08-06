#include "pch.h"
#include "CGrompAlertState.h"
#include <Engine/CAnim3D.h>
#include "CGrompScript.h"
CGrompAlertState::CGrompAlertState()
	: m_iAggroAnimNum(1)
{
}

CGrompAlertState::~CGrompAlertState()
{

}
void CGrompAlertState::Enter()
{
	m_iAggroAnimNum = 1;
	wstring basename = L"gromp\\_idle1_aggro";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.0f);
	CJungleAlertState::Enter();
}

void CGrompAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 4) m_iAggroAnimNum = 1;

		wstring basename = L"gromp\\_idle1_aggro";
		if (m_iAggroAnimNum != 1) {
			basename += std::to_wstring(m_iAggroAnimNum);
		}

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false,false, false, 0.0f);
	}
	CJungleAlertState::tick();
}

void CGrompAlertState::Exit()
{
	CJungleAlertState::Exit();
}


