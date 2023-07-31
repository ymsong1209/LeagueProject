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
	// Pause상태가 아닐 경우 현재 재생중인 Clip 의 시간을 진행한다.
	if (!m_bPause) {
		float speed = m_pOwner->GetSpeed();
		if (m_pOwner->IsDebugAnimator()) {
			m_fClipUpdateTime += EditorDT * speed;
		}
		else {
			m_fClipUpdateTime += EditorDT * speed;
		}
	}

	//마지막 프레임에 왔음
	if (m_fClipUpdateTime >= m_pClip.dTimeLength)
	{
		m_bFinish = true;
		m_fRatio = 0.f;
		m_pOwner->SetFrameRatio(0.f);
		return;
	}

	m_dCurTime = m_pClip.dStartTime + (double)m_fClipUpdateTime;

	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_iFrameRate;
	m_iFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iFrameIdx >= m_pClip.iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	m_pOwner->SetCurFrm(m_iFrameIdx);
	m_pOwner->SetNextFrame(m_iNextFrameIdx);
	m_pOwner->SetFrameRatio(m_fRatio);

}

void CAnim3D::Create(const tMTAnimClip& _OriginalVecClip, const wstring& _AnimName)
{
	//Anim 처음 만들때는 원본 그대로 복사
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
	//경로가 설정되어있던 애니메이션들은 한번 저장되었던것들임
	if (m_RelativePath != L"") return;

	wstring name = ReplacePipeCharacter(GetName());
	SetName(name);
	m_pClip.strAnimName = name;

	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	wstring RelativePath;
	RelativePath += L"animation\\";
	wstring MeshDataPath = m_pOwner->GetMeshDataRelativePath();
	if (_nameincludesMeshPath == false) {
		//자동 세이브는 meshdata이름을 딴 폴더 안에 저장됨
		
		filesystem::path meshpath = MeshDataPath;
		wstring meshdataname = meshpath.stem();
		RelativePath += meshdataname + L"\\";
	}
	RelativePath += name + L".anim3d";
	strFilePath += RelativePath;

	m_RelativePath = RelativePath;

	
	// 해당경로에 애니메이션이 저장되어있으면 return
	// fbx로딩할때 모든 애니메이션 합쳐진 기다란 animation은 아직 상대경로 지정안되있어서
	// 여기서 체크해줘야함.
	if (filesystem::exists(strFilePath)) return;
	
	// 자동으로 폴더 생성
	filesystem::path directory = filesystem::path(strFilePath).parent_path();
	if (!filesystem::exists(directory))
		filesystem::create_directories(directory);


	// 파일 쓰기모드로 열기
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

	// 상대경로 저장
	m_RelativePath = _strRelativePath;

	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	// 파일 쓰기모드로 열기
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
	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	// 읽기모드로 파일열기
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

	// 빈 애니메이터에 애니메이션을 넣는경우
	// 애니메이션을 로딩할때 원본 meshdata의 정보를 같이 집어넣어줘야함
	Ptr<CMeshData> MeshData = CResMgr::GetInst()->FindRes<CMeshData>(MeshDataPath);
	assert(MeshData.Get());

	m_pOwner->SetMeshDataRelativePath(MeshDataPath);

	// LevelSaveLoad에선 Animator이후에 meshrender가 생김
	// Mesh, Material로드는 Meshrender Load된 후에 이루어져야함.
	// 아래 코드는 mesh, material 지정안된 meshrender에 새로운 애니메이션 로딩할때만 이루어져야함.
	// animator 생성후, addcompnent으로 주인 지정하기 때문에
	// levelsaveload 단계에서는 anim이 아닌 animator에서 mesh,material을 로딩시킨다.
	if (m_pOwner->GetOwner()) {
		m_pOwner->MeshRender()->SetMesh(MeshData->GetMesh());
		for (UINT i = 0; i < MeshData->GetMaterial().size(); ++i) {
			m_pOwner->MeshRender()->SetMaterial(MeshData->GetMaterial()[i], i);
		}
		m_pOwner->SetBones(MeshData->GetMesh()->GetBones());
	}


	fclose(pFile);
}
