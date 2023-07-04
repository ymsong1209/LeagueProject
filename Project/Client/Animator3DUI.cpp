#include "pch.h"
#include "Animator3DUI.h"
#include <Engine\CGameObject.h>
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

Animator3DUI::Animator3DUI()
	: ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR3D)
{
	SetName("Animator3D");
}

Animator3DUI::~Animator3DUI()
{
}

int Animator3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	ImGui::PushItemWidth(80.f);
	ImGui::Columns(2, "##mycolumns");
	ImGui::BeginGroup();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.401f, 0.701f, 1.0f));
	ImGui::SetNextItemWidth(200.f);
	ImGui::Text("Clip Animation Name   : ");
	ImGui::SameLine();
	wstring name = GetTarget()->Animator3D()->GetCurAnim3D()->GetVecClip()->at(0).strAnimName;
	char szBuff[50]; // 충분히 큰 크기의 배열을 선언합니다.
	wstringTostring(name, szBuff, 50);
	ImGui::InputText("##ClipAnimName", szBuff, 50);
	ImGui::PopStyleColor();


	ImGui::Text("Clip Animation Key    : ");
	ImGui::SameLine();
	wstring key = GetTarget()->Animator3D()->GetCurAnim3D()->GetClipName();
	wstringTostring(key, szBuff, 50);
	ImGui::InputText("##ClipAnimkey", szBuff, 50);

	ImGui::Text("Clip Time Length      : ");
	ImGui::SameLine();
	double TLength = GetTarget()->Animator3D()->GetCurAnim3D()->GetVecClip()->at(0).dTimeLength;
	ImGui::InputDouble("##TLength", &TLength, ImGuiInputTextFlags_ReadOnly);
	ImGui::EndGroup();

	ImGui::NextColumn();

	ImGui::BeginGroup();
	ImGui::Text("Clip Frame Length     : ");
	ImGui::SameLine();
	int FLength = GetTarget()->Animator3D()->GetCurAnim3D()->GetVecClip()->at(0).iFrameLength;
	ImGui::InputInt("##FLength", &FLength, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Clip Anim Play Time   : ");
	ImGui::SameLine();
	double ClipTime = GetTarget()->Animator3D()->GetCurAnim3D()->Get_ClipAnimCurTime();
	ImGui::InputDouble("##ClipTime", &ClipTime, ImGuiInputTextFlags_ReadOnly);
	ImGui::EndGroup();
	ImGui::Columns(1);

	ImGui::Separator();

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.f / 7.0f, 0.9f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.f / 7.0f, 0.9f, 0.4f));

	ImGui::Button("<< Animation Info >>", ImVec2(200.f, 20.f));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::PopItemWidth();

	ImGui::SetNextItemWidth(200.f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.401f, 0.701f, 1.0f));
	ImGui::Text(" Animation Name  : ");
	ImGui::SameLine();
	wstring Name = GetTarget()->Animator3D()->GetCurAnim3D()->GetName();
	wstringTostring(Name, szBuff, 50);
	ImGui::InputText("##AnimName", szBuff, 50);
	ImGui::PopStyleColor();


	ImGui::PushItemWidth(80.f);

	ImGui::Columns(2, "##mycolumns2");
	ImGui::BeginGroup();

	ImGui::Text("Anim Time Length       : ");
	ImGui::SameLine();
	int ATLength = GetTarget()->Animator3D()->GetCurAnim3D()->GetTimeLength();
	ImGui::InputInt("##AFLength", &ATLength, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Anim Start Time        : ");
	ImGui::SameLine();
	double StartTime = GetTarget()->Animator3D()->GetCurAnim3D()->GetStartTime();
	ImGui::InputDouble("##StartTime", &StartTime, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Anim End Time          : ");
	ImGui::SameLine();
	double EndTime = GetTarget()->Animator3D()->GetCurAnim3D()->GetEndTime();
	ImGui::InputDouble("##EndTime", &EndTime, ImGuiInputTextFlags_ReadOnly);


	ImGui::Text("Anim Play Time         : ");
	ImGui::SameLine();
	double AnimTime = GetTarget()->Animator3D()->GetCurAnim3D()->GetAnimPlayTime();
	ImGui::InputDouble("##AnimTime", &AnimTime, ImGuiInputTextFlags_ReadOnly);


	ImGui::EndGroup();
	ImGui::NextColumn();


	ImGui::BeginGroup();
	ImGui::Text("Anim Frame Length      : ");
	ImGui::SameLine();
	int AFLength = GetTarget()->Animator3D()->GetCurAnim3D()->GetFrameLength();
	ImGui::InputInt("##ATLength", &AFLength, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Anim Start Frame       : ");
	ImGui::SameLine();
	int StartFrame = GetTarget()->Animator3D()->GetCurAnim3D()->GetStartFrame();
	ImGui::InputInt("##StartFrame", &StartFrame, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Anim End Frame         : ");
	ImGui::SameLine();
	int EndFrame = GetTarget()->Animator3D()->GetCurAnim3D()->GetEndFrame();
	ImGui::InputInt("##EndFrame", &EndFrame, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Animation Frame Idx    : ");
	ImGui::SameLine();
	int FrameIdx = GetTarget()->Animator3D()->GetCurAnim3D()->GetFrameIdx();
	ImGui::InputInt("##FrameIdx", &FrameIdx, ImGuiInputTextFlags_ReadOnly);

	ImGui::EndGroup();
	ImGui::Columns(1);


	ImGui::Separator();
	//---------------
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.5f, 0.5f, 1.0f));
	ImGui::Text("Use MotionBlend");
	bool UseBlend = GetTarget()->Animator3D()->IsUseMotionBlend();
	ImGui::SameLine();
	if (ImGui::Checkbox("##Checkbox label", &UseBlend))
	{
		// Checkbox was clicked, checkbox_state will be updated.
		if (UseBlend)
			GetTarget()->Animator3D()->SetUseMotionBlend(true);
		else
			GetTarget()->Animator3D()->SetUseMotionBlend(false);
	}
	ImGui::SameLine();
	ImGui::Text("Anim Start - End Blending");
	bool UseStartEndBlend = GetTarget()->Animator3D()->GetCurAnim3D()->GetStartEndLerp();
	ImGui::SameLine();
	if (ImGui::Checkbox("##Start-End", &UseStartEndBlend))
	{
		if (UseStartEndBlend)
			GetTarget()->Animator3D()->GetCurAnim3D()->SetStartEndLerp(true);
		else
			GetTarget()->Animator3D()->GetCurAnim3D()->SetStartEndLerp(false);
	}

	ImGui::PopItemWidth();
	ImGui::PopStyleColor();

	ImGui::Separator();


	ImGui::Text("Blending Speed : ");
	ImGui::SameLine();
	float BlendTime = GetTarget()->Animator3D()->GetBlendTime();
	ImGui::SetNextItemWidth(90.f);
	ImGui::DragFloat("##BlendTime", &BlendTime, 0.03f);
	if (BlendTime <= 5.f)
		GetTarget()->Animator3D()->SetBlendTime(BlendTime);


	ImGui::Text("Anim Speed     : ");
	float Speed = GetTarget()->Animator3D()->GetCurAnim3D()->GetAnimSpeed();
	ImGui::SameLine();
	ImGui::SetNextItemWidth(90.f);
	ImGui::DragFloat("##AnimSpeed", &Speed, 0.02f, 0.0f, 5.f);
	GetTarget()->Animator3D()->GetCurAnim3D()->SetAnimSpeed(Speed);




	return TRUE;
}

void Animator3DUI::wstringTostring(wstring _wstring, char* _Buff, size_t _BufferSize)
{
	memset(_Buff, 0, sizeof(char) * _BufferSize);

	wstring wstrKey = _wstring;
	string	strKey = string(wstrKey.begin(), wstrKey.end());

	memcpy(_Buff, strKey.data(), sizeof(char) * strKey.length());
}