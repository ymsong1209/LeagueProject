#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine\CMeshRender.h>
#include <Engine\CResMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"

#include "TreeUI.h"

#include <algorithm>


MeshRenderUI::MeshRenderUI()
    : ComponentUI("##MeshRender", COMPONENT_TYPE::MESHRENDER)
{
    SetName("MeshRender");
}

MeshRenderUI::~MeshRenderUI()
{
}


int MeshRenderUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    char szBuff[50] = {};

    Ptr<CMesh> pMesh = GetTarget()->MeshRender()->GetMesh();

    ImGui::Text("Mesh    ");
    ImGui::SameLine();
    GetResKey(pMesh.Get(), szBuff, 50);
    ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

    // Mesh 드랍 체크
    if (ImGui::BeginDragDropTarget())
    {
        // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
        const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
        if (pPayLoad)
        {
            TreeNode* pNode = (TreeNode*)pPayLoad->Data;
            CRes* pRes = (CRes*)pNode->GetData();
            if (RES_TYPE::MESH == pRes->GetType())
            {
                GetTarget()->MeshRender()->SetMesh((CMesh*)pRes);
            }
        }
        ImGui::EndDragDropTarget();
    }

    // Mesh가 선택 안되어 있을 경우 Mesh만 선택할 수 있도록 해주고 Skip
    // Mesh가 선택 되어 있으면 최소한 Material(0)번은 체워지게 된다.
    if (pMesh == nullptr)
        return true;

    Ptr<CMaterial> pMtrl = GetTarget()->MeshRender()->GetMaterial(0);


    ImGui::SameLine();
    if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
    {
        const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::MESH);

        ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
        pListUI->Reset("Mesh List", ImVec2(300.f, 500.f));
        for (const auto& pair : mapMesh)
        {
            pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
        }

        // 항목 선택시 호출받을 델리게이트 등록
        pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMesh);
    }

    ImGui::Text("Material");
    ImGui::SameLine();
    GetResKey(pMtrl.Get(), szBuff, 50);
    ImGui::InputText("##MtrlName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

    if (ImGui::BeginDragDropTarget())
    {
        // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
        const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
        if (pPayLoad)
        {
            TreeNode* pNode = (TreeNode*)pPayLoad->Data;
            CRes* pRes = (CRes*)pNode->GetData();
            if (RES_TYPE::MATERIAL == pRes->GetType())
            {
                GetTarget()->MeshRender()->SetMaterial(((CMaterial*)pRes), 0);

                // Mtrl을 새로 선택한경우 Dynamic Mtrl을 지워준다.
                GetTarget()->MeshRender()->ClearDynamicMtrl(0);
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::SameLine();

    if (pMtrl != nullptr && pMtrl->GetShader() != nullptr)
    {
        int DSType = (int)GetTarget()->MeshRender()->GetMaterial(0)->GetShader()->GetDSType();
        int BSType = (int)GetTarget()->MeshRender()->GetMaterial(0)->GetShader()->GetBSType();
        int DomainType = (int)GetTarget()->MeshRender()->GetMaterial(0)->GetShader()->GetDomain();
        Ptr<CGraphicsShader> Shader = GetTarget()->MeshRender()->GetMaterial(0)->GetShader();

        ImGui::Text("Set BSType : ");
        if (ImGui::RadioButton("DEFAULT", &BSType, 0))
            Shader->SetBSType(BS_TYPE::DEFAULT);
        ImGui::SameLine();
        if (ImGui::RadioButton("MASK", &BSType, 1))
            Shader->SetBSType(BS_TYPE::MASK);
        ImGui::SameLine();
        if (ImGui::RadioButton("ALPHA_BLEND", &BSType, 2))
            Shader->SetBSType(BS_TYPE::ALPHA_BLEND);
        ImGui::SameLine();
        if (ImGui::RadioButton("ONE_ONE", &BSType, 3))
            Shader->SetBSType(BS_TYPE::ONE_ONE);

        ImGui::Separator();

        ImGui::Text("Set DSType : ");
        if (ImGui::RadioButton("LESS", &DSType, 0))
            Shader->SetDSType(DS_TYPE::LESS);
        ImGui::SameLine();
        if (ImGui::RadioButton("LESS_EQUAL", &DSType, 1))
            Shader->SetDSType(DS_TYPE::LESS_EQUAL);
        ImGui::SameLine();
        if (ImGui::RadioButton("NO_WRITE", &DSType, 4))
            Shader->SetDSType(DS_TYPE::NO_WRITE);
        ImGui::SameLine();
        if (ImGui::RadioButton("NO_TEST_NO_WRITE", &DSType, 5))
            Shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        ImGui::Separator();

        ImGui::Text("Set DomainType : ");
        if (ImGui::RadioButton("DOMAIN_OPAQUE", &DomainType, 3))
            Shader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
        ImGui::SameLine();
        if (ImGui::RadioButton("DOMAIN_MASK", &DomainType, 4))
            Shader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
        ImGui::SameLine();
        if (ImGui::RadioButton("DOMAIN_TRANSPARENT", &DomainType, 5))
            Shader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
        ImGui::SameLine();
        if (ImGui::RadioButton("DOMAIN_UI", &DomainType, 7))
            Shader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);
    }
   

    if (ImGui::Button("##MtrlSelectBtn", ImVec2(18, 18)))
    {
        const map<wstring, Ptr<CRes>>& mapMtrl = CResMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

        ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
        pListUI->Reset("Material", ImVec2(300.f, 500.f));
        for (const auto& pair : mapMtrl)
        {
            pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
        }

        // 항목 선택시 호출받을 델리게이트 등록
        pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMaterial);
    }

    if (ImGui::TreeNode("Output   Texture"))
    {
        GetResKey(pMtrl->GetTexParam(TEX_PARAM(0)).Get(), szBuff, 50);
        string TexName = "##OutputTex";
        ImGui::InputText(TexName.c_str(), szBuff, 50, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            //해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
            const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
            if (pPayLoad)
            {
                TreeNode* pNode = (TreeNode*)pPayLoad->Data;
                CRes* pRes = (CRes*)pNode->GetData();
                if (RES_TYPE::TEXTURE == pRes->GetType())
                    GetTarget()->MeshRender()->SetOutputTexture((CTexture*)pRes);
            }
            ImGui::EndDragDropTarget();
        }

        // Texture를 비울 수 있도록 해줌
        ImGui::SameLine();
        string TexClearButton = "Clear##OutputTex";
        if (ImGui::Button(TexClearButton.c_str(), ImVec2(50.f, 20.f)))
            GetTarget()->MeshRender()->SetOutputTexture((CTexture*)nullptr);

        int Target = (int)GetTarget()->MeshRender()->GetTexMovingStyle(MovTexType::OUTPUT);
        Vec4 FuncValue = GetTarget()->MeshRender()->GetFuncValue(MovTexType::OUTPUT);
        Vec2 Offset = GetTarget()->MeshRender()->GetOffsetValue(MovTexType::OUTPUT);

        SetTexMovingTypeAndValue(Target, FuncValue, Offset, 10);

        GetTarget()->MeshRender()->SetTexMovingStyle(MovTexType::OUTPUT, (MovTexMoveType)(Target));
        GetTarget()->MeshRender()->SetFuncValue(MovTexType::OUTPUT, FuncValue);
        GetTarget()->MeshRender()->SetOffsetValue(MovTexType::OUTPUT, Offset);
        ImGui::TreePop();
    }


    if (ImGui::TreeNode("Puncture Texture"))
    {
        GetResKey(GetTarget()->MeshRender()->GetPunctureTex().Get(), szBuff, 50);
        string TexName = "##PunctureTex";
        ImGui::InputText(TexName.c_str(), szBuff, 50, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            //해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
            const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
            if (pPayLoad)
            {
                TreeNode* pNode = (TreeNode*)pPayLoad->Data;
                CRes* pRes = (CRes*)pNode->GetData();
                if (RES_TYPE::TEXTURE == pRes->GetType())
                    GetTarget()->MeshRender()->SetPunctureTexture((CTexture*)pRes);
            }
            ImGui::EndDragDropTarget();
        }

        // Texture를 비울 수 있도록 해줌
        ImGui::SameLine();
        string TexClearButton = "Clear##OutputTex";
        if (ImGui::Button(TexClearButton.c_str(), ImVec2(50.f, 20.f)))
            GetTarget()->MeshRender()->SetPunctureTexture((CTexture*)nullptr);


        int Target = (int)GetTarget()->MeshRender()->GetTexMovingStyle(MovTexType::PUNCTURE);
        Vec4 FuncValue = GetTarget()->MeshRender()->GetFuncValue(MovTexType::PUNCTURE);
        Vec2 Offset = GetTarget()->MeshRender()->GetOffsetValue(MovTexType::PUNCTURE);

        SetTexMovingTypeAndValue(Target, FuncValue, Offset, 20);

        GetTarget()->MeshRender()->SetTexMovingStyle(MovTexType::PUNCTURE, (MovTexMoveType)(Target));
        GetTarget()->MeshRender()->SetFuncValue(MovTexType::PUNCTURE, FuncValue);
        GetTarget()->MeshRender()->SetOffsetValue(MovTexType::PUNCTURE, Offset);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Additive Texture"))
    {
        GetResKey(GetTarget()->MeshRender()->GetAdditiveTex().Get(), szBuff, 50);
        ImGui::Text("Additive Texture"); ImGui::SameLine();
        ImGui::InputText("##AdditiveTex", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget())
        {
            // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
            const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
            if (pPayLoad)
            {
                TreeNode* pNode = (TreeNode*)pPayLoad->Data;
                CRes* pRes = (CRes*)pNode->GetData();
                if (RES_TYPE::TEXTURE == pRes->GetType())
                    GetTarget()->MeshRender()->SetAdditiveTexture((CTexture*)pRes);
            }
            ImGui::EndDragDropTarget();
        }


        // Texture를 비울 수 있도록 해줌 (Additive Texture)
        ImGui::SameLine();
        string TexClearButton = "Clear##" + std::to_string(3);
        if (ImGui::Button(TexClearButton.c_str(), ImVec2(50.f, 20.f)))
            GetTarget()->MeshRender()->SetAdditiveTexture(nullptr);


        Vec4 AdditiveColor = GetTarget()->MeshRender()->GetAdditiveTexColor();
        float AdditiveVec[4] = { AdditiveColor.x * 255, AdditiveColor.y * 255 , AdditiveColor.z * 255, 0.f };

        ImGui::Text("Additive Color"); ImGui::SameLine();
        ImGui::InputFloat4("##AdditiveColor", AdditiveVec);

        AdditiveColor = Vec4{ AdditiveVec[0] / 255, AdditiveVec[1] / 255, AdditiveVec[2] / 255, 0.f };

        GetTarget()->MeshRender()->SetAdditiveTexColor(AdditiveColor);

        ImGui::TreePop();
    }

    return TRUE;
}

void MeshRenderUI::SelectMesh(DWORD_PTR _Key)
{
    string strKey = (char*)_Key;
    Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(wstring(strKey.begin(), strKey.end()));
    GetTarget()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::SelectMaterial(DWORD_PTR _Key)
{
    string strKey = (char*)_Key;
    Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstring(strKey.begin(), strKey.end()));

    GetTarget()->MeshRender()->SetMaterial(pMtrl, 0);
    GetTarget()->MeshRender()->ClearDynamicMtrl(0);
}

void MeshRenderUI::SetTexMovingTypeAndValue(int& _Target, Vec4& _FuncValue, Vec2& _Offest, int _RandNum)
{
    // 해당 Texture의 움직이는 함수를 알려줌
    ImGui::Text("Moving Style"); ImGui::SameLine();

    string MoveOffsetClearBtn = "Clear Move##" + std::to_string(_RandNum);
    if (ImGui::Button(MoveOffsetClearBtn.c_str(), ImVec2(100.f, 20.f)))
        _Offest = Vec2(0.f, 0.f);


    const char* items[] = { "None", "Horizonetal", "Vertical", "Linear", "Parabola", "Sin", "Cos" };
    //int item_current_idx = (int)MovingVec[i].MovingStyle;; // Here we store our selection data as an index.
    string MovingFuncName = "##MovingFuncName" + std::to_string(_RandNum);
    const char* combo_preview_value = items[_Target];  // Pass in the preview value visible before opening the combo (it could be anything)
    if (ImGui::BeginCombo(MovingFuncName.c_str(), combo_preview_value))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (_Target == n);
            if (ImGui::Selectable(items[n], is_selected))
                _Target = n;
        }
        ImGui::EndCombo();
    }

    switch (MovTexMoveType(_Target))
    {
    case MovTexMoveType::NONE:
        break;
    case MovTexMoveType::HORIZONTAL: {
        ImGui::Text("FuncValue.x : dx / dt"); }
                                   break;
    case MovTexMoveType::VERTICAL: {
        ImGui::Text("FuncValue.x : dy / dt"); }
                                 break;
    case MovTexMoveType::LINEAR: {
        ImGui::Text("FuncValue.x : dx / dt"); ImGui::SameLine(); ImGui::Text("FuncValue.y : x  coef");
        ImGui::Text("FuncValue.z : y-inter"); }
                               break;
    case MovTexMoveType::PARABOLA: {
        ImGui::Text("FuncValue.x : dx / dt"); ImGui::SameLine(); ImGui::Text("FuncValue.y : x^2coef");
        ImGui::Text("FuncValue.z : x  coef"); ImGui::SameLine(); ImGui::Text("FuncValue.w : y-inter"); }
                                 break;
    case MovTexMoveType::SIN: {
        ImGui::Text("FuncValue.x : dx / dt"); ImGui::SameLine(); ImGui::Text("FuncValue.y : Coef   ");
        ImGui::Text("FuncValue.z : Freq   "); ImGui::SameLine(); ImGui::Text("FuncValue.w : y-inter"); }
                            break;
    case MovTexMoveType::COS: {
        ImGui::Text("FuncValue.x : dx / dt"); ImGui::SameLine(); ImGui::Text("FuncValue.x : Coef   ");
        ImGui::Text("FuncValue.x : Freq   "); ImGui::SameLine(); ImGui::Text("FuncValue.x : y-inter"); }
                            break;
    case MovTexMoveType::END:
        break;
    default:
        break;
    }

    ImGui::Text("Func Value"); ImGui::SameLine();
    string FuncValueName = "##FuncValue" + std::to_string(_RandNum);
    ImGui::InputFloat4(FuncValueName.c_str(), _FuncValue);

    //ImGui::Separator();

}