#include "pch.h"
#include "CJinxEState.h"
#include "CChampionScript.h"
#include "CSkill.h"
#include "CJinxScript.h"
#include <thread>
CJinxEState::CJinxEState()
{
	SetName(L"E");
}

CJinxEState::~CJinxEState()
{
}

void CJinxEState::tick()
{
	CChampionSkillState::tick();
}

void CJinxEState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* JinxE = GetOwner()->GetScript<CChampionScript>()->GetSkill(3);
	JinxE->SetUserObj(m_UserObj);
	JinxE->SetTargetObj(m_TargetObj);
	JinxE->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	thread t([=]() {
		Sleep(300);
	JinxE->Use();
		});
	t.detach();


	// 애니메이션
	wstring animName = L"";
	if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\Spell3";
	else if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\jinx_rlauncher_spell3";
	
	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);

	// 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	// 가야할 방향 구하기
	Vec3 Dir = (JinxE->GetMousePos() - CurPos).Normalize();

	float targetYaw = atan2f(-Dir.x, -Dir.z);
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, targetYaw, 0.f));

	if (m_iJinxSoundNum == 1) {
		Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\jinx_champ_E1.wav", 1, 0.5f, true, 100.f, JinxPos, Faction::NONE);
		m_iJinxSoundNum = 2;
	}
	else {
		Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\jinx_champ_E2.wav", 1, 0.5f, true, 100.f, JinxPos, Faction::NONE);
		m_iJinxSoundNum = 1;
	}
	

}

void CJinxEState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxEState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
