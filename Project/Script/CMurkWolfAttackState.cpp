#include "pch.h"
#include "CMurkWolfAttackState.h"
#include <Engine/CAnim3D.h>

CMurkWolfAttackState::CMurkWolfAttackState()
	: m_iAttackAnimNum(1)
{
}

CMurkWolfAttackState::~CMurkWolfAttackState()
{
}

void CMurkWolfAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 2) m_iAttackAnimNum = 1;

	wstring basename = L"MurkWolf\\sru_murkwolf_attack";
	if (m_iAttackAnimNum == 1) {
		basename += std::to_wstring(1);
	}
	else if (m_iAttackAnimNum == 2) {
		basename += std::to_wstring(4);
	}

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAttackState::Enter();
}

void CMurkWolfAttackState::tick()
{
	CJungleAttackState::tick();
}


void CMurkWolfAttackState::Exit()
{
	CJungleAttackState::Exit();
}



