#include "pch.h"
#include "CMalphiteQScript.h"
#include "CProjectileScript.h"
#include "CMalphiteQDecalScript.h"

CMalphiteQScript::CMalphiteQScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::MALPHITEQSCRIPT)
{
	m_fProjectileSpeed = 300.f;
	m_fSkillRange = 150.f;
}

CMalphiteQScript::~CMalphiteQScript()
{
}

void CMalphiteQScript::begin()
{
	// 첫 생성 위치 기억
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();

	m_pMalphiteQDecal = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteQDecal.prefab")->Instantiate();
	SpawnGameObject(m_pMalphiteQDecal, GetOwner()->Transform()->GetRelativePos(), L"Default");

	CMalphiteQDecalScript* script = m_pMalphiteQDecal->GetScript<CMalphiteQDecalScript>();
	script->SetSpawnPos(m_vSpawnPos);
	script->SetTargetPos(m_TargetObj->Transform()->GetRelativePos());
}

void CMalphiteQScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();
	Vec3 CurRot = GetOwner()->Transform()->GetRelativeRot();
	GetOwner()->Transform()->SetRelativeRot(Vec3(CurRot.x + DT * 3.f, CurRot.y, CurRot.z));

	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 UserPos = m_UserObj->Transform()->GetRelativePos();
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// 방향 계산
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();

	// 투사체 이동
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * EditorDT;

	GetOwner()->Transform()->SetRelativePos(NewPos);

	CMalphiteQDecalScript* script = m_pMalphiteQDecal->GetScript<CMalphiteQDecalScript>();
	script->SetTargetPos(TargetPos);

}

void CMalphiteQScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_bUnitDead) return;

	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;


	// 타겟과 부딪치면
	if (_Other == m_TargetObj->Collider2D())
	{
		// 방장컴이 서버에게 이 투사체가 피격자와 충돌했다고 전달
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), m_iServerTargetID, m_iServerUserID, 1, SkillType::MALPHITE_Q);

		// 이후 사라짐
		m_fProjectileSpeed = 0.f;
		m_bUnitDead = true;
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);

		Despawn();
	}
}

void CMalphiteQScript::Despawn()
{
	CMalphiteQDecalScript* script = m_pMalphiteQDecal->GetScript<CMalphiteQDecalScript>();
	script->DeleteDecal();
}
