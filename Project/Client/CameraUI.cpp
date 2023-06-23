#include "pch.h"
#include "CameraUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CCamera.h>

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

	// Show Debug
	bool ShowDebug = GetTarget()->Camera()->GetShowDebug();
	ImGui::Text("ShowDebug");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ShowDebug", &ShowDebug)) {
		GetTarget()->Camera()->SetShowDebug(ShowDebug);
	}

	// Set Far
    float fFar = GetTarget()->Camera()->GetFar();
    ImGui::Text("Far");
    ImGui::SameLine();
	if (ImGui::DragFloat("##CameraFar", &fFar, 10, 2, 50000)) {
		if (fFar <= 2.0f) {
			fFar = 2.0f;
		}
		GetTarget()->Camera()->SetFar(fFar);
	}
   
    return TRUE;
}
