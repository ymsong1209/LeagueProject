#include "pch.h"
#include "CKrugMiniSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CKrugMiniScript.h"

CKrugMiniSpawnState::CKrugMiniSpawnState()
{
}

CKrugMiniSpawnState::~CKrugMiniSpawnState()
{

}
void CKrugMiniSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug_Mini\\krug_spawn_mini");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug_Mini\\krug_spawn_mini", false, false, false, 0.f);
}

void CKrugMiniSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CKrugMiniSpawnState::Exit()
{
	CKrugMiniScript* script = GetOwner()->GetScript<CKrugMiniScript>();
	script->SetAggroRange(85.f);
	script->SetAttackRange(20.f);

	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D) == nullptr) {
		GetOwner()->AddComponent(new CCollider3D);
		GetOwner()->Collider3D()->SetAbsolute(true);
		GetOwner()->Collider3D()->SetDrawCollision(false);
		GetOwner()->Collider3D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));
	}
	
	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) == nullptr) {
		GetOwner()->AddComponent(new CCollider2D);
		GetOwner()->Collider2D()->SetAbsolute(true);
		GetOwner()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		GetOwner()->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));
	}
	

	CJungleSpawnState::Exit();
}