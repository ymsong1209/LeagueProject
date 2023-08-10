#include "pch.h"
#include "CJinxRScript.h"

CJinxRScript::CJinxRScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXRSCRIPT)
{
	m_fProjectileSpeed = 300.f;
	m_fSkillRange = 4000.f;
}

CJinxRScript::~CJinxRScript()
{
}

void CJinxRScript::begin()
{
	// ù ���� ��ġ ���
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();
}

void CJinxRScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();

	// ¡ũ�� ������ �������� �߻�
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// ����ü �̵�
	Vec3 NewPos = ProjectilePos + m_vDir * m_fProjectileSpeed * DT;
	NewPos = Vec3(NewPos.x, 0.f, NewPos.z);
	GetOwner()->Transform()->SetRelativePos(NewPos);

	// ���� ��ġ�κ��� ��ų ��Ÿ����� �߻�Ǿ��ٸ� �����
	float distance = sqrt((pow(m_vSpawnPos.x - NewPos.x, 2) + pow(m_vSpawnPos.z - NewPos.z, 2)));
	if (distance >= m_fSkillRange)
	{
		if (!m_bUnitDead) // ���� �����
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.2f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
	}
}

void CJinxRScript::BeginOverlap(CCollider2D* _Other)
{
	// ¡ũ�� R�� �÷��̾ �浹�Ѵ�. 
	if (m_bUnitDead || _Other->GetOwner()->GetLayerIndex() != 5) return;
	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;

	// �����ڿ� �ٸ� ������ ������Ʈ�� �ε�ģ�ٸ�
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		// �ǰ����� ���� ���̵�
		UINT64 TargetServerID = _Other->GetOwner()->GetScript<CUnitScript>()->GetServerID();

		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		if (!m_bUnitDead)// ���� �����
		{
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_R);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.5f);

			m_fProjectileSpeed = 0.f;
			m_bUnitDead = true;
		}

	}
}
