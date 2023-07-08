#include "pch.h"
#include "CAnim2D.h"

#include "CTimeMgr.h"
#include "CPathMgr.h"

CAnim2D::CAnim2D()
	: m_pOwner(nullptr)
	, m_iCurFrm(0)
	, m_fTime(0.f)
	, m_bFinish(false)
	, m_bDynamicTransform(false)
{
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{
	if (m_bFinish)
		return;

	m_vAtlasResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_fTime += EditorDT;

	// 프레임의 Duration보다 누적시간이 커지면
	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		// 누적시간을 Duration만큼 빼주어 초기화하고 프레임을 넘긴다.
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;

		// 마지막 프레임을 넘어가면 마지막 프레임으로 고정하고 Finish옵션을 켠다
		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}
}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSizeUV, int _FrameCount, int _FPS)
{
	// 아틀라스 텍스처 없으면 Assert
	assert(_AtlasTex.Get());

	// 애니메이션 이름 설정
	SetName(_strAnimName);

	// 아틀라스 이미지, 해상도 설정
	m_AtlasTex = _AtlasTex;
	m_vAtlasResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	// FPS 대입
	m_iFPS = _FPS;

	// BackSize 설정
	m_vBackSizeUV = _vBackSizeUV;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		// 아틀라스 텍스처에 애니메이션 프레임이 일렬로 정렬되어있을 때 기준
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / m_vAtlasResolution;
		frm.SliceUV = _vSlice / m_vAtlasResolution;

		// DynamicTransform 초기값
		frm.DynamicPos = Vec3(0.f, 0.f, 0.f);
		frm.DynamicScale = Vec3(1.f, 1.f, 1.f);
		frm.DynamicRot = Vec3(0.f, 0.f, 0.f);

		m_vecFrm.push_back(frm);
	}
}

void CAnim2D::SaveToLevelFile(FILE* _File)
{
	SaveWString(GetName(), _File);
	fwrite(&m_bDynamicTransform, sizeof(bool), 1, _File);

	size_t FrameCount = m_vecFrm.size();
	fwrite(&FrameCount, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), FrameCount, _File);
	fwrite(&m_vBackSizeUV, sizeof(Vec2), 1, _File);

	SaveResRef(m_AtlasTex.Get(), _File);
}

void CAnim2D::LoadFromLevelFile(FILE* _File)
{
	wstring name;
	LoadWString(name, _File);
	SetName(name);
	fread(&m_bDynamicTransform, sizeof(bool), 1, _File);

	size_t FrameCount = 0;
	fread(&FrameCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < FrameCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, _File);
		m_vecFrm.push_back(frm);
	}

	fread(&m_vBackSizeUV, sizeof(Vec2), 1, _File);

	LoadResRef(m_AtlasTex, _File);
}

void CAnim2D::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	_objValue.AddMember("AnimName", Value(wStrToStr(GetName()).c_str(), allocator).Move(), allocator);
	
	size_t FrameCount = m_vecFrm.size();
	_objValue.AddMember("FrameCount", FrameCount, allocator);
	
	Value frameArray(kArrayType);
	int idx = 0;
	for (const auto& frame : m_vecFrm) {
		// tAnim2DFrm 구조체 데이터를 rapidjson의 객체 타입 생성
		Value frameObject(kObjectType);
		
		// 프레임 인덱스 (가독성용)
		frameObject.AddMember("Index", idx, allocator); 
		idx++;
	
		// frameObject에 tAnim2DFrm 구조체의 멤버를 rapidjson의 값(Value)으로 추가
		frameObject.AddMember("LeftTopUV", SaveVec2Json(frame.LeftTopUV, allocator), allocator);
		frameObject.AddMember("SliceUV", SaveVec2Json(frame.SliceUV, allocator), allocator);
		frameObject.AddMember("Offset", SaveVec2Json(frame.Offset, allocator), allocator);
		frameObject.AddMember("fDuration", frame.fDuration, allocator);
	
		// frameObject를 frameArray에 추가
		frameArray.PushBack(frameObject, allocator);
	}
	
	_objValue.AddMember("vecFrm", frameArray, allocator);

	_objValue.AddMember("vBackSize", SaveVec2Json(m_vBackSize, allocator), allocator);
	
	// m_AtlasTex
	string key = "AtlasTex";
	Value keyName(kStringType);
	keyName.SetString(key.c_str(), key.length(), allocator);
	_objValue.AddMember(keyName, SaveResRefJson(m_AtlasTex.Get(), allocator), allocator);
}

