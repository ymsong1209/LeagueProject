#include "pch.h"
#include "CRedAttackState.h"
#include <Engine/CAnim3D.h>

CRedAttackState::CRedAttackState()
	: m_iAttackAnimNum(1)
{
}

CRedAttackState::~CRedAttackState()
{
}

void CRedAttackState::Enter()
{
	m_iAttackAnimNum = 1;
	wstring basename = L"jungle_red\\Attack1";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAttackState::Enter();
}

void CRedAttackState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAttackAnimNum;
		if (m_iAttackAnimNum > 3) m_iAttackAnimNum = 1;

		wstring basename = L"jungle_red\\Attack";
		basename += std::to_wstring(m_iAttackAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
	}

	CJungleAttackState::tick();
}


void CRedAttackState::Exit()
{
	CJungleAttackState::Exit();
}
