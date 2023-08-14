#include "pch.h"
#include "CRazorBeakMiniAttackState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniAttackState::CRazorBeakMiniAttackState()
{
}

CRazorBeakMiniAttackState::~CRazorBeakMiniAttackState()
{
}

void CRazorBeakMiniAttackState::Enter()
{

	wstring basename = L"RazorBeak_Mini\\Attack1";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);

	Vec3 RazorbeakPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\razorbeak_mini\\attack.mp3", 1, 0.5f, true, 120.f, RazorbeakPos, Faction::NONE);
	CJungleAttackState::Enter();
}

void CRazorBeakMiniAttackState::tick()
{
	CJungleAttackState::tick();
}


void CRazorBeakMiniAttackState::Exit()
{
	CJungleAttackState::Exit();
}
