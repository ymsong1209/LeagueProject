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
	//  절두체 컬링 UI
	// =======================
	CRenderComponent* pRenderCom = m_pTarget->GetRenderComponent();

	ImGui::Text("RaySight Culling");
	ImGui::SameLine();
	bool useRaySightCulling = pRenderCom->IsUsingRaySightCulling();
	if (ImGui::Checkbox("##RaySightCullingCheck", &useRaySightCulling))
	{
		if (useRaySightCulling) { // 체크된 경우
			pRenderCom->SetRaySightCulling(true);

		}
		else { // 체크 해제 된 경우
			pRenderCom->SetRaySightCulling(false);
		}
	}

	ImGui::Text("Frustum Culling");
	ImGui::SameLine();
	bool useFrustum = pRenderCom->IsUseFrustumCheck();
	if (ImGui::Checkbox("##FrustumCheck", &useFrustum))
	{
		if (useFrustum) { // 체크된 경우
			pRenderCom->SetFrustumCheck(true);

		}
		else { // 체크 해제 된 경우
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
			if (bShowDebugBound) { // 체크된 경우
				pRenderCom->SetShowDebugBound(true);
				
			}
			else { // 체크 해제 된 경우
				pRenderCom->SetShowDebugBound(false);
			}
		}

		// Bounding Scale
		ImGui::Text("Bounding Scale");
		ImGui::SameLine();
		float bounding = pRenderCom->GetBounding();
		ImGui::DragFloat("##BoundingScale", &bounding, 5.f);

		pRenderCom->SetBounding(bounding);

		// Bounding Pos를 기본 transform으로 쓸건지, 아니면 OffsetPos로 줄 것인지
		ImGui::Text("Set Bound Offset");
		ImGui::SameLine();
		bool bShowBoundOffset = pRenderCom->GetBoundingBoxOffsetUse();
		if (ImGui::Checkbox("##SetBoundOffsetRenderComUI", &bShowBoundOffset))
		{
			pRenderCom->SetBoundingBoxOffsetUse(bShowBoundOffset);
		}

		//Bounding Pos를 OffsetPos로 설정하겠다.
		if (bShowBoundOffset) {
			// Bounding Scale
			ImGui::Text("Bound Offset");
			ImGui::SameLine();
			Vec3 boundoffset = pRenderCom->GetBoundingBoxOffset();
			if (ImGui::DragFloat3("##BoundingOffsetRenderComUI", boundoffset, 10.f)) {
				pRenderCom->SetBoundingBoxOffset(boundoffset);
			};
		}

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