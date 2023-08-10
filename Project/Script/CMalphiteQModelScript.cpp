#include "pch.h"
#include "CMalphiteQModelScript.h"
#include "CUnitScript.h"

CMalphiteQModelScript::CMalphiteQModelScript()
	:CScript((UINT)SCRIPT_TYPE::MALPHITEQMODELSCRIPT)
{
}

CMalphiteQModelScript::~CMalphiteQModelScript()
{
}


void CMalphiteQModelScript::begin()
{
	
}

void CMalphiteQModelScript::tick()
{
	Vec3 CurRot = GetOwner()->Transform()->GetRelativeRot();
	Vec3 m_vDiff = m_vTargetPos - m_vSpawnPos;  // 두 점 사이의 차이 계산
	float rotation_angle = atan2(m_vDiff.x, m_vDiff.z);  // atan2 함수를 사용하여 회전 각도 계산

	GetOwner()->Transform()->SetRelativeRot(CurRot.x + DT * 3.f, rotation_angle, CurRot.z);
}

