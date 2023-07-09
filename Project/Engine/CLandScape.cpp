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
	, m_eMod(LANDSCAPE_MOD::NONE)
	, m_bIsClicking(false)
	, m_rstype(RS_TYPE::CULL_BACK)
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

	if (LANDSCAPE_MOD::NONE == m_eMod)
	{
		return;
	}
	else {
		Raycasting();
	}

	if (KEY_PRESSED(KEY::LBTN))
	{
		m_bIsClicking = true;
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

	if (KEY_NONE(KEY::LBTN))
		m_bIsClicking = false;

}

void CLandScape::render()
{
	if (GetMesh() == nullptr || GetMaterial(0) == nullptr)
		return;

	Transform()->UpdateData();

	GetMaterial(0)->GetShader()->SetRSType(m_rstype);

	GetMaterial(0)->SetScalarParam(INT_0, &m_iFaceX);
	GetMaterial(0)->SetScalarParam(INT_1, &m_iFaceZ);

	Vec2 vResolution = Vec2(m_pHeightMap->Width(), m_pHeightMap->Height());
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, &vResolution);
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_2, m_pHeightMap);

	// 가중치 버퍼 전달
	m_pWeightMapBuffer->UpdateData(17, PIPELINE_STAGE::PS_PIXEL);
	m_pCrossBuffer->UpdateData(16, PIPELINE_STAGE::PS_PIXEL);

	// 가중치 버퍼 해상도 전달
	Vec2 vWeightMapResolution = Vec2((float)m_iWeightWidth, (float)m_iWeightHeight);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, &vWeightMapResolution);

	// 타일 배열 개수 전달
	float m_fTileCount = float(m_pTileArrTex->GetArraySize() / 2); // 색상, 노말 합쳐져있어서 나누기 2 해줌
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, &m_fTileCount);

	// 타일 텍스쳐 전달
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_ARR_0, m_pTileArrTex);

	//Brush 크기 전달
	GetMaterial(0)->SetScalarParam(VEC2_2, &m_vBrushScale);

	//Click 정보 전달
	GetMaterial(0)->SetScalarParam(INT_2, &m_bIsClicking);

	GetMaterial(0)->UpdateData();

	GetMesh()->render(0);	

	// ==========
	// 리소스 정리
	// ==========
	m_pWeightMapBuffer->Clear();
	m_pCrossBuffer->Clear();
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

void CLandScape::SetLandScapeMod(LANDSCAPE_MOD _mod)
{
	m_eMod = _mod;
	if (m_eMod == LANDSCAPE_MOD::NONE) {
		m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);
	}
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
	// rendercomponent의 savetolevelfile은 mesh까지 저장을 한다.
	// landscape의 mesh는 직접 생성할것이므로 key값이 저장이 안됨.
	// 나머지 부분만 수동으로 저장
	float bound = GetBounding();
	bool frustumUse = IsUseFrustumCheck();
	bool ShadowUse = IsDynamicShadow();

	fwrite(&bound, sizeof(float), 1, _File);
	fwrite(&frustumUse, sizeof(bool), 1, _File);
	fwrite(&ShadowUse, sizeof(bool), 1, _File);

	//HeightMap은 자신의 id를 이름으로 한다.
	if (m_pHeightMap->GetKey() == L"HeightMap") {
		wstring HeightMapName = L"";
		HeightMapName += L"HeightMap";
		HeightMapName += std::to_wstring(m_pHeightMap->GetID());
		m_pHeightMap->SetName(HeightMapName);
	}

	wstring strFolderPath = L"";
	strFolderPath += L"texture\\LandScape\\";

	//heightmap 파일로 저장
	m_pHeightMap->SaveTextureAsDDS(strFolderPath);
	//heightmap 이름 저장
	wstring test = m_pHeightMap->GetName();
	SaveWString(m_pHeightMap->GetName(), _File);


	//WeightMap 구조화버퍼 저장
	fwrite(&m_iWeightWidth, sizeof(UINT), 1, _File);
	fwrite(&m_iWeightHeight, sizeof(UINT), 1, _File);
	fwrite(&m_iWeightIdx, sizeof(UINT), 1, _File);

	UINT bufferSize = m_pWeightMapBuffer->GetBufferSize();
	tWeight_4* data = new tWeight_4[bufferSize / sizeof(tWeight_4)];
	m_pWeightMapBuffer->GetData((void*)data);
	size_t numItems = bufferSize / sizeof(tWeight_4);
	fwrite(&numItems, sizeof(size_t), 1, _File);
	fwrite(data, sizeof(tWeight_4), numItems, _File);

	delete[] data;
	
	fwrite(&m_iFaceX, sizeof(UINT), 1, _File);
	fwrite(&m_iFaceZ, sizeof(UINT), 1, _File);
}

