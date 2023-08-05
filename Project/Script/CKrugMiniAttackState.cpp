#include "pch.h"
#include "CKrugMiniAttackState.h"
#include <Engine/CAnim3D.h>
CKrugMiniAttackState::CKrugMiniAttackState()
	:m_iAttackAnimNum(1)
{
}

CKrugMiniAttackState::~CKrugMiniAttackState()
{
}

void CKrugMiniAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 5) m_iAttackAnimNum = 1;

	wstring basename = L"Krug_Mini\\krug_attack";
	basename += std::to_wstring(m_iAttackAnimNum);

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAttackState::Enter();
}

void CKrugMiniAttackState::tick()
{
	CJungleAttackState::tick();
}


void CKrugMiniAttackState::Exit()
{
	CJungleAttackState::Exit();
}
