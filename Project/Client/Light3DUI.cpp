#include "pch.h"
#include "Light3DUI.h"
#include <Engine\CGameObject.h>
#include <Engine/CLight3D.h>




Light3DUI::Light3DUI()
	: ComponentUI("##Light3D", COMPONENT_TYPE::LIGHT3D)
{
	SetName("Light3D");
}

Light3DUI::~Light3DUI()
{
}


int Light3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	tLightInfo lightinfo = GetTarget()->Light3D()->GetLightInfo();

	UINT		LightType = lightinfo.LightType; // 빛의 타입(방향성, 점, 스포트)
	float		Radius = lightinfo.Radius;		 // 빛의 반경(사거리)
	float		Angle = GetTarget()->Light3D()->GetAngle();		 // 빛의 각도
	float		InnerAngle = GetTarget()->Light3D()->GetInnerAngle();

	Vec4		vDiffuse = lightinfo.Color.vDiffuse;	// 빛의 색상
	Vec4		vAmbient = lightinfo.Color.vAmbient;	// 주변 광(환경 광), 최소한 존재할 수 있는 빛
	Vec4		vSpecular = lightinfo.Color.vSpecular;  // 반사광

	ChooseLightType();
	ShowDebug();

	ImGuiInputTextFlags textflag = 0;

	ImGui::Text("Radius");
	ImGui::SameLine();
	if (ImGui::SliderFloat("##3DLightSlider", &Radius, 0, 10000.f)) {
		GetTarget()->Light3D()->SetRadius(Radius);
	}

	if (ImGui::InputFloat("##3DLightRadius", &Radius)) {
		GetTarget()->Light3D()->SetRadius(Radius);
	}

	if (LightType == (UINT)LIGHT_TYPE::SPOT)
	{
		ImGui::Text("Angle");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##3DLightAngle", &Angle, 0.f, 90.f)) {
			GetTarget()->Light3D()->SetAngle(Angle);
			if (InnerAngle > Angle) {
				GetTarget()->Light3D()->SetInnerAngle(Angle);
			}
		}

		ImGui::Text("InnerAngle");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##3DLightInnerAngle", &InnerAngle, 0.f, 90.f)) {
			if (InnerAngle > Angle) InnerAngle = Angle;
			GetTarget()->Light3D()->SetInnerAngle(InnerAngle);
		}

	}

	static bool alpha_preview = false;
	static bool alpha_half_preview = true;
	static bool options_menu = true;

	ImGuiColorEditFlags misc_flags = (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

	ImGui::Text("Diffuse");
	ImGui::SameLine();

	if (ImGui::ColorEdit4("##3DLightDiffuse", (float*)&vDiffuse, ImGuiColorEditFlags_Float | misc_flags)) {
		GetTarget()->Light3D()->SetLightDiffuse(vDiffuse);
	}


	ImGui::Text("Ambient");
	ImGui::SameLine();
	if (ImGui::ColorEdit4("##3DLightAmbient", (float*)&vAmbient, ImGuiColorEditFlags_Float | misc_flags)) {
		GetTarget()->Light3D()->SetLightAmbient(vAmbient);
	}

	ImGui::Text("Specular");
	ImGui::SameLine();
	if (ImGui::ColorEdit4("##3DLightSpecular", (float*)&vSpecular, ImGuiColorEditFlags_Float | misc_flags)) {
		GetTarget()->Light3D()->SetLightSpecular(vSpecular);
	}

	return TRUE;
}

void Light3DUI::ChooseLightType()
{

	const char* LightType[] = { "DIRECTIONAL", "POINT", "SPOT" };

	tLightInfo lightinfo = GetTarget()->Light3D()->GetLightInfo();

	UINT lightinfo_idx = lightinfo.LightType;
	const char* lightinfo_preview_value = LightType[lightinfo_idx];
	static ImGuiComboFlags flags = 0;

	if (ImGui::BeginCombo("LightType", lightinfo_preview_value, flags)) {
		for (int i = 0; i < IM_ARRAYSIZE(LightType); i++) {
			const bool is_selected = (lightinfo_idx == i);
			if (ImGui::Selectable(LightType[i], is_selected))
				lightinfo_idx = i;


			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				GetTarget()->Light3D()->SetLightType(static_cast<LIGHT_TYPE>(i));
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

}

void Light3DUI::ShowDebug()
{
	bool Show = GetTarget()->Light3D()->GetShowDebug();
	ImGui::Text("ShowDebug");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ShowDebugLight3D", &Show)) {
		GetTarget()->Light3D()->SetShowDebug(Show);
	}
}
