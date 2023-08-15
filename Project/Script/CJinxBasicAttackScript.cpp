#include "pch.h"
#include "CJinxBasicAttackScript.h"

CJinxBasicAttackScript::CJinxBasicAttackScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXBASICATTACKSCRIPT)
{
	m_fProjectileSpeed = 500.f;
}


CJinxBasicAttackScript::~CJinxBasicAttackScript()
{
}

void CJinxBasicAttackScript::begin()
{
	// 첫 생성 위치 기억
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();

	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// Target을 향한 방향 계산
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();
	float targetAngle = atan2f(Direction.z, Direction.x);

	// z축 회전(-90도)
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, targetAngle -XMConvertToRadians(90)));
}

void CJinxBasicAttackScript::tick()
{
	if (m_bUnitDead) return;
	CProjectileScript::tick();

	if (m_TargetObj == nullptr || m_TargetObj->IsDead())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetOwner()->GetScript<CUnitScript>()->GetServerID(), 0.f);
		return;
	}

	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// 방향 계산
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();

	// 투사체 이동
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * DT;
	GetOwner()->Transform()->SetRelativePos(NewPos);
}


void CJinxBasicAttackScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_TargetObj == nullptr || m_TargetObj->IsDead())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
		return;
	}

	// 타겟과 부딪치면
	if (_Other == m_TargetObj->Collider2D())
	{
		// 방장컴이 서버에게 이 투사체가 피격자와 충돌했다고 전달
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), m_iServerTargetID, m_iServerUserID, 1, m_skillType);

		// 이후 사라짐
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
	}
}
