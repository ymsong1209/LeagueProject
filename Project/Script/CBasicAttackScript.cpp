#include "pch.h"
#include "CBasicAttackScript.h"
#include "CSendServerEventMgr.h"
CBasicAttackScript::CBasicAttackScript()
	:CScript((UINT)SCRIPT_TYPE::BASICATTACKSCRIPT)
	, m_fProjectileSpeed(100.f)
{
}

CBasicAttackScript::~CBasicAttackScript()
{
}

void CBasicAttackScript::begin()
{
	//// ID값으로 검색한 결과 멤버로 저장
	//CGameObject* Target = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByID(m_iTargetID);
	//m_TargetObj = Target;
	//
	//CGameObject* User = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByID(m_iUserID);
	//m_UserObj = User;
}

void CBasicAttackScript::tick()
{
	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 UserPos = m_UserObj->Transform()->GetRelativePos();
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// 방향 계산
	Vec3 Direction = Vec3(TargetPos.x - ProjectilePos.x, 0.f, TargetPos.z - ProjectilePos.z);
	Direction.Normalize();

	// 투사체 이동
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * EditorDT;

	// 타겟을 향해 날아감. 
	//if (abs(NewPos.x - TargetPos.x) < 3.f)
		//return;

	GetOwner()->Transform()->SetRelativePos(NewPos);
}


void CBasicAttackScript::OnOverlap(CCollider2D* _Other)
{
	if (m_TargetObj == nullptr)
		return;

	// 타겟과 부딪치면
	if (_Other == m_TargetObj->Collider2D())
	{
		//Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
		//Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();
		// 방장컴이 서버에게 이 투사체가 피격자와 충돌했다고 전달
		// 투사체 사라짐
			//Destroy();
		this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;

		UINT64 projectileId = m_iServerID; // 스킬 투사체 id
		UINT64 userId = m_iServerUserID; // 스킬 사용자 id
		UINT64 hitId = m_iServerTargetID; // 스킬 피격자 id
		CSendServerEventMgr::GetInst()->SendHitPacket(m_iServerTargetID, m_iServerUserID, 1, SkillType::BASIC_ATTACK);
			// 타겟을 향해 날아감. 
		//if (abs(ProjectilePos.x - TargetPos.x) < 3.f)
		{
			
		}
		
	}
}
