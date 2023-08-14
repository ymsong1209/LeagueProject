#include "pch.h"
#include "CJinxBasicAttackScript.h"

CJinxBasicAttackScript::CJinxBasicAttackScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXBASICATTACKSCRIPT)
{
	m_fProjectileSpeed = 100.f;
}


CJinxBasicAttackScript::~CJinxBasicAttackScript()
{
}

void CJinxBasicAttackScript::begin()
{

}

void CJinxBasicAttackScript::tick()
{
	CProjectileScript::tick();

	if (m_TargetObj == nullptr || m_TargetObj->IsDead())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetOwner()->GetScript<CUnitScript>()->GetServerID(), 0.f);
		return;
	}

	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();

	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// ���� ���
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();

	// ����ü �̵�
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * EditorDT;

	GetOwner()->Transform()->SetRelativePos(NewPos);
}


void CJinxBasicAttackScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_TargetObj == nullptr || m_TargetObj->IsDead())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
		return;
	}

	// Ÿ�ٰ� �ε�ġ��
	if (_Other == m_TargetObj->Collider2D())
	{
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), m_iServerTargetID, m_iServerUserID, 1, m_skillType);

		// ���� �����
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
	}
}
