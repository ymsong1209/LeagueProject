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

	// OutlinerUI �ȿ� �ڽ����� Tree �� �߰��Ѵ�.
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

		// Ʈ�� ���� ��, ���� ���ó��� ������ DATA �� ������, ã�Ƽ� ���ó��� ����
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

	// ���ҽ� �Ŵ������� ���� ��� ���ҽ� ��� �޾ƿ�
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

	// Inspector �� ���õ� GameObject �� �˷��ش�.	
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
		//ContentUI�� �ߺ��� Prefab�ִ��� Ȯ��
		// ���ҽ� �Ŵ������� ���� ��� ���ҽ� ��� �޾ƿ�
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources(RES_TYPE::PREFAB);
		wstring objectName = pSelectObject->GetName();
		objectName = L"prefab\\" + objectName;
		if (mapRes.find(objectName) != mapRes.end()) {
			return;
		}

		Ptr<CPrefab> NewPrefab = new CPrefab;
		CGameObject* PrefabObject = pSelectObject->Clone(); // Clone�� �ؼ� LayerIdx�� -1�� ����
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

		//InspectorUI�� �� Object�� ��� ������ reset��Ų��.	
		InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
		if (pInspector->GetTargetObject() == pSelectObject) {
			pInspector->SetTargetObject(nullptr);
		}

		DestroyObject(pSelectObject);
	}
}

void OutlinerUI::AddGameObject(CGameObject* _Obj, TreeNode* _ParentNode)
{
	// ������Ʈ�� Ʈ���� �ְ�, ������ ��� �ּҸ� �޾Ƶд�.
	TreeNode* pNode = m_Tree->AddItem(string(_Obj->GetName().begin(), _Obj->GetName().end())
									, (DWORD_PTR)_Obj
									, _ParentNode);

	// ������Ʈ�� �ڽĿ�����Ʈ ���� ������Ʈ ��带 �θ�� �ؼ� �� ������ �ٽ� �־��ش�.
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

	// �ڽ����� �� ������Ʈ�� ������ ������Ʈ�� ����(�θ����) �� �ϳ����, 
	// AddChild ó������ �ʴ´�.
	if (nullptr != pDropObj)
	{
		if (pDropObj->IsAncestor(pDragObj))
			return;
	}

	// �̺�Ʈ �Ŵ����� ���ؼ� ó���Ѵ�.
	tEvent evn = {};
	evn.Type = EVENT_TYPE::ADD_CHILD;
	evn.wParam = (DWORD_PTR)pDropObj;
	evn.lParam = (DWORD_PTR)pDragObj;
	CEventMgr::GetInst()->AddEvent(evn);
}
