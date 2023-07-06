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
	Ptr<CMaterial> pMtrl = GetTarget()->MeshRender()->GetMaterial(0);
		
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
				GetTarget()->MeshRender()->SetMaterial(((CMaterial*)pRes));

				GetTarget()->MeshRender()->ClearDynamicMtrl();

				/*SelectMaterial((DWORD_PTR))(pRes->GetKey());
				GetTarget()->MeshRender()->SetMesh((CMesh*)pRes);*/
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();

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

	int isDynamicMtrlUse = (int)(GetTarget()->MeshRender()->IsDynamicMtrlEmpty());
	ImGui::Text("Dynamic Mtrl Use :"); ImGui::SameLine();
	if (ImGui::RadioButton("Use", &isDynamicMtrlUse, 0))
	{
		GetTarget()->MeshRender()->GetDynamicMaterial();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Not Use", &isDynamicMtrlUse, 1))
	{
		GetTarget()->MeshRender()->ClearDynamicMtrl();
	}
	

 


	

	ImGui::Separator();



	int isUseMovingVec = (int)(GetTarget()->MeshRender()->IsUsingMovingVec());


	ImGui::Text("Moving Vec Use :"); ImGui::SameLine();
	if (ImGui::RadioButton("Use##MovingVec", &isUseMovingVec, 1))
	{
		GetTarget()->MeshRender()->SetUsingMovingVec(true);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Not Use##MovingVec", &isUseMovingVec, 0))
	{
		GetTarget()->MeshRender()->SetUsingMovingVec(false);
	}



	
	 

	// Std2DMtrl_Dynamic Mtrl을 쓰고 있는 경우
	if (GetTarget()->MeshRender()->IsUsingMovingVec())
	{
		const vector<MovingStruct>   MovingVec = GetTarget()->MeshRender()->GetMovingStruct();

		for (int i = 0; i < MovingVec.size(); ++i)
		{
			MovingStruct UpdateMovingStruct;


			Vec4 FunctionValue = MovingVec[i].FuncValue;
			Vec2 UpdateOffsetValue = MovingVec[i].PreviousPos;


			// 무슨 역할의 Texture인지 이름을 알려줌
			switch (MovingVec[i].TargetTex)
			{
			case eTargetTexture::OUTPUT:
			{
				ImGui::Text("Output Texture");
			}
			break;
			case eTargetTexture::PUNCTURE:
			{
				ImGui::Text("Puncture Texture");
			}
			break;
			}


			// Texture선택할 수 있도록 해줌
			ImGui::SameLine();
			GetResKey(pMtrl->GetTexParam((TEX_PARAM(i))).Get(), szBuff, 50);

			string TexName = "##TexName" + std::to_string(i);
			ImGui::InputText(TexName.c_str(), szBuff, 50, ImGuiInputTextFlags_ReadOnly);

			if (ImGui::BeginDragDropTarget())
			{
				// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
				const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
				if (pPayLoad)
				{
					TreeNode* pNode = (TreeNode*)pPayLoad->Data;
					CRes* pRes = (CRes*)pNode->GetData();
					if (RES_TYPE::TEXTURE == pRes->GetType())
					{
						GetTarget()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM(i), (CTexture*)pRes);  
					}
				}

				ImGui::EndDragDropTarget();
			}


			// Texture를 비울 수 있도록 해줌
			ImGui::SameLine();
			string TexClearButton = "Clear##" +  std::to_string(i);
			if (ImGui::Button(TexClearButton.c_str(), ImVec2(50.f, 20.f)))
			{
				GetTarget()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM(i), (CTexture*)nullptr);
			}



			// 해당 Texture의 움직이는 함수를 알려줌
			ImGui::Text("Moving Style"); ImGui::SameLine();

			string MoveOffsetClearBtn = "Clear Move##" + std::to_string(i);
			if (ImGui::Button(MoveOffsetClearBtn.c_str(), ImVec2(100.f, 20.f)))
			{
				UpdateOffsetValue = Vec2(0.f, 0.f);
			}


			const char* items[] = { "None", "Horizonetal", "Vertical", "Linear", "Parabola", "Sin", "Cos" };
			int item_current_idx = (int)MovingVec[i].MovingStyle;; // Here we store our selection data as an index.
			string MovingFuncName = "##MovingFuncName" + std::to_string(i);
			const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
			if (ImGui::BeginCombo(MovingFuncName.c_str(), combo_preview_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				/*	if (is_selected)
						ImGui::SetItemDefaultFocus();*/
				}
				ImGui::EndCombo();
			}




			switch (MovingVec[i].MovingStyle)
			{
			case eTexMovingStyle::NONE:
				break;
			case eTexMovingStyle::HORIZONTAL:
			{
				ImGui::Text("FuncValue.x : dx / dt");
			}
			break;
			case eTexMovingStyle::VERTICAL:
			{
				ImGui::Text("FuncValue.x : dy / dt");
			}
			break;
			case eTexMovingStyle::LINEAR:
			{
				ImGui::Text("FuncValue.x : dx / dt"); ImGui::SameLine(); ImGui::Text("FuncValue.y : x  coef");
				ImGui::Text("FuncValue.z : y-inter");
			}
			break;
			case eTexMovingStyle::PARABOLA:
			{
				ImGui::Text("FuncValue.x : dx / dt"); ImGui::SameLine(); ImGui::Text("FuncValue.y : x^2coef");
				ImGui::Text("FuncValue.z : x  coef"); ImGui::SameLine(); ImGui::Text("FuncValue.w : y-inter");

			}
			break;
			case eTexMovingStyle::SIN:
			{
				ImGui::Text("FuncValue.x : dx / dt"); ImGui::SameLine(); ImGui::Text("FuncValue.y : Coef   ");
				ImGui::Text("FuncValue.z : Freq   "); ImGui::SameLine(); ImGui::Text("FuncValue.w : y-inter");
			}
			break;
			case eTexMovingStyle::COS:
			{
				ImGui::Text("FuncValue.x : dx / dt"); ImGui::SameLine(); ImGui::Text("FuncValue.x : Coef   ");
				ImGui::Text("FuncValue.x : Freq   "); ImGui::SameLine(); ImGui::Text("FuncValue.x : y-inter");
			}
			break;
			}
			 
			ImGui::Text("Func Value"); ImGui::SameLine();

			string FuncValueName = "##FuncValue" + std::to_string(i);
			ImGui::InputFloat4(FuncValueName.c_str(), FunctionValue);

			UpdateMovingStruct.FuncValue = FunctionValue;
			UpdateMovingStruct.MovingStyle = (eTexMovingStyle)item_current_idx;
			UpdateMovingStruct.PreviousPos = UpdateOffsetValue;

			UpdateMovingStruct.TargetTex = MovingVec[i].TargetTex;

			GetTarget()->MeshRender()->SetMovingStruct(i, UpdateMovingStruct);


			ImGui::Separator();

		}

		// Additive Texture 확인부분
		//ImGui::SameLine();
		GetResKey(pMtrl->GetTexParam((TEX_PARAM(2))).Get(), szBuff, 50);
		 
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
				{
					GetTarget()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM(2), (CTexture*)pRes);
				}
			}

			ImGui::EndDragDropTarget();
		}


		// Texture를 비울 수 있도록 해줌
		ImGui::SameLine();
		string TexClearButton = "Clear##" + std::to_string(3);
		if (ImGui::Button(TexClearButton.c_str(), ImVec2(50.f, 20.f)))
		{
			GetTarget()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM(2), (CTexture*)nullptr);
		}



		Ptr<CMaterial> Material = GetTarget()->MeshRender()->GetMaterial();

		// Additive Texture가 있는 지 확인 
		if (Material->GetTexParam(TEX_PARAM::TEX_2).Get() != nullptr)
		{
			Vec4 AdditiveColor;

			Material->GetScalarParam(SCALAR_PARAM::VEC4_0, &AdditiveColor);

			float AdditiveVec[4] = { AdditiveColor.x * 255, AdditiveColor.y * 255 , AdditiveColor.z *255, 0.f };

			ImGui::Text("Additive Color"); ImGui::SameLine();
			ImGui::InputFloat4("##AdditiveColor", AdditiveVec);

			AdditiveColor = Vec4{ AdditiveVec[0] / 255, AdditiveVec[1] / 255, AdditiveVec[2] / 255, 0.f };

			 
			Material->SetScalarParam(SCALAR_PARAM::VEC4_0, &AdditiveColor);

			ImGui::Separator();
		}
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
	GetTarget()->MeshRender()->SetMaterial(pMtrl);

	GetTarget()->MeshRender()->ClearDynamicMtrl();
 
}