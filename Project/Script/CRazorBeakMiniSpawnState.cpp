#include "pch.h"
#include "CRazorBeakMiniSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniSpawnState::CRazorBeakMiniSpawnState()
{
}

CRazorBeakMiniSpawnState::~CRazorBeakMiniSpawnState()
{

}
void CRazorBeakMiniSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak_Mini\\Spawn2");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak_Mini\\Spawn2", false, false, false, 0.f);
}

void CRazorBeakMiniSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CRazorBeakMiniSpawnState::Exit()
{
	CRazorBeakMiniScript* script = GetOwner()->GetScript<CRazorBeakMiniScript>();
	script->SetAggroRange(75.f);
	script->SetAttackRange(25.f);

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