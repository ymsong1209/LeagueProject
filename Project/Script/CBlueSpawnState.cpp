#include "pch.h"
#include "CBlueSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CBlueScript.h"

CBlueSpawnState::CBlueSpawnState()
{
}

CBlueSpawnState::~CBlueSpawnState()
{

}
void CBlueSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_blue\\sru_blue_spawn");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_blue\\sru_blue_spawn", false, false, false, 0.f);

	Vec3 BluePos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\blue\\death.mp3", 1, 0.5f, true, 200.f, BluePos, Faction::NONE);
	CJungleSpawnState::Enter();
}

void CBlueSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CBlueSpawnState::Exit()
{
	CBlueScript* script = GetOwner()->GetScript<CBlueScript>();
	script->SetAggroRange(85.f);
	script->SetAttackRange(35.f);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D) == nullptr) {
		GetOwner()->AddComponent(new CCollider3D);
		GetOwner()->Collider3D()->SetAbsolute(true);
		GetOwner()->Collider3D()->SetDrawCollision(false);
		GetOwner()->Collider3D()->SetOffsetScale(Vec3(55.f, 55.f, 55.f));
	}
	
	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) == nullptr) {
		GetOwner()->AddComponent(new CCollider2D);
		GetOwner()->Collider2D()->SetAbsolute(true);
		GetOwner()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		GetOwner()->Collider2D()->SetOffsetScale(Vec2(60.f, 60.f));
	}

	CJungleSpawnState::Exit();
}