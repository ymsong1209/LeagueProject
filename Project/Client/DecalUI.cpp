#include "pch.h"
#include "DecalUI.h"
#include <Engine\CGameObject.h>
#include <Engine\CDecal.h>
#include "TreeUI.h"
#include <Engine/CResMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include "ListUI.h"

DecalUI::DecalUI()
	: ComponentUI("##Decal", COMPONENT_TYPE::DECAL)
{
	SetName("Decal");
}

DecalUI::~DecalUI()
{
}

int DecalUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;
	ShowDebug();
	SetBSType();
	SelectAlpha();
	SetLayerCheck();
	ShowAsLight();
	ChooseTexture();



	return 0;
}

void DecalUI::ShowDebug()
{
	bool Show = GetTarget()->Decal()->GetShowDebug();
	ImGui::Text("ShowDebug");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ShowDebugDecal", &Show)) {
		GetTarget()->Decal()->SetShowDebug(Show);
	}
}

void DecalUI::ShowAsLight()
{
	bool Show = GetTarget()->Decal()->GetShowAsLight();
	ImGui::Text("ShowLight");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ShowLightDecal", &Show)) {
		GetTarget()->Decal()->SetAsLight(Show);
	}
	if (Show) {
		static bool alpha_preview = false;
		static bool alpha_half_preview = true;
		static bool options_menu = true;

		ImGuiColorEditFlags misc_flags = (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
		Vec4 vDiffuse = GetTarget()->Decal()->GetLightDiffuse();
		if (ImGui::ColorEdit4("##DecalDiffuse", (float*)&vDiffuse, ImGuiColorEditFlags_Float | misc_flags)) {
			GetTarget()->Decal()->SetLightDiffuse(vDiffuse);
		}
	}
}

int DecalUI::ChooseTexture()
{

	// Material 이름
	ImGui::Text("Texture");
	ImGui::SameLine();

	Ptr<CTexture> pTex = GetTarget()->Decal()->GetDecalTex();
	string strKey = "";
	if (pTex.Get()) {
		strKey = string(pTex->GetKey().begin(), pTex->GetKey().end());
	}

	ImGui::InputText("##DecalTexName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	if (ImGui::Button("##DecalTextureSelect", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Texture", ImVec2(300.f, 500.f));
		for (const auto& pair : mapTex)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&DecalUI::SelectTextureBtn);

		return 1;
	}

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
	static float DecalUIzoom = 0.5f;

	if (pTex == nullptr)
	{
		ImGui::Image((ImTextureID)0, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
	}
	else {

		ID3D11ShaderResourceView* Atlas = (pTex->GetSRV()).Get();
		float AtlasWidth = pTex->Width();
		float AtlasHeight = pTex->Height();



		if (ImGui::BeginChild("DecalTeximage", ImVec2(200.f, 200.f), false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::Image((void*)Atlas, ImVec2(150 * DecalUIzoom, 150 * DecalUIzoom), uv_min, uv_max, tint_col, border_col);
		}
		ImGui::EndChild();
	}


	// 드랍 체크
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
				Ptr<CTexture> tex = (CTexture*)pRes;
				GetTarget()->Decal()->SetDecalTex(tex);
			}
		}

		ImGui::EndDragDropTarget();

		return 0;
	}


	if (ImGui::InputFloat("zoom", &DecalUIzoom, 0.5f, 1.0f))
	{
		if (DecalUIzoom < 0.1f)
			DecalUIzoom = 0.1f;

	}

	if (ImGui::SliderFloat("zoom2", &DecalUIzoom, 0.0f, 20.f)) {

	};

	return 0;
}

void DecalUI::SetLayerCheck()
{
	int DecalLayer = GetTarget()->Decal()->GetLayerCheck();

	// Showing a menu with toggles
	if (ImGui::Button("CheckDecal##DecalUIButton"))
		ImGui::OpenPopup("CheckDecal##DecalUIPopUp");
	if (ImGui::BeginPopup("CheckDecal##DecalUIPopUp"))
	{
		CLevel* Curlevel = CLevelMgr::GetInst()->GetCurLevel();
		for (UINT i = 0; i < MAX_LAYER; ++i) {
			ImGui::Text("%d layer : ", i);
			ImGui::SameLine();

			CLayer* CurLayer = Curlevel->GetLayer(i);
			string layername = string(CurLayer->GetName().begin(), CurLayer->GetName().end());
			char input_buffer[256]; // allocate a buffer for the input
			strcpy_s(input_buffer, layername.c_str());
			string layerid = "##layerNum_DecalUI" + std::to_string(i);
			ImGui::PushItemWidth(150);
			ImGui::InputText(layerid.c_str(), input_buffer, 256, ImGuiInputTextFlags_ReadOnly);

			ImGui::SameLine();
			bool layerchecked = false;
			if (DecalLayer & (1 << i)) {
				layerchecked = true;
			}
			layerid = "##LayerCheck_DecalUI" + std::to_string(i);
			if (ImGui::Checkbox(layerid.c_str(), &layerchecked)) {
				if (layerchecked) {
					DecalLayer |= (0x1 << i);
				}
				else {
					DecalLayer &= ~(0x1 << i);
				}
				GetTarget()->Decal()->SetLayerCheck(DecalLayer);
			}
		}
		ImGui::EndPopup();
	}

}

void DecalUI::SetBSType()
{

	const char* BSType[] = { "DEFAULT", "MASK", "ALPHA_BLEND", "ONE_ONE" };

	UINT BS_idx = (UINT)GetTarget()->Decal()->GetBSType();
	const char* BS_preview_value = BSType[BS_idx];
	static ImGuiComboFlags flags = 0;
	ImGui::Text("BS_Type");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##BSTypeDecalUI", BS_preview_value, flags)) {
		for (int i = 0; i < IM_ARRAYSIZE(BSType); i++) {
			const bool is_selected = (BS_idx == i);
			if (ImGui::Selectable(BSType[i], is_selected))
				BS_idx = i;

			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				GetTarget()->Decal()->SetBSType(static_cast<BS_TYPE>(i));
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void DecalUI::SelectAlpha()
{
	float alpha = GetTarget()->Decal()->GetAlpha();
	ImGui::Text("Alpha");
	ImGui::SameLine();
	if (ImGui::SliderFloat("##AlphaDecalUI", &alpha, 0.f, 1.f)) {
		GetTarget()->Decal()->SetAlpha(alpha);
	}
}


void DecalUI::SelectTextureBtn(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

	GetTarget()->Decal()->SetDecalTex(pTex);
}