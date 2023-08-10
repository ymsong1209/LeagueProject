#include "pch.h"
#include "CKrugSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CKrugScript.h"

CKrugSpawnState::CKrugSpawnState()
{
}

CKrugSpawnState::~CKrugSpawnState()
{

}
void CKrugSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug\\krug_spawn");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug\\krug_spawn", false, false, false, 0.f);
	CJungleSpawnState::Enter();
}

void CKrugSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CKrugSpawnState::Exit()
{
	CKrugScript* script = GetOwner()->GetScript<CKrugScript>();
	script->SetAggroRange(85.f);
	script->SetAttackRange(25.f);

	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D) == nullptr) {
		GetOwner()->AddComponent(new CCollider3D);
		GetOwner()->Collider3D()->SetAbsolute(true);
		GetOwner()->Collider3D()->SetDrawCollision(false);
		GetOwner()->Collider3D()->SetOffsetScale(Vec3(40.f, 40.f, 40.f));
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