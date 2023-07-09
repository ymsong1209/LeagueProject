#include "pch.h"
#include "LevelUI.h"
#include "ImGuiMgr.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CEventMgr.h>
#include "CLevelSaveLoad.h"
#include <Engine/CKeyMgr.h>
#include "InspectorUI.h"
#include <Engine/CTimeMgr.h>


LevelUI::LevelUI()
	: UI("##Level")
	, m_LeftLayer(1)
	, m_RightLayer(1)
{
	SetName("Level");
}

LevelUI::~LevelUI()
{

}



int LevelUI::render_update()
{
	CLevel* level = CLevelMgr::GetInst()->GetCurLevel();
	string levelname = string(level->GetName().begin(), level->GetName().end());


	if (ImGui::BeginChild("LevelSelect", ImVec2(300.f, 500.f), false, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::Text("CurLevel : ");
		ImGui::SameLine();
		ImGui::Text(levelname.c_str());

		ImGui::Text("Level State : ");
		ImGui::SameLine();
		if (level->GetState() == LEVEL_STATE::PLAY) {
			ImGui::Text("Play");
		}
		else if (level->GetState() == LEVEL_STATE::PAUSE) {
			ImGui::Text("Pause");
		}
		else if (level->GetState() == LEVEL_STATE::STOP) {
			ImGui::Text("Stop");
		}


		ImGui::Text("SelectLevel");
		static int item_current_idx = 0; // Here we store our selection data as an index.
		if (ImGui::BeginListBox("##SelectLevel"))
		{
			for (int n = 0; n < m_vecLevelName.size(); n++)
			{
				string levelname = string(m_vecLevelName[n].begin(), m_vecLevelName[n].end());
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(levelname.c_str(), is_selected)) {
					item_current_idx = n;

					// Level 불러오기
					CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevelFromJson(m_vecLevelName[n]);

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pLoadedLevel;

					CEventMgr::GetInst()->AddEvent(evn);

					//inspector 초기화
					InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
					inspector->SetTargetObject(nullptr);

					//현재 레벨이 stop이였으면 다음 레벨도 stop이여야함
					if (level->GetState() == LEVEL_STATE::STOP) {
						CTimeMgr::GetInst()->SetTimeScale(0.f);
					}


				}


				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}


	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("LayerName", ImVec2(300.f, 500.f), false, ImGuiWindowFlags_HorizontalScrollbar)) {
		CLevel* Curlevel = CLevelMgr::GetInst()->GetCurLevel();
		for (UINT i = 0; i < MAX_LAYER; ++i) {
			ImGui::Text("%d layer : ", i);
			ImGui::SameLine();

			CLayer* CurLayer = Curlevel->GetLayer(i);
			string layername = string(CurLayer->GetName().begin(), CurLayer->GetName().end());
			char input_buffer[256]; // allocate a buffer for the input
			strcpy_s(input_buffer, layername.c_str());
			string layerid = "##layerNum" + std::to_string(i);
			if (ImGui::InputText(layerid.c_str(), input_buffer, 256)) {

				if (KEY_TAP(KEY::ENTER)) {
					layername = input_buffer;
					wstring name;
					name.assign(layername.begin(), layername.end());// = wstring.assign(EditedName.begin(), EditedName.end());
					CurLayer->SetName(name);

					if (Curlevel->GetName() == L"")
					{
						wchar_t szStr[256] = {};
						wsprintf(szStr, L"레벨 이름 없음 / 레벨을 먼저 저장하십시요");
						MessageBox(nullptr, szStr, L"레이어 이름 변경후 저장실패.", MB_OK);
					}
					else {
						CLevelSaveLoad::SaveLevel(Curlevel->GetName(), Curlevel);
					}

				}
			}
		}

	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("LayerCollision", ImVec2(300.f, 500.f), false, ImGuiWindowFlags_HorizontalScrollbar)) {
		CLevel* Curlevel = CLevelMgr::GetInst()->GetCurLevel();
		UINT CollisionMatrix[MAX_LAYER];
		for (UINT i = 0; i < MAX_LAYER; ++i) {
			CollisionMatrix[i] = CCollisionMgr::GetInst()->GetMatrix()[i];
		}

		//어느 레이어끼리 충돌시키고 싶은지 세팅
		ImGui::Text("Layer Collision");
		vector<string> Layername;
		for (UINT i = 0; i < MAX_LAYER; ++i) {
			CLayer* CurLayer = Curlevel->GetLayer(i);
			string name = string(CurLayer->GetName().begin(), CurLayer->GetName().end());
			Layername.push_back(name);
		}

		if (ImGui::BeginCombo("##ChooseLeftLayer", Layername[m_LeftLayer].c_str(), ImGuiComboFlags_None)) {
			for (UINT i = 0; i < Layername.size(); i++) {
				const bool is_left_selected = (m_LeftLayer == i);
				if (Layername[i] != "") {
					if (ImGui::Selectable(Layername[i].c_str(), is_left_selected)) {
						m_LeftLayer = i;
					}
				}


				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_left_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}


		if (ImGui::BeginCombo("##ChooseRightLayer", Layername[m_RightLayer].c_str(), ImGuiComboFlags_None)) {
			for (UINT i = 0; i < Layername.size(); i++) {
				const bool is_right_selected = (m_LeftLayer == i);

				if (Layername[i] != "") {
					if (ImGui::Selectable(Layername[i].c_str(), is_right_selected)) {
						m_RightLayer = i;
					}
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_right_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}


		ImGui::SameLine();

		if (ImGui::Button("Add Collision")) {
			CCollisionMgr::GetInst()->LayerCheck(m_LeftLayer, m_RightLayer);
		}

		//현재 세팅되어 있는 레이어 표시

		for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
		{
			for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
			{
				if (CollisionMatrix[iRow] & (1 << iCol)) {
					CLayer* Rowlayer = Curlevel->GetLayer(iRow);
					string RowName = string(Rowlayer->GetName().begin(), Rowlayer->GetName().end());
					ImGui::Text(RowName.c_str());

					ImGui::SameLine();
					ImGui::Text("    ");
					ImGui::SameLine();

					CLayer* Collayer = Curlevel->GetLayer(iCol);
					string ColName = string(Collayer->GetName().begin(), Collayer->GetName().end());
					ImGui::Text(ColName.c_str());
					ImGui::SameLine();

					string buttonID = "Remove Collision##" + std::to_string((iRow + 1) * iCol);
					if (ImGui::Button(buttonID.c_str())) {

						if (iRow > iCol)
						{
							UINT iTemp = iCol;
							iCol = iRow;
							iRow = iTemp;
						}

						CollisionMatrix[iRow] -= (1 << iCol);
						CCollisionMgr::GetInst()->SetMatrix(CollisionMatrix);
					}
				}
			}
		}

	}
	ImGui::EndChild();



	return 0;
}