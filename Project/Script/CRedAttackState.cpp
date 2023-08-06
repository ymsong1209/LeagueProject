#include "pch.h"
#include "CRedAttackState.h"
#include <Engine/CAnim3D.h>
#include "CRedScript.h"

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
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
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
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
	}

	CJungleAttackState::tick();
}


void CRedAttackState::Exit()
{
	CJungleAttackState::Exit();
}
