#include "pch.h"
#include "CJinxRocketBasicAttackScript.h"

CJinxRocketBasicAttackScript::CJinxRocketBasicAttackScript()
{
	m_fProjectileSpeed = 300.f;
}

CJinxRocketBasicAttackScript::~CJinxRocketBasicAttackScript()
{
}

void CJinxRocketBasicAttackScript::begin()
{
	// ù ���� ��ġ ���
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();

	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// Target�� ���� ���� ���
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();
	float targetAngle = atan2f(Direction.z, Direction.x);

	// z�� ȸ��(-90��)
	//GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, targetAngle - XMConvertToRadians(90)));
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, targetAngle));

	// ¡ũ�� ���Ϸ�ó �� �Ҹ�
	Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\JINX_Rocket_Gun_Start.mp3", 1, 0.4f, true, 100.f, JinxPos, Faction::NONE);
}

void CJinxRocketBasicAttackScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();

	if (m_TargetObj == nullptr || m_TargetObj->IsDead())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetOwner()->GetScript<CUnitScript>()->GetServerID(), 0.f);
		return;
	}

	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// ���� ���
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();

	// ����ü �̵�
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * DT;
	GetOwner()->Transform()->SetRelativePos(NewPos);
}

void CJinxRocketBasicAttackScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_TargetObj == nullptr || m_TargetObj->IsDead())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
		return;
	}

	// Ÿ�ٰ� �ε�ġ��
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetServerID() == m_TargetObj->GetScript<CUnitScript>()->GetServerID())
	{
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), m_iServerTargetID, m_iServerUserID, 1, SkillType::JINX_ROCKET_BASIC_ATTACK);

		// ���� �����
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);


		// ¡ũ�� �� ���� ��Ÿ ��Ʈ �Ҹ�
		Vec3 JinxPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\jinx\\JINX_Rocket_Gun_Hit1.mp3", 1, 0.4f, true, 100.f, JinxPos, Faction::NONE);
		
	}
}
