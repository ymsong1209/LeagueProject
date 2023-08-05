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
}

void CRazorBeakMiniSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CRazorBeakMiniSpawnState::Exit()
{
	CRazorBeakMiniScript* script = GetOwner()->GetScript<CRazorBeakMiniScript>();
	script->SetAggroRange(75.f);

	GetOwner()->AddComponent(new CCollider3D);
	GetOwner()->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetAbsolute(true);
	GetOwner()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	GetOwner()->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));

	CJungleSpawnState::Exit();
}