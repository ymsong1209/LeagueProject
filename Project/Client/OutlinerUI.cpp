#include "pch.h"
#include "OutlinerUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CEventMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>

#include <Engine/CResMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CPrefab.h>
#include <Engine/CRenderMgr.h>

#include "ImGuiMgr.h"
#include "ContentUI.h"
#include "TreeUI.h"

OutlinerUI::OutlinerUI()
    : UI("##Outliner")
	, m_Tree(nullptr)
{
    SetName("Outliner");

	// OutlinerUI 안에 자식으로 Tree 를 추가한다.
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(false);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&OutlinerUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&OutlinerUI::DragDrop);
	m_Tree->AddDynamic_RightClick(this, (UI_DELEGATE_1)&OutlinerUI::RightClickFunction);

	m_Tree->SetDragDropID("GameObject");


	AddChildUI(m_Tree);
}

OutlinerUI::~OutlinerUI()
{
}

void OutlinerUI::tick()
{
	if (CEventMgr::GetInst()->IsLevelChanged())
	{
		ResetOutliner();

		// 트리 리셋 후, 다음 선택노드로 지정된 DATA 가 있으면, 찾아서 선택노드로 변경
		if (0 != m_dwSelectedData)
		{
			m_Tree->GetSelectedNode(m_dwSelectedData);
		}		
	}
}

int OutlinerUI::render_update()
{
    return 0;
}


void OutlinerUI::ResetOutliner()
{
	// Tree Clear
	m_Tree->Clear();
	m_Tree->AddItem("Root", 0);

	// 리소스 매니저에서 현재 모든 리소스 목록 받아옴
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < (UINT)MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);

		const vector<CGameObject*>& vecParentObj = pLayer->GetParentObject();

		for (size_t i = 0; i < vecParentObj.size(); ++i)
		{
			AddGameObject(vecParentObj[i], nullptr);			
		}
	}
}

void OutlinerUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
	CGameObject* pSelectObject = (CGameObject*)pSelectedNode->GetData();

	// Inspector 에 선택된 GameObject 를 알려준다.	
	InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pSelectObject);
}


void OutlinerUI::RightClickFunction(DWORD_PTR _RightClickedNode)
{
	TreeNode* pNode = (TreeNode*)_RightClickedNode;
	CGameObject* pSelectObject = (CGameObject*)pNode->GetData();

	ImGui::Text("Edit name:");
	string EditedName = pNode->GetName();
	string EditedNameID = "##RightClickFuncOutliner" + EditedName;

	char input_buffer[256]; // allocate a buffer for the input
	strcpy_s(input_buffer, EditedName.c_str());

	if (ImGui::InputText(EditedNameID.c_str(), input_buffer, 256)) {

		if (KEY_TAP(KEY::ENTER)) {
			EditedName = input_buffer;
			wstring name;
			name.assign(EditedName.begin(), EditedName.end());// = wstring.assign(EditedName.begin(), EditedName.end());
			pSelectObject->SetName(name);
			ResetOutliner();
		}
		return;
	}
	if (ImGui::Button("SaveAsPrefab##Outliner")) {
		//ContentUI에 중복된 Prefab있는지 확인
		// 리소스 매니저에서 현재 모든 리소스 목록 받아옴
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources(RES_TYPE::PREFAB);
		wstring objectName = pSelectObject->GetName();
		objectName = L"prefab\\" + objectName;
		if (mapRes.find(objectName) != mapRes.end()) {
			return;
		}

		Ptr<CPrefab> NewPrefab = new CPrefab;
		CGameObject* PrefabObject = pSelectObject->Clone(); // Clone을 해서 LayerIdx를 -1로 만듬
		NewPrefab->RegisterProtoObject(PrefabObject);

		CResMgr::GetInst()->AddRes<CPrefab>(objectName, NewPrefab);
		ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
		content->ResetContent();

		return;
	}

	if (ImGui::Button("Clone##RightClickFuncOutliner")) {
		CGameObject* newObject = pSelectObject->Clone();
		wstring name = newObject->GetName();
		wstring id = std::to_wstring(newObject->GetID());
		name += id;
		newObject->SetName(name);
		SpawnGameObject(newObject, pSelectObject->Transform()->GetRelativePos(), pSelectObject->GetLayerIndex());
		//m_Tree->SetRightClickActivate(false);
	}

	if (ImGui::Button("Delete##RightClickFuncOutliner")) {

		//InspectorUI가 이 Object를 들고 있으면 reset시킨다.	
		InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
		if (pInspector->GetTargetObject() == pSelectObject) {
			pInspector->SetTargetObject(nullptr);
		}

		DestroyObject(pSelectObject);
	}
}

void OutlinerUI::AddGameObject(CGameObject* _Obj, TreeNode* _ParentNode)
{
	// 오브젝트를 트리에 넣고, 생성된 노드 주소를 받아둔다.
	TreeNode* pNode = m_Tree->AddItem(string(_Obj->GetName().begin(), _Obj->GetName().end())
									, (DWORD_PTR)_Obj
									, _ParentNode);

	// 오브젝트의 자식오브젝트 들을 오브젝트 노드를 부모로 해서 그 밑으로 다시 넣어준다.
	const vector<CGameObject*>& vecChild = _Obj->GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(vecChild[i], pNode);
	}
}


CGameObject* OutlinerUI::GetSelectedObject()
{
	TreeNode* pSelectedNode = m_Tree->GetSelectedNode();

	if (nullptr == pSelectedNode)
		return nullptr;

	return (CGameObject*)pSelectedNode->GetData();
}






void OutlinerUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
	TreeNode* pDragNode = (TreeNode*)_DragNode;
	TreeNode* pDropNode = (TreeNode*)_DropNode;

	CGameObject* pDragObj = (CGameObject*)pDragNode->GetData();
	CGameObject* pDropObj = nullptr;
	if (nullptr != pDropNode)
	{
		pDropObj = (CGameObject*)pDropNode->GetData();
	}

	// 자식으로 들어갈 오브젝트가 목적지 오브젝트의 조상(부모계층) 중 하나라면, 
	// AddChild 처리하지 않는다.
	if (nullptr != pDropObj)
	{
		if (pDropObj->IsAncestor(pDragObj))
			return;
	}

	// 이벤트 매니저를 통해서 처리한다.
	tEvent evn = {};
	evn.Type = EVENT_TYPE::ADD_CHILD;
	evn.wParam = (DWORD_PTR)pDropObj;
	evn.lParam = (DWORD_PTR)pDragObj;
	CEventMgr::GetInst()->AddEvent(evn);
}
