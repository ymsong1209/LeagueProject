#include "pch.h"
#include "CGrompAlertState.h"
#include <Engine/CAnim3D.h>

CGrompAlertState::CGrompAlertState()
	: m_iAggroAnimNum(1)
	, m_bEnterAnimPlayed(false)
{
}

CGrompAlertState::~CGrompAlertState()
{

}
void CGrompAlertState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_idle1_n2a", true, 0.5f);
	m_iAggroAnimNum = 1;
}

void CGrompAlertState::tick()
{
	if (m_bEnterAnimPlayed == false && GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Animator3D()->PlayOnce(L"gromp\\_idle1_aggro");
		m_bEnterAnimPlayed = true;
		return;
	}

	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 4) m_iAggroAnimNum = 1;

		wstring basename = L"_idle1_aggro";
		if (m_iAggroAnimNum != 1) {
			basename += std::to_wstring(m_iAggroAnimNum);
		}

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
	}
}

void CGrompAlertState::Exit()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_idle1_a2n",true, 0.5f);
}


