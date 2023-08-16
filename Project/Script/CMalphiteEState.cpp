#include "pch.h"
#include "CMalphiteEState.h"
#include "CChampionScript.h"
#include "CSkill.h"

CMalphiteEState::CMalphiteEState()
{
	SetName(L"E");
}

CMalphiteEState::~CMalphiteEState()
{
}

void CMalphiteEState::tick()
{
	CChampionSkillState::tick();
}

void CMalphiteEState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* MalphiteE = GetOwner()->GetScript<CChampionScript>()->GetSkill(3);
	MalphiteE->SetUserObj(m_UserObj);
	MalphiteE->SetTargetObj(nullptr);
	MalphiteE->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	MalphiteE->Use();

	// 애니메이션
	wstring animName = L"Malphite\\Spell3";
	GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\malphite\\Malph_E.mp3", 1, 0.4f, true, 100.f, vPos, Faction::NONE);

}

void CMalphiteEState::Exit()
{
	CChampionSkillState::Exit();
}

void CMalphiteEState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
