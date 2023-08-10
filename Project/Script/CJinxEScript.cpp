#include "pch.h"
#include "CJinxEScript.h"
#include "CProjectileScript.h"
#include <thread>
CJinxEScript::CJinxEScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXESCRIPT)
	, m_fAccTime(0.f)
	, m_fMaxTime(3.f)
{
	//m_fProjectileSpeed = 100.f;
	//m_fSkillRange = 50.f;
}

CJinxEScript::~CJinxEScript()
{
}


void CJinxEScript::begin()
{
	// ù ���� ��ġ ���
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();

	float distance = 40;
	m_vDir = m_vDir.Normalize(); // ����ȭ�Ͽ� ���� ���ͷ� ����

	if (GetServerID() % 3 == 0)
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + Vec3(m_vDir.x * cos(XM_PI / 6.0f) + m_vDir.z * sin(XM_PI / 6.0f), m_vDir.y, -m_vDir.x * sin(XM_PI / 6.0f) + m_vDir.z * cos(XM_PI / 6.0f)) * distance); // ���� 30��
	else if (GetServerID() % 3 == 1)
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + m_vDir * distance); // �߰�
	else
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + Vec3(m_vDir.x * cos(-XM_PI / 6.0f) + m_vDir.z * sin(-XM_PI / 6.0f), m_vDir.y, -m_vDir.x * sin(-XM_PI / 6.0f) + m_vDir.z * cos(-XM_PI / 6.0f)) * distance); // ������ 30��
}

void CJinxEScript::tick()
{
	if (m_bUnitDead)return;


	CProjectileScript::tick();

	if (m_fAccTime >= m_fMaxTime)
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.2f);
		m_bUnitDead = true;
		m_fProjectileSpeed = 0.f;

		m_fAccTime = 0.f;
	}
	m_fAccTime += DT;
}

void CJinxEScript::BeginOverlap(CCollider2D* _Other)
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
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_E);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.5f);
	
			m_fProjectileSpeed = 0.f;
			m_bUnitDead = true;
		}
	
	}
}
