#include "pch.h"
#include "CJinxWScript.h"
#include "CProjectileScript.h"

CJinxWScript::CJinxWScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXWSCRIPT)
{
	m_fProjectileSpeed = 300.f;
	m_fSkillRange = 150.f;
}

CJinxWScript::~CJinxWScript()
{
}

void CJinxWScript::begin()
{
	// ù ���� ��ġ ���
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();


	// ���콺�� ���� �������� ȸ���ϱ� ����.
	float targetAngle = atan2f(m_vDir.z, m_vDir.x);
	targetAngle = fmod(targetAngle + XM_PI, 2 * XM_PI) - XM_PI; // ������ -�� ~ �� �� �ٲٱ�
	targetAngle += XMConvertToRadians(90); // ���� ȸ���� ���¶� z�� 90�� ȸ��(�̰ɷ� ��� ���� �ٲ�)
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, targetAngle));
}


void CJinxWScript::tick()
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
		if(!m_bUnitDead) // ���� �����
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.2f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
	}
}

void CJinxWScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_bUnitDead) return;
	
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
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_W);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.5f);
		
			m_fProjectileSpeed = 0.f;
			m_bUnitDead = true;
		}
	}
}
