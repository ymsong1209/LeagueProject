#include "pch.h"
#include "CLandScape.h"

#include "CKeyMgr.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CCamera.h"

#include "CRenderMgr.h"
#include "CStructuredBuffer.h"


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
	if (nullptr != m_pCrossBuffer)
		delete m_pCrossBuffer;
}


void CLandScape::finaltick()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();

		// 교점 위치정보를 토대로 높이를 수정 함
		m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // 픽킹 정보를 HeightMapShader 에 세팅

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



void CLandScape::Raycasting()
{
	// 시점 카메라를 가져옴
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// 월드 기준 광선을 지형의 로컬로 보냄
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	tRaycastOut out = { Vec2(0.f, 0.f), 0x7fffffff, 0 };
	m_pCrossBuffer->SetData(&out, 1);

	m_pCSRaycast->SetHeightMap(m_pHeightMap);
	m_pCSRaycast->SetFaceCount(m_iFaceX, m_iFaceZ);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Execute();
}


void CLandScape::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	fwrite(&m_iFaceX, sizeof(UINT), 1, _File);
	fwrite(&m_iFaceZ, sizeof(UINT), 1, _File);
}

void CLandScape::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);
	fread(&m_iFaceX, sizeof(UINT), 1, _File);
	fread(&m_iFaceZ, sizeof(UINT), 1, _File);
	//생성자 호출 후 LoadFromLevelFile실행
	//생성자 시점에서는 m_ifacex가 0로 설정되어있음.
	SetFace(m_iFaceX, m_iFaceZ);
}