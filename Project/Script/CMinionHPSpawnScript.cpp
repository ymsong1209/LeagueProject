#include "pch.h"
#include "CMinionHPSpawnScript.h"
#include "CUnitScript.h"
#include "CMinionHPRatioScript.h"

CMinionHPSpawnScript::CMinionHPSpawnScript()
	:CScript((UINT)SCRIPT_TYPE::MINIONHPSPAWNSCRIPT)
	, m_bInit(false)
{
}

CMinionHPSpawnScript::~CMinionHPSpawnScript()
{
}

void CMinionHPSpawnScript::begin()
{

}

void CMinionHPSpawnScript::tick()
{
}

void CMinionHPSpawnScript::BeginOverlap(CCollider2D* _Other)
{
}


