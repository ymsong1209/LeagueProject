#include "pch.h"
#include "CJinxWalkState.h"
#include <Engine/CAnimator3D.h>
#include <Engine\CAnim3D.h>

#include "CSendServerEventMgr.h"
#include "CUnitScript.h"


CJinxWalkState::CJinxWalkState()
{
	SetName(L"Run");
}

CJinxWalkState::~CJinxWalkState()
{
}

void CJinxWalkState::tick()
{
	// SFX / Voice 재생
}

void CJinxWalkState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Jinx\\Run_Base", true, 0.1f);

	// server에 보낼 애니메이션 패킷 이벤트 등록
	AnimInfo* animInfo = new AnimInfo();
	animInfo->animIdx = GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetServerID();
	animInfo->animName = L"Jinx\\Run_Base";
	animInfo->blend = true;
	animInfo->blendTime = 0.1f;
	
	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::SEND_ANIM_PACKET;
	//evn.wParam = (DWORD_PTR);
	evn.lParam = (DWORD_PTR)animInfo;
	CSendServerEventMgr::GetInst()->AddServerSendEvent(evn);

	CChampionWalkState::Enter();
}

void CJinxWalkState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Jinx\\Run_Base")->Reset();
	CChampionWalkState::Exit();
}

void CJinxWalkState::HandleEvent(CGameEvent& event)
{
	CChampionWalkState::HandleEvent(event);
}
