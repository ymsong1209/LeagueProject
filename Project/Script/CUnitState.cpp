#include "pch.h"
#include "CUnitState.h"
#include <Engine\CFsm.h>
#include "CUnitScript.h"
#include "CSendServerEventMgr.h"

CUnitState::CUnitState()
	: m_Listener(new CUnitScriptEventListener(this))
{
	CGameEventMgr::GetInst()->AddListener(m_Listener);
}

CUnitState::~CUnitState()
{
}

void CUnitState::tick()
{
}

void CUnitState::Enter()
{
	CState::Enter();
}

void CUnitState::Exit()
{
	CState::Exit();
}

void CUnitState::SendAnimPacket(wstring _animName, bool _repeat, bool _blend, float _blentTime)
{
	AnimInfo* animInfo = new AnimInfo();
	animInfo->animName = _animName;
	animInfo->targetId = GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetServerID();
	animInfo->bRepeat = _repeat;
	animInfo->blend = _blend;
	animInfo->blendTime = _blentTime;

	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::SEND_ANIM_PACKET;
	//evn.wParam = (DWORD_PTR);
	evn.lParam = (DWORD_PTR) animInfo;
	CSendServerEventMgr::GetInst()->AddServerSendEvent(evn);
}
