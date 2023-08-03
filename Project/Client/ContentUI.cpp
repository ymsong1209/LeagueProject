#include "pch.h"
#include "ContentUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CPathMgr.h>
#include <Engine\CEventMgr.h>

#include "TreeUI.h"
#include "ImGuiMgr.h"
#include "InspectorUI.h"
#include "LevelUI.h"
#include "CLevelSaveLoad.h"

#include <filesystem>
ContentUI::ContentUI()
    : UI("##Content")
{
    SetName("Content");

    // ContentUI 안에 자식으로 Tree 를 추가한다.
    m_Tree = new TreeUI;
    m_Tree->SetName("ContentTree");
    m_Tree->SetActive(true);
    m_Tree->ShowRoot(false);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&ContentUI::SetTargetToInspector);
	m_Tree->SetDragDropID("Resource");
    AddChildUI(m_Tree);   
}

ContentUI::~ContentUI()
{

}

void ContentUI::init()
{
	Reload();
}

void ContentUI::tick()
{
    UI::tick();

	if (CResMgr::GetInst()->IsResourceChanged())
	{
		ResetContent();
	}
}

int ContentUI::render_update()
{   
    return 0;
}

void ContentUI::Reload()
{
	// Content 폴더에 있는 파일 이름들을 확인
	m_vecResPath.clear();
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	FindFileName(strContentPath);


	// 레벨 로딩
	LevelUI* levelui = (LevelUI*)ImGuiMgr::GetInst()->FindUI("##Level");
	levelui->m_vecLevelName.clear();


	// 파일명으로 리소스 로딩
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		RES_TYPE type = GetResTypeByExt(m_vecResPath[i]);

		if (type == RES_TYPE::END)
		//로딩 된 것들 중 레벨은 레벨UI로 옮긴다.
		{
			wchar_t szExt[50] = {};
			_wsplitpath_s(m_vecResPath[i].c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);
			wstring strExt = szExt;
			if (L".json" == strExt) {
				levelui->m_vecLevelName.push_back(m_vecResPath[i]);
			}
			else {
				continue;
			}
		}

		switch (type)
		{
		case RES_TYPE::MESHDATA:
			CResMgr::GetInst()->Load<CMeshData>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::PREFAB:
		{
			FILE* pFile = nullptr;
			wstring path = strContentPath + m_vecResPath[i].c_str();

			_wfopen_s(&pFile, path.c_str(), L"rb");

			Ptr<CPrefab> pRes = CResMgr::GetInst()->FindRes<CPrefab>(m_vecResPath[i]).Get();

			// 이미 해당 키로 리소스가 있다면, break;
			if (nullptr != pRes) break;

			pRes = new CPrefab;
			//상대경로 지정
			pRes.Get()->Load(m_vecResPath[i]);
			CGameObject* prefabobject = CLevelSaveLoad::LoadGameObject(pFile);
			pRes.Get()->RegisterProtoObject(prefabobject);
			CResMgr::GetInst()->AddRes(m_vecResPath[i], pRes);
			fclose(pFile);
		}
			break;
		case RES_TYPE::MESH:

			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->LoadTexture(m_vecResPath[i], m_vecResPath[i],1);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(m_vecResPath[i], m_vecResPath[i]);
			break;		
		}
	}


	// 리소스의 원본파일 체크
	for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes())
				continue;

			wstring strFilePath = strContentPath + pair.first;
			if (!std::filesystem::exists(strFilePath))
			{
				tEvent evn = {};
				evn.Type = EVENT_TYPE::DELETE_RESOURCE;
				evn.wParam = (DWORD_PTR)i;
				evn.lParam = (DWORD_PTR)pair.second.Get();
				CEventMgr::GetInst()->AddEvent(evn);
			}
		}
	}

	// 트리 갱신
	ResetContent();
}


void ContentUI::ResetContent()
{
	// Tree Clear
	m_Tree->Clear();
	m_Tree->AddItem("Root", 0);

	// 리소스 매니저에서 현재 모든 리소스 목록 받아옴
	for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		// m_Tree 에 현재 리소스 목록을 AddItem
		TreeNode* pCategory = m_Tree->AddItem(ToString((RES_TYPE)i), 0);
        pCategory->SetCategoryNode(true);

		for (const auto& pair : mapRes)
		{
			m_Tree->AddItem(string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), pCategory);
		}
	}
}

void ContentUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
	CRes* pSelectObject = (CRes*)pSelectedNode->GetData();

	if (nullptr == pSelectObject)
		return;

	// Inspector 에 선택된 Resource 를 알려준다.	
	InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetResource(pSelectObject);
}


void ContentUI::FindFileName(const wstring& _FolderPath)
{
	WIN32_FIND_DATA FindData = {};

	wstring FolderPath = _FolderPath + L"*.*";

	HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

	while (FindNextFile(hFindHandle, &FindData))
	{
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!wcscmp(FindData.cFileName, L".."))
			{
				continue;
			}

			FindFileName(_FolderPath + FindData.cFileName + L"\\");
			continue;
		}

		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
		wstring RelativePath = _FolderPath + FindData.cFileName;
		RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

		m_vecResPath.push_back(RelativePath);
	}

	FindClose(hFindHandle);
}

RES_TYPE ContentUI::GetResTypeByExt(const wstring& _relativepath)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_relativepath.c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);	
	wstring strExt = szExt;
		
	if (L".mdat" == strExt)
		return RES_TYPE::MESHDATA;
	else if (L".prefab" == strExt)
		return RES_TYPE::PREFAB;
	else if (L".mesh" == strExt)
		return RES_TYPE::MESH;
	else if (L".mtrl" == strExt)
		return RES_TYPE::MATERIAL;
	else if (L".png" == strExt || L".jpg" == strExt
		|| L".jpeg" == strExt || L".bmp" == strExt
		|| L".tga" == strExt || L".dds" == strExt)
		return RES_TYPE::TEXTURE;
	else if (L".mp3" == strExt || L".wav" == strExt || L".oga" == strExt)
		return RES_TYPE::SOUND;
	else
		return RES_TYPE::END;
}

