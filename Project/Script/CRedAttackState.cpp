#include "pch.h"
#include "CRedAttackState.h"
#include <Engine/CAnim3D.h>
#include "CRedScript.h"
#include "CSkill.h"

CRedAttackState::CRedAttackState()
	: m_iAttackAnimNum(1)
{
}

CRedAttackState::~CRedAttackState()
{
}

void CRedAttackState::Enter()
{
	++m_iAttackAnimNum;
	if (m_iAttackAnimNum > 3) m_iAttackAnimNum = 1;
	wstring basename = L"jungle_red\\Attack";
	basename += std::to_wstring(m_iAttackAnimNum);

	
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);

	Vec3 RedPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\red\\attack.mp3", 1, 0.5f, true, 120.f, RedPos, Faction::NONE);
	CJungleAttackState::Enter();
}

void CRedAttackState::tick()
{
	CJungleAttackState::tick();
}


void CRedAttackState::Exit()
{
	CJungleAttackState::Exit();
}
