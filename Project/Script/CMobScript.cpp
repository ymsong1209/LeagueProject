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
	//m_Skill[0]->SetProjectileObj(); // 투사체 프리팹 설정
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

		// 다른 유닛의 위치와 collider 크기 가져오기
		Vec3 OtherUnitPos = OtherUnit->Transform()->GetRelativePos();
		float OtherUnitColliderRadius = _collider->GetOffsetScale().x * 0.5f;

		// 내 위치와 collider 크기
		Vec3 MyPos = GetOwner()->Transform()->GetRelativePos();
		float MyColliderRadius = (GetOwner()->Collider2D()->GetOffsetScale().x) * 0.5f;

		// 두 원의 중심 사이의 벡터와 거리 계산 (Y는 무시)
		Vec3 delta = Vec3(OtherUnitPos.x - MyPos.x, 0.0f, OtherUnitPos.z - MyPos.z);
		float distance = sqrt(delta.x * delta.x + delta.z * delta.z);

		// 겹치는 경우만 처리
		if (distance < MyColliderRadius + OtherUnitColliderRadius)
		{
			// 겹치는 거리 계산
			float overlap = (MyColliderRadius + OtherUnitColliderRadius) - distance;

			// 겹치는 거리를 기반으로 밀어내기 위한 방향 벡터 계산
			Vec3 direction = Vec3(delta.x / distance, 0.0f, delta.z / distance);

			// 겹치지 않도록 현재 유닛만을 천천히 밀어냄
			MyPos -= direction * (overlap * 0.05f);

			// 수정된 위치로 업데이트
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
