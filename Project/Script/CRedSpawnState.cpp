#include "pch.h"
#include "CRedSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CRedScript.h"

CRedSpawnState::CRedSpawnState()
{
}

CRedSpawnState::~CRedSpawnState()
{

}
void CRedSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_red\\sru_red_spawn");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_red\\sru_red_spawn", false, false, false, 0.f);
	CJungleSpawnState::Enter();
}

void CRedSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CRedSpawnState::Exit()
{
	CRedScript* script = GetOwner()->GetScript<CRedScript>();
	script->SetAggroRange(75.f);
	script->SetAttackRange(45.f);

	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D) == nullptr) {
		GetOwner()->AddComponent(new CCollider3D);
		GetOwner()->Collider3D()->SetAbsolute(true);
		GetOwner()->Collider3D()->SetDrawCollision(false);
		GetOwner()->Collider3D()->SetOffsetScale(Vec3(65.f, 65.f, 65.f));
	}

	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) == nullptr) {
		GetOwner()->AddComponent(new CCollider2D);
		GetOwner()->Collider2D()->SetAbsolute(true);
		GetOwner()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		GetOwner()->Collider2D()->SetOffsetScale(Vec2(80.f, 80.f));
	}
	

	CJungleSpawnState::Exit();
}