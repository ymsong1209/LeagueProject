#include "pch.h"
#include "Light2DUI.h"

#include <Engine\CGameObject.h>
#include <Engine/CLight2D.h>




Light2DUI::Light2DUI()
	: ComponentUI("##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetName("Light2D");
}

Light2DUI::~Light2DUI()
{
}


int Light2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	tLightInfo lightinfo = GetTarget()->Light2D()->GetLightInfo();

	UINT		LightType = lightinfo.LightType;   // ���� Ÿ��(���⼺, ��, ����Ʈ)
	float		Radius = lightinfo.Radius;		 // ���� �ݰ�(��Ÿ�)
	float		Angle = GetTarget()->Light2D()->GetAngle();		 // ���� ����

	Vec4		vDiffuse = lightinfo.Color.vDiffuse;	// ���� ����
	Vec4		vAmbient = lightinfo.Color.vAmbient;	// �ֺ� ��(ȯ�� ��), �ּ��� ������ �� �ִ� ��

	ChooseLightType();

	ImGuiInputTextFlags textflag = 0;

	ImGui::Text("Radius");
	ImGui::SameLine();
	if (ImGui::InputFloat("##LightRadius", &Radius)) {
		GetTarget()->Light2D()->SetRadius(Radius);
	}

	if (LightType == (UINT)LIGHT_TYPE::SPOT)
	{
		ImGui::Text("Angle");
		ImGui::SameLine();
		if (ImGui::InputFloat("##LightAngle", &Angle)) {
			GetTarget()->Light2D()->SetAngle(Angle);
		}
	}


	ImGui::Text("Diffuse");
	ImGui::SameLine();
	if (ImGui::InputFloat4("##LightDiffuse", vDiffuse)) {
		GetTarget()->Light2D()->SetLightDiffuse(vDiffuse);
	}

	ImGui::Text("Ambient");
	ImGui::SameLine();
	if (ImGui::InputFloat4("##LightAmbient", vAmbient)) {
		GetTarget()->Light2D()->SetLightDiffuse(vAmbient);
	}

	return TRUE;
}

void Light2DUI::ChooseLightType()
{

	const char* LightType[] = { "DIRECTIONAL", "POINT", "SPOT" };

	tLightInfo lightinfo = GetTarget()->Light2D()->GetLightInfo();

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
				GetTarget()->Light2D()->SetLightType(static_cast<LIGHT_TYPE>(i));
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

}