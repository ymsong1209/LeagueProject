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
	//// ID������ �˻��� ��� ����� ����
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

	// ���� ���
	Vec3 Direction = Vec3(TargetPos.x - UserPos.x, 0.f, TargetPos.z - UserPos.z);
	Direction.Normalize();

	// ����ü �̵�
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * EditorDT;

	// Ÿ���� ���� ���ư�. 
	if (abs(NewPos.x - TargetPos.x) < 1.f)
		return;

	GetOwner()->Transform()->SetRelativePos(NewPos);
}


void CBasicAttackScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_TargetObj == nullptr)
		return;

	// Ÿ�ٰ� �ε�ġ��
	if (_Other == m_TargetObj->Collider2D())
	{
		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����


		// ����ü �����
		//Destroy();
		this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;

		UINT64 projectileId = m_iServerID; // ��ų ����ü id
		UINT64 userId = m_iServerUserID; // ��ų ����� id
		UINT64 hitId = m_iServerTargetID; // ��ų �ǰ��� id
		CSendServerEventMgr::GetInst()->SendHitPacket(m_iServerTargetID, m_iServerUserID, 1, SkillType::BASIC_ATTACK);
	}
}
