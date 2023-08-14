#include "pch.h"
#include "CBlueAttackState.h"
#include <Engine/CAnim3D.h>
#include "CBlueScript.h"
CBlueAttackState::CBlueAttackState()
{
}

CBlueAttackState::~CBlueAttackState()
{
}

void CBlueAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 3) m_iAttackAnimNum = 1;

	wstring basename = L"jungle_blue\\Attack";
	basename += std::to_wstring(m_iAttackAnimNum);

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);

	Vec3 BluePos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\blue\\attack.mp3", 1, 0.5f, true, 130.f, BluePos, Faction::NONE);
	CJungleAttackState::Enter();
}

void CBlueAttackState::tick()
{
	CJungleAttackState::tick();
}


void CBlueAttackState::Exit()
{
	CJungleAttackState::Exit();
}



