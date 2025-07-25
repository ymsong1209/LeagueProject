#include "pch.h"
#include "CSendServerEventMgr.h"
#include <mutex>
CSendServerEventMgr::CSendServerEventMgr()
	:m_myKillCnt(0)
	,m_myDeathCnt(0)
	,m_myCSCnt(0)
	,m_RedScore(0)
	,m_BlueScore(0)
	,m_bIsGameStop(false)
{
}

CSendServerEventMgr::~CSendServerEventMgr()
{
}


void CSendServerEventMgr::SendHitPacket(UINT64 _skillObjId, UINT64 _hitObjId, UINT64 _useObjId, int _skillLevel, SkillType _skillType)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

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
}

void CSendServerEventMgr::SendAnimPacket(UINT64 _targetId, wstring _animName, bool _repeat, bool _bRepeatBlend, bool _bUseBlend, float _blentTime, float _animSpeed)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);


		AnimInfo* animInfo = new AnimInfo();
		animInfo->animName = _animName;
		animInfo->targetId = _targetId;
		animInfo->bRepeat = _repeat;
		animInfo->bRepeatBlend = _bRepeatBlend;
		animInfo->blend = _bUseBlend;
		animInfo->blendTime = _blentTime;
		animInfo->animSpeed = _animSpeed;

		tServerEvent evn = {};
		evn.Type = SERVER_EVENT_TYPE::SEND_ANIM_PACKET;
		//evn.wParam
		evn.lParam = (DWORD_PTR)animInfo;
		CSendServerEventMgr::GetInst()->AddServerSendEvent(evn);
	}

}

void CSendServerEventMgr::SendUseSkillPacket(UINT64 _ownerId, UINT64 _targetObjId
							, int _skillLevel, SkillType _skillType, Vec3 _offsetPos
							, int _projectileCount
							, bool _useMousePos, Vec3 _mousePos
							, bool _useMouseDir, Vec3 _mouseDir)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);


		// 서버에게 보낼 이벤트
		SkillInfo* skillInfo = new SkillInfo();
		skillInfo->OwnerId = _ownerId;
		skillInfo->TargetId = _targetObjId;
		skillInfo->SkillLevel = _skillLevel;
		skillInfo->skillType = _skillType;
		skillInfo->offsetPos.x = _offsetPos.x;
		skillInfo->offsetPos.y = _offsetPos.y;
		skillInfo->offsetPos.z = _offsetPos.z;
		skillInfo->projectileCount = _projectileCount;

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
}

void CSendServerEventMgr::SendDespawnPacket(UINT64 _objId, float _lifeSpan)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		tServerEvent serverEvn = {};
		serverEvn.Type = SERVER_EVENT_TYPE::SEND_DESPAWN_PACKET;
		serverEvn.wParam = (DWORD_PTR)_objId;
		serverEvn.lParam = (DWORD_PTR)_lifeSpan;
		CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	}
}

void CSendServerEventMgr::SendKDACSPacket(UINT64 _killerId, UINT64 _victimId, UnitType _deadObjUnitType)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		KDACSInfo* kdacsInfo = new KDACSInfo();
		kdacsInfo->killerId = _killerId;
		kdacsInfo->victimId = _victimId;
		kdacsInfo->deadObjUnitType = _deadObjUnitType;

		tServerEvent serverEvn = {};
		serverEvn.Type = SERVER_EVENT_TYPE::SEND_KDA_CS_PACKET;
		serverEvn.wParam = (DWORD_PTR)kdacsInfo;
		//serverEvn.lParam;
		CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	}
}


void CSendServerEventMgr::SendSoundPacket(wstring _soundName, int _loopCount, float _fVolume, bool _bOverlap, float _fRange, Vec3 _soundPos, Faction _faction)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		SoundInfo* soundInfo = new SoundInfo();
		soundInfo->soundName = _soundName;
		soundInfo->faction = _faction;
		soundInfo->iLoopCount = _loopCount;
		soundInfo->fVolume = _fVolume;
		soundInfo->bOverlap = _bOverlap;
		soundInfo->fRange = _fRange;
		soundInfo->soundPos.x = _soundPos.x;
		soundInfo->soundPos.y = _soundPos.y;
		soundInfo->soundPos.z = _soundPos.z;

		tServerEvent serverEvn = {};
		serverEvn.Type = SERVER_EVENT_TYPE::SEND_SOUND_PACKET;
		serverEvn.wParam = (DWORD_PTR)soundInfo;
		//serverEvn.lParam;
		CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	}
}

void CSendServerEventMgr::SendSetTexParamPacket(UINT64 _objId, int _mtrlIndex, TEX_PARAM _texParam, wstring _TexName)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		MtrlInfo* mtrlInfo = new MtrlInfo();
		mtrlInfo->IsSetTexParamUsage = true;
		mtrlInfo->targetId = _objId;
		mtrlInfo->iMtrlIndex = _mtrlIndex;
		mtrlInfo->tex_param = _texParam;
		mtrlInfo->wTexName = _TexName;

		tServerEvent serverEvn = {};
		serverEvn.Type = SERVER_EVENT_TYPE::SEND_MTRL_PACKET;
		serverEvn.wParam = (DWORD_PTR)mtrlInfo;
		//serverEvn.lParam;
		CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	}
}

void CSendServerEventMgr::SendSetMtrlPacket(UINT64 _objId, int _mtrlIndex, wstring _mtrlName)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		MtrlInfo* mtrlInfo = new MtrlInfo();
		mtrlInfo->IsSetTexParamUsage = false;
		mtrlInfo->targetId = _objId;
		mtrlInfo->iMtrlIndex = _mtrlIndex;
		mtrlInfo->wMtrlName = _mtrlName;

		tServerEvent serverEvn = {};
		serverEvn.Type = SERVER_EVENT_TYPE::SEND_MTRL_PACKET;
		serverEvn.wParam = (DWORD_PTR)mtrlInfo;
		//serverEvn.lParam;
		CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	}

}

void CSendServerEventMgr::SendChatPacket(UINT64 _userId, wstring _chatLog)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		std::wstring* pChatLog = new std::wstring(_chatLog);

		tServerEvent serverEvn = {};
		serverEvn.Type = SERVER_EVENT_TYPE::SEND_CHAT_PACKET;
		serverEvn.wParam = (DWORD_PTR)_userId;
		serverEvn.lParam = (DWORD_PTR)pChatLog;

		CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	}
}

void CSendServerEventMgr::SendEffectPacket(wstring _prefabName, float _lifespan, Vec3 _pos, Vec3 _dir)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		std::wstring* pPrefabName = new std::wstring(_prefabName);
		EffectInfo* effectInfo = new EffectInfo();
		effectInfo->lifespan = _lifespan;
		effectInfo->Pos.x = _pos.x;
		effectInfo->Pos.y = _pos.y;
		effectInfo->Pos.z = _pos.z;
		effectInfo->Dir.x = _dir.x;
		effectInfo->Dir.y = _dir.y;
		effectInfo->Dir.z = _dir.z;

		tServerEvent serverEvn = {};
		serverEvn.Type = SERVER_EVENT_TYPE::SEND_EFFECT_PACKET;
		serverEvn.wParam = (DWORD_PTR)pPrefabName;
		serverEvn.lParam = (DWORD_PTR)effectInfo;

		CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	}
}
