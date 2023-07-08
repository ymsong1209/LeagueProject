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

	// �������� Duration���� �����ð��� Ŀ����
	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		// �����ð��� Duration��ŭ ���־� �ʱ�ȭ�ϰ� �������� �ѱ��.
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;

		// ������ �������� �Ѿ�� ������ ���������� �����ϰ� Finish�ɼ��� �Ҵ�
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
	// ��Ʋ�� �ؽ�ó ������ Assert
	assert(_AtlasTex.Get());

	// �ִϸ��̼� �̸� ����
	SetName(_strAnimName);

	// ��Ʋ�� �̹���, �ػ� ����
	m_AtlasTex = _AtlasTex;
	m_vAtlasResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	// FPS ����
	m_iFPS = _FPS;

	// BackSize ����
	m_vBackSizeUV = _vBackSizeUV;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		// ��Ʋ�� �ؽ�ó�� �ִϸ��̼� �������� �Ϸķ� ���ĵǾ����� �� ����
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / m_vAtlasResolution;
		frm.SliceUV = _vSlice / m_vAtlasResolution;

		// DynamicTransform �ʱⰪ
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
		// tAnim2DFrm ����ü �����͸� rapidjson�� ��ü Ÿ�� ����
		Value frameObject(kObjectType);
		
		// ������ �ε��� (��������)
		frameObject.AddMember("Index", idx, allocator); 
		idx++;
	
		// frameObject�� tAnim2DFrm ����ü�� ����� rapidjson�� ��(Value)���� �߰�
		frameObject.AddMember("LeftTopUV", SaveVec2Json(frame.LeftTopUV, allocator), allocator);
		frameObject.AddMember("SliceUV", SaveVec2Json(frame.SliceUV, allocator), allocator);
		frameObject.AddMember("Offset", SaveVec2Json(frame.Offset, allocator), allocator);
		frameObject.AddMember("fDuration", frame.fDuration, allocator);
	
		// frameObject�� frameArray�� �߰�
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

	// PathMgr���� �⺻ ��θ� �����´�
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

	// ������ anim2d �̸��� .anim2d�� ���ٸ� �ڵ����� Ȯ���� �ٿ���
	wstring filePath = wstring(szFilePath);
	int length = filePath.length();
	if (length < 7 || filePath.substr(length - 7) != L".anim2d") 
		filePath.append(L".anim2d");

	// ���� �����
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, filePath.c_str(), L"wb");

	//���� �����Ͱ� nullptr��� ���� ������ ����� ���� ���� ������ ����
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Anim2D Save ����, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"���� ���� ����", MB_OK);
		return;
	}

	// ��ΰ����� ���� �̸�+Ȯ���� ����
	size_t i = 0;
	wstring ContentFolderPath = CPathMgr::GetInst()->GetContentPath();
	while (i < wstring(szFilePath).length() && i < strFileFolderPath.length()
		&& wstring(szFilePath)[i] == strFileFolderPath[i])
	{
		++i;
	}
	wstring FileName = wstring(filePath).substr(i);
	
	// Ȯ���ڸ� ������ �̸��� �ִϸ��̼� �̸����� ����
	size_t dotPos = FileName.find_last_of('.');
	wstring AnimName = FileName.substr(0, dotPos);

	//===== 01. ������ �ִϸ��̼� �̸� ����=====
	SaveWString(AnimName, pFile);

	//===== 02. �ִϸ��̼� ��� ����=====
	wstring RelativePath = L"anim2d\\" + FileName;
	SaveWString(RelativePath, pFile);
	SetRelativePath(RelativePath);

	//===== 03. ����ϴ� ��Ʋ���� Ű��, ��ΰ� ����=====
	SaveResRef(m_AtlasTex.Get(), pFile);

	//===== 04. �ִϸ��̼� ������ ������ ������ ����ü ����=====
	size_t FrameCount = m_vecFrm.size();
	fwrite(&FrameCount, sizeof(size_t), 1, pFile);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), FrameCount, pFile);

	//===== 05. Dynamic Transform �ɼ� ��� ���� ���� =====
	fwrite(&m_bDynamicTransform, sizeof(bool), 1, pFile);

	//===== 06. BackSize ���� =====
	fwrite(&m_vBackSizeUV, sizeof(Vec2), 1, pFile);

	//===== 07. ������ ���ϸ����� �ִϸ��̼� �̸� ���� =====
	SetName(AnimName);

	//===== 08. FPS �����ϱ� =====
	fwrite(&m_iFPS, sizeof(int), 1, pFile);

	//���� ������ ��ġ�� ��Ʈ���� �ݾ��ش�.
	fclose(pFile);
}

