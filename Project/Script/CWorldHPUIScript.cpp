#include "pch.h"
#include "CWorldHPUIScript.h"

void CWorldHPUIScript::begin()
{
}


void CWorldHPUIScript::tick()
{
	//------µð¹ö±ë¿ë-------------

	if (KEY_PRESSED(KEY::B))
		CurrentHP += DT * 50.f;

	if (KEY_PRESSED(KEY::V))
		CurrentHP -= DT * 50.f;

	if (KEY_PRESSED(KEY::G))
		CurrentMP += DT * 50.f;

	if (KEY_PRESSED(KEY::F))
		CurrentMP -= DT * 50.f;
	//--------------------------------

	//CurrentExpRatio = CurrentExp / LevelTotalExp;
	//if (CurrentExp >= LevelTotalExp)
	//	CurrentExp = LevelTotalExp;
	//if (CurrentExp <= 0.f)
	//	CurrentExp = 0.f;

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

	//µð¹ö±ë¿ë
	CurrentHPRatio = 0.6f;
	CurrentMPRatio = 0.3f;
	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &CurrentHPRatio);
	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, &CurrentMPRatio);

}

void CWorldHPUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CWorldHPUIScript::CWorldHPUIScript()
	:CScript((UINT)SCRIPT_TYPE::WORLDHPUISCRIPT)
	, TotalMP(100.f)
	, TotalHP(100.f)
{
}

CWorldHPUIScript::~CWorldHPUIScript()
{
}
