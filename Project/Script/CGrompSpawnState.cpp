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
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_spawn", false, false, false, 0.0f);

	Vec3 GrompPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\gromp\\spawn.mp3", 1, 0.3f, true, 130.f, GrompPos, Faction::NONE);
	CJungleSpawnState::Enter();
}

void CGrompSpawnState::tick()
{

	CJungleSpawnState::tick();
	
}

void CGrompSpawnState::Exit()
{
	CGrompScript* script = GetOwner()->GetScript<CGrompScript>();
	script->SetAggroRange(75.f);
	script->SetAttackRange(45.f);
	
	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D) == nullptr) {
		GetOwner()->AddComponent(new CCollider3D);
		GetOwner()->Collider3D()->SetAbsolute(true);
		GetOwner()->Collider3D()->SetDrawCollision(false);
		GetOwner()->Collider3D()->SetOffsetScale(Vec3(50.f, 50.f, 50.f));
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




