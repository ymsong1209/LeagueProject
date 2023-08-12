#include "pch.h"
#include "CJinxWState.h"
#include "CChampionScript.h"
#include "CSkill.h"
#include "CJinxScript.h"

#include <thread>

CJinxWState::CJinxWState()
{
	SetName(L"W");
}

CJinxWState::~CJinxWState()
{
}

void CJinxWState::tick()
{
	CChampionSkillState::tick();
}

void CJinxWState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* JinxW = GetOwner()->GetScript<CChampionScript>()->GetSkill(2);
	JinxW->SetUserObj(m_UserObj);
	JinxW->SetTargetObj(m_TargetObj);
	JinxW->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	
	thread t([=]() {
		Sleep(500);
		JinxW->Use();
		});
	t.detach();


	// 애니메이션
	wstring animName = L"";
	if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\Spell2";
	else if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\jinx_rlauncher_spell2";
	
	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);


	// 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	// 가야할 방향 구하기
	Vec3 Dir = (JinxW->GetMousePos() - CurPos).Normalize();

	float targetYaw = atan2f(-Dir.x, -Dir.z);
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, targetYaw, 0.f));

}

void CJinxWState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxWState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
