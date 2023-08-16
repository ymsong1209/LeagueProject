#include "pch.h"
#include "CRazorBeakAttackState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakScript.h"

CRazorBeakAttackState::CRazorBeakAttackState()
{
}

CRazorBeakAttackState::~CRazorBeakAttackState()
{
}

void CRazorBeakAttackState::Enter()
{
	
	wstring basename = L"RazorBeak\\Attack1";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);

	Vec3 RazorbeakPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\razorbeak\\attack.mp3", 1, 0.3f, true, 120.f, RazorbeakPos, Faction::NONE);
	CJungleAttackState::Enter();
}

void CRazorBeakAttackState::tick()
{
	CJungleAttackState::tick();
}


void CRazorBeakAttackState::Exit()
{
	CJungleAttackState::Exit();
}
