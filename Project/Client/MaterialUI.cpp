#include "pch.h"
#include "MaterialUI.h"

#include "ParamUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CMaterial.h>

MaterialUI::MaterialUI()
    : ResUI(RES_TYPE::MATERIAL)
{
    SetName("Material");
}

MaterialUI::~MaterialUI()
{
}

int MaterialUI::render_update()
{
    ResUI::render_update();

    // Material �̸�
    ImGui::Text("Material"); 
    ImGui::SameLine();

    Ptr<CMaterial> pMtrl = (CMaterial*)GetTargetRes().Get();
    string strKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
    ImGui::InputText("##MtrlUIName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    // GraphicsShader �̸�
    ImGui::Text("Shader  ");
    ImGui::SameLine();

    Ptr<CGraphicsShader> pShader = pMtrl->GetShader();
    if (nullptr != pShader)
    {
        string strKey = string(pShader->GetKey().begin(), pShader->GetKey().end());
        ImGui::InputText("##ShaderUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    }
    else
    {
        char szEmtpy[10] = {};
        ImGui::InputText("##ShaderUIName", szEmtpy, 10, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    }

    if (ImGui::Button("Save"))
    {
        pMtrl->Save(pMtrl->GetKey());
    }

    ImGui::NewLine();
    ImGui::Text("Parameter");
    

    // Shader ���� �䱸�ϴ� ScalarParameter �� UI �� ����
    if (nullptr == pShader)
        return 0;

    const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();

    for (size_t i = 0; i < vecScalarParam.size(); ++i)
    {
        switch (vecScalarParam[i].eParam)
        {
        // Shader Parameter �� Int Ÿ���� ���
        case INT_0:
        case INT_1:
        case INT_2:
        case INT_3:
        case INT_4:
        case INT_5:
        case INT_6:
        case INT_7:
        case INT_8:
        case INT_9:
        case INT_10:
        case INT_11:
        {
            // ���� ��Ƽ���� ���õ� ���� ����   
            int data = 0;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Int(vecScalarParam[i].strDesc, &data))
            {
                // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case FLOAT_0:
        case FLOAT_1:
        case FLOAT_2:
        case FLOAT_3:
        case FLOAT_4:
        case FLOAT_5:
        case FLOAT_6:
        case FLOAT_7:
        case FLOAT_8:
        case FLOAT_9:
        case FLOAT_10:
        case FLOAT_11:
        {
            // ���� ��Ƽ���� ���õ� ���� ����   
            float data = 0;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Float(vecScalarParam[i].strDesc, &data, true))
            {
                // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case VEC2_0:
        case VEC2_1:
        case VEC2_2:
        case VEC2_3:
        case VEC2_4:
        case VEC2_5:
        case VEC2_6:
        case VEC2_7:
        case VEC2_8:
        case VEC2_9:
        case VEC2_10:
        case VEC2_11:
        {
            // ���� ��Ƽ���� ���õ� ���� ����   
            Vec2 data;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Vec2(vecScalarParam[i].strDesc, &data))
            {
                // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case VEC4_0:
        case VEC4_1:
        case VEC4_2:
        case VEC4_3:
        case VEC4_4:
        case VEC4_5:
        case VEC4_6:
        case VEC4_7:
        case VEC4_8:
        case VEC4_9:
        case VEC4_10:
        case VEC4_11:
        {
            // ���� ��Ƽ���� ���õ� ���� ����   
            Vec4 data;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Vec4(vecScalarParam[i].strDesc, &data))
            {
                // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case MAT_0:
        case MAT_1:
        case MAT_2:
        case MAT_3:
            break;
        }        
    }

    // Shader ���� �䱸�ϴ� Texture Parameter �� UI �� ����
    const vector<tTexParam>& vecTexParam = pShader->GetTexParam();
    for (size_t i = 0; i < vecTexParam.size(); ++i)
    {
        ImGui::NewLine();
        Ptr<CTexture> pCurTex = pMtrl->GetTexParam(vecTexParam[i].eParam);
        if (ParamUI::Param_Tex(vecTexParam[i].strDesc, pCurTex, this, (UI_DELEGATE_1)&MaterialUI::SelectTexture))
        {
            m_eSelected = vecTexParam[i].eParam;
        }
        pMtrl->SetTexParam(vecTexParam[i].eParam, pCurTex);
    }

    return 0;
}


void MaterialUI::SelectTexture(DWORD_PTR _Key)
{
    string strKey = (char*)_Key;
    Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

    Ptr<CMaterial> pMtrl = (CMaterial*)GetTargetRes().Get();
    pMtrl->SetTexParam(m_eSelected, pTex);
}