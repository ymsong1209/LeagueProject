#include "pch.h"
#include "CProjectileScript.h"
#include "CSendServerEventMgr.h"

CProjectileScript::CProjectileScript(UINT ScriptType)
	: m_UserObj(nullptr)
	, m_iServerUserID(-999)
	, m_TargetObj(nullptr)
	, m_iServerTargetID(-999)
	, m_fProjectileSpeed(0)
	, m_vDir(Vec3(0,0,0))
	, m_vSpawnPos(Vec3(0,0,0))
	, m_fSkillRange(0)
{
}

CProjectileScript::CProjectileScript()
	:CUnitScript((UINT)SCRIPT_TYPE::PROJECTILESCRIPT)
{
}

CProjectileScript::~CProjectileScript()
{
}

void CProjectileScript::begin()
{
}

void CProjectileScript::tick()
{
	if (m_bUnitDead)
	{
		Despawn();
		return;
	}

	if (m_TargetObj)
	{
		if (m_TargetObj->IsDead() || m_TargetObj->GetScript<CUnitScript>()->IsUnitDead())
		{
			CSendServerEventMgr::GetInst()->SendDespawnPacket(this->GetServerID(), 0.f);
			Despawn();
			return;
		}

	}

	if (m_UserObj == nullptr || m_UserObj->IsDead() || m_UserObj->GetScript<CUnitScript>()->IsUnitDead())
	{
		CSendServerEventMgr::GetInst()->SendDespawnPacket(this->GetServerID(), 0.f);
		return;
	}	
}

void CProjectileScript::BeginOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;
}

void CProjectileScript::OnOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;
}
