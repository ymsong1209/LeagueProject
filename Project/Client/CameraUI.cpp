#include "pch.h"
#include "CameraUI.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

CameraUI::CameraUI()
    : ComponentUI("##Camera", COMPONENT_TYPE::CAMERA)
{
    SetName("Camera");
}

CameraUI::~CameraUI()
{
}

int CameraUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;
	//Camera Index-----------//
	ImGui::Text("Camera Index : ");
	ImGui::SameLine();
	ImGui::Text("%d", m_iCameraIdx);

	ImGui::Text("Set Camera Index");
	ImGui::SameLine();
	int CameraIdx = GetTarget()->Camera()->GetCameraIndex();
	ImGui::SetNextItemWidth(200.f);
	if (ImGui::InputInt("##CameraIndex", &m_iCameraIdx)) 
	{
	}

	////SetMainCam--------//
	ImGui::SameLine();
	if (ImGui::Button("SetCamIdx")) {
		GetTarget()->Camera()->SetCameraIndex(m_iCameraIdx);
	}

	//ShowDebug---------//
	ImGui::Text("ShowDebug");
	ImGui::SameLine();
	bool ShowDebug = GetTarget()->Camera()->GetShowDebug();
	if (ImGui::Checkbox("##ShowDebugCameraUI", &ShowDebug)) {

		GetTarget()->Camera()->SetShowDebug(ShowDebug);
	}

	//SetFar--------//
	ImGui::Text("Fov");
	ImGui::SameLine();
	float CamFov = GetTarget()->Camera()->GetCameraFov();
	if (ImGui::DragFloat("##Fov", &CamFov)) {
		if (CamFov >= 360.f) {
			CamFov = 360.f;
		}
		if (CamFov <= 2.f)
			CamFov = 2.f;
		GetTarget()->Camera()->SetCameraFov(CamFov);
	}


	//SetFar--------//
	ImGui::Text("Far");
	ImGui::SameLine();
	float CamFar = GetTarget()->Camera()->GetFar();
	if (ImGui::DragFloat("##CameraFarCameraUI", &CamFar)) {
		if (CamFar <= 2.0f) {
			CamFar = 2.0f;
		}
		GetTarget()->Camera()->SetFar(CamFar);
	}

	//SetScale--------//
	ImGui::Text("SetScale");
	ImGui::SameLine();
	float CamScale = GetTarget()->Camera()->GetScale();
	if (ImGui::DragFloat("##SetScale", &CamScale)) {
		GetTarget()->Camera()->SetScale(CamScale);
	}

	//Proj Type------------//
	ImGui::Text("Proj Type : ");
	ImGui::SameLine();
	string projtype;
	if (GetTarget()->Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC) {
		projtype = "OrthoGraphic";
	}
	else if (GetTarget()->Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE) {
		projtype = "Perspective";
	}
	if (ImGui::Button(projtype.c_str())) {
		if (GetTarget()->Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC) {
			GetTarget()->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		}
		else if (GetTarget()->Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE) {
			GetTarget()->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		}
	}

	// Orthogonal일 경우 Scale정할 수 있도록 함
	if (GetTarget()->Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
	{
		float Scale = GetTarget()->Camera()->GetScale();
		ImGui::Text("Camera Scale");
		ImGui::InputFloat("##OrthogonalScale", &Scale);
		GetTarget()->Camera()->SetScale(Scale);
	}

	//LayerMask---------------//
	UINT layermask = GetTarget()->Camera()->GetLayerMask();
	ImGui::Text("SetLayerMaskAll");
	ImGui::SameLine();
	if (ImGui::Button("True##Setlayermaskall")) {
		GetTarget()->Camera()->SetLayerMaskAll(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("False##Setlayermaskall")) {
		GetTarget()->Camera()->SetLayerMaskAll(false);
	}

	ImGui::Text("Visible List");
	ImGui::Text("SetLayerMask : ");
	ImGui::SameLine();
	if (ImGui::InputInt("##SetLayerMask", &m_iCameraMask)) {
		if (m_iCameraMask < MAX_LAYER && 0 <= m_iCameraMask) {

		}
		else {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Max Layer를 초과한 값이 들어왔습니다.");
			MessageBox(nullptr, szStr, L"LayerMask 설정 실패.", MB_OK);
			m_iCameraMask = 0;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("True##SetLayerMask")) {

		// 레이어 마스크 확인
		if (layermask & (1 << m_iCameraMask))
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"현재 레이어가 이미 True상태입니다.");
			MessageBox(nullptr, szStr, L"LayerMask 설정 실패.", MB_OK);
		}
		else {
			GetTarget()->Camera()->SetLayerMask(m_iCameraMask, true);
		}

	}
	ImGui::SameLine();
	if (ImGui::Button("False##SetLayerMask")) {
		if (!(layermask & (1 << m_iCameraMask))) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"현재 레이어가 이미 False상태입니다.");
			MessageBox(nullptr, szStr, L"LayerMask 설정 실패.", MB_OK);
		}
		else {
			GetTarget()->Camera()->SetLayerMask(m_iCameraMask, false);
		}
	}

	int scale = 0;
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{

		// 레이어 마스크 확인
		if (layermask & (1 << i))
		{
			CLevel* Curlevel = CLevelMgr::GetInst()->GetCurLevel();
			CLayer* layer = Curlevel->GetLayer(i);
			string layername = string(layer->GetName().begin(), layer->GetName().end());
			if (layername.empty()) {
				string totalname = std::to_string(i) + " Layer : Empty Layer";
				ImGui::Text(totalname.c_str());
			}
			else {
				string totalname = std::to_string(i) + " Layer : " + layername;
				ImGui::Text(totalname.c_str());
			}

			++scale;
		}

	}

	

	SetSize(0.f, ImGui::GetTextLineHeight() * (scale + 10));
   
    return TRUE;
}