void CAnim2D::LoadFromLevelJsonFile(const Value& _componentValue)
{
	SetName(StrToWStr(_componentValue["AnimName"].GetString()));
	
	size_t FrameCount = _componentValue["FrameCount"].GetUint64();
	
	for (Value::ConstValueIterator itr = _componentValue["vecFrm"].Begin(); itr != _componentValue["vecFrm"].End(); ++itr)
	{
		const Value& frameObject = *itr;
	
		tAnim2DFrm frm = {};
		frm.LeftTopUV = LoadVec2Json(frameObject["LeftTopUV"]);
		frm.SliceUV = LoadVec2Json(frameObject["SliceUV"]);
		frm.Offset = LoadVec2Json(frameObject["Offset"]);
		frm.fDuration = frameObject["fDuration"].GetFloat();
		m_vecFrm.push_back(frm);
	}
	
	m_vBackSize = LoadVec2Json(_componentValue["vBackSize"]);
	
	LoadResRefJson(m_AtlasTex, _componentValue["AtlasTex"]);
void CAnim2D::Save()
{
	// open a file name
	OPENFILENAME ofn = {};

	// PathMgr에서 기본 경로를 가져온다
	wstring strFileFolderPath = CPathMgr::GetInst()->GetContentPath();
	strFileFolderPath += L"anim2d\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"anim2d\0*.anim2d\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strFileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	// 저장할 anim2d 이름에 .anim2d가 없다면 자동으로 확장자 붙여줌
	wstring filePath = wstring(szFilePath);
	int length = filePath.length();
	if (length < 7 || filePath.substr(length - 7) != L".anim2d") 
		filePath.append(L".anim2d");

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, filePath.c_str(), L"wb");

	//파일 포인터가 nullptr라면 파일 생성이 제대로 되지 않은 것으로 리턴
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Anim2D Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return;
	}

	// 경로값에서 파일 이름+확장자 추출
	size_t i = 0;
	wstring ContentFolderPath = CPathMgr::GetInst()->GetContentPath();
	while (i < wstring(szFilePath).length() && i < strFileFolderPath.length()
		&& wstring(szFilePath)[i] == strFileFolderPath[i])
	{
		++i;
	}
	wstring FileName = wstring(filePath).substr(i);
	
	// 확장자명 제거한 이름을 애니메이션 이름으로 지정
	size_t dotPos = FileName.find_last_of('.');
	wstring AnimName = FileName.substr(0, dotPos);

	//===== 01. 추출한 애니메이션 이름 저장=====
	SaveWString(AnimName, pFile);

	//===== 02. 애니메이션 경로 저장=====
	wstring RelativePath = L"anim2d\\" + FileName;
	SaveWString(RelativePath, pFile);
	SetRelativePath(RelativePath);

	//===== 03. 사용하는 아틀라스의 키값, 경로값 저장=====
	SaveResRef(m_AtlasTex.Get(), pFile);

	//===== 04. 애니메이션 프레임 개수와 프레임 구조체 저장=====
	size_t FrameCount = m_vecFrm.size();
	fwrite(&FrameCount, sizeof(size_t), 1, pFile);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), FrameCount, pFile);

	//===== 05. Dynamic Transform 옵션 사용 여부 저장 =====
	fwrite(&m_bDynamicTransform, sizeof(bool), 1, pFile);

	//===== 06. BackSize 저장 =====
	fwrite(&m_vBackSizeUV, sizeof(Vec2), 1, pFile);

	//===== 07. 저장한 파일명으로 애니메이션 이름 설정 =====
	SetName(AnimName);

	//===== 08. FPS 저장하기 =====
	fwrite(&m_iFPS, sizeof(int), 1, pFile);

	//파일 저장을 마치면 스트림을 닫아준다.
	fclose(pFile);
}

void CAnim2D::Load()
{
	// open a file name
	OPENFILENAME ofn = {};

	//PathManager에서 기본 경로를 가져온다
	wstring strFileFolderPath = CPathMgr::GetInst()->GetContentPath();
	strFileFolderPath += L"anim2d\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Anim2d\0*.anim2d\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strFileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	// Load 해오기 전 벡터에 남아있는 Frm데이터 Clear
	if (!m_vecFrm.empty())
		m_vecFrm.clear();

	// 파일입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"rb");

	//===== 01. 애니메이션 이름 불러오기=====
	wstring name;
	LoadWString(name, pFile);
	SetName(name);

	//===== 02. 애니메이션 경로 불러오기=====
	wstring path;
	LoadWString(path, pFile);
	SetRelativePath(path);

	//===== 03. 사용하는 아틀라스 키 경로 불러오기=====
	LoadResRef(m_AtlasTex, pFile);

	//===== 04. 애니메이션 프레임 개수 불러오기=====
	size_t FrameCount = 0;
	fread(&FrameCount, sizeof(size_t), 1, pFile);

	//===== 05. 프레임 개수만큼 프레임 구조체 불러오기=====
	for (size_t i = 0; i < FrameCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, pFile);
		m_vecFrm.push_back(frm);
	}

	//===== 06. Dynamic Transform 옵션 사용 여부 불러오기 =====
	fread(&m_bDynamicTransform, sizeof(bool), 1, pFile);

	//===== 07. BackSize 불러오기 =====
	fread(&m_vBackSizeUV, sizeof(Vec2), 1, pFile);

	//===== 08. FPS 불러오기 =====
	fread(&m_iFPS, sizeof(int), 1, pFile);

	Reset();

	//파일 로딩을 마치면 스트림을 닫아준다. 
	fclose(pFile);
}

void CAnim2D::Load(const wstring& _strFilePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strFilePath;

	// 파일입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	//===== 01. 애니메이션 이름 불러오기=====
	wstring name;
	LoadWString(name, pFile);
	SetName(name);

	//===== 02. 애니메이션 경로 불러오기=====
	wstring path;
	LoadWString(path, pFile);
	SetRelativePath(path);

	//===== 03. 사용하는 아틀라스 키 경로 불러오기=====
	LoadResRef(m_AtlasTex, pFile);

	//===== 04. 애니메이션 프레임 개수 불러오기=====
	size_t FrameCount = 0;
	fread(&FrameCount, sizeof(size_t), 1, pFile);

	//===== 05. 프레임 개수만큼 프레임 구조체 불러오기=====
	for (size_t i = 0; i < FrameCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, pFile);
		m_vecFrm.push_back(frm);
	}

	//===== 06. Dynamic Transform 옵션 사용 여부 불러오기 =====
	fread(&m_bDynamicTransform, sizeof(bool), 1, pFile);

	//===== 07. BackSize 불러오기 =====
	fread(&m_vBackSizeUV, sizeof(Vec2), 1, pFile);

	//===== 08. FPS 불러오기 =====
	fread(&m_iFPS, sizeof(int), 1, pFile);

	Reset();

	//파일 로딩을 마치면 스트림을 닫아준다. 
	fclose(pFile);
}
