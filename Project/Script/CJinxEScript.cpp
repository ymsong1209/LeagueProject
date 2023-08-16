#include "pch.h"
#include "CJinxEScript.h"
#include "CProjectileScript.h"

#include <Engine/CAnim3D.h>
CJinxEScript::CJinxEScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::JINXESCRIPT)
	, m_fAccTime(0.f)
	, m_fMaxTime(5.f)
	, m_bUsed(false)
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

	float distance = 50;
	m_vDir = m_vDir.Normalize(); // 정규화하여 단위 벡터로 만듦

	wstring animName = L"";
	if (GetServerID() % 3 == 0)
	{
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + Vec3(m_vDir.x * cos(XM_PI / 6.0f) + m_vDir.z * sin(XM_PI / 6.0f), m_vDir.y, -m_vDir.x * sin(XM_PI / 6.0f) + m_vDir.z * cos(XM_PI / 6.0f)) * distance); // 왼쪽 30도
		animName = L"wazak\\Idle1";
	}
	else if (GetServerID() % 3 == 1)
	{
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + m_vDir * distance); // 중간
		animName = L"wazak\\Idle1";
	}
	else
	{
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + Vec3(m_vDir.x * cos(-XM_PI / 6.0f) + m_vDir.z * sin(-XM_PI / 6.0f), m_vDir.y, -m_vDir.x * sin(-XM_PI / 6.0f) + m_vDir.z * cos(-XM_PI / 6.0f)) * distance); // 오른쪽 30도
		animName = L"wazak\\Idle1";
	}

	GetOwner()->Animator3D()->PlayRepeat(animName, true, true, 0.8);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, true, true,false, 0.8f, 1.f);

	// 투사체 앞 방향 구하기
	Vec3 projectileFront = (GetOwner()->Transform()->GetRelativePos() - m_vSpawnPos).Normalize();
	float targetYaw = atan2f(m_vDir.x, m_vDir.z);
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, targetYaw + XMConvertToRadians(30), 0.f));
}

void CJinxEScript::tick()
{
	if (m_bUnitDead)return;


	CProjectileScript::tick();

	// 5초가 지나면 공격 애니메이션
	if (m_fAccTime >= m_fMaxTime && !m_bUsed)
	{
		m_bUsed = true;
		wstring animName = L"wazak\\Attack1";
		GetOwner()->Animator3D()->PlayOnce(animName, true);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, false, false, 1.f);
		m_fAccTime = 0.f;
	}
	m_fAccTime += DT;


	// 공격애니메이션이 끝날때 무조건 despawn
	if(Animator3D()->GetCurAnim()->GetName() == L"wazak\\Attack1" && Animator3D()->GetCurAnim()->IsFinish())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);

		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
	}
}

void CJinxEScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_bUnitDead || m_bUsed) return;
	
	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;
	
	// 시전자와 다른 진영의 오브젝트가 부딪친다면
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		wstring animName = L"wazak\\Attack1";

		GetOwner()->Animator3D()->PlayOnce(animName, true);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, false, false, 1.f);
		
		// 피격자의 서버 아이디
		UINT64 TargetServerID = _Other->GetOwner()->GetScript<CUnitScript>()->GetServerID();
		
		// 방장컴이 서버에게 이 투사체가 피격자와 충돌했다고 전달
		if (!m_bUnitDead)// 이후 사라짐
		{
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_E);
		}
		m_bUsed = true;
	}
}
