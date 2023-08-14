#include "pch.h"
#include "CMalphiteQScript.h"
#include "CProjectileScript.h"
#include "CMalphiteQDecalScript.h"
#include "CMalphiteQModelScript.h"

CMalphiteQScript::CMalphiteQScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::MALPHITEQSCRIPT)
{
	m_fProjectileSpeed = 100.f;
	m_fSkillRange = 150.f;
}

CMalphiteQScript::~CMalphiteQScript()
{
}

void CMalphiteQScript::begin()
{
	// 첫 생성 위치 기억
	m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();

	m_pMalphiteQDecal = GetOwner()->GetChild()[0];
	//SpawnGameObject(m_pMalphiteQDecal, GetOwner()->Transform()->GetRelativePos(), L"Default");

	CMalphiteQDecalScript* decalscript = m_pMalphiteQDecal->GetScript<CMalphiteQDecalScript>();
	decalscript->SetSpawnPos(m_vSpawnPos);
	decalscript->SetTargetPos(m_TargetObj->Transform()->GetRelativePos());

	//m_pMalphiteQModel = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteQShard.prefab")->Instantiate();
	//SpawnGameObject(m_pMalphiteQModel, GetOwner()->Transform()->GetRelativePos(), L"Default");

	//CMalphiteQModelScript* modelscript = m_pMalphiteQModel->GetScript<CMalphiteQModelScript>();
	//modelscript->SetSpawnPos(m_vSpawnPos);
	//modelscript->SetTargetPos(m_TargetObj->Transform()->GetRelativePos());
}

void CMalphiteQScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();
	
	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 UserPos = m_UserObj->Transform()->GetRelativePos();
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// 방향 계산
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();

	// 투사체 이동
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * EditorDT;

	GetOwner()->Transform()->SetRelativePos(NewPos);


	Vec3 CurRot = GetOwner()->Transform()->GetRelativeRot();
	Vec3 m_vDiff = TargetPos - m_vSpawnPos;  // 두 점 사이의 차이 계산
	float rotation_angle = atan2(m_vDiff.x, m_vDiff.z);  // atan2 함수를 사용하여 회전 각도 계산

	//GetOwner()->Transform()->SetRelativeRot(0.f, rotation_angle, 0.f);
	GetOwner()->Transform()->SetRelativeRot(CurRot.x + DT * 3.f, rotation_angle, CurRot.z);

	//CMalphiteQDecalScript* script = m_pMalphiteQDecal->GetScript<CMalphiteQDecalScript>();
	//script->SetTargetPos(TargetPos);
	//
	//CMalphiteQModelScript* Modelscript = m_pMalphiteQModel->GetScript<CMalphiteQModelScript>();
	//Modelscript->SetTargetPos(TargetPos);
	//Modelscript->SetPos(ProjectilePos);
	//
	//
	//
	//Vec3 CurRot = GetOwner()->Transform()->GetRelativeRot();
	//Vec3 m_vDiff = m_vTargetPos - m_vSpawnPos;  // 두 점 사이의 차이 계산
	//float rotation_angle = atan2(m_vDiff.x, m_vDiff.z);  // atan2 함수를 사용하여 회전 각도 계산

	//GetOwner()->Transform()->SetRelativeRot(CurRot.x + DT * 3.f, rotation_angle, CurRot.z);

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
	//DestroyObject(m_pMalphiteQModel);
}
