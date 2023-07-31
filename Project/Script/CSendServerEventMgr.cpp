#include "pch.h"
#include "CSendServerEventMgr.h"

CSendServerEventMgr::CSendServerEventMgr()
{
}

CSendServerEventMgr::~CSendServerEventMgr()
{
}


void CSendServerEventMgr::SendHitPacket(UINT64 _hitObjId, UINT64 _useObjId, int _skillLevel, SkillType _skillType)
{
	HitInfo* hitInfo = new HitInfo();
	hitInfo->hitObjId = _hitObjId;
	hitInfo->useObjId = _useObjId;
	hitInfo->SkillLevel = _skillLevel;
	hitInfo->skillType = _skillType;

	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::SEND_HIT_PACKET;
	evn.wParam = (DWORD_PTR)hitInfo;
	//evn.lParam = (DWORD_PTR)
	CSendServerEventMgr::GetInst()->AddServerSendEvent(evn);
}
