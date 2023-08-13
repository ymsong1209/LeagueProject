#include "pch.h"
#include "CMalphiteBasicAttackScript.h"

#include <Engine/CKeyMgr.h>

CMalphiteBasicAttackScript::CMalphiteBasicAttackScript()
	: CScript((UINT)SCRIPT_TYPE::MALPHITEBASICATTACKSCRIPT)
	, m_bStartingPoint(false)
{
}

CMalphiteBasicAttackScript::~CMalphiteBasicAttackScript()
{
}

void CMalphiteBasicAttackScript::begin()
{
	m_bStartingPoint = true;
	GetOwner()->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	GetOwner()->Transform()->SetBillBoard(true);
	GetOwner()->GetChild()[0]->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	GetOwner()->MeshRender()->GetMaterial(0)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std2DEffectShaderOneOne"));
}

void CMalphiteBasicAttackScript::tick()
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


