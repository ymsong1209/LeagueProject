#include "pch.h"
#include "FsmUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CFsm.h>
#include <Engine\CState.h>

FsmUI::FsmUI()
	: ComponentUI("##Fsm", COMPONENT_TYPE::FSM)
{
	SetName("Fsm");
}

FsmUI::~FsmUI()
{
}

void FsmUI::init()
{
}

int FsmUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	// 현재 상태 출력

	CState* pCurState = GetTarget()->Fsm()->GetCurState();
	char szBuff[50] = {};
	if (pCurState != nullptr)
	{
		memset(szBuff, 0, sizeof(char) * 50);
		string StateName = string(pCurState->GetName().begin(), pCurState->GetName().end());
		memcpy(szBuff, StateName.data(), sizeof(char) * StateName.length());
	}

	ImGui::Text("Current State : ");
	ImGui::SameLine();
	ImGui::InputText("##StateName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

	return 0;
}
