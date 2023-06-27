#include "pch.h"
#include "CMissileScript.h"

CMissileScript::CMissileScript()
	: CScript((UINT)SCRIPT_TYPE::MISSILESCRIPT)
	, m_vDir(Vec3(0.f, 1.f, 0.f))
	, m_fSpeed(100.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	vPos += m_vDir * DT * m_fSpeed;

	Transform()->SetRelativePos(vPos);

	int a = 1;
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_2, &a);
}
