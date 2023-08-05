#include "pch.h"
#include "CMurkWolfSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfScript.h"

CMurkWolfSpawnState::CMurkWolfSpawnState()
{
}

CMurkWolfSpawnState::~CMurkWolfSpawnState()
{

}
void CMurkWolfSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolf_spawn");
}

void CMurkWolfSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CMurkWolfSpawnState::Exit()
{
	CMurkWolfScript* script = GetOwner()->GetScript<CMurkWolfScript>();
	script->SetAggroRange(95.f);

	GetOwner()->AddComponent(new CCollider3D);
	GetOwner()->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetAbsolute(true);
	GetOwner()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	GetOwner()->Collider2D()->SetOffsetScale(Vec2(40.f, 40.f));

	CJungleSpawnState::Exit();
}