void CLandScape::LoadFromLevelFile(FILE* _File)
{
	float bound;
	bool frustumUse;
	bool ShadowUse;

	fread(&bound, sizeof(float), 1, _File);
	fread(&frustumUse, sizeof(bool), 1, _File);
	fread(&ShadowUse, sizeof(bool), 1, _File);
	SetBounding(bound);
	SetFrustumCheck(frustumUse);
	SetDynamicShadow(ShadowUse);

	//HeightMap 로딩
	wstring HeighMaptexturepath;
	LoadWString(HeighMaptexturepath, _File);
	CopyFromLoadedTexture(HeighMaptexturepath);

	//WeightMap 구조화버퍼 로딩
	fread(&m_iWeightWidth, sizeof(UINT), 1, _File);
	fread(&m_iWeightHeight, sizeof(UINT), 1, _File);
	fread(&m_iWeightIdx, sizeof(UINT), 1, _File);

	size_t numItems;
	fread(&numItems, sizeof(size_t), 1, _File);
	tWeight_4* data = new tWeight_4[numItems];
	size_t readItems = fread(data, sizeof(tWeight_4), numItems, _File);
	m_pWeightMapBuffer->SetData((void*)data, numItems * sizeof(tWeight_4));
	delete[] data;

	fread(&m_iFaceX, sizeof(UINT), 1, _File);
	fread(&m_iFaceZ, sizeof(UINT), 1, _File);
	//생성자 호출 후 LoadFromLevelFile실행
	//생성자 시점에서는 m_ifacex가 0로 설정되어있음.
	SetFace(m_iFaceX, m_iFaceZ);
	//GetMaterial()->SetTexParam(TEX_2, m_pHeightMap);
}

void CLandScape::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	// rendercomponent의 savetolevelfile은 mesh까지 저장을 한다.
	// landscape의 mesh는 직접 생성할것이므로 key값이 저장이 안됨.
	// 나머지 부분만 수동으로 저장
	float bound = GetBounding();
	bool frustumUse = IsUseFrustumCheck();
	bool ShadowUse = IsDynamicShadow();

	_objValue.AddMember("bound", bound, allocator);
	_objValue.AddMember("frustumUse", frustumUse, allocator);
	_objValue.AddMember("ShadowUse", ShadowUse, allocator);

	//HeightMap은 자신의 id를 이름으로 한다.
	if (m_pHeightMap->GetKey() == L"HeightMap") {
		wstring HeightMapName = L"";
		HeightMapName += L"HeightMap";
		HeightMapName += std::to_wstring(m_pHeightMap->GetID());
		m_pHeightMap->SetName(HeightMapName);
	}

	wstring strFolderPath = L"";
	strFolderPath += L"texture\\LandScape\\";

	//heightmap 파일로 저장
	m_pHeightMap->SaveTextureAsDDS(strFolderPath);

	//heightmap 이름 저장
	wstring test = m_pHeightMap->GetName();
	_objValue.AddMember("HeightMapName", SaveWStringJson(m_pHeightMap->GetName(), allocator), allocator);


	//WeightMap 구조화버퍼 저장
	_objValue.AddMember("WeightWidth", m_iWeightWidth, allocator);
	_objValue.AddMember("WeightHeight", m_iWeightHeight, allocator);
	_objValue.AddMember("WeightIdx", m_iWeightIdx, allocator);

	UINT bufferSize = m_pWeightMapBuffer->GetBufferSize();
	tWeight_4* data = new tWeight_4[bufferSize / sizeof(tWeight_4)];
	m_pWeightMapBuffer->GetData((void*)data);
	size_t numItems = bufferSize / sizeof(tWeight_4);

	_objValue.AddMember("numItems", numItems, allocator);

	// data (tweight_4) 구조체 저장
	Value tweight4Value(kObjectType);
	Value arrWeightArray(kArrayType);
	for (int i = 0; i < 4; ++i) {
		arrWeightArray.PushBack(data->arrWeight[i], allocator);
	}
	tweight4Value.AddMember("arrWeight[4]", arrWeightArray, allocator);
	_objValue.AddMember("data", tweight4Value, allocator);
	delete[] data;

	_objValue.AddMember("iFaceX", m_iFaceX, allocator);
	_objValue.AddMember("iFaceZ", m_iFaceZ, allocator);
}

void CLandScape::LoadFromLevelJsonFile(const Value& _componentValue)
{
	float bound;
	bool frustumUse;
	bool ShadowUse;

	bound = _componentValue["bound"].GetFloat();
	frustumUse = _componentValue["frustumUse"].GetBool();
	ShadowUse = _componentValue["ShadowUse"].GetBool();
	SetBounding(bound);
	SetFrustumCheck(frustumUse);
	SetDynamicShadow(ShadowUse);

	//HeightMap 로딩
	wstring HeighMaptexturepath;
	HeighMaptexturepath = StrToWStr(_componentValue["HeightMapName"].GetString());
	CopyFromLoadedTexture(HeighMaptexturepath);

	//WeightMap 구조화버퍼 로딩
	m_iWeightWidth = _componentValue["WeightWidth"].GetUint();
	m_iWeightHeight = _componentValue["WeightHeight"].GetUint();
	m_iWeightIdx = _componentValue["WeightIdx"].GetUint();

	size_t numItems;
	numItems = _componentValue["numItems"].GetUint64();

	tWeight_4* data = new tWeight_4[numItems];
	const Value& arrWeightArray = _componentValue["data"]["arrWeight[4]"];
	for (int i = 0; i < arrWeightArray.Size(); ++i)
	{
		data->arrWeight[i] = arrWeightArray[i].GetFloat();
	}

	m_pWeightMapBuffer->SetData((void*)data, numItems * sizeof(tWeight_4));
	delete[] data;
	
	m_iFaceX = _componentValue["iFaceX"].GetUint();
	m_iFaceZ = _componentValue["iFaceZ"].GetUint();

	//생성자 호출 후 LoadFromLevelFile실행
	//생성자 시점에서는 m_ifacex가 0로 설정되어있음.
	SetFace(m_iFaceX, m_iFaceZ);
	//GetMaterial()->SetTexParam(TEX_2, m_pHeightMap);
}


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