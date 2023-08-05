#include "pch.h"
#include "CKrugAlertState.h"
#include <Engine/CAnim3D.h>

CKrugAlertState::CKrugAlertState()
	: m_iAggroAnimNum(1)
{
}

CKrugAlertState::~CKrugAlertState()
{

}
void CKrugAlertState::Enter()
{
	m_iAggroAnimNum = 1;

	wstring basename = L"Krug\\krug_idle_aggro";
	GetOwner()->Animator3D()->PlayOnce(basename);

	CJungleAlertState::Enter();
}

void CKrugAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 4) m_iAggroAnimNum = 1;

		wstring basename = L"Krug\\krug_idle_aggro";
		if (m_iAggroAnimNum != 1) {
			basename += std::to_wstring(m_iAggroAnimNum);
		}

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
	}
	CJungleAlertState::tick();
}

void CKrugAlertState::Exit()
{
	CJungleAlertState::Exit();
}
