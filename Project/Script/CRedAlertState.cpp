#include "pch.h"
#include "CRedAlertState.h"
#include <Engine/CAnim3D.h>

CRedAlertState::CRedAlertState()
	: m_iAggroAnimNum(1)
{
}

CRedAlertState::~CRedAlertState()
{

}
void CRedAlertState::Enter()
{
	m_iAggroAnimNum = 1;
	wstring basename = L"jungle_red\\sru_red_idle1_aggro";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAlertState::Enter();
}

void CRedAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 2) m_iAggroAnimNum = 1;

		wstring basename = L"jungle_red\\sru_red_idle1_aggro";
		if (m_iAggroAnimNum == 2) {
			basename += std::to_wstring(m_iAggroAnimNum);
		}
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
	}
	CJungleAlertState::tick();
}

void CRedAlertState::Exit()
{
	CJungleAlertState::Exit();
}