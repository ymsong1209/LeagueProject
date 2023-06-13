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

		// ���� ��ġ������ ���� ���̸� ���� ��
		m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // ��ŷ ������ HeightMapShader �� ����

		m_pCSHeightMap->SetBrushTex(m_pBrushTex);		// ����� �귯�� �ؽ��� ����
		m_pCSHeightMap->SetBrushIndex(0);				// �귯�� �ε��� ����
		m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // �귯�� ũ��
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


void CLandScape::SaveToLevelFile(FILE* _File)
{
	
	if (m_pHeightMap->GetKey() == L"HeightMap") {
		wstring HeightMapName = L"";
		HeightMapName += L"HeightMap";
		HeightMapName += std::to_wstring(m_pHeightMap->GetID());
		m_pHeightMap->SetName(HeightMapName);
	}

	wstring strFolderPath = L"";
	strFolderPath += L"texture\\LandScape\\";

	m_pHeightMap->SaveTextureAsDDS(strFolderPath);
	

	wstring test = m_pHeightMap->GetName();
	SaveWString(m_pHeightMap->GetName(), _File);
	

	fwrite(&m_iFaceX, sizeof(UINT), 1, _File);
	fwrite(&m_iFaceZ, sizeof(UINT), 1, _File);
}

void CLandScape::LoadFromLevelFile(FILE* _File)
{

	wstring texturepath;
	LoadWString(texturepath, _File);
	CopyFromLoadedTexture(texturepath);

	fread(&m_iFaceX, sizeof(UINT), 1, _File);
	fread(&m_iFaceZ, sizeof(UINT), 1, _File);
	//������ ȣ�� �� LoadFromLevelFile����
	//������ ���������� m_ifacex�� 0�� �����Ǿ�����.
	SetFace(m_iFaceX, m_iFaceZ);
	//GetMaterial()->SetTexParam(TEX_2, m_pHeightMap);
}