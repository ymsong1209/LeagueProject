#include "pch.h"
#include "CMurkWolfAlertState.h"
#include <Engine/CAnim3D.h>

CMurkWolfAlertState::CMurkWolfAlertState()
	: m_iAggroAnimNum(1)
{
}

CMurkWolfAlertState::~CMurkWolfAlertState()
{

}
void CMurkWolfAlertState::Enter()
{
	m_iAggroAnimNum = 1;
	wstring basename = L"MurkWolf\\sru_murkwolf_idle_aggro";
	basename += std::to_wstring(m_iAggroAnimNum);

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAlertState::Enter();
}

void CMurkWolfAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 3) m_iAggroAnimNum = 1;

		wstring basename = L"MurkWolf\\sru_murkwolf_idle_aggro";
		basename += std::to_wstring(m_iAggroAnimNum);

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
	}
	CJungleAlertState::tick();
}

void CMurkWolfAlertState::Exit()
{
	CJungleAlertState::Exit();
}
