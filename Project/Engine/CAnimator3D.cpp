#include "pch.h"

#include "CAnimator3D.h"
#include "CAnim3D.h"


#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"



CAnimator3D::CAnimator3D()
	: m_mapAnim()
	, m_pVecBones(nullptr)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bBlend(false)
	, m_bRepeatBlend(false)
	, m_bRepeatBlending(false)
	, m_fCurBlendTime(0.f)
	, m_fMaxBlendTime(0.f)
	, m_iBlendStartFrm(0)
	, m_iStartFrm(0)
	, m_iNextFrm(0)
	, m_fBlendRatio(0)
	, m_bDebugAnimator(false)
	, m_vecFinalBoneMat()
	, m_MeshDataRelativePath()
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_mapAnim()
	, m_pVecBones(_origin.m_pVecBones)
	, m_pCurAnim(nullptr)
	, m_bRepeat(_origin.m_bRepeat)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bBlend(_origin.m_bBlend)
	, m_bRepeatBlend(_origin.m_bRepeatBlend)
	, m_fCurBlendTime(_origin.m_fCurBlendTime)
	, m_fMaxBlendTime(_origin.m_fCurBlendTime)
	, m_iBlendStartFrm(_origin.m_iBlendStartFrm)
	, m_iStartFrm(_origin.m_iStartFrm)
	, m_iNextFrm(_origin.m_iNextFrm)
	, m_fBlendRatio(_origin.m_fBlendRatio)
	, m_bDebugAnimator(_origin.m_bDebugAnimator)
	, m_vecFinalBoneMat(_origin.m_vecFinalBoneMat)
	, m_MeshDataRelativePath(_origin.m_MeshDataRelativePath)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;

	for (const auto& pair : _origin.m_mapAnim)
	{
		CAnim3D* Animation = new CAnim3D(*pair.second);
		Animation->m_pOwner = this;
		m_mapAnim.insert(make_pair(Animation->GetName(), Animation));
	}

	if (_origin.m_pCurAnim) {
		map<wstring, CAnim3D*>::iterator iter = m_mapAnim.find(_origin.m_pCurAnim->GetName());
		m_pCurAnim = iter->second;
	}
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;

	Safe_Del_Map(m_mapAnim);
}


void CAnimator3D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		//blend 옵션 켜졌으면 애니메이션이 blend되면서 재생
		//애니메이션1 -> 애니메이션2로 재생중
		if (m_bBlend) {
			if (m_bDebugAnimator) {
				m_fCurBlendTime += EditorDT;
			}
			else {
				m_fCurBlendTime += DT;
			}
			m_fBlendRatio = m_fCurBlendTime / m_fMaxBlendTime;

			if (m_fBlendRatio >= 1.f) {
				m_bBlend = false;
				m_fCurBlendTime = 0.f;
				m_fBlendRatio = 0.f;
			}
		}
		else {
			//반복재생
			if (m_bRepeat)
			{
				if (m_pCurAnim->IsFinish()) {
					m_pCurAnim->Reset();
					if (m_bRepeatBlend) {
						m_bRepeatBlending = true;
					}
				}
				//처음 프레임으로 blend해서 돌아오기
				if (m_bRepeatBlending) {

					if (m_bDebugAnimator) {
						m_fCurBlendTime += EditorDT;
					}
					else {
						m_fCurBlendTime += DT;
					}
					m_iBlendStartFrm = m_pCurAnim->GetClipList().iEndFrame;
					m_fBlendRatio = m_fCurBlendTime / m_fMaxBlendTime;

					if (m_fBlendRatio >= 1.f) {
						m_fCurBlendTime = 0.f;
						m_fBlendRatio = 0.f;
						m_bRepeatBlending = false;
					}
				}
			}
		}

		m_pCurAnim->finaltick();
	}
}

