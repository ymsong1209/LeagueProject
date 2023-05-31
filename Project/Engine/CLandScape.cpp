#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iFaceX(0)
	, m_iFaceZ(0)
{	
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"));
}

CLandScape::~CLandScape()
{
}

void CLandScape::SetFace(UINT _iFaceX, UINT _iFaceZ)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
}

void CLandScape::finaltick()
{
}

void CLandScape::render()
{
	if (GetMesh() == nullptr || GetMaterial() == nullptr)
		return;

	Transform()->UpdateData();

	GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
	GetMaterial()->UpdateData();

	GetMesh()->render();	
}