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

	// ���� ���
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();

	// ����ü �̵�
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * EditorDT;

	GetOwner()->Transform()->SetRelativePos(NewPos);
}


void CBasicAttackScript::OnOverlap(CCollider2D* _Other)
{
	if (m_TargetObj == nullptr)
		return;

	// Ÿ�ٰ� �ε�ġ��
	if (_Other == m_TargetObj->Collider2D())
	{
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), m_iServerTargetID, m_iServerUserID, 1, SkillType::BASIC_ATTACK);
	
		// ���� �����
		this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;

	}
}
