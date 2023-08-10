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
	// 첫 생성 위치 기억
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();

	// 5초 뒤 덫 삭제
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
	
	// 시전자와 다른 진영의 오브젝트가 부딪친다면
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		// 피격자의 서버 아이디
		UINT64 TargetServerID = _Other->GetOwner()->GetScript<CUnitScript>()->GetServerID();
		
		// 방장컴이 서버에게 이 투사체가 피격자와 충돌했다고 전달
		if (!m_bUnitDead)// 이후 사라짐
		{
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_E);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.5f);
	
			m_fProjectileSpeed = 0.f;
			m_bUnitDead = true;
		}
	
	}
}
