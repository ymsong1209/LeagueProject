#include "pch.h"
#include "CreateMaterialUI.h"
#include <Engine\CResMgr.h>
#include "ListUI.h"
#include "ParamUI.h"
#include <Engine\CRes.h>

void CreateMaterialUI::finaltick()
{
	UI::finaltick();
}

int CreateMaterialUI::render_update()
{
	if (!SetWindowSize)
	{
		ImGui::SetWindowSize(ImVec2(700.f, 600.f));
		SetWindowSize = true;
	}

	// Material �̸�
	ImGui::Text("Material");
	ImGui::SameLine();


	char szBuff[50] = {};
	ImGui::Text("Level Name :");
	ImGui::SameLine();

	ImGui::InputText("##Level Name", MtrlName, 50);
	wstring BurffChar = charTowstring(MtrlName);
	CulMaterial->SetKey(BurffChar);


	//// GraphicsShader �̸�
	ImGui::Text("Shader  ");
	ImGui::SameLine();
	if (nullptr != CulShader)
	{
		string strKey = string(CulShader->GetKey().begin(), CulShader->GetKey().end());
		ImGui::InputText("##ShaderUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	}
	else
	{
		char szEmtpy[10] = {};
		ImGui::InputText("##ShaderUIName", szEmtpy, 10, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	}

	ImGui::SameLine();
	if (ImGui::Button("SetShader", ImVec2(100, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapShader = CResMgr::GetInst()->GetResources(RES_TYPE::GRAPHICS_SHADER);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Shader", ImVec2(300.f, 500.f));
		for (const auto& pair : mapShader)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// �׸� ���ý� ȣ����� ��������Ʈ ���
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&CreateMaterialUI::SelectShader);
	}


	ImGui::NewLine();
	ImGui::Text("Parameter");


	//// Shader ���� �䱸�ϴ� ScalarParameter �� UI �� ����

	if (nullptr != CulShader)
	{
		const vector<tScalarParam>& vecScalarParam = CulShader->GetScalarParam();

		for (size_t i = 0; i < vecScalarParam.size(); ++i)
		{
			switch (vecScalarParam[i].eParam)
			{
				// Shader Parameter �� Int Ÿ���� ���
			case INT_0:
			case INT_1:
			case INT_2:
			case INT_3:
			{
				// ���� ��Ƽ���� ���õ� ���� ����   
				int data = 0;
				CulMaterial->GetScalarParam(vecScalarParam[i].eParam, &data);
				if (ParamUI::Param_Int(vecScalarParam[i].strDesc, &data))
				{
					// UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
					CulMaterial->SetScalarParam(vecScalarParam[i].eParam, &data);
				}
			}
			break;
			case FLOAT_0:
			case FLOAT_1:
			case FLOAT_2:
			case FLOAT_3:
			{
				// ���� ��Ƽ���� ���õ� ���� ����   
				float data = 0;
				CulMaterial->GetScalarParam(vecScalarParam[i].eParam, &data);
				if (ParamUI::Param_Float(vecScalarParam[i].strDesc, &data))
				{
					// UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
					CulMaterial->SetScalarParam(vecScalarParam[i].eParam, &data);
				}
			}
			break;
			case VEC2_0:
			case VEC2_1:
			case VEC2_2:
			case VEC2_3:
			{
				// ���� ��Ƽ���� ���õ� ���� ����   
				Vec2 data;
				CulMaterial->GetScalarParam(vecScalarParam[i].eParam, &data);
				if (ParamUI::Param_Vec2(vecScalarParam[i].strDesc, &data))
				{
					// UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
					CulMaterial->SetScalarParam(vecScalarParam[i].eParam, &data);
				}
			}
			break;
			case VEC4_0:
			case VEC4_1:
			case VEC4_2:
			case VEC4_3:
			{
				// ���� ��Ƽ���� ���õ� ���� ����   
				Vec4 data;
				CulMaterial->GetScalarParam(vecScalarParam[i].eParam, &data);
				if (ParamUI::Param_Vec4(vecScalarParam[i].strDesc, &data))
				{
					// UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
					CulMaterial->SetScalarParam(vecScalarParam[i].eParam, &data);
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
		const vector<tTexParam>& vecTexParam = CulShader->GetTexParam();
		for (size_t i = 0; i < vecTexParam.size(); ++i)
		{
			ImGui::NewLine();
			Ptr<CTexture> pCurTex = CulMaterial->GetTexParam(vecTexParam[i].eParam);
			if (ParamUI::Param_Tex(vecTexParam[i].strDesc, pCurTex, this, (UI_DELEGATE_1)&CreateMaterialUI::SelectTexture))
			{
				m_eSelected = vecTexParam[i].eParam;
			}
			CulMaterial->SetTexParam(vecTexParam[i].eParam, pCurTex);
		}

	}

	//ImGui::Separator();
	//if (ImGui::Button("New Material Create"))
	//{
	//    UI* MaterialEditor = ImGuiMgr::GetInst()->FindUI("##MaterialEditor");
	//    MaterialEditor->SetActive(true);
	//}

	if (ImGui::Button("Save"))
	{
		wstring MaterialName = CulMaterial->GetKey();
		wstring path = L"material\\";
		wstring Name = MaterialName;
		wstring FileName = L".mtrl";
		wstring pathName = path + Name + FileName;
		CulMaterial->Save(pathName);
	}

	return TRUE;
}

CreateMaterialUI::CreateMaterialUI()
	:UI("##MaterialEditor")
	, SetWindowSize(false)
{
	SetName("Material Editor");
	CulMaterial = new CMaterial;

	CResMgr::GetInst()->AddRes<CMaterial>(L"material\\DummyMaterial.mtrl", CulMaterial);
}

CreateMaterialUI::~CreateMaterialUI()
{
}

void CreateMaterialUI::SelectShader(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	CulShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(wstring(strKey.begin(), strKey.end()));
	wstring Path = CulShader->GetRelativePath();

	CulMaterial->SetShader(CulShader);

}


void CreateMaterialUI::SelectTexture(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

	CulMaterial->SetTexParam(m_eSelected, pTex);
}