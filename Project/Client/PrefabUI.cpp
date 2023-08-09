#include "pch.h"
#include "PrefabUI.h"
#include <Engine/CCamera.h>
#include <Engine/CResMgr.h>
#include <Engine/CDevice.h>
#include <Engine/CRenderMgr.h>

#include <Engine/CTransform.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CPrefab.h>

#include "ImGuiMgr.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include "CLevelSaveLoad.h"

PrefabUI::PrefabUI()
	:ResUI(RES_TYPE::PREFAB)
	, m_iSpawnLayerIdx(1)
	, m_bClickToSpawn(true)
	, m_bIsTransformAbsolute(false)
{
	SetName("Prefab");
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::tick()
{

}

int PrefabUI::render_update()
{
	ResUI::render_update();
	m_pEditorCam = CRenderMgr::GetInst()->GetMainCam();
	if (!m_pEditorCam) {
		ImGui::Text("No EditorCam");
		return 0;
	}


	Vec2 pos = CKeyMgr::GetInst()->GetMousePos();
	m_vMousePos = Vec4(pos.x, pos.y, 1.f, 0.f);

	Vec3 RayPos = m_pEditorCam->GetRay().vDir;
	Matrix CamWorld = m_pEditorCam->Transform()->GetWorldMat();

	//EditorCamera의 Position에 스폰됩니다.
	CGameObject* LoLMap = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
	if (LoLMap != nullptr) {
		tRay ray = m_pEditorCam->GetRay();
		IntersectResult result = m_pEditorCam->IsCollidingBtwRayRect(ray, LoLMap);
		Vec3 EndPos = result.vCrossPoint;
		if (result.bResult) {
			m_vConvertedMousePos = Vec4(EndPos.x, EndPos.y, EndPos.z, 1.f);
		}
		else {
			m_vConvertedMousePos = m_pEditorCam->Transform()->GetRelativePos();
		}

	}
	else {
		m_vConvertedMousePos = m_pEditorCam->Transform()->GetRelativePos();
	}

	CPrefab* pPrefab = (CPrefab*)GetTargetRes().Get();
	CGameObject* PrefabObject = pPrefab->GetProtoObject();
	ImGui::Text("MousePos : %f, %f, %f", m_vConvertedMousePos.x, m_vConvertedMousePos.y, m_vConvertedMousePos.z);
	PrefabObject->Transform()->SetRelativePos(m_vConvertedMousePos.x, m_vConvertedMousePos.y, m_vConvertedMousePos.z);
	string ClickOrDrag;
	if (m_bClickToSpawn) ClickOrDrag = "Click##PrefabUI";
	else ClickOrDrag = "Drag##PrefabUI";
	if (ImGui::Button(ClickOrDrag.c_str())) {
		m_bClickToSpawn = !m_bClickToSpawn;
	}
	ImGui::Text("Transform Absolute : ");
	ImGui::Checkbox("##TransformAbsolutePrefabUI", &m_bIsTransformAbsolute);
	SelectLayer();
	SpawnPrefab();


	return 0;
}

void PrefabUI::SelectLayer()
{
	CLevel* level = CLevelMgr::GetInst()->GetCurLevel();
	vector<string> layernamearray;
	layernamearray.clear();
	for (int i = 0; i < MAX_LAYER; ++i) {
		CLayer* layer = level->GetLayer(i);
		wstring name = layer->GetName();
		if (name.empty()) {
			layernamearray.push_back(std::to_string(i));
		}
		else {
			string layername = string(layer->GetName().begin(), layer->GetName().end());
			layernamearray.push_back(layername);
		}

	}

	static int LayerIdx = 0;
	const char* colliderinfo_preview_value = layernamearray[LayerIdx].c_str();
	static ImGuiComboFlags flags = 0;

	ImGui::Text("Layer Type : ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##PrefabUI_LayerType", colliderinfo_preview_value, flags)) {
		for (int i = 0; i < layernamearray.size(); i++) {
			const bool is_selected = (LayerIdx == i);
			if (ImGui::Selectable(layernamearray[i].c_str(), is_selected))
				LayerIdx = i;

			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				m_iSpawnLayerIdx = i;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void PrefabUI::SpawnPrefab()
{
	

	//클릭해서 Spawn
	if (KEY_TAP(KEY::LBTN) && m_bClickToSpawn && !ImGui::IsAnyItemHovered()&& !ImGui::IsWindowHovered())
	{
		CPrefab* pPrefab = (CPrefab*)GetTargetRes().Get();
		wstring path = pPrefab->GetRelativePath();
		if (path == L"") {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Prefab이 저장되지 않았습니다. 먼저 저장하십시요.");
			MessageBox(nullptr, szStr, L"Prefab Spawn 실패.", MB_OK);
			return;
		}

		CGameObject* PrefabObject = pPrefab->Instantiate();
		Vec3 Scale = PrefabObject->Transform()->GetRelativeScale();
		Vec3 pos = Vec3(m_vConvertedMousePos.x, m_vConvertedMousePos.y + Scale.y/2.f, m_vConvertedMousePos.z);
		

		if (m_bIsTransformAbsolute) {
			PrefabObject->Transform()->SetAbsolute(true);
		}
		SpawnGameObject(PrefabObject, pos, m_iSpawnLayerIdx);
	}
	//Drag해서 Spawn
	if (KEY_TAP(KEY::LBTN) && !m_bClickToSpawn && !ImGui::IsAnyItemHovered())
	{
		CPrefab* pPrefab = (CPrefab*)GetTargetRes().Get();
		wstring path = pPrefab->GetRelativePath();
		if (path == L"") {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Prefab이 저장되지 않았습니다. 먼저 저장하십시요.");
			MessageBox(nullptr, szStr, L"Prefab Spawn 실패.", MB_OK);
			return;
		}

		m_pClonedObject = pPrefab->Instantiate();
		m_vClickedPosition = Vec3(m_vConvertedMousePos.x, m_vConvertedMousePos.y, m_vConvertedMousePos.z);
		SpawnGameObject(m_pClonedObject, m_vClickedPosition, m_iSpawnLayerIdx);
	}
	if (KEY_PRESSED(KEY::LBTN) && m_pClonedObject) {
		Vec3 pos = Vec3(m_vConvertedMousePos.x, m_vConvertedMousePos.y, m_vConvertedMousePos.z);
		Vec2 Scale = Vec2(0.f, 0.f);
		Vec3 CenterPos = Vec3(0.f, 0.f, 0.f);
		if (pos.x > m_vClickedPosition.x) {
			//좌측상단 -> 우측하단
			if (pos.y < m_vClickedPosition.y) {
				Scale = Vec2(abs(pos.x - m_vClickedPosition.x), abs(pos.y - m_vClickedPosition.y));
				CenterPos = Vec3(m_vClickedPosition.x + Scale.x / 2.0f, m_vClickedPosition.y - Scale.y / 2.0f, m_vClickedPosition.z);
				m_pClonedObject->Transform()->SetRelativeScale(Vec3(Scale.x, Scale.y, m_pClonedObject->Transform()->GetRelativeScale().z));
			}
			//좌측하단 ->우측상단
			else if (m_vClickedPosition.y < pos.y) {
				Scale = Vec2(abs(pos.x - m_vClickedPosition.x), abs(pos.y - m_vClickedPosition.y));
				CenterPos = Vec3(m_vClickedPosition.x + Scale.x / 2.0f, m_vClickedPosition.y + Scale.y / 2.0f, m_vClickedPosition.z);
				m_pClonedObject->Transform()->SetRelativeScale(Vec3(Scale.x, Scale.y, m_pClonedObject->Transform()->GetRelativeScale().z));
			}
		}
		else if (m_vClickedPosition.x > pos.x) {
			//우측 하단->좌측 상단
			if (m_vClickedPosition.y < pos.y) {
				Scale = Vec2(abs(pos.x - m_vClickedPosition.x), abs(pos.y - m_vClickedPosition.y));
				CenterPos = Vec3(m_vClickedPosition.x - Scale.x / 2.0f, m_vClickedPosition.y + Scale.y / 2.0f, m_vClickedPosition.z);
				m_pClonedObject->Transform()->SetRelativeScale(Vec3(Scale.x, Scale.y, m_pClonedObject->Transform()->GetRelativeScale().z));
			}
			//우측 상단->좌측 하단
			else if (m_vClickedPosition.y > pos.y) {
				Scale = Vec2(abs(pos.x - m_vClickedPosition.x), abs(pos.y - m_vClickedPosition.y));
				CenterPos = Vec3(m_vClickedPosition.x - Scale.x / 2.0f, m_vClickedPosition.y - Scale.y / 2.0f, m_vClickedPosition.z);
			}
		}
		m_pClonedObject->Transform()->SetRelativeScale(Vec3(Scale.x, Scale.y, m_pClonedObject->Transform()->GetRelativeScale().z));
		m_pClonedObject->Transform()->SetRelativePos(CenterPos);
	}
	if (KEY_RELEASE(KEY::LBTN) && m_pClonedObject) {
		Vec3 pos = Vec3(m_vConvertedMousePos.x, m_vConvertedMousePos.y, m_vConvertedMousePos.z);
		Vec2 Scale = Vec2(0.f, 0.f);
		Vec3 CenterPos = Vec3(0.f, 0.f, 0.f);
		if (pos.x > m_vClickedPosition.x) {
			//좌측상단 -> 우측하단
			if (pos.y < m_vClickedPosition.y) {
				Scale = Vec2(abs(pos.x - m_vClickedPosition.x), abs(pos.y - m_vClickedPosition.y));
				CenterPos = Vec3(m_vClickedPosition.x + Scale.x / 2.0f, m_vClickedPosition.y - Scale.y / 2.0f, m_vClickedPosition.z);
				m_pClonedObject->Transform()->SetRelativeScale(Vec3(Scale.x, Scale.y, m_pClonedObject->Transform()->GetRelativeScale().z));
			}
			//좌측하단 ->우측상단
			else if (m_vClickedPosition.y < pos.y) {
				Scale = Vec2(abs(pos.x - m_vClickedPosition.x), abs(pos.y - m_vClickedPosition.y));
				CenterPos = Vec3(m_vClickedPosition.x + Scale.x / 2.0f, m_vClickedPosition.y + Scale.y / 2.0f, m_vClickedPosition.z);
				m_pClonedObject->Transform()->SetRelativeScale(Vec3(Scale.x, Scale.y, m_pClonedObject->Transform()->GetRelativeScale().z));
			}
		}
		else if (m_vClickedPosition.x > pos.x) {
			//우측 하단->좌측 상단
			if (m_vClickedPosition.y < pos.y) {
				Scale = Vec2(abs(pos.x - m_vClickedPosition.x), abs(pos.y - m_vClickedPosition.y));
				CenterPos = Vec3(m_vClickedPosition.x - Scale.x / 2.0f, m_vClickedPosition.y + Scale.y / 2.0f, m_vClickedPosition.z);
				m_pClonedObject->Transform()->SetRelativeScale(Vec3(Scale.x, Scale.y, m_pClonedObject->Transform()->GetRelativeScale().z));
			}
			//우측 상단->좌측 하단
			else if (m_vClickedPosition.y > pos.y) {
				Scale = Vec2(abs(pos.x - m_vClickedPosition.x), abs(pos.y - m_vClickedPosition.y));
				CenterPos = Vec3(m_vClickedPosition.x - Scale.x / 2.0f, m_vClickedPosition.y - Scale.y / 2.0f, m_vClickedPosition.z);
			}
		}
		m_pClonedObject->Transform()->SetRelativeScale(Vec3(Scale.x, Scale.y, m_pClonedObject->Transform()->GetRelativeScale().z));
		m_pClonedObject->Transform()->SetRelativePos(CenterPos);

		CGameObject* finalObject = m_pClonedObject->Clone();
		if (m_bIsTransformAbsolute) {
			finalObject->Transform()->SetAbsolute(true);
		}
		SpawnGameObject(finalObject, finalObject->Transform()->GetRelativePos(), m_iSpawnLayerIdx);
		DestroyObject(m_pClonedObject);
		m_pClonedObject = nullptr;
		m_vClickedPosition = Vec3(0.f, 0.f, 0.f);
	}

}

