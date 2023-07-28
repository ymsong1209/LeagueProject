#include "pch.h"
#include "CExpRatioUIScript.h"
#include <Engine\define.h>
#include <Engine\CKeyMgr.h>

void CExpRatioUIScript::begin()
{
}

void CExpRatioUIScript::tick()
{
	//------µð¹ö±ë¿ë-------------
	if (KEY_PRESSED(KEY::P))
		CurrentExp += DT * 50.f;

	if (KEY_PRESSED(KEY::O))
		CurrentExp -= DT * 50.f;
	//--------------------------------
	
	CurrentExpRatio = CurrentExp / LevelTotalExp;

	if (CurrentExp >= LevelTotalExp)
		CurrentExp = LevelTotalExp;
	if (CurrentExp <= 0.f)
		CurrentExp = 0.f;

	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &CurrentExpRatio);
}

void CExpRatioUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CExpRatioUIScript::CExpRatioUIScript()
	:CScript((UINT)SCRIPT_TYPE::EXPRATIOUISCRIPT)
	, LevelTotalExp(100.f)
	, CurrentExp(0.f)
{

}

CExpRatioUIScript::~CExpRatioUIScript()
{
}
