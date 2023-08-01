#include "pch.h"
#include "CTurretAttackScript.h"


CTurretAttackScript::CTurretAttackScript()
	: CScript((UINT)SCRIPT_TYPE::TURRETATTACKSCRIPT)
	, m_fSpeed(15.f)
	, m_pTarget(nullptr)
	, m_vTargetPos()
	, m_bPosChase(true)
{

}	  

CTurretAttackScript::~CTurretAttackScript()
{
}

void CTurretAttackScript::begin()
{
	return;
}

void CTurretAttackScript::tick()
{
	Vec3 curpos = GetOwner()->Transform()->GetRelativePos();
	Vec3 dist = m_vTargetPos - curpos;
	float distance = sqrt(pow(dist.x, 2.f) + pow(dist.y, 2.f) + pow(dist.z, 2.f));
	if (distance < 0.2f) {
		DestroyObject(GetOwner());
		return;
	}

	//������ �ƴ϶�� ��� Ÿ����ġ ã�ƾ���
	if (m_bPosChase) {
		//CUnitScript* script = m_pTarget->GetScript<CUnitScript>();
		////Dead �ִϸ��̼� ������ destroy�Ǵϱ� m_ptarget�� ���� nullptr�� �ƴ�
		//if (script->IsUnitDead()) {
		//	m_bPosChase = false;
		//}

		m_vTargetPos = m_pTarget->Transform()->GetRelativePos();

	}
	
	Vec3 Dir = m_vTargetPos - curpos;
	Dir = Dir.Normalize();
	curpos += Dir * m_fSpeed * EditorDT;
	GetOwner()->Transform()->SetRelativePos(curpos);

}

void CTurretAttackScript::BeginOverlap(CCollider3D* _other)
{
	if (_other->GetOwner() == m_pTarget) {
		DestroyObject(GetOwner());
	}
}
