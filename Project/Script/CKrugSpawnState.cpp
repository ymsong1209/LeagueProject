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
}

void CKrugSpawnState::tick()
{

	CJungleSpawnState::tick();

}

void CKrugSpawnState::Exit()
{
	CKrugScript* script = GetOwner()->GetScript<CKrugScript>();
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