#include "pch.h"
#include "CHpMpRatioUIScript.h"


CHpMpRatioUIScript::CHpMpRatioUIScript(BARTYPE _Type)
	:CScript((UINT)SCRIPT_TYPE::HPMPRATIOUISCRIPT)
	,BarType(_Type)
	,Total(100.f)
{
}

CHpMpRatioUIScript::CHpMpRatioUIScript()
	:CScript((UINT)SCRIPT_TYPE::HPMPRATIOUISCRIPT)
{
}


CHpMpRatioUIScript::~CHpMpRatioUIScript()
{
}


void CHpMpRatioUIScript::begin()
{

}

void CHpMpRatioUIScript::tick()
{
	//매틱마다 현재 체력, 전체체력 가져와야함(전체체력도 변할 수 있음)
	// 바 타입이 mp라면 mp를 가져오고 hp라면 hp를 가져오면 됨.
	//--------------------------------

	//------디버깅용-------------
	if (BarType == BARTYPE::HP)
	{
		if (KEY_PRESSED(KEY::B))
			Current += DT * 50.f;

		if (KEY_PRESSED(KEY::V))
			Current -= DT * 50.f;
	}

	if (BarType == BARTYPE::MP)
	{
		if (KEY_PRESSED(KEY::G))
			Current += DT * 50.f;

		if (KEY_PRESSED(KEY::F))
			Current -= DT * 50.f;
	}
	//--------------------------------

	//CurrentExpRatio = CurrentExp / LevelTotalExp;

	//if (CurrentExp >= LevelTotalExp)
	//	CurrentExp = LevelTotalExp;
	//if (CurrentExp <= 0.f)
	//	CurrentExp = 0.f;

	CurrentRatio = Current / Total;
	if (Current >= Total)
		Current = Total;

	if (Current <= 0.f)
		Current = 0.f;

	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &CurrentRatio);
}

void CHpMpRatioUIScript::BeginOverlap(CCollider2D* _Other)
{
}

