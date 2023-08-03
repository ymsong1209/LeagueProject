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
	// 첫 생성 위치 기억
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();
}

void CJinxWScript::tick()
{
	CProjectileScript::tick();

	// 징크스 본인의 방향으로 발사
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// 투사체 이동
	Vec3 NewPos = ProjectilePos + m_vDir * m_fProjectileSpeed * EditorDT;
	NewPos = Vec3(NewPos.x, 0.f, NewPos.z);
	GetOwner()->Transform()->SetRelativePos(NewPos);

	// 시전 위치로부터 스킬 사거리까지 발사되었다면 사라짐
	float distance = sqrt((pow(m_vSpawnPos.x - NewPos.x, 2) + pow(m_vSpawnPos.z - NewPos.z, 2)));
	if (distance >= m_fSkillRange)
	{
		//this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
	}
}

void CJinxWScript::OnOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;
	
	// 시전자와 다른 진영의 오브젝트가 부딪친다면
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		// 피격자의 서버 아이디
		UINT64 TargetServerID = _Other->GetOwner()->GetScript<CUnitScript>()->GetServerID();
		// 방장컴이 서버에게 이 투사체가 피격자와 충돌했다고 전달
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_W);

		// 이후 사라짐
		this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
	}
}
