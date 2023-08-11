#include "pch.h"
#include "CSkillLvEffectUIScript.h"
#include <Engine\CAnim2D.h>

void CSkillLvEffectUIScript::begin()
{
}

void CSkillLvEffectUIScript::tick()
{
	if (Animator2D()->GetCurAnim()->IsFinish() == true)
	{
		DestroyObject(GetOwner());
	}
}

void CSkillLvEffectUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CSkillLvEffectUIScript::CSkillLvEffectUIScript()
	:CScript((UINT)SCRIPT_TYPE::SKILLLVEFFECTUISCRIPT)
{
}

CSkillLvEffectUIScript::~CSkillLvEffectUIScript()
{
}
