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
}

void CJinxWScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();

	// ¡ũ�� ������ �������� �߻�
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// ����ü �̵�
	Vec3 NewPos = ProjectilePos + m_vDir * m_fProjectileSpeed * EditorDT;
	NewPos = Vec3(NewPos.x, 0.f, NewPos.z);
	GetOwner()->Transform()->SetRelativePos(NewPos);

	// ���� ��ġ�κ��� ��ų ��Ÿ����� �߻�Ǿ��ٸ� �����
	float distance = sqrt((pow(m_vSpawnPos.x - NewPos.x, 2) + pow(m_vSpawnPos.z - NewPos.z, 2)));
	if (distance >= m_fSkillRange)
	{
		if(!m_bUnitDead) // ���� �����
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 2.f);
		//this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
	}
}

void CJinxWScript::OnOverlap(CCollider2D* _Other)
{
	if (m_bUnitDead) return;
	
	// �����̸�, _Other->GetOwner() ����
	// ������ �ƴϸ�  _Other->GetOwner()->GetParent()
	if (_Other->GetOwner()->GetParent()->GetScript<CUnitScript>() == nullptr)
		return;
	
	// �����ڿ� �ٸ� ������ ������Ʈ�� �ε�ģ�ٸ�
	if (_Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		// �ǰ����� ���� ���̵�
		UINT64 TargetServerID = _Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetServerID();
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		

		if (!m_bUnitDead)// ���� �����
		{
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_W);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.5f);
		}
			
		//this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
	}
}
