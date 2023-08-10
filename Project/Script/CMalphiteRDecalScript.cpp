#include "pch.h"
#include "CMalphiteRDecalScript.h"
#include "CUnitScript.h"

CMalphiteRDecalScript::CMalphiteRDecalScript()
	:CScript((UINT)SCRIPT_TYPE::MALPHITERDECALSCRIPT)
{
}

CMalphiteRDecalScript::~CMalphiteRDecalScript()
{
}


void CMalphiteRDecalScript::begin()
{
	SetLifeSpan(3.f);
}

void CMalphiteRDecalScript::tick()
{
	m_fTime += DT;

	GetOwner()->Decal()->SetAlpha(1.f - m_fTime / 3.f);

}

