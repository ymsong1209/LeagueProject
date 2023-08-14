#include "pch.h"
#include "CJinxDeathState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CUnitScript.h"
#include "CJinxScript.h"

CJinxDeathState::CJinxDeathState()
{
	SetName(L"Death");
}

CJinxDeathState::~CJinxDeathState()
{
}

void CJinxDeathState::tick()
{
}

void CJinxDeathState::Enter()
{
	// 애니메이션
	wstring animName = L"";
	if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\Death_model.002";
	else if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\jinx_rlauncher_death";

	GetOwner()->Animator3D()->PlayOnce(animName, false);


	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, false, false, 0.0f);

	Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\jinx_champ_Death.wav", 1, 0.5f, true, 100.f, JinxPos, Faction::NONE);

	CChampionDeathState::Enter();
}  

void CJinxDeathState::Exit()
{
	//GetOwner()->Animator3D()->FindAnim(L"Jinx\\Death_model.002")->Reset();
	CChampionDeathState::Exit();
}

void CJinxDeathState::HandleEvent(CGameEvent& event)
{
	CChampionDeathState::HandleEvent(event);
}
