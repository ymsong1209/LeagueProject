#include "pch.h"
#include "CMalphiteDeathState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CUnitScript.h"

CMalphiteDeathState::CMalphiteDeathState()
{
	SetName(L"Death");
}

CMalphiteDeathState::~CMalphiteDeathState()
{
}

void CMalphiteDeathState::tick()
{
}

void CMalphiteDeathState::Enter()
{
	wstring animName = L"Malphite\\Death";
	GetOwner()->Animator3D()->PlayOnce(animName, false);


	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, false, false, 0.0f);

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\malphite\\Malph_Death.wav", 1, 0.4f, true, 100.f, vPos, Faction::NONE);

	CChampionDeathState::Enter();
}

void CMalphiteDeathState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CChampionDeathState::Exit();
}

void CMalphiteDeathState::HandleEvent(CGameEvent& event)
{
	CChampionDeathState::HandleEvent(event);
}
