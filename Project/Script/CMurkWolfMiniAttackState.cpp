#include "pch.h"
#include "CMurkWolfMiniAttackState.h"
#include <Engine/CAnim3D.h>

CMurkWolfMiniAttackState::CMurkWolfMiniAttackState()
	: m_iAttackAnimNum(1)
{
}

CMurkWolfMiniAttackState::~CMurkWolfMiniAttackState()
{
}

void CMurkWolfMiniAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 2) m_iAttackAnimNum = 1;

	wstring basename = L"MurkWolf_Mini\\sru_murkwolf_attack";
	if (m_iAttackAnimNum == 1) {
		basename += std::to_wstring(2);
	}
	else if (m_iAttackAnimNum == 2) {
		basename += L"3.002";
	}

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAttackState::Enter();
}

void CMurkWolfMiniAttackState::tick()
{
	CJungleAttackState::tick();
}


void CMurkWolfMiniAttackState::Exit()
{
	CJungleAttackState::Exit();
}

