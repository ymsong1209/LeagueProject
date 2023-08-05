#include "pch.h"
#include "CRazorBeakAlertState.h"
#include <Engine/CAnim3D.h>

CRazorBeakAlertState::CRazorBeakAlertState()
	: m_iAggroAnimNum(1)
{
}

CRazorBeakAlertState::~CRazorBeakAlertState()
{

}
void CRazorBeakAlertState::Enter()
{
	m_iAggroAnimNum = 1;
	CJungleAlertState::Enter();
}

void CRazorBeakAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 3) m_iAggroAnimNum = 1;

		wstring basename = L"RazorBeak\\Idle_Aggro";
		if (m_iAggroAnimNum == 1) {
			basename += L"1_model.002";
		}
		else {
			basename += std::to_wstring(m_iAggroAnimNum);
		}
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
	}
	CJungleAlertState::tick();
}

void CRazorBeakAlertState::Exit()
{
	CJungleAlertState::Exit();
}