void CAnimator3D::UpdateData()
{
	if (!m_pCurAnim) return;

	//if (!m_pCurAnim->GetFinalMatUpdate())
	//{


	//	m_pCurAnim->SetFinalMatUpdate(true);
	//}

	// Animation3D Update Compute Shader
	CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

	// Bone Data
	Ptr<CMesh> pMesh = MeshRender()->GetMesh();
	check_mesh(pMesh);

	pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
	pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
	pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

	UINT iBoneCount = (UINT)m_pVecBones->size();
	pUpdateShader->SetBoneCount(iBoneCount);
	pUpdateShader->SetFrameIndex(m_iStartFrm);
	pUpdateShader->SetNextFrameIdx(m_iNextFrm);
	pUpdateShader->SetBlendFrameIdx(m_iBlendStartFrm);
	pUpdateShader->SetFrameRatio(m_fFrameRatio);
	pUpdateShader->SetBlendRatio(m_fBlendRatio);

	// 업데이트 쉐이더 실행
	pUpdateShader->Execute();


	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::Play(const wstring& _strName, bool _bRepeat, bool _RepeatBlend, bool _blend, float _blendtime)
{

	CAnim3D* pAnim = FindAnim(_strName);
	assert(pAnim);


	if (_blend) {
		//현재 애니메이션에서 다른 애니메이션으로 blend하면서 들어가기
		if (GetCurAnim()) {
			m_pCurAnim->Reset();
			m_bBlend = true;
			m_fCurBlendTime = 0.f;
			m_fMaxBlendTime = _blendtime;
			m_iBlendStartFrm = GetCurAnim()->GetCurFrameIdx();
		}
		//현재 애니메이션이 없을 경우에는 blend 옵션 없이 그대로 재생
		else {
			m_pCurAnim = pAnim;
			m_pCurAnim->Reset();
			m_bRepeat = _bRepeat;
			m_pCurAnim->Play();
		}
	}
	else {
		m_pCurAnim = pAnim;
		m_pCurAnim->Reset();
		m_bRepeat = _bRepeat;
		m_pCurAnim->Play();
	}

	m_bRepeatBlend = _RepeatBlend;
}
void CAnimator3D::Play(const wstring& _strName, bool _blend, float _blendtime)
{
	CAnim3D* pAnim = FindAnim(_strName);
	assert(pAnim);

	//현재 애니메이션에서 다른 애니메이션으로 blend하면서 들어가기
	if (_blend) {
		//현재 애니메이션이 있음
		if (GetCurAnim()) {
			m_pCurAnim->Reset();
			m_bBlend = true;
			m_fCurBlendTime = 0.f;
			m_fMaxBlendTime = _blendtime;
			m_iBlendStartFrm = GetCurAnim()->GetCurFrameIdx();
			m_pCurAnim = pAnim;
			m_pCurAnim->Reset();
			m_pCurAnim->Play();
		}
		//현재 애니메이션이 없을 경우에는 blend 옵션 없이 그대로 재생
		else {
			m_pCurAnim = pAnim;
			m_pCurAnim->Reset();
			m_pCurAnim->Play();
		}
	}
	//blend없이 바로 재생
	else {
		//현재 애니메이션 존재할 경우 리셋
		if (m_pCurAnim) {
			m_pCurAnim->Reset();
		}

		m_pCurAnim = pAnim;
		m_pCurAnim->Reset();
		m_pCurAnim->Play();
	}
	m_bRepeatBlend = false;
}



void CAnimator3D::Pause()
{
	if (m_pCurAnim) {
		m_pCurAnim->Pause();
	}
}

void CAnimator3D::Reset()
{
	if (m_pCurAnim) {
		m_pCurAnim->Reset();
	}
}

void CAnimator3D::Stop()
{
	if (m_pCurAnim) {
		m_pCurAnim->Reset();
		m_pCurAnim->Pause();
		m_iStartFrm = m_pCurAnim->GetClipList().iStartFrame;
		m_iBlendStartFrm = m_pCurAnim->GetClipList().iStartFrame;
		m_fFrameRatio = 0.f;
		m_fBlendRatio = 0.f;
	}
}

CAnim3D* CAnimator3D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnim3D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

CAnim3D* CAnimator3D::LoadAnim(const wstring& _strRelativePath)
{
	//Find Animation Name from Filepath
	size_t pos = _strRelativePath.find_last_of(L"\\");
	wstring filename = (pos == std::wstring::npos) ? _strRelativePath : _strRelativePath.substr(pos + 1);
	size_t dot_pos = filename.find_last_of(L".");
	wstring filename_without_extension = (dot_pos == wstring::npos) ? filename : filename.substr(0, dot_pos);
	//현재 Map에 애니메이션 있으면 return
	CAnim3D* pAnim = FindAnim(filename_without_extension);
	if (pAnim == nullptr) {
		pAnim = new CAnim3D;
	}
	else
	{
		return nullptr;
	}
	pAnim->m_pOwner = this;
	pAnim->Load(_strRelativePath);
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));


	return pAnim;
}

