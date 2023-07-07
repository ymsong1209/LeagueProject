#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CCollider2D.h>


Collider2DUI::Collider2DUI()
	: ComponentUI("##Coillider2D", COMPONENT_TYPE::COLLIDER2D)
{
	SetName("Coillider2D");
}

Collider2DUI::~Collider2DUI()
{
}

int Collider2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	ChooseColliderType();


	ImGuiInputTextFlags textflag = 0;

	Vec3					vOffsetPos = GetTarget()->Collider2D()->GetOffsetPos();
	Vec3					vOffsetScale = GetTarget()->Collider2D()->GetOffsetScale();
	bool					bAbsolute = GetTarget()->Collider2D()->GetColliderAbsolute();

	int						iCollisionCount = GetTarget()->Collider2D()->GetCollisionCount();  // Ãæµ¹ È½¼ö

	ImGui::Text("IsAbsolute");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ColliderAbsolute", &bAbsolute)) {
		GetTarget()->Collider2D()->SetAbsolute(bAbsolute);
	}

	ImGui::Text("OffsetPos");
	ImGui::SameLine();
	if (ImGui::InputFloat3("##ColliderOffsetPos", vOffsetPos)) {
		Vec2 pos = Vec2(vOffsetPos.x, vOffsetPos.y);
		GetTarget()->Collider2D()->SetOffsetPos(pos);
	}

	ImGui::Text("OffsetScale");
	ImGui::SameLine();
	if (ImGui::InputFloat3("##ColliderOffsetScale", vOffsetScale)) {
		Vec2 scale = Vec2(vOffsetScale.x, vOffsetScale.y);
		GetTarget()->Collider2D()->SetOffsetScale(scale);
	}

	ImGui::Text("CollisionCount : %d", iCollisionCount);

	return TRUE;
}

void Collider2DUI::ChooseColliderType()
{
	const char* ColliderType[] = { "RECT", "CIRCLE" };

	COLLIDER2D_TYPE CurType = GetTarget()->Collider2D()->GetCollider2DType();

	static int ColliderInfo_idx = 0;
	ColliderInfo_idx = (UINT)CurType;
	const char* colliderinfo_preview_value = ColliderType[ColliderInfo_idx];
	static ImGuiComboFlags flags = 0;

	if (ImGui::BeginCombo("ColliderType", colliderinfo_preview_value, flags)) {
		for (int i = 0; i < IM_ARRAYSIZE(ColliderType); i++) {
			const bool is_selected = (ColliderInfo_idx == i);
			if (ImGui::Selectable(ColliderType[i], is_selected))
				ColliderInfo_idx = i;


			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				GetTarget()->Collider2D()->SetCollider2DType(static_cast<COLLIDER2D_TYPE>(i));
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}