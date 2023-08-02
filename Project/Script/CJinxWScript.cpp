#include "pch.h"
#include "CJinxWScript.h"
#include "CUnitScript.h"

CJinxWScript::CJinxWScript()
	:CScript((UINT)SCRIPT_TYPE::JINXWSCRIPT)
	, m_fProjectileSpeed(100.f)
	, m_fSkillRange(100.f)
{
}

CJinxWScript::~CJinxWScript()
{
}

void CJinxWScript::begin()
{
}

void CJinxWScript::tick()
{
	static bool dead = false; 

	if (dead)
		return;

	// ¡ũ�� ������ �������� �߻�
	Vec3 ProjectilePos = GetOwner()->Transform()->GetRelativePos();

	// ����ü �̵�
	Vec3 NewPos = ProjectilePos + m_vDir * m_fProjectileSpeed * EditorDT;
	NewPos = Vec3(NewPos.x, 0.f, NewPos.z);
	GetOwner()->Transform()->SetRelativePos(NewPos);

	// �����ڷκ��� ��ų ��Ÿ����� �߻�Ǿ��ٸ� �����
	float distance = sqrt((pow(m_vSpawnPos.x - NewPos.x, 2) + pow(m_vSpawnPos.z - NewPos.z, 2)));
	if (distance >= m_fSkillRange)
	{
		this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;
		dead = true;
	}
}

void CJinxWScript::OnOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetScript<CUnitScript>() == nullptr)
		return;

	// �����ڿ� �ٸ� ������ ������Ʈ�� �ε�ģ�ٸ�
	if (_Other->GetOwner()->GetScript<CUnitScript>()->GetFaction() != m_UserObj->GetScript<CUnitScript>()->GetFaction())
	{
		this->GetOwner()->Transform()->SetRelativePos(-666.f, -666.f, -666.f);
		m_fProjectileSpeed = 0.f;

		// �������� �������� �� ����ü�� �ǰ��ڿ� �浹�ߴٰ� ����
		CSendServerEventMgr::GetInst()->SendHitPacket(GetServerID(), m_iServerTargetID, m_iServerUserID, 1, SkillType::JINX_W);

	}
}
