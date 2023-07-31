#include "pch.h"
#include "CState.h"

#include "CFsm.h"

CState::CState()
	: m_pOwnerFSM(nullptr)
{
}

CState::~CState()
{
}

CGameObject* CState::GetOwner()
{
	return m_pOwnerFSM->GetOwner();
}
