#pragma once
#include "UI.h"

class TreeUI;
class TreeNode;

class OutlinerUI :
    public UI
{
private:
    TreeUI*     m_Tree;
    DWORD_PTR   m_dwSelectedData;


public:
    virtual void tick() override;
    virtual int render_update() override;

public:
    void ResetOutliner();
    void SetTargetToInspector(DWORD_PTR _SelectedNode);
    CGameObject* GetSelectedObject();
    DWORD_PTR GetSelectedData() { return m_dwSelectedData; }

    // 트리가 리셋된 경우, 입력으로 들어온 데이터와 동일한 노드를  선택상태로 둔다.
    void SetSelectedNodeData(DWORD_PTR _data)
    {
        m_dwSelectedData = _data;
    }
     
    void RightClickFunction(DWORD_PTR _RightClickedNode);

    TreeUI* GetTreeOutliner() { return m_Tree; }

private:
    void AddGameObject(CGameObject* _Obj, TreeNode* _ParentNode);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);



public:
    OutlinerUI();
    ~OutlinerUI();
};