CAnim3D* CAnimator3D::CreateAnimation(const tMTAnimClip& _OriginalVecClip, const wstring& _AnimName)
{
	CAnim3D* pAnim = new CAnim3D;

	pAnim->Create(_OriginalVecClip, _AnimName);
	FbxTime::EMode timeMode = _OriginalVecClip.eMode;	// 시간 모드
	int frameRate = FbxTime::GetFrameRate(timeMode);	// 프레임 레이트
	pAnim->SetFrameRate(frameRate);
	pAnim->m_pOwner = this;

	if (_AnimName == L"") {
		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
	}
	else {
		pAnim->SetName(_AnimName);
		m_mapAnim.insert(make_pair(_AnimName, pAnim));
	}

	return pAnim;

}

void CAnimator3D::DeleteCurrentAnim()
{
	assert(m_pCurAnim);
	wstring animname = m_pCurAnim->GetClipList().strAnimName;
	m_mapAnim.erase(animname);
	delete m_pCurAnim;
	m_pCurAnim = nullptr;
}

void CAnimator3D::DeleteEveryAnim()
{
	m_pCurAnim = nullptr;
	if (m_mapAnim.empty()) return;
	for (auto it = m_mapAnim.begin(); it != m_mapAnim.end(); ++it) {
		delete it->second;
	}
	m_mapAnim.clear();
}

void CAnimator3D::LoadEveryAnimFromFolder(const std::wstring& _strRelativePath) {
	std::wstring folderPath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	WIN32_FIND_DATA findFileData;
	HANDLE hFind;

	std::wstring search_path = folderPath + L"/*.anim3d";
	hFind = FindFirstFile(search_path.c_str(), &findFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		// 폴더 내 .anim3d 파일 없음
		return;
	}
	do {
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			std::wstring animRelativePath = _strRelativePath + L"/" + findFileData.cFileName;
			CAnim3D* pNewAnim = new CAnim3D;
			pNewAnim->m_pOwner = this;
			pNewAnim->Load(animRelativePath);
			m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
		}
	} while (FindNextFile(hFind, &findFileData) != 0);
	FindClose(hFind);
}



void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}




void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
	// 빈 애니메이터는 저장 안함
	// 빈 애니메이터는 meshdata path가 저장안되어있음
	bool isEmpty = false;
	if (m_MeshDataRelativePath == L"") {
		isEmpty = true;
		fwrite(&isEmpty, sizeof(bool), 1, _pFile);
		return;
	}
	else {
		fwrite(&isEmpty, sizeof(bool), 1, _pFile);
	}

	SaveWString(m_MeshDataRelativePath, _pFile);
	fwrite(&m_bRepeat, sizeof(bool), 1, _pFile);
	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _pFile);

	for (const auto& pair : m_mapAnim)
	{
		pair.second->Save();
		wstring path = pair.second->GetAnimRelativePath();
		SaveWString(path, _pFile);
	}

	wstring strCurAnimName;
	if (nullptr != m_pCurAnim)
	{
		strCurAnimName = m_pCurAnim->GetName();
	}
	SaveWString(strCurAnimName, _pFile);

}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
	bool isempty;
	fread(&isempty, sizeof(bool), 1, _pFile);
	if (isempty) return;

	LoadWString(m_MeshDataRelativePath, _pFile);
	// Mesh Load
	Ptr<CMeshData> MeshData = CResMgr::GetInst()->FindRes<CMeshData>(m_MeshDataRelativePath);
	assert(MeshData.Get());
	SetBones(MeshData->GetMesh()->GetBones());

	fread(&m_bRepeat, sizeof(bool), 1, _pFile);
	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _pFile);

	for (size_t i = 0; i < AnimCount; ++i)
	{
		CAnim3D* pNewAnim = new CAnim3D;

		wstring AnimPath;
		LoadWString(AnimPath, _pFile);
		pNewAnim->m_pOwner = this;
		pNewAnim->Load(AnimPath);

		m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));

	}

	wstring strCurAnimName;
	LoadWString(strCurAnimName, _pFile);

	m_pCurAnim = FindAnim(strCurAnimName);
}