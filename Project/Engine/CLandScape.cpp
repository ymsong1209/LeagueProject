#include "pch.h"
#include "CLandScape.h"

#include "CKeyMgr.h"
#include "CEventMgr.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CCamera.h"

#include "CRenderMgr.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"


CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iFaceX(0)
	, m_iFaceZ(0)
	, m_vBrushScale(0.25f, 0.25f)
	, m_pCrossBuffer(nullptr)
	, m_pWeightMapBuffer(nullptr)
	, m_iWeightWidth(0)
	, m_iWeightHeight(0)
	, m_iWeightIdx(1)
	, m_eMod(LANDSCAPE_MOD::SPLAT)
{	
	init();
}

CLandScape::~CLandScape()
{
	if (nullptr != m_pCrossBuffer)
		delete m_pCrossBuffer;
	if (nullptr != m_pWeightMapBuffer)
		delete m_pWeightMapBuffer;
}


void CLandScape::finaltick()
{
	if (KEY_TAP(KEY::_8))
		m_eMod = LANDSCAPE_MOD::NONE;
	else if (KEY_TAP(KEY::_9))
		m_eMod = LANDSCAPE_MOD::HEIGHT_MAP;
	else if (KEY_TAP(KEY::_0))
		m_eMod = LANDSCAPE_MOD::SPLAT;

	if (LANDSCAPE_MOD::NONE == m_eMod)
	{
		return;
	}

	if (KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();

		if (LANDSCAPE_MOD::HEIGHT_MAP == m_eMod)
		{
			// 교점 위치정보를 토대로 높이를 수정 함
			m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // 픽킹 정보를 HeightMapShader 에 세팅

			m_pCSHeightMap->SetBrushTex(m_pBrushTex);		// 사용할 브러쉬 텍스쳐 세팅
			m_pCSHeightMap->SetBrushIndex(0);				// 브러쉬 인덱스 설정
			m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // 브러쉬 크기
			m_pCSHeightMap->SetHeightMap(m_pHeightMap);
			m_pCSHeightMap->Execute();
		}
		else if (LANDSCAPE_MOD::SPLAT == m_eMod)
		{
			// 교점 위치정보를 가중치를 수정함	
			m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // 레이 캐스트 위치
			m_pCSWeightMap->SetBrushArrTex(m_pBrushTex);
			m_pCSWeightMap->SetBrushIndex(0);
			m_pCSWeightMap->SetBrushScale(m_vBrushScale); // 브러쉬 크기
			m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // 가중치맵, 가로 세로 개수			
			m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
			m_pCSWeightMap->Execute();
		}
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

	// 가중치 버퍼 전달
	m_pWeightMapBuffer->UpdateData(17, PIPELINE_STAGE::PS_PIXEL);

	// 가중치 버퍼 해상도 전달
	Vec2 vWeightMapResolution = Vec2((float)m_iWeightWidth, (float)m_iWeightHeight);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &vWeightMapResolution);

	// 타일 배열 개수 전달
	float m_fTileCount = float(m_pTileArrTex->GetArraySize() / 2); // 색상, 노말 합쳐져있어서 나누기 2 해줌
	GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, &m_fTileCount);

	// 타일 텍스쳐 전달
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_ARR_0, m_pTileArrTex);

	GetMaterial()->UpdateData();

	GetMesh()->render();	

	// ==========
	// 리소스 정리
	// ==========
	m_pWeightMapBuffer->Clear();
}

void CLandScape::render_depthmap()
{
	CRenderComponent::render_depthmap();

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
	/*
	if (m_pHeightMap->GetKey() == L"HeightMap") {
		wstring HeightMapName = L"";
		HeightMapName += L"HeightMap";
		HeightMapName += std::to_wstring(m_pHeightMap->GetID());
		m_pHeightMap->SetName(HeightMapName);
	}

	wstring strFolderPath = L"";
	strFolderPath += L"texture\\LandScape\\";

	//m_pHeightMap->Save(strFolderPath);
	//m_pHeightMap->SaveTextureAsDDS(strFolderPath);
	
	wstring test = m_pHeightMap->GetName();
	SaveWString(m_pHeightMap->GetName(), _File);
	*/
	//wstring strFolderPath = L"";
	//strFolderPath += L"texture\\LandScape\\";
	//wstring HeightTexture = L"HeightMap";
	//strFolderPath += HeightTexture;
	//
	//m_pHeightMap->Save(m_pHeightMap->GetKey());

	CRenderComponent::SaveToLevelFile(_File);

	SaveResRef(m_pHeightMap.Get(), _File);
	fwrite(&m_iFaceX, sizeof(UINT), 1, _File);
	fwrite(&m_iFaceZ, sizeof(UINT), 1, _File);
}

void CLandScape::LoadFromLevelFile(FILE* _File)
{
	//wstring texturepath;
	//LoadWString(texturepath, _File);
	//CopyFromLoadedTexture(texturepath);

	CRenderComponent::LoadFromLevelFile(_File);

	LoadResRef(m_pHeightMap, _File);
	fread(&m_iFaceX, sizeof(UINT), 1, _File);
	fread(&m_iFaceZ, sizeof(UINT), 1, _File);
	//생성자 호출 후 LoadFromLevelFile실행
	//생성자 시점에서는 m_ifacex가 0로 설정되어있음.
	SetFace(m_iFaceX, m_iFaceZ);
	//GetMaterial()->SetTexParam(TEX_2, m_pHeightMap);
}


/*
void CLandScape::CopyFromLoadedTexture(wstring _FilePath)
{
	Ptr<CTexture> stagingtex = CResMgr::GetInst()->FindRes<CTexture>(_FilePath);

	string fileName = string(_FilePath.begin(), _FilePath.end());

	// 파일 경로에서 마지막 디렉토리 구분 기호 위치 찾기
	size_t lastSeparatorIndex = fileName.find_last_of("/\\");
	// 마지막 디렉토리 구분 기호 이후의 문자열 추출 (파일명)
	fileName = fileName.substr(lastSeparatorIndex + 1);
	// 파일명에서 확장자 제거
	size_t extensionIndex = fileName.rfind('.');
	if (extensionIndex != std::string::npos)
	{
		fileName = fileName.substr(0, extensionIndex);
	}

	wstring wFileName(fileName.begin(), fileName.end());

	m_pHeightMap = nullptr;
	m_pHeightMap = CResMgr::GetInst()->FindRes<CTexture>(wFileName);
	if (!m_pHeightMap.Get()) {
		m_pHeightMap = CResMgr::GetInst()->CreateTexture(wFileName
			, 2048, 2048
			, DXGI_FORMAT_R32_FLOAT
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
			, D3D11_USAGE_DEFAULT);
	}

	if (stagingtex == nullptr) return;
	CDevice::GetInst()->GetDeviceContext()->CopyResource(m_pHeightMap.Get()->GetTex2D().Get(), stagingtex.Get()->GetTex2D().Get());
}
*/