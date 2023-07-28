#include "pch.h"
#include "CAnim3D.h"
#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"

CAnim3D::CAnim3D()
	: m_pOwner(nullptr)
	, m_pClip{}
	, m_fClipUpdateTime(0.f)
	, m_iFrameRate(30)
	, m_dCurTime(0.f)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_bFinish(false)
	, m_bPause(false)
	, m_RelativePath(L"")
{

}


CAnim3D::CAnim3D(const CAnim3D& _other)
	: m_pOwner(nullptr)
	, m_pClip{ _other.m_pClip }
	, m_fClipUpdateTime(_other.m_fClipUpdateTime)
	, m_iFrameRate(_other.m_iFrameRate)
	, m_dCurTime(_other.m_dCurTime)
	, m_iFrameIdx(_other.m_iFrameIdx)
	, m_iNextFrameIdx(_other.m_iFrameIdx)
	, m_fRatio(_other.m_fRatio)
	, m_bFinish(_other.m_bFinish)
	, m_bPause(_other.m_bPause)
	, m_RelativePath(_other.m_RelativePath)
{
	SetName(_other.m_pClip.strAnimName);
}

CAnim3D::~CAnim3D()
{
}


void CAnim3D::finaltick()
{
	if (m_bFinish) return;

	m_dCurTime = 0.f;
	// Pause���°� �ƴ� ��� ���� ������� Clip �� �ð��� �����Ѵ�.
	if (!m_bPause) {
		float speed = m_pOwner->GetSpeed();
		if (m_pOwner->IsDebugAnimator()) {
			m_fClipUpdateTime += EditorDT * speed;
		}
		else {
			m_fClipUpdateTime += EditorDT * speed;
		}
	}

	//������ �����ӿ� ����
	if (m_fClipUpdateTime >= m_pClip.dTimeLength)
	{
		m_bFinish = true;
		m_fRatio = 0.f;
		m_pOwner->SetFrameRatio(0.f);
		return;
	}

	m_dCurTime = m_pClip.dStartTime + (double)m_fClipUpdateTime;

	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_dCurTime * (double)m_iFrameRate;
	m_iFrameIdx = (int)(dFrameIdx);

	// ���� ������ �ε���
	if (m_iFrameIdx >= m_pClip.iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// ���̸� ���� �ε����� ����
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// �����Ӱ��� �ð��� ���� ������ �����ش�.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	m_pOwner->SetCurFrm(m_iFrameIdx);
	m_pOwner->SetNextFrame(m_iNextFrameIdx);
	m_pOwner->SetFrameRatio(m_fRatio);

}

void CAnim3D::Create(const tMTAnimClip& _OriginalVecClip, const wstring& _AnimName)
{
	//Anim ó�� ���鶧�� ���� �״�� ����
	if (_AnimName == L"") {
		if (_OriginalVecClip.strAnimName == L"") {
			assert(nullptr);
		}
		m_pClip = _OriginalVecClip;
		wstring name = ReplacePipeCharacter(_OriginalVecClip.strAnimName);
		m_pClip.strAnimName = name;
		SetName(name);
	}
	else {
		SetName(_AnimName);
		m_pClip.strAnimName = _AnimName;
		m_pClip.iStartFrame = _OriginalVecClip.iStartFrame;
		m_pClip.iEndFrame = _OriginalVecClip.iEndFrame;
		m_pClip.iFrameLength = m_pClip.iEndFrame - m_pClip.iStartFrame;
		m_pClip.eMode = _OriginalVecClip.eMode;
		m_pClip.dStartTime = _OriginalVecClip.dStartTime;
		m_pClip.dEndTime = _OriginalVecClip.dEndTime;
		m_pClip.dTimeLength = m_pClip.dEndTime - m_pClip.dStartTime;
	}

	m_fClipUpdateTime = 0;
}

void CAnim3D::Save(bool _nameincludesMeshPath)
{
	//��ΰ� �����Ǿ��ִ� �ִϸ��̼ǵ��� �ѹ� ����Ǿ����͵���
	if (m_RelativePath != L"") return;

	wstring name = ReplacePipeCharacter(GetName());
	SetName(name);
	m_pClip.strAnimName = name;

	// ���� ��� �����
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	wstring RelativePath;
	RelativePath += L"animation\\";
	wstring MeshDataPath = m_pOwner->GetMeshDataRelativePath();
	if (_nameincludesMeshPath == false) {
		//�ڵ� ���̺�� meshdata�̸��� �� ���� �ȿ� �����
		
		filesystem::path meshpath = MeshDataPath;
		wstring meshdataname = meshpath.stem();
		RelativePath += meshdataname + L"\\";
	}
	RelativePath += name + L".anim3d";
	strFilePath += RelativePath;

	m_RelativePath = RelativePath;

	
	// �ش��ο� �ִϸ��̼��� ����Ǿ������� return
	// fbx�ε��Ҷ� ��� �ִϸ��̼� ������ ��ٶ� animation�� ���� ����� �����ȵ��־
	// ���⼭ üũ�������.
	if (filesystem::exists(strFilePath)) return;
	
	// �ڵ����� ���� ����
	filesystem::path directory = filesystem::path(strFilePath).parent_path();
	if (!filesystem::exists(directory))
		filesystem::create_directories(directory);


	// ���� ������� ����
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	SaveWString(m_RelativePath, pFile);
	SaveWString(GetName(), pFile);

	SaveWString(m_pClip.strAnimName, pFile);
	fwrite(&m_pClip.iStartFrame, sizeof(int), 1, pFile);
	fwrite(&m_pClip.iEndFrame, sizeof(int), 1, pFile);
	fwrite(&m_pClip.iFrameLength, sizeof(int), 1, pFile);
	fwrite(&m_pClip.dStartTime, sizeof(double), 1, pFile);
	fwrite(&m_pClip.dEndTime, sizeof(double), 1, pFile);
	fwrite(&m_pClip.dTimeLength, sizeof(double), 1, pFile);
	fwrite(&m_pClip.eMode, sizeof(FbxTime::EMode), 1, pFile);
	fwrite(&m_iFrameRate, sizeof(int), 1, pFile);
	SaveWString(MeshDataPath, pFile);
	fclose(pFile);
}


void CAnim3D::Save(const wstring& _strRelativePath)
{

	// ����� ����
	m_RelativePath = _strRelativePath;

	// ���� ��� �����
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	// ���� ������� ����
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	SaveWString(m_RelativePath, pFile);
	SaveWString(GetName(), pFile);

	SaveWString(m_pClip.strAnimName, pFile);
	fwrite(&m_pClip.iStartFrame, sizeof(int), 1, pFile);
	fwrite(&m_pClip.iEndFrame, sizeof(int), 1, pFile);
	fwrite(&m_pClip.iFrameLength, sizeof(int), 1, pFile);
	fwrite(&m_pClip.dStartTime, sizeof(double), 1, pFile);
	fwrite(&m_pClip.dEndTime, sizeof(double), 1, pFile);
	fwrite(&m_pClip.dTimeLength, sizeof(double), 1, pFile);
	fwrite(&m_pClip.eMode, sizeof(FbxTime::EMode), 1, pFile);
	fwrite(&m_iFrameRate, sizeof(int), 1, pFile);
	wstring MeshDataPath = m_pOwner->GetMeshDataRelativePath();
	SaveWString(MeshDataPath, pFile);
	fclose(pFile);
}

void CAnim3D::Load(const wstring& _strRelativePath)
{
	// ���� ��� �����
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	// �б���� ���Ͽ���
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	LoadWString(m_RelativePath, pFile);
	wstring name;
	LoadWString(name, pFile);
	SetName(name);


	LoadWString(m_pClip.strAnimName, pFile);
	fread(&m_pClip.iStartFrame, sizeof(int), 1, pFile);
	fread(&m_pClip.iEndFrame, sizeof(int), 1, pFile);
	fread(&m_pClip.iFrameLength, sizeof(int), 1, pFile);
	fread(&m_pClip.dStartTime, sizeof(double), 1, pFile);
	fread(&m_pClip.dEndTime, sizeof(double), 1, pFile);
	fread(&m_pClip.dTimeLength, sizeof(double), 1, pFile);
	fread(&m_pClip.eMode, sizeof(FbxTime::EMode), 1, pFile);
	fread(&m_iFrameRate, sizeof(int), 1, pFile);
	wstring MeshDataPath;
	LoadWString(MeshDataPath, pFile);

	// �� �ִϸ����Ϳ� �ִϸ��̼��� �ִ°��
	// �ִϸ��̼��� �ε��Ҷ� ���� meshdata�� ������ ���� ����־������
	Ptr<CMeshData> MeshData = CResMgr::GetInst()->FindRes<CMeshData>(MeshDataPath);
	assert(MeshData.Get());

	m_pOwner->SetMeshDataRelativePath(MeshDataPath);

	// LevelSaveLoad���� Animator���Ŀ� meshrender�� ����
	// Mesh, Material�ε�� Meshrender Load�� �Ŀ� �̷��������.
	// �Ʒ� �ڵ�� mesh, material �����ȵ� meshrender�� ���ο� �ִϸ��̼� �ε��Ҷ��� �̷��������.
	// animator ������, addcompnent���� ���� �����ϱ� ������
	// levelsaveload �ܰ迡���� anim�� �ƴ� animator���� mesh,material�� �ε���Ų��.
	if (m_pOwner->GetOwner()) {
		m_pOwner->MeshRender()->SetMesh(MeshData->GetMesh());
		for (UINT i = 0; i < MeshData->GetMaterial().size(); ++i) {
			m_pOwner->MeshRender()->SetMaterial(MeshData->GetMaterial()[i], i);
		}
		m_pOwner->SetBones(MeshData->GetMesh()->GetBones());
	}


	fclose(pFile);
}
