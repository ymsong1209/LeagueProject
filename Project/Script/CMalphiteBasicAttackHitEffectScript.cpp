#include "pch.h"
#include "CMalphiteBasicAttackHitEffectScript.h"

#include <Engine/CKeyMgr.h>

CMalphiteBasicAttackHitEffectScript::CMalphiteBasicAttackHitEffectScript()
	: CScript((UINT)SCRIPT_TYPE::MALPHITEBASICATTACKHITEFFECTSCRIPT)
	, m_bStartingPoint(false)
{
}

CMalphiteBasicAttackHitEffectScript::~CMalphiteBasicAttackHitEffectScript()
{
}

void CMalphiteBasicAttackHitEffectScript::begin()
{
	m_bStartingPoint = true;
	GetOwner()->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	GetOwner()->Transform()->SetBillBoard(true);
	GetOwner()->GetChild()[0]->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	GetOwner()->MeshRender()->GetMaterial(0)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std2DEffectShaderOneOne"));
}

void CMalphiteBasicAttackHitEffectScript::tick()
{
	if (m_bStartingPoint == true)
	{
		Vec3 Scale = GetOwner()->Transform()->GetRelativeScale();

		if (Scale.x > 35.f)
			DestroyObject(GetOwner());

		float NextScale = Scale.x + DT * 650.f;

		GetOwner()->Transform()->SetRelativeScale(NextScale, NextScale, 1.f);
	}
}


