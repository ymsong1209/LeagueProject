#include "pch.h"
#include "CState.h"

#include "CFsm.h"

CState::CState()
	: m_pOwnerFSM(nullptr)
	, m_bActive(false)
{
}

CState::~CState()
{
}

void CState::Enter()
{
	m_bActive = true;
}

void CState::Exit()
{
	m_bActive = false;
}

CGameObject* CState::GetOwner()
{
	return m_pOwnerFSM->GetOwner();
}
