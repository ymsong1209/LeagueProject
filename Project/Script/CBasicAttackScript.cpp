#include "pch.h"
#include "CBasicAttackScript.h"

CBasicAttackScript::CBasicAttackScript()
	:CScript((UINT)SCRIPT_TYPE::BASICATTACKSCRIPT)
{
}

CBasicAttackScript::~CBasicAttackScript()
{
}

void CBasicAttackScript::begin()
{
	// ID������ �˻��� ��� ����� ����
	CGameObject* Target = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByID(m_iTargetID);
	m_TargetObj = Target;

	CGameObject* User = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByID(m_iUserID);
	m_UserObj = User;
}

void CBasicAttackScript::tick()
{
	// Ÿ���� ���� ���ư�. 

	Vec3 TargetPos = m_TargetObj->Transform()->GetRelativePos();
	Vec3 UserPos = m_UserObj->Transform()->GetRelativePos();
	
	// ���� ���
	Vec3 Direction = Vec3(TargetPos.x - UserPos.x, 0.f, TargetPos.z - UserPos.z);
	Direction.Normalize();

	// ����ü �̵�
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();
	Vec3 NewPos = ProjectilePos + Direction * m_fProjectileSpeed * DT;
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
		Destroy();
	}
}