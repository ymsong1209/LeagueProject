#include "pch.h"
#include "CDragonSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"

CDragonSpawnState::CDragonSpawnState()
{
}

CDragonSpawnState::~CDragonSpawnState()
{

}
void CDragonSpawnState::Enter()
{
	CUnitState::Enter();
	m_bSpawnFinished = false;
	GetOwner()->Animator3D()->PlayOnce(L"Elder_Dragon\\sru_dragon_elder_spawn");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Elder_Dragon\\sru_dragon_elder_spawn", false, false, false, 0.f);

	Vec3 DragonPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\dragon\\spawn.mp3", 1, 0.5f, true, 200.f, DragonPos, Faction::NONE);
}

void CDragonSpawnState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish() && m_bSpawnFinished == false) {
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(L"Elder_Dragon\\sru_dragon_idle_ag2n");
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Elder_Dragon\\sru_dragon_idle_ag2n", false, false, false, 0.f);
		m_bSpawnFinished = true;
	}
	else if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish() && m_bSpawnFinished == true) {
		GetOwner()->Fsm()->ChangeState(L"Idle");
	}
}

void CDragonSpawnState::Exit()
{
	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	script->SetAlertRange(150.f);
	script->SetAggroRange(100.f);

	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D) == nullptr) {
		GetOwner()->AddComponent(new CCollider3D);
		GetOwner()->Collider3D()->SetAbsolute(true);
		GetOwner()->Collider3D()->SetDrawCollision(false);
		GetOwner()->Collider3D()->SetOffsetScale(Vec3(125.f, 125.f, 125.f));
	}
	
	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) == nullptr) {
		GetOwner()->AddComponent(new CCollider2D);
		GetOwner()->Collider2D()->SetAbsolute(true);
		GetOwner()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		GetOwner()->Collider2D()->SetOffsetScale(Vec2(200.f, 200.f));
	}
	
	
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	script->SetSpawnPos(CurPos);
	Vec3 CurRot = GetOwner()->Transform()->GetRelativeRot();
	script->SetSpawnRot(CurRot);
	CUnitState::Exit();
}