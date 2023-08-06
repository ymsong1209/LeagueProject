#include "pch.h"
#include "CRazorBeakSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakScript.h"

CRazorBeakSpawnState::CRazorBeakSpawnState()
{
}

CRazorBeakSpawnState::~CRazorBeakSpawnState()
{

}
void CRazorBeakSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak\\Spawn");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak\\Spawn", false, false, false, 0.f);
}

void CRazorBeakSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CRazorBeakSpawnState::Exit()
{
	CRazorBeakScript* script = GetOwner()->GetScript<CRazorBeakScript>();
	script->SetAggroRange(85.f);

	GetOwner()->AddComponent(new CCollider3D);
	GetOwner()->Collider3D()->SetAbsolute(true);
	GetOwner()->Collider3D()->SetDrawCollision(false);
	GetOwner()->Collider3D()->SetOffsetScale(Vec3(25.f, 25.f, 25.f));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetAbsolute(true);
	GetOwner()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	GetOwner()->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));

	CJungleSpawnState::Exit();
}