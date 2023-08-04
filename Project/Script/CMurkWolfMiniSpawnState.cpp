#include "pch.h"
#include "CMurkWolfMiniSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfMiniScript.h"

CMurkWolfMiniSpawnState::CMurkWolfMiniSpawnState()
{
}

CMurkWolfMiniSpawnState::~CMurkWolfMiniSpawnState()
{

}
void CMurkWolfMiniSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf_Mini\\sru_murkwolfmini_spawn_left");
}

void CMurkWolfMiniSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CMurkWolfMiniSpawnState::Exit()
{
	CMurkWolfMiniScript* script = GetOwner()->GetScript<CMurkWolfMiniScript>();
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



