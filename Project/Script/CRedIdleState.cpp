#include "pch.h"
#include "CRedIdleState.h"
#include <Engine/CAnim3D.h>
CRedIdleState::CRedIdleState()
	:m_iIdleAnimNum(1)
{
}

CRedIdleState::~CRedIdleState()
{
}

void CRedIdleState::Enter()
{
	m_iIdleAnimNum = 1;
	GetOwner()->Animator3D()->PlayOnce(L"jungle_red\\sru_red_idle1");
}

void CRedIdleState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iIdleAnimNum;
		if (m_iIdleAnimNum > 2) m_iIdleAnimNum = 1;
		wstring basestring = L"jungle_red\\sru_red_idle";
		basestring += std::to_wstring(m_iIdleAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basestring);
	}
	CJungleIdleState::tick();
}

void CRedIdleState::Exit()
{
	m_iIdleAnimNum = 1;
	CJungleIdleState::Exit();
}
