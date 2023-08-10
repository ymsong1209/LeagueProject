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
	, m_bFinalMatUpdate(false)
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
	, m_fAnimSpeed(1.f)
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
	, m_bFinalMatUpdate(false)
	, m_bBlend(_origin.m_bBlend)
	, m_bRepeatBlend(_origin.m_bRepeatBlend)
	, m_fCurBlendTime(_origin.m_fCurBlendTime)
	, m_fMaxBlendTime(_origin.m_fCurBlendTime)
	, m_iBlendStartFrm(_origin.m_iBlendStartFrm)
	, m_iStartFrm(_origin.m_iStartFrm)
	, m_iNextFrm(_origin.m_iNextFrm)
	, m_fBlendRatio(_origin.m_fBlendRatio)
	, m_bDebugAnimator(_origin.m_bDebugAnimator)
	, m_fAnimSpeed(_origin.m_fAnimSpeed)
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
				m_fCurBlendTime += EditorDT;
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
						m_fCurBlendTime += EditorDT;
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

	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;
}

bool CAnimator3D::UpdateData()
{
	if (!m_pCurAnim) return false;
	if (!m_bFinalMatUpdate)
	{

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
		m_bFinalMatUpdate = true;

	}
	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
	return true;
}

void CAnimator3D::ClearData()
{
	if (!m_pCurAnim) return;
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

void CAnimator3D::PlayRepeat(const wstring& _strName, bool _RepeatBlend, bool _blend, float _blendtime, float _animspeed)
{
	m_fAnimSpeed = _animspeed;

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
	else {
		m_pCurAnim = pAnim;
		m_pCurAnim->Reset();
		m_pCurAnim->Play();
	}

	m_bRepeat = true;
	m_bRepeatBlend = _RepeatBlend;

}
void CAnimator3D::PlayOnce(const wstring& _strName, bool _blend, float _blendtime, float _animspeed)
{
	m_fAnimSpeed = _animspeed;

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
	m_bRepeat = false;
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
	ClearData();
	wstring animname = m_pCurAnim->GetClipList().strAnimName;
	m_mapAnim.erase(animname);
	delete m_pCurAnim;
	m_pCurAnim = nullptr;
}

void CAnimator3D::DeleteEveryAnim()
{
	ClearData();
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
			auto it = m_mapAnim.find(pNewAnim->GetName());
			if (it != m_mapAnim.end()) {
				//동일 key값 존재
				delete pNewAnim;
			}
			else {
				m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
			}
		}
	} while (FindNextFile(hFind, &findFileData) != 0);
	FindClose(hFind);
}

void CAnimator3D::CreateAnimFromText(const wstring& _strRelativePath)
{
	wstring FilePath = CPathMgr::GetInst()->GetContentPath();
	FilePath += _strRelativePath;

	// _strRelativePath에서 파일명만 가져옴
	filesystem::path path(_strRelativePath);
	wstring FileName = path.stem();

	wstring FullName = L"fbx\\";
	FullName += FileName;
	FullName += L".fbx";
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(FullName);

	m_MeshDataRelativePath = pMeshData->GetRelativePath();

	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	wstring RelativePath;
	RelativePath += L"animation\\";
	//자동 세이브는 meshdata이름을 딴 폴더 안에 저장됨
	filesystem::path meshpath = m_MeshDataRelativePath;
	wstring meshdataname = meshpath.stem();
	RelativePath += meshdataname;

	// 자동으로 폴더 생성
	filesystem::path directory = filesystem::path(strFilePath + RelativePath);
	if (!filesystem::exists(directory))
		filesystem::create_directories(directory);

	std::wifstream file(FilePath); // 파일을 읽기 모드로 연다
	if (!file) {
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"animator3d / 텍스트 로드 실패.");
		MessageBox(nullptr, szStr, L"텍스트 로드 실패.", MB_OK);
		return;
	}
	std::wstring line;

	while (std::getline(file, line)) { // 각 줄을 읽는다
		std::wistringstream iss(line);

		vector<wstring> tokens = split(line, ',');
		if (tokens.size() != 3) { // 올바른 포맷이 아니면 건너뛴다
			continue;
		}

		std::wstring strAnimName = tokens[0];
		int startFrame = stoi(tokens[1]);
		int endFrame = stoi(tokens[2]);

		wstring AnimFinalName;
		AnimFinalName += FileName;
		AnimFinalName += L"\\";
		if (strAnimName.find(FileName) != wstring::npos) // strAnimName에 FileName이 있다면,
		{
			strAnimName.erase(strAnimName.find(FileName), FileName.length()); // 지운다
		}
		AnimFinalName += strAnimName; // 그리고 AnimFinalName에 붙인다

		CAnim3D* panim = new CAnim3D;
		panim->SetName(AnimFinalName);
		tMTAnimClip clip;
		clip.strAnimName = AnimFinalName;
		clip.iStartFrame = startFrame;
		clip.iEndFrame = endFrame;


		const vector<tMTAnimClip>* animClipPtr = pMeshData->GetMesh()->GetAnimClip();  // GetAnimClip()로부터 포인터를 가져옴
		tMTAnimClip originclip = (*animClipPtr)[0];
		clip.iFrameLength = originclip.iEndFrame - originclip.iStartFrame;
		clip.eMode = originclip.eMode;
		FbxTime::EMode timeMode = clip.eMode;	// 시간 모드
		int frameRate = FbxTime::GetFrameRate(timeMode);	// 프레임 레이트
		panim->SetFrameRate(frameRate);
		double TimePerFrm = 1.f / frameRate;
		clip.dStartTime = TimePerFrm * clip.iStartFrame;
		clip.dEndTime = TimePerFrm * clip.iEndFrame;
		clip.dTimeLength = clip.dEndTime - clip.dStartTime;

		panim->Create(clip);
		panim->m_pOwner = this;
		panim->Save(true);
		delete panim;
	}
	file.close();
}

