#include "pch.h"
#include "CJinxWScript.h"
#include "CProjectileScript.h"

CJinxWScript::CJinxWScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXWSCRIPT)
{
	m_fProjectileSpeed = 100.f;//300.f;
	m_fSkillRange = 150.f;
}

CJinxWScript::~CJinxWScript()
{
}

void CJinxWScript::begin()
{
	// 첫 생성 위치 기억
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();
	//GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XMConvertToRadians(90)));

	// 마우스를 향한 방향으로 회전하기 위함.
	float targetAngle = atan2f(m_vDir.z, m_vDir.x);
	targetAngle = fmod(targetAngle + XM_PI, 2 * XM_PI) - XM_PI; // 범위를 -π ~ π 로 바꾸기
	targetAngle += XMConvertToRadians(90); // 모델이 회전된 상태라서 z축 90도 회전(이걸로 모든 축이 바뀜)
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, targetAngle));

}
void CJinxWScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();

	// 징크스 본인의 방향으로 발사
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();
	Vec3 ProjectileRot = GetOwner()->Transform()->GetRelativeRot();

	// 투사체 이동
	Vec3 NewPos = ProjectilePos + m_vDir * m_fProjectileSpeed * DT;
	NewPos = Vec3(NewPos.x, 30.f, NewPos.z);
	GetOwner()->Transform()->SetRelativePos(NewPos);

	// 매번 회전한다.
	GetOwner()->Transform()->SetRelativeRot(ProjectileRot.x, ProjectileRot.y+ XMConvertToRadians(15), ProjectileRot.z);

	// 시전 위치로부터 스킬 사거리까지 발사되었다면 사라짐
	float distance = sqrt((pow(m_vSpawnPos.x - NewPos.x, 2) + pow(m_vSpawnPos.z - NewPos.z, 2)));
	if (distance >= m_fSkillRange)
	{
		if(!m_bUnitDead) // 이후 사라짐
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
	
	// 시전자와 다른 진영의 오브젝트가 부딪친다면
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		// 피격자의 서버 아이디
		UINT64 TargetServerID = _Other->GetOwner()->GetScript<CUnitScript>()->GetServerID();
		
		// 방장컴이 서버에게 이 투사체가 피격자와 충돌했다고 전달
		if (!m_bUnitDead)// 이후 사라짐
		{
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_W);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.5f);
		
			m_fProjectileSpeed = 0.f;
			m_bUnitDead = true;
		}
	}
}
