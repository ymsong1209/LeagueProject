#include "pch.h"
#include "CDragonBasicAttackScript.h"

CDragonBasicAttackScript::CDragonBasicAttackScript()
{
	m_fProjectileSpeed = 100.f;
}

CDragonBasicAttackScript::~CDragonBasicAttackScript()
{
}

void CDragonBasicAttackScript::begin()
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
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, 0.f, targetAngle - XMConvertToRadians(90)));

	GetOwner()->MeshRender()->GetMaterial(0)
		->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std2DEffectShaderAlpha"));

	GetOwner()->Animator2D()->Play(L"DragonBasicAttack", true);
}

void CDragonBasicAttackScript::tick()
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

void CDragonBasicAttackScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_TargetObj == nullptr || m_TargetObj->IsDead())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
		return;
	}

	// Ÿ�ٰ� �ε�ġ��
	if (_Other == m_TargetObj->Collider2D())
	{
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), m_iServerTargetID, m_iServerUserID, 1, m_skillType);

		// ���� �����
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
	}
}