vector<wstring> CAnimator3D::split(const std::wstring& s, wchar_t delimiter)
{
	std::vector<std::wstring> tokens;
	std::wstring token;
	std::wistringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
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
	fwrite(&m_fAnimSpeed, sizeof(float), 1, _pFile);

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
	fread(&m_fAnimSpeed, sizeof(float), 1, _pFile);

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

void CAnimator3D::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	// 빈 애니메이터는 저장 안함
	// 빈 애니메이터는 meshdata path가 저장안되어있음
	bool isEmpty = false;
	if (m_MeshDataRelativePath == L"") {
		isEmpty = true;
		_objValue.AddMember("IsEmpty", isEmpty, allocator);
		return;
	}
	else {
		_objValue.AddMember("IsEmpty", isEmpty, allocator);
	}


	_objValue.AddMember("MeshDataRelativePath", SaveWStringJson(m_MeshDataRelativePath, allocator), allocator);
	_objValue.AddMember("bRepeat", m_bRepeat, allocator);
	_objValue.AddMember("fAnimSpeed", m_fAnimSpeed, allocator);

	// Anim Save
	Value animArray(kArrayType);
	for (const auto& pair : m_mapAnim)
	{
		Value AnimObject(kObjectType);

		pair.second->Save();
		wstring path = pair.second->GetAnimRelativePath();
		AnimObject.AddMember("Anim3DRelativePath", SaveWStringJson(path, allocator), allocator);

		animArray.PushBack(AnimObject, allocator);
	}
	_objValue.AddMember("mapAnim", animArray, allocator);


	wstring strCurAnimName;
	if (nullptr != m_pCurAnim)
	{
		strCurAnimName = m_pCurAnim->GetName();
	}
	_objValue.AddMember("strCurAnimName", SaveWStringJson(strCurAnimName, allocator), allocator);
}

void CAnimator3D::LoadFromLevelJsonFile(const Value& _componentValue)
{
	bool isEmpty;
	isEmpty = _componentValue["IsEmpty"].GetBool();
	if (isEmpty) return;

	m_MeshDataRelativePath = StrToWStr(_componentValue["MeshDataRelativePath"].GetString());

	// Mesh Load
	Ptr<CMeshData> MeshData = CResMgr::GetInst()->FindRes<CMeshData>(m_MeshDataRelativePath);
	assert(MeshData.Get());
	SetBones(MeshData->GetMesh()->GetBones());

	m_bRepeat = _componentValue["bRepeat"].GetBool();
	m_fAnimSpeed = _componentValue["fAnimSpeed"].GetFloat();

	const Value& mapAnimArray = _componentValue["mapAnim"];
	for (size_t i = 0; i < mapAnimArray.Size(); ++i)
	{
		CAnim3D* pNewAnim = new CAnim3D;

		wstring AnimPath;
		AnimPath = StrToWStr(mapAnimArray[i]["Anim3DRelativePath"].GetString());
		pNewAnim->m_pOwner = this;
		pNewAnim->Load(AnimPath);

		m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
	}

	wstring strCurAnimName;
	strCurAnimName = StrToWStr(_componentValue["strCurAnimName"].GetString());
	m_pCurAnim = FindAnim(strCurAnimName);
}
