#include "pch.h"
#include "CMurkWolfMiniIdleState.h"
#include <Engine/CAnim3D.h>
CMurkWolfMiniIdleState::CMurkWolfMiniIdleState()
	:m_iIdleAnimNum(1)
{
}

CMurkWolfMiniIdleState::~CMurkWolfMiniIdleState()
{
}

void CMurkWolfMiniIdleState::Enter()
{
	m_iIdleAnimNum = 1;
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolfmini_idle1");
}

void CMurkWolfMiniIdleState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iIdleAnimNum;
		if (m_iIdleAnimNum > 7) m_iIdleAnimNum = 1;
		wstring basestring = L"MurkWolf\\sru_murkwolf_idle";
		basestring += std::to_wstring(m_iIdleAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basestring);
	}
	CJungleIdleState::tick();
}

void CMurkWolfMiniIdleState::Exit()
{
	m_iIdleAnimNum = 1;
	CJungleIdleState::Exit();
}
