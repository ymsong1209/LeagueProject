#include "pch.h"
#include "CBlueAlertState.h"
#include <Engine/CAnim3D.h>

CBlueAlertState::CBlueAlertState()
	: m_iAggroAnimNum(1)
{
}

CBlueAlertState::~CBlueAlertState()
{

}
void CBlueAlertState::Enter()
{
	m_iAggroAnimNum = 1;
	CJungleAlertState::Enter();
}

void CBlueAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 3) m_iAggroAnimNum = 1;

		wstring basename = L"jungle_blue\\sru_blue_idle_aggro";
		if (m_iAggroAnimNum != 1) {
			basename += std::to_wstring(m_iAggroAnimNum);
		}

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
	}
	CJungleAlertState::tick();
}

void CBlueAlertState::Exit()
{
	CJungleAlertState::Exit();
}
