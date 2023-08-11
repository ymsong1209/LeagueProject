#include "pch.h"
#include "CJinxRScript.h"

CJinxRScript::CJinxRScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXRSCRIPT)
{
	m_fProjectileSpeed = 300.f;
	m_fSkillRange = 3000.f;
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

	// ���� ��ġ
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// ����ü �̵� Pos
	Vec3 NewPos = CurPos + m_vDir * m_fProjectileSpeed * DT;
	NewPos = Vec3(NewPos.x, m_vSpawnPos.z +4, NewPos.z); // ���̸� ���д�.
	GetOwner()->Transform()->SetRelativePos(NewPos);


	// ����ü�� ������ ���� ���ϱ�  Rot
	Vec3 Dir = - m_vDir.Normalize();

	float targetYaw = atan2f(-Dir.x, -Dir.z);
	targetYaw = fmod(targetYaw + XM_PI, 2 * XM_PI) - XM_PI; // ������ -�� ~ �� �� �ٲٱ�
	float currentYaw = GetOwner()->Transform()->GetRelativeRot().y;
	currentYaw = fmod(currentYaw + XM_PI, 2 * XM_PI) - XM_PI; // ������ -�� ~ �� �� �ٲٱ�

	// ���� ���� ���
	float diff = targetYaw - currentYaw;

	// ���̰� �� ������ ������ �ݴ� �������� ����
	if (diff > XM_PI)
		targetYaw -= 2 * XM_PI;
	else if (diff < -XM_PI)
		targetYaw += 2 * XM_PI;

	float lerpFactor = DT * 18.f;

	// Lerp�� �̿��� ���� ȸ�� ������ ��ǥ ȸ�� ������ ����
	float newYaw = currentYaw + (targetYaw - currentYaw) * lerpFactor;

	// ���ο� ȸ�� ������ ����
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));


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
