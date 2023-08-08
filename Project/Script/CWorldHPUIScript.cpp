#include "pch.h"
#include "CWorldHPUIScript.h"
#include "CUnitScript.h"

void CWorldHPUIScript::begin()
{
}


void CWorldHPUIScript::tick()
{
	if (m_OwnerObj)
	{
		bool IsCull = m_OwnerObj->GetRenderComponent()->IsCulled();
		bool UseRaySight = m_OwnerObj->GetRenderComponent()->IsUsingRaySightCulling();
		if (IsCull && UseRaySight)
		{
			GetOwner()->GetRenderComponent()->SetSortExcept(true );
			return;
		}

		CurrentHP = (int)m_OwnerObj->GetScript<CUnitScript>()->GetCurHP();
		TotalHP = (int)m_OwnerObj->GetScript<CUnitScript>()->GetMaxHP();
		CurrentMP = (int)m_OwnerObj->GetScript<CUnitScript>()->GetCurMP();
		TotalMP = (int)m_OwnerObj->GetScript<CUnitScript>()->GetMaxMP();

		//----------HP------------
		CurrentHPRatio = CurrentHP / TotalHP;
		if (CurrentHP >= TotalHP)
			CurrentHP = TotalHP;

		if (CurrentHP <= 0.f)
			CurrentHP = 0.f;
		//-------------------------

		//----------MP------------
		CurrentMPRatio = CurrentMP / TotalMP;
		if (CurrentMP >= TotalMP)
			CurrentMP = TotalMP;

		if (CurrentMP <= 0.f)
			CurrentMP = 0.f;
		//-------------------------

		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &CurrentHPRatio);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_1, &CurrentMPRatio);
	}
	
}

void CWorldHPUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CWorldHPUIScript::CWorldHPUIScript()
	:CScript((UINT)SCRIPT_TYPE::WORLDHPUISCRIPT)
	, TotalMP(100.f)
	, TotalHP(100.f)
	, m_OwnerObj(nullptr)
{
}

CWorldHPUIScript::CWorldHPUIScript(CGameObject* _Obj)
	:CScript((UINT)SCRIPT_TYPE::WORLDHPUISCRIPT)
	, TotalMP(100.f)
	, TotalHP(100.f)
	, CurrentMP(0.f)
	, CurrentHP(0.f)
	, m_OwnerObj(_Obj)
{
}

CWorldHPUIScript::~CWorldHPUIScript()
{
}
