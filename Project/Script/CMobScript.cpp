#include "pch.h"
#include "CMobScript.h"
#include "CBasicAttack.h"

CMobScript::CMobScript(UINT ScriptType)
	:CUnitScript(ScriptType)
{
}

CMobScript::CMobScript()
	:CUnitScript((UINT)SCRIPT_TYPE::MOBSCRIPT)
{
}

CMobScript::~CMobScript()
{
}


void CMobScript::begin()
{
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	//m_Skill[0]->SetProjectileObj(); // ����ü ������ ����
	m_SkillLevel[0] = 1;

	CUnitScript::begin();
}

void CMobScript::tick()
{
	CUnitScript::tick();
}

void CMobScript::BeginOverlap(CCollider2D* _collider)
{
	if (_collider->GetOwner()->GetLayerIndex() == 4 || _collider->GetOwner()->GetLayerIndex() == 5)
	{

		CGameObject* OtherUnit = _collider->GetOwner();

		// �ٸ� ������ ��ġ�� collider ũ�� ��������
		Vec3 OtherUnitPos = OtherUnit->Transform()->GetRelativePos();
		float OtherUnitColliderRadius = _collider->GetOffsetScale().x * 0.5f;

		// �� ��ġ�� collider ũ��
		Vec3 MyPos = GetOwner()->Transform()->GetRelativePos();
		float MyColliderRadius = (GetOwner()->Collider2D()->GetOffsetScale().x) * 0.5f;

		// �� ���� �߽� ������ ���Ϳ� �Ÿ� ��� (Y�� ����)
		Vec3 delta = Vec3(OtherUnitPos.x - MyPos.x, 0.0f, OtherUnitPos.z - MyPos.z);
		float distance = sqrt(delta.x * delta.x + delta.z * delta.z);

		// ��ġ�� ��츸 ó��
		if (distance < MyColliderRadius + OtherUnitColliderRadius)
		{
			// ��ġ�� �Ÿ� ���
			float overlap = (MyColliderRadius + OtherUnitColliderRadius) - distance;

			// ��ġ�� �Ÿ��� ������� �о�� ���� ���� ���� ���
			Vec3 direction = Vec3(delta.x / distance, 0.0f, delta.z / distance);

			// ��ġ�� �ʵ��� ���� ���ָ��� õõ�� �о
			MyPos -= direction * (overlap * 0.05f);

			// ������ ��ġ�� ������Ʈ
			GetOwner()->Transform()->SetRelativePos(MyPos);
		}
	}
}

void CMobScript::SaveToLevelFile(FILE* _File)
{
	CUnitScript::SaveToLevelFile(_File);
}

void CMobScript::LoadFromLevelFile(FILE* _FILE)
{
	CUnitScript::LoadFromLevelFile(_FILE);
}

void CMobScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	CUnitScript::SaveToLevelJsonFile(_objValue, allocator);
}

void CMobScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CUnitScript::LoadFromLevelJsonFile(_componentValue);
}
