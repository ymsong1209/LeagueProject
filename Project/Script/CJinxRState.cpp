#include "pch.h"
#include "CJinxRState.h"
#include "CChampionScript.h"
#include "CSkill.h"
#include "CJinxScript.h"
#include <thread>
#include <Engine/CRenderMgr.h>

CJinxRState::CJinxRState()
{
	SetName(L"R");
}

CJinxRState::~CJinxRState()
{
}

void CJinxRState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* JinxR = GetOwner()->GetScript<CChampionScript>()->GetSkill(4);
	JinxR->SetUserObj(m_UserObj);
	JinxR->SetTargetObj(m_TargetObj);
	JinxR->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	thread t([=]() {
		Sleep(1000);
	JinxR->Use();
		});
	t.detach();


	// 애니메이션
	wstring animName = L"";
	if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\Spell4";
	else if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\jinx_rlauncher_spell4";

	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);


	// 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	// 가야할 방향 구하기
	Vec3 Dir = (JinxR->GetMousePos() - CurPos).Normalize();

	float targetYaw = atan2f(-Dir.x, -Dir.z);
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, targetYaw, 0.f));

	if (m_iSoundNum == 1) {
		Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\jinx_champ_R1.wav", 1, 0.5f, true, 100.f, JinxPos, Faction::NONE);
		m_iSoundNum = 2;
	}
	else if (m_iSoundNum == 2) {
		Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\jinx_champ_R2.wav", 1, 0.5f, true, 100.f, JinxPos, Faction::NONE);
		m_iSoundNum = 3;
	}
	else {
		Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\jinx_champ_R3.wav", 1, 0.5f, true, 100.f, JinxPos, Faction::NONE);
		m_iSoundNum = 1;
	}

}

void CJinxRState::tick()
{

	CChampionSkillState::tick();
}



void CJinxRState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxRState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
