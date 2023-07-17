#include "pch.h"
#include "RenderComponentUI.h"

#include <Engine\CGameObject.h>
#include <Engine/CRenderComponent.h>

RenderComponentUI::RenderComponentUI()
	:UI("RenderComponent")
{
	SetName("RenderComponent");
}

RenderComponentUI::~RenderComponentUI()
{
}

int RenderComponentUI::render_update()
{
	if (nullptr == m_pTarget || nullptr == m_pTarget->GetRenderComponent())
		return FALSE;

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.173, 0.227, 0.192, 1.0);
	//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.173, 0.227, 0.192, 1.0);
	//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.173, 0.227, 0.192, 1.0);
	ImGui::Button(GetName().c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();


	// =======================
	//  例滴眉 拿傅 UI
	// =======================
	CRenderComponent* pRenderCom = m_pTarget->GetRenderComponent();

	ImGui::Text("Frustum Culling");
	ImGui::SameLine();
	bool useFrustum = pRenderCom->IsUseFrustumCheck();
	if (ImGui::Checkbox("##FrustumCheck", &useFrustum))
	{
		if (useFrustum) { // 眉农等 版快
			pRenderCom->SetFrustumCheck(true);

		}
		else { // 眉农 秦力 等 版快
			pRenderCom->SetFrustumCheck(false);
		}
	}

	if (useFrustum)
	{
		// Show Debug bound sphere
		ImGui::Text("Show Debug Bound");
		ImGui::SameLine(); 
		bool bShowDebugBound = pRenderCom->IsShowDebugBound();
		if (ImGui::Checkbox("##showDebugBound", &bShowDebugBound))
		{
			if (bShowDebugBound) { // 眉农等 版快
				pRenderCom->SetShowDebugBound(true);
				
			}
			else { // 眉农 秦力 等 版快
				pRenderCom->SetShowDebugBound(false);
			}
		}

		// Bounding Scale
		ImGui::Text("Bounding Scale");
		ImGui::SameLine();
		float bounding = pRenderCom->GetBounding();
		ImGui::DragFloat("##BoundingScale", &bounding, 5.f);

		pRenderCom->SetBounding(bounding);
	}

	//Dynamic Shadow check
	ImGui::Text("Dynamic Shdaow Use : ");
	ImGui::SameLine();
	bool DynamicShadow = pRenderCom->IsDynamicShadow();
	if (ImGui::Checkbox("##DynaicShadowRenderComUI", &DynamicShadow)) {
		pRenderCom->SetDynamicShadow(DynamicShadow);
	}

    return 0;
}


void RenderComponentUI::SetTarget(CGameObject* _target)
{
	m_pTarget = _target;

	if (nullptr == m_pTarget)
	{
		SetActive(false);
		return;
	}

	if (nullptr == m_pTarget->GetRenderComponent())
	{
		SetActive(false);
	}
	else
	{
		SetActive(true);
	}
}