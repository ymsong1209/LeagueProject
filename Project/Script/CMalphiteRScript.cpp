#include "pch.h"
#include "CMalphiteRScript.h"
#include "CProjectileScript.h"

CMalphiteRScript::CMalphiteRScript()
	:CProjectileScript((UINT)SCRIPT_TYPE::MALPHITERSCRIPT)
{
	m_fProjectileSpeed = 300.f;
	m_fSkillRange = 150.f;
}

CMalphiteRScript::~CMalphiteRScript()
{
}

void CMalphiteRScript::begin()
{
	CGameObject* MalphiteRDecal = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteRDecal.prefab")->Instantiate();
	SpawnGameObject(MalphiteRDecal, GetOwner()->Transform()->GetRelativePos(), L"Default");
}

void CMalphiteRScript::tick()
{
	if (m_bUnitDead) return;

	CProjectileScript::tick();

	m_fTime += DT;
	if (m_fTime > 0.1f) {

		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetServerID(), 0.f);
	}

}

void CMalphiteRScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_bUnitDead) return;

	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;

	// �����ڿ� �ٸ� ������ ������Ʈ�� �ε�ģ�ٸ�
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		// �ǰ����� ���� ���̵�
		UINT64 TargetServerID = _Other->GetOwner()->GetScript<CUnitScript>()->GetServerID();
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), TargetServerID, m_iServerUserID, 1, SkillType::MALPHITE_R);
	}
}
