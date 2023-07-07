#include "pch.h"
#include "ScriptUI.h"

#include <Script\CScriptMgr.h>

#include "ParamUI.h"

ScriptUI::ScriptUI()
    : ComponentUI("##Script", COMPONENT_TYPE::SCRIPT)
	, m_pTargetScript(nullptr)
{

}

ScriptUI::~ScriptUI()
{

}

int ScriptUI::render_update()
{
	if (nullptr == GetTarget())
		return 0;

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));

	ImGui::Button(m_strScriptName.c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::NewLine();

	// Script 에서 요구하는 ScalarParameter 를 UI 에 노출	
	const vector<tScriptParam>& vecScriptParam = m_pTargetScript->GetScritpParam();
	/*if (vecScriptParam.empty())
		return 1;*/
	
	ImGui::Text("Parameter");

	for (size_t i = 0; i < vecScriptParam.size(); ++i)
	{
		switch (vecScriptParam[i].eParam)
		{
		case SCRIPT_PARAM::INT:
			ParamUI::Param_Int(vecScriptParam[i].strDesc, (int*)vecScriptParam[i].pData);
			break;
		case SCRIPT_PARAM::FLOAT:
			ParamUI::Param_Float(vecScriptParam[i].strDesc, (float*)vecScriptParam[i].pData);
			break;
		case SCRIPT_PARAM::VEC2:
			ParamUI::Param_Vec2(vecScriptParam[i].strDesc, (Vec2*)vecScriptParam[i].pData);
			break;
		case SCRIPT_PARAM::VEC4:
			ParamUI::Param_Vec4(vecScriptParam[i].strDesc, (Vec4*)vecScriptParam[i].pData);
			break;
		default:
			break;
		}
	}
	
	vector<tScriptTexParam>& vecScriptTexParam = m_pTargetScript->GetScriptTexParam();
	for (size_t i = 0; i < vecScriptTexParam.size(); ++i) {
		Ptr<CTexture> pCurTex = vecScriptTexParam[i].tex;
		m_strTexDesc = vecScriptTexParam[i].strDesc;
		ParamUI::Param_Tex(vecScriptTexParam[i].strDesc, pCurTex, this, (UI_DELEGATE_1)&ScriptUI::SelectTexture);
		m_pTargetScript->SetScriptTexParam(pCurTex, vecScriptTexParam[i].strDesc);
	}

    return 1;
}

void ScriptUI::SetScript(CScript* _Script)
{
	assert(_Script);
	m_pTargetScript = _Script;
	wstring wstr = CScriptMgr::GetScriptName(m_pTargetScript);
	m_strScriptName = string(wstr.begin(), wstr.end());
}

void ScriptUI::SelectTexture(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

	m_pTargetScript->SetScriptTexParam(pTex, m_strTexDesc);
}