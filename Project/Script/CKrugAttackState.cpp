#include "pch.h"
#include "CKrugAttackState.h"
#include <Engine/CAnim3D.h>
CKrugAttackState::CKrugAttackState()
	:m_iAttackAnimNum(1)
{
}

CKrugAttackState::~CKrugAttackState()
{
}

void CKrugAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 5) m_iAttackAnimNum = 1;

	wstring basename = L"Krug\\krug_attack";
	basename += std::to_wstring(m_iAttackAnimNum);

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAttackState::Enter();
}

void CKrugAttackState::tick()
{
	CJungleAttackState::tick();
}


void CKrugAttackState::Exit()
{
	CJungleAttackState::Exit();
}


