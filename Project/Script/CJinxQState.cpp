#include "pch.h"
#include "CJinxQState.h"
#include "CChampionScript.h"
#include "CSkill.h"
#include "CJinxScript.h"

CJinxQState::CJinxQState()
{
}

CJinxQState::~CJinxQState()
{
}

void CJinxQState::tick()
{
	CChampionSkillState::tick();
}

void CJinxQState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* JinxQ = GetOwner()->GetScript<CChampionScript>()->GetSkill(1);
	JinxQ->SetUserObj(m_UserObj);
	JinxQ->SetTargetObj(m_UserObj);
	JinxQ->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	JinxQ->Use();

	
	// 애니메이션
	wstring animName = L"";
	if(GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\minigun_spell1_weapon2";
	else if(GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\launcher_spell1_weapon";

	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);

	// 징크스 총 바꾸는 소리
	Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\JINX_Change_Gun.mp3", 1, 0.5f, true, 100.f, JinxPos, Faction::NONE);
}

void CJinxQState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxQState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
