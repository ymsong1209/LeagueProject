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
			// ���� ��ġ������ ���� ���̸� ���� ��
			m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // ��ŷ ������ HeightMapShader �� ����

			m_pCSHeightMap->SetBrushTex(m_pBrushTex);		// ����� �귯�� �ؽ��� ����
			m_pCSHeightMap->SetBrushIndex(0);				// �귯�� �ε��� ����
			m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // �귯�� ũ��
			m_pCSHeightMap->SetHeightMap(m_pHeightMap);
			m_pCSHeightMap->Execute();
		}
		else if (LANDSCAPE_MOD::SPLAT == m_eMod)
		{
			// ���� ��ġ������ ����ġ�� ������	
			m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // ���� ĳ��Ʈ ��ġ
			m_pCSWeightMap->SetBrushArrTex(m_pBrushTex);
			m_pCSWeightMap->SetBrushIndex(0);
			m_pCSWeightMap->SetBrushScale(m_vBrushScale); // �귯�� ũ��
			m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // ����ġ��, ���� ���� ����			
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

	// ����ġ ���� ����
	m_pWeightMapBuffer->UpdateData(17, PIPELINE_STAGE::PS_PIXEL);

	// ����ġ ���� �ػ� ����
	Vec2 vWeightMapResolution = Vec2((float)m_iWeightWidth, (float)m_iWeightHeight);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &vWeightMapResolution);

	// Ÿ�� �迭 ���� ����
	float m_fTileCount = float(m_pTileArrTex->GetArraySize() / 2); // ����, �븻 �������־ ������ 2 ����
	GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, &m_fTileCount);

	// Ÿ�� �ؽ��� ����
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_ARR_0, m_pTileArrTex);

	GetMaterial()->UpdateData();

	GetMesh()->render();	

	// ==========
	// ���ҽ� ����
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
	// ���� ī�޶� ������
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// ���� ���� ������ ������ ���÷� ����
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// ������ ī�޶� Ray �� ������ ����
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
	//������ ȣ�� �� LoadFromLevelFile����
	//������ ���������� m_ifacex�� 0�� �����Ǿ�����.
	SetFace(m_iFaceX, m_iFaceZ);
	//GetMaterial()->SetTexParam(TEX_2, m_pHeightMap);
}


/*
void CLandScape::CopyFromLoadedTexture(wstring _FilePath)
{
	Ptr<CTexture> stagingtex = CResMgr::GetInst()->FindRes<CTexture>(_FilePath);

	string fileName = string(_FilePath.begin(), _FilePath.end());

	// ���� ��ο��� ������ ���丮 ���� ��ȣ ��ġ ã��
	size_t lastSeparatorIndex = fileName.find_last_of("/\\");
	// ������ ���丮 ���� ��ȣ ������ ���ڿ� ���� (���ϸ�)
	fileName = fileName.substr(lastSeparatorIndex + 1);
	// ���ϸ��� Ȯ���� ����
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