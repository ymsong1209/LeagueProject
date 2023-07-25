#include "pch.h"
#include "Collider3DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CCollider3D.h>


Collider3DUI::Collider3DUI()
	: ComponentUI("##Collider3D", COMPONENT_TYPE::COLLIDER3D)
{
	SetName("Collider3D");
}

Collider3DUI::~Collider3DUI()
{
}

int Collider3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	ChooseColliderType();


	ImGuiInputTextFlags textflag = 0;

	Vec3					vOffsetPos = GetTarget()->Collider3D()->GetOffsetPos();
	Vec3					vOffsetScale = GetTarget()->Collider3D()->GetOffsetScale();
	bool					bAbsolute = GetTarget()->Collider3D()->GetColliderAbsolute();
	bool					bShowDebug = GetTarget()->Collider3D()->GetDrawCollision();

	int						iCollisionCount = GetTarget()->Collider3D()->GetCollisionCount();  // Ãæµ¹ È½¼ö


	ImGui::Text("ShowDebug");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ShowDebugCollider3D", &bShowDebug)) {
		GetTarget()->Collider3D()->SetDrawCollision(bShowDebug);
	}

	ImGui::Text("IsAbsolute");
	ImGui::SameLine();
	if (ImGui::Checkbox("##Collider3DAbsolute", &bAbsolute)) {
		GetTarget()->Collider3D()->SetAbsolute(bAbsolute);
	}

	SetWall();

	ImGui::Text("OffsetPos");
	ImGui::SameLine();
	if (ImGui::InputFloat3("##Collider3DOffsetPos", vOffsetPos)) {
		Vec3 pos = Vec3(vOffsetPos.x, vOffsetPos.y, vOffsetPos.z);
		GetTarget()->Collider3D()->SetOffsetPos(pos);
	}

	ImGui::Text("OffsetScale");
	ImGui::SameLine();
	if (GetTarget()->Collider3D()->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE) {
		if (ImGui::InputFloat("##Collider3DOffsetSPHEREScale", vOffsetScale)) {
			Vec3 scale = Vec3(vOffsetScale.x, vOffsetScale.x, vOffsetScale.x);
			GetTarget()->Collider3D()->SetOffsetScale(scale);
		}
	}
	else if (GetTarget()->Collider3D()->GetCollider3DType() == COLLIDER3D_TYPE::CUBE) {
		if (ImGui::InputFloat3("##Collider3DOffsetCUBEScale", vOffsetScale)) {
			Vec3 scale = Vec3(vOffsetScale.x, vOffsetScale.y, vOffsetScale.z);
			GetTarget()->Collider3D()->SetOffsetScale(scale);
		}
	}


	ImGui::Text("CollisionCount : %d", iCollisionCount);

	return TRUE;
}

void Collider3DUI::SetWall()
{
	bool iswall = GetTarget()->Collider3D()->IsWall();
	ImGui::Text("IsWall : "); ImGui::SameLine();
	if (ImGui::Checkbox("##IsWallCollider3D", &iswall)) {
		GetTarget()->Collider3D()->SetIsWall(iswall);
	}
}

void Collider3DUI::ChooseColliderType()
{
	const char* ColliderType[] = { "SPHERE", "CUBE" };

	COLLIDER3D_TYPE CurType = GetTarget()->Collider3D()->GetCollider3DType();

	static int ColliderInfo_idx = 0;
	ColliderInfo_idx = (UINT)CurType;
	const char* colliderinfo_preview_value = ColliderType[ColliderInfo_idx];
	static ImGuiComboFlags flags = 0;

	if (ImGui::BeginCombo("ColliderType##Collider3D", colliderinfo_preview_value, flags)) {
		for (int i = 0; i < IM_ARRAYSIZE(ColliderType); i++) {
			const bool is_selected = (ColliderInfo_idx == i);
			if (ImGui::Selectable(ColliderType[i], is_selected))
				ColliderInfo_idx = i;


			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				GetTarget()->Collider3D()->SetCollider3DType(static_cast<COLLIDER3D_TYPE>(i));
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}