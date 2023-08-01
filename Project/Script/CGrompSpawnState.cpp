#include "pch.h"
#include "CGrompSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CGrompScript.h"

CGrompSpawnState::CGrompSpawnState()
{
}

CGrompSpawnState::~CGrompSpawnState()
{

}
void CGrompSpawnState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_spawn");
}

void CGrompSpawnState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Idle");
	}
}

void CGrompSpawnState::Exit()
{
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	CGrompScript* script = GetOwner()->GetScript<CGrompScript>();
	script->SetAggroRange(300.f);
	script->SetSpawnPos(CurPos);
	GetOwner()->AddComponent(new CCollider3D);
	GetOwner()->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetAbsolute(true);
	GetOwner()->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	GetOwner()->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
}




