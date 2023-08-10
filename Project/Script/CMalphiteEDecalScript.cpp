#include "pch.h"
#include "CMalphiteEDecalScript.h"
#include "CUnitScript.h"

CMalphiteEDecalScript::CMalphiteEDecalScript()
	:CScript((UINT)SCRIPT_TYPE::MALPHITEEDECALSCRIPT)
{
}

CMalphiteEDecalScript::~CMalphiteEDecalScript()
{
}


void CMalphiteEDecalScript::begin()
{
	SetLifeSpan(3.f);
}

void CMalphiteEDecalScript::tick()
{
	m_fTime += DT;

	GetOwner()->Decal()->SetAlpha(1.f - m_fTime/3.f);

}

