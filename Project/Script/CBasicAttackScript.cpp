#include "pch.h"
#include "CBasicAttackScript.h"
#include "CSendServerEventMgr.h"

CBasicAttackScript::CBasicAttackScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::BASICATTACKSCRIPT)
{
	m_fProjectileSpeed = 100.f;
}

CBasicAttackScript::~CBasicAttackScript()
{
}

void CBasicAttackScript::begin()
{
}

void CBasicAttackScript::tick()
{
	CProjectileScript::tick();

	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 UserPos = m_UserObj->Transform()->GetRelativePos();
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// 방향 계산
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();

	// 투사체 이동
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * EditorDT;

	GetOwner()->Transform()->SetRelativePos(NewPos);
}


void CBasicAttackScript::OnOverlap(CCollider2D* _Other)
{
	if (m_TargetObj == nullptr)
		return;

	// 타겟과 부딪치면
	if (_Other == m_TargetObj->Collider2D())
	{
		// 방장컴이 서버에게 이 투사체가 피격자와 충돌했다고 전달
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), m_iServerTargetID, m_iServerUserID, 1, SkillType::BASIC_ATTACK);
	
		// 이후 사라짐
		this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;

	}
}
