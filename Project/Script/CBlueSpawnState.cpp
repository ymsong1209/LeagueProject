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
}

void CBlueSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CBlueSpawnState::Exit()
{
	CBlueScript* script = GetOwner()->GetScript<CBlueScript>();
	script->SetAggroRange(85.f);

	GetOwner()->AddComponent(new CCollider3D);
	GetOwner()->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetAbsolute(true);
	GetOwner()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	GetOwner()->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));

	CJungleSpawnState::Exit();
}