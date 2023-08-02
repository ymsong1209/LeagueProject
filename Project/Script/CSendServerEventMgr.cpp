#include "pch.h"
#include "CSendServerEventMgr.h"

CSendServerEventMgr::CSendServerEventMgr()
{
}

CSendServerEventMgr::~CSendServerEventMgr()
{
}


void CSendServerEventMgr::SendHitPacket(UINT64 _skillObjId, UINT64 _hitObjId, UINT64 _useObjId, int _skillLevel, SkillType _skillType)
{
	HitInfo* hitInfo = new HitInfo();
	hitInfo->skillObjId = _skillObjId;
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

void CSendServerEventMgr::SendAnimPacket(UINT64 _targetId, wstring _animName, bool _repeat, bool _blend, float _blentTime)
{
	AnimInfo* animInfo = new AnimInfo();
	animInfo->animName = _animName;
	animInfo->targetId = _targetId;
	animInfo->bRepeat = _repeat;
	animInfo->blend = _blend;
	animInfo->blendTime = _blentTime;

	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::SEND_ANIM_PACKET;
	//evn.wParam
	evn.lParam = (DWORD_PTR)animInfo;
	CSendServerEventMgr::GetInst()->AddServerSendEvent(evn);
}

void CSendServerEventMgr::SendUseSkillPacket(UINT64 _ownerId, UINT64 _targetObjId
							, int _skillLevel, SkillType _skillType, Vec3 _offsetPos
							, bool _useMousePos, Vec3 _mousePos
							, bool _useMouseDir, Vec3 _mouseDir)
{
	// 서버에게 보낼 이벤트
	SkillInfo* skillInfo = new SkillInfo();
	skillInfo->OwnerId = _ownerId;
	skillInfo->TargetId = _targetObjId;
	skillInfo->SkillLevel = _skillLevel;
	skillInfo->skillType = _skillType;
	skillInfo->offsetPos.x = _offsetPos.x;
	skillInfo->offsetPos.y = _offsetPos.y;
	skillInfo->offsetPos.z = _offsetPos.z;

	skillInfo->UseMousePos = _useMousePos;
	skillInfo->MousePos.x = _mousePos.x;
	skillInfo->MousePos.y = _mousePos.y;
	skillInfo->MousePos.z = _mousePos.z;
	skillInfo->UseMouseDir = _useMouseDir;
	skillInfo->MouseDir.x = _mouseDir.x;
	skillInfo->MouseDir.y = _mouseDir.y;
	skillInfo->MouseDir.z = _mouseDir.z;

	tServerEvent serverEvn = {};
	serverEvn.Type = SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET;
	serverEvn.wParam = (DWORD_PTR)skillInfo;
	//serverEvn.lParam 
	CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
}

void CSendServerEventMgr::SendDespawnPacket(UINT64 _objId, float _lifeSpan)
{
	tServerEvent serverEvn = {};
	serverEvn.Type = SERVER_EVENT_TYPE::SEND_DESPAWN_PACKET;
	serverEvn.wParam = _objId;
	serverEvn.lParam = _lifeSpan;
	CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
}

void CSendServerEventMgr::SendDeadPacket(SkillType _skillType)
{
}

void CSendServerEventMgr::SendSoundPacket()
{
}
