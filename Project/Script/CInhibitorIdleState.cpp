#include "pch.h"
#include "CInhibitorIdleState.h"
#include "CInhibitorScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CInhibitorIdleState::CInhibitorIdleState()
{
}

CInhibitorIdleState::~CInhibitorIdleState()
{
}

void CInhibitorIdleState::tick()
{
	// 죽으면 Broken상태로 전환
	if (GetOwnerFSM()->GetOwner()->GetScript<CInhibitorScript>()->GetCurHP() <= 0)
	{
		GetOwnerFSM()->ChangeState(L"Broken");
		GetOwnerFSM()->GetOwner()->GetScript<CInhibitorScript>()->SetUnitDead(true);
	}
}

void CInhibitorIdleState::Enter()
{
	CUnitState::Enter();

	CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();

	// 재질 설정
	if (InhibitorScript->GetFaction() == Faction::RED)
	{
		// Red 재질 설정
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);

		// 재질 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_red_Mtrl.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	}
	else
	{
		// Red 재질 설정
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_Mtrl.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);

		// 재질 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_blue_Mtrl.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	}

	// 애니메이션 재생
	GetOwner()->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f, 0.2f);

	// 애니메이션 패킷 전송
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001"
		, true
		, true
		, true
		, 0.1f
		, 0.2f);
}

void CInhibitorIdleState::Exit()
{
	CUnitState::Exit();
}

void CInhibitorIdleState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	if (GetOwner()->GetScript<CUnitScript>()->IsUnitDead())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::GET_HIT:
	{
		GetHitEvent* HitEvent = dynamic_cast<GetHitEvent*>(&event);

		// 맞은 타겟이 본인인 경우에만 이벤트에 반응
		if (HitEvent->GetTargetObj() == GetOwner())
		{
			CGameObject* SkillUser = HitEvent->GetUserObj();
			CGameObject* SkillTarget = HitEvent->GetTargetObj();
			SkillType skilltype = HitEvent->GetSkillType();
			int	skillLevel = HitEvent->GetSkillLevel();

			// 기본 공격에만 HP 감소
						// 기본 공격에만 HP 감소
			if (skilltype == SkillType::BASIC_ATTACK
				|| skilltype == SkillType::BASIC_RANGED_ATTACK
				|| skilltype == SkillType::JINX_BASIC_ATTACK
				|| skilltype == SkillType::MALPHITE_BASIC_ATTACK)
				GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetHit(skilltype, SkillTarget, SkillUser, skillLevel);
		}
	}
	break;
	}
}

