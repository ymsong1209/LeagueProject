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
	// ù ���� ��ġ ���
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();

	float distance = 50;
	m_vDir = m_vDir.Normalize(); // ����ȭ�Ͽ� ���� ���ͷ� ����

	wstring animName = L"";
	if (GetServerID() % 3 == 0)
	{
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + Vec3(m_vDir.x * cos(XM_PI / 6.0f) + m_vDir.z * sin(XM_PI / 6.0f), m_vDir.y, -m_vDir.x * sin(XM_PI / 6.0f) + m_vDir.z * cos(XM_PI / 6.0f)) * distance); // ���� 30��
		animName = L"wazak\\Idle1";
	}
	else if (GetServerID() % 3 == 1)
	{
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + m_vDir * distance); // �߰�
		animName = L"wazak\\Idle1";
	}
	else
	{
		GetOwner()->Transform()->SetRelativePos(m_vSpawnPos + Vec3(m_vDir.x * cos(-XM_PI / 6.0f) + m_vDir.z * sin(-XM_PI / 6.0f), m_vDir.y, -m_vDir.x * sin(-XM_PI / 6.0f) + m_vDir.z * cos(-XM_PI / 6.0f)) * distance); // ������ 30��
		animName = L"wazak\\Idle1";
	}

	GetOwner()->Animator3D()->PlayRepeat(animName, true, true, 0.8);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, true, true,false, 0.8f, 1.f);

	// ����ü �� ���� ���ϱ�
	Vec3 projectileFront = (GetOwner()->Transform()->GetRelativePos() - m_vSpawnPos).Normalize();
	float targetYaw = atan2f(m_vDir.x, m_vDir.z);
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, targetYaw + XMConvertToRadians(30), 0.f));
}

void CJinxEScript::tick()
{
	if (m_bUnitDead)return;


	CProjectileScript::tick();

	// 5�ʰ� ������ ���� �ִϸ��̼�
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


	// ���ݾִϸ��̼��� ������ ������ despawn
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
	
	// �����ڿ� �ٸ� ������ ������Ʈ�� �ε�ģ�ٸ�
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		wstring animName = L"wazak\\Attack1";

		GetOwner()->Animator3D()->PlayOnce(animName, true);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, false, false, 1.f);
		
		// �ǰ����� ���� ���̵�
		UINT64 TargetServerID = _Other->GetOwner()->GetScript<CUnitScript>()->GetServerID();
		
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		if (!m_bUnitDead)// ���� �����
		{
			CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::JINX_E);
		}
		m_bUsed = true;
	}
}
