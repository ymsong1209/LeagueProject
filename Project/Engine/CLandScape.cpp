#include "pch.h"
#include "CLandScape.h"

#include "CKeyMgr.h"

#include "CResMgr.h"
#include "CTransform.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iFaceX(0)
	, m_iFaceZ(0)
	, m_vBrushScale(0.1f, 0.1f)
{	
	init();
}

CLandScape::~CLandScape()
{
}


void CLandScape::finaltick()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		// 교점 위치정보를 토대로 높이를 수정 함
		//m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // 픽킹 정보를 HeightMapShader 에 세팅

		m_pCSHeightMap->SetBrushTex(m_pBrushTex);		// 사용할 브러쉬 텍스쳐 세팅
		m_pCSHeightMap->SetBrushIndex(0);				// 브러쉬 인덱스 설정
		m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // 브러쉬 크기
		m_pCSHeightMap->SetHeightMap(m_pHeightMap);
		m_pCSHeightMap->Execute();
	}
}

void CLandScape::render()
{
	if (GetMesh() == nullptr || GetMaterial() == nullptr)
		return;

	Transform()->UpdateData();

	//GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	GetMaterial()->SetScalarParam(INT_0, &m_iFaceX);
	GetMaterial()->SetScalarParam(INT_1, &m_iFaceZ);
	GetMaterial()->SetTexParam(TEX_2, m_HeightMap);

	Vec2 vResolution = Vec2(m_pHeightMap->Width(), m_pHeightMap->Height());
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &vResolution);
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_2, m_pHeightMap);



	GetMaterial()->UpdateData();

	GetMesh()->render();	
}


void CLandScape::SetFace(UINT _iFaceX, UINT _iFaceZ)
{
	m_iFaceX = _iFaceX;
	m_iFaceZ = _iFaceZ;


	CreateMesh();
}