void CAnim2D::Load()
{
	// open a file name
	OPENFILENAME ofn = {};

	//PathManager���� �⺻ ��θ� �����´�
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

	// Load �ؿ��� �� ���Ϳ� �����ִ� Frm������ Clear
	if (!m_vecFrm.empty())
		m_vecFrm.clear();

	// ���������
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"rb");

	//===== 01. �ִϸ��̼� �̸� �ҷ�����=====
	wstring name;
	LoadWString(name, pFile);
	SetName(name);

	//===== 02. �ִϸ��̼� ��� �ҷ�����=====
	wstring path;
	LoadWString(path, pFile);
	SetRelativePath(path);

	//===== 03. ����ϴ� ��Ʋ�� Ű ��� �ҷ�����=====
	LoadResRef(m_AtlasTex, pFile);

	//===== 04. �ִϸ��̼� ������ ���� �ҷ�����=====
	size_t FrameCount = 0;
	fread(&FrameCount, sizeof(size_t), 1, pFile);

	//===== 05. ������ ������ŭ ������ ����ü �ҷ�����=====
	for (size_t i = 0; i < FrameCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, pFile);
		m_vecFrm.push_back(frm);
	}

	//===== 06. Dynamic Transform �ɼ� ��� ���� �ҷ����� =====
	fread(&m_bDynamicTransform, sizeof(bool), 1, pFile);

	//===== 07. BackSize �ҷ����� =====
	fread(&m_vBackSizeUV, sizeof(Vec2), 1, pFile);

	//===== 08. FPS �ҷ����� =====
	fread(&m_iFPS, sizeof(int), 1, pFile);

	Reset();

	//���� �ε��� ��ġ�� ��Ʈ���� �ݾ��ش�. 
	fclose(pFile);
}

void CAnim2D::Load(const wstring& _strFilePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strFilePath;

	// ���������
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	//===== 01. �ִϸ��̼� �̸� �ҷ�����=====
	wstring name;
	LoadWString(name, pFile);
	SetName(name);

	//===== 02. �ִϸ��̼� ��� �ҷ�����=====
	wstring path;
	LoadWString(path, pFile);
	SetRelativePath(path);

	//===== 03. ����ϴ� ��Ʋ�� Ű ��� �ҷ�����=====
	LoadResRef(m_AtlasTex, pFile);

	//===== 04. �ִϸ��̼� ������ ���� �ҷ�����=====
	size_t FrameCount = 0;
	fread(&FrameCount, sizeof(size_t), 1, pFile);

	//===== 05. ������ ������ŭ ������ ����ü �ҷ�����=====
	for (size_t i = 0; i < FrameCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, pFile);
		m_vecFrm.push_back(frm);
	}

	//===== 06. Dynamic Transform �ɼ� ��� ���� �ҷ����� =====
	fread(&m_bDynamicTransform, sizeof(bool), 1, pFile);

	//===== 07. BackSize �ҷ����� =====
	fread(&m_vBackSizeUV, sizeof(Vec2), 1, pFile);

	//===== 08. FPS �ҷ����� =====
	fread(&m_iFPS, sizeof(int), 1, pFile);

	Reset();

	//���� �ε��� ��ġ�� ��Ʈ���� �ݾ��ش�. 
	fclose(pFile);
}
