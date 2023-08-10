#include "pch.h"
#include "CJinxEScript.h"
#include "CProjectileScript.h"
#include <thread>
CJinxEScript::CJinxEScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXESCRIPT)
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

	// 5�� �� �� ����
	thread t([=]() {
		Sleep(3000);
	CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.5f);
	m_bUnitDead = true;
	m_fProjectileSpeed = 0.f;
		});
	t.detach();
}

void CJinxEScript::tick()
{
	if (m_bUnitDead)return;


	CProjectileScript::tick();
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
