#include "pch.h"
#include "LandScapeUI.h"

#include <Engine/CResMgr.h>
#include <Engine\CLandScape.h>
#include <Engine/CTexture.h>

#include "ImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

LandScapeUI::LandScapeUI()
	: ComponentUI("##LandScape", COMPONENT_TYPE::LANDSCAPE)
{
	SetName("LandScape");
}

LandScapeUI::~LandScapeUI()
{
}


int LandScapeUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;


	SetFace();
	HeightMapDropCheck();
	BrushTexDropCheck();
	SetLandScapeMod();
	SetBrushScale();


	int weightidx = GetTarget()->LandScape()->GetWeightIdx();
	ImGui::Text("WeightIdx : ");
	ImGui::SameLine();
	if (ImGui::InputInt("##weightidxLandSacpe", &weightidx)) {
		GetTarget()->LandScape()->SetWeightIdx(weightidx);
	}
	SelectRSType();
	return TRUE;
}

void LandScapeUI::SetFace()
{
	ImGui::Text("Face X : ");
	ImGui::SameLine();
	int facex = (int)GetTarget()->LandScape()->GetFaceX();
	int facez = (int)GetTarget()->LandScape()->GetFaceZ();
	if (ImGui::InputInt("##LandScapeFaceX", &facex)) {
		GetTarget()->LandScape()->SetFace(facex, facez);
	}

	ImGui::Text("Face Z : ");
	ImGui::SameLine();
	if (ImGui::InputInt("##LandScapeFaceZ", &facez)) {
		GetTarget()->LandScape()->SetFace(facex, facez);
	}
}

void LandScapeUI::HeightMapDropCheck()
{
	ImGui::Text("HeightMap : ");
	ImGui::SameLine();
	Ptr<CTexture> HeightMap = GetTarget()->LandScape()->GetHeightMap();
	char szBuff[50] = {};
	GetResKey(HeightMap.Get(), szBuff, 50);
	ImGui::InputText("##HeightMapName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

	//Mtrl 드랍 체크
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
				GetTarget()->LandScape()->SetHeightMap(tex);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##HeightMapSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapMtrl = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Texture", ImVec2(300.f, 500.f));
		for (const auto& pair : mapMtrl)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&LandScapeUI::SelectHeightMap);
	}
}

void LandScapeUI::BrushTexDropCheck()
{
	ImGui::Text("BrushTex : ");
	ImGui::SameLine();
	Ptr<CTexture> HeightMap = GetTarget()->LandScape()->GetBrushTex();
	char szBuff[50] = {};
	GetResKey(HeightMap.Get(), szBuff, 50);
	ImGui::InputText("##LandScapeBrushTexName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

	//Mtrl 드랍 체크
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
				GetTarget()->LandScape()->SetBrushTex(tex);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##LandScapeBrushTexSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapMtrl = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Texture", ImVec2(300.f, 500.f));
		for (const auto& pair : mapMtrl)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&LandScapeUI::SelectBrushTex);
	}
}

void LandScapeUI::SetBrushScale()
{
	float Scale = GetTarget()->LandScape()->GetBrushScale().x;
	ImGui::Text("Brush Scale : ");
	ImGui::SameLine();
	if (ImGui::SliderFloat("##LandScapeBrushScale", &Scale, 0.01, 1.0)) {
		GetTarget()->LandScape()->SetBrushScale(Scale);
	}
}

void LandScapeUI::SetLandScapeMod()
{
	const char* LandScapeModChar[] = { "HeightMap", "Splat", "None" };

	LANDSCAPE_MOD CurType = GetTarget()->LandScape()->GetLandScapeMod();
	UINT TypeIdx = 0;
	if (CurType == LANDSCAPE_MOD::HEIGHT_MAP) TypeIdx = 0;
	else if (CurType == LANDSCAPE_MOD::SPLAT) TypeIdx = 1;
	else TypeIdx = 2;

	const char* prev_value = LandScapeModChar[TypeIdx];
	static ImGuiComboFlags flags = 0;
	ImGui::Text("LandScape Mod");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##LandScapeModLandScapeUI", prev_value, flags)) {
		for (int i = 0; i < IM_ARRAYSIZE(LandScapeModChar); i++) {
			const bool is_selected = (TypeIdx == i);
			if (ImGui::Selectable(LandScapeModChar[i], is_selected))
				TypeIdx = i;
			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				if (i == 0)
					GetTarget()->LandScape()->SetLandScapeMod(LANDSCAPE_MOD::HEIGHT_MAP);
				else if (i == 1)
					GetTarget()->LandScape()->SetLandScapeMod(LANDSCAPE_MOD::SPLAT);
				else
					GetTarget()->LandScape()->SetLandScapeMod(LANDSCAPE_MOD::NONE);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void LandScapeUI::SelectHeightMap(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pHeightMapTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->LandScape()->SetHeightMap(pHeightMapTex);
}

void LandScapeUI::SelectBrushTex(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pBrushTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->LandScape()->SetBrushTex(pBrushTex);
}

void LandScapeUI::SelectRSType()
{
	const char* RsTypeChar[] = { "CULL_BACK", "WIREFRAME" };

	RS_TYPE CurType = GetTarget()->LandScape()->GetRSType();
	UINT TypeIdx = 0;
	if (CurType == RS_TYPE::CULL_BACK) TypeIdx = 0;
	else if (CurType == RS_TYPE::WIRE_FRAME) TypeIdx = 1;

	const char* lightinfo_preview_value = RsTypeChar[TypeIdx];
	static ImGuiComboFlags flags = 0;
	ImGui::Text("RSType");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##RSTypeLandScapeUI", lightinfo_preview_value, flags)) {
		for (int i = 0; i < IM_ARRAYSIZE(RsTypeChar); i++) {
			const bool is_selected = (TypeIdx == i);
			if (ImGui::Selectable(RsTypeChar[i], is_selected))
				TypeIdx = i;
			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				if (i == 0)
					GetTarget()->LandScape()->SetRSType(RS_TYPE::CULL_BACK);
				else if (i == 1)
					GetTarget()->LandScape()->SetRSType(RS_TYPE::WIRE_FRAME);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

