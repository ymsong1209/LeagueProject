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
	// 첫 생성 위치 기억
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();

}

void CJinxRScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();

	// 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// 투사체 이동 Pos
	Vec3 NewPos = CurPos + m_vDir * m_fProjectileSpeed * DT;
	NewPos = Vec3(NewPos.x, m_vSpawnPos.z +4, NewPos.z); // 높이를 띄어둔다.
	GetOwner()->Transform()->SetRelativePos(NewPos);


	// 투사체가 가야할 방향 구하기  Rot
	Vec3 Dir = - m_vDir.Normalize();

	float targetYaw = atan2f(-Dir.x, -Dir.z);
	targetYaw = fmod(targetYaw + XM_PI, 2 * XM_PI) - XM_PI; // 범위를 -π ~ π 로 바꾸기
	float currentYaw = GetOwner()->Transform()->GetRelativeRot().y;
	currentYaw = fmod(currentYaw + XM_PI, 2 * XM_PI) - XM_PI; // 범위를 -π ~ π 로 바꾸기

	// 각도 차이 계산
	float diff = targetYaw - currentYaw;

	// 차이가 π를 넘으면 각도를 반대 방향으로 보간
	if (diff > XM_PI)
		targetYaw -= 2 * XM_PI;
	else if (diff < -XM_PI)
		targetYaw += 2 * XM_PI;

	float lerpFactor = DT * 18.f;

	// Lerp를 이용해 현재 회전 각도와 목표 회전 각도를 보간
	float newYaw = currentYaw + (targetYaw - currentYaw) * lerpFactor;

	// 새로운 회전 각도를 적용
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));


	// 시전 위치로부터 스킬 사거리까지 발사되었다면 사라짐
	float distance = sqrt((pow(m_vSpawnPos.x - NewPos.x, 2) + pow(m_vSpawnPos.z - NewPos.z, 2)));
	if (distance >= m_fSkillRange)
	{
		if (!m_bUnitDead) // 이후 사라짐
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.2f);
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
	}
}

void CJinxRScript::BeginOverlap(CCollider2D* _Other)
{
	// 징크스 R은 플레이어만 충돌한다. 
	if (m_bUnitDead || _Other->GetOwner()->GetLayerIndex() != 5) return;
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
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_R);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.5f);

			m_fProjectileSpeed = 0.f;
			m_bUnitDead = true;
		}

	}
}
