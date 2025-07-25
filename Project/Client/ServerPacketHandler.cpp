﻿#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "ServerSession.h"

#include "ServerFunc.h"
#include "GameObjMgr.h"
#include "TestLevel.h"
#include "ServerEventMgr.h"

// json level load  
#include <Engine/CEventMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include "CLevelSaveLoad.h"
#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine/CSound.h>
#include <Engine/CSoundMgr.h>

#include <Engine/CComponent.h>
#include <Script/CSendServerEventMgr.h>
#include <Script/CCharacterUIScript.h>
#include <Script/CWorldHPSpawnScript.h>
#include <Script/CInventoryUIScript.h>
#include <Script/CMinimapUIScript.h>
#include <Script/CMouseCursorUIScript.h>
#include <Script/CFontUIScript.h>
#include <Script/CScorePanelScript.h>
#include <Script\CKillLogUIScript.h>

#include <Script/CUnitScript.h>
#include <Engine/CCollisionMgr.h>

void ServerPacketHandler::HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(session, buffer, len);
		break;

	case S_LOGIN:
		Handle_S_LOGIN(session, buffer, len);
		break;

	case S_PICK_FACTION:
		Handle_S_PICK_FACTION(session, buffer, len);
		break;

	case S_PICK_CHAMPION:
		Handle_S_PICK_CHAMPION(session, buffer, len);
		break;

	case S_GAME_START:
		Handle_S_GAME_START(session, buffer, len);
		break;

	case S_PLAYER_MOVE:
		Handle_S_PLAYER_MOVE(session, buffer, len);
		break;

	case S_OBJECT_ANIM:
		Handle_S_OBJECT_ANIM(session, buffer, len);
		break;

	case S_SPAWN_OBJECT:
		Handle_S_SPAWN_OBJECT(session, buffer, len);
		break;

	case S_OBJECT_MOVE:
		Handle_S_OBJECT_MOVE(session, buffer, len);
		break;

	case S_SKILL_PROJECTILE:
		Handle_S_SKILL_PROJECTILE(session, buffer, len);
		break;

	case S_SKILL_HIT:
		Handle_S_SKILL_HIT(session, buffer, len);
		break;

    case S_DESPAWN_OBJECT:
		Handle_S_DESPAWN_OBJECT(session, buffer, len);
		break;

	case S_KDA_CS:
		Handle_S_KDA_CS(session, buffer, len);
		break;

	case S_SOUND:
		Handle_S_SOUND(session, buffer, len);
		break;

	case S_TIME:
		Handle_S_TIME(session, buffer, len);
		break;

	case S_OBJECT_MTRL:
		Handle_S_OBJECT_MTRL(session, buffer, len);
		break;

	case S_CHAT:
		Handle_S_CHAT(session, buffer, len);
		break;

	case S_EFFECT:
		Handle_S_EFFECT(session, buffer, len);
		break;
	}
}

// [ PKT_S_TEST ][BuffsListItem BuffsListItem BuffsListItem]
void ServerPacketHandler::Handle_S_TEST(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PKT_S_TEST* pkt = reinterpret_cast<PKT_S_TEST*>(buffer);

	if (pkt->Validate() == false)
		return;

	//cout << "ID: " << id << " HP : " << hp << " ATT : " << attack << endl;

	PKT_S_TEST::BuffsList buffs = pkt->GetBuffsList();
	
	cout << "BufCount : " << buffs.Count() << endl;

	for (auto& buff : buffs)
	{
		cout << "BufInfo : " << buff.buffId << " " << buff.remainTime << endl;

		PKT_S_TEST::BuffsVictimsList victims =  pkt->GetBuffsVictimList(&buff);

		cout << "Victim Count : " << victims.Count() << endl;

		for (auto& victim : victims)
		{
			cout << "Victim : " << victim << endl;
		}

	}
}

void ServerPacketHandler::Handle_S_LOGIN(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	cout << "S_LOGIN Packet" << endl;

	BufferReader br(buffer, len);

	PKT_S_LOGIN* pkt = reinterpret_cast<PKT_S_LOGIN*>(buffer);

	if (pkt->Validate() == false)
		return;

	bool _Success = pkt->success;
	uint64 _PlayerId = pkt->playerId;
	if (_Success)
		cout << "S_LOGIN Success" << endl;
	else
		cout << "S_LOGIN Fail" << endl;


	if(MyPlayer.id == 0)
		MyPlayer.id = _PlayerId;

	PKT_S_LOGIN::PlayerList playerIdBuffs = pkt->GetPlayerList();
	for (auto& playerIdBuff : playerIdBuffs)
	{
		// 다른 플레이어들의 대한 정보가 UI에 활용되어야 한다. 
		// 전체 플레이어 id
		playerIdBuff.playerId;

		PKT_S_LOGIN::NickNameList playerNickNamebuffs = pkt->GetNickNameList(&playerIdBuff);
		
		// 전체 플레이어 진영
		playerIdBuff.playerFaction;

		// 전체 플레이어 이름
		wstring playerNickName = L"";
		for (auto& playerNameBuff : playerNickNamebuffs)
		{
			playerNickName.push_back(playerNameBuff.nickname);
		}

		// 내 플레이어일시, continue;
		if (playerIdBuff.playerId == MyPlayer.id)
		{
			MyPlayer.faction = playerIdBuff.playerFaction;
			MyPlayer.nickname = playerNickName;
			MyPlayer.host = playerIdBuff.host;

			if(MyPlayer.host)
				cout << "!!!! You are a HOST !!!! " << endl;

			cout << "My Id : " << MyPlayer.id
				<< ", My Faction : " << (int)MyPlayer.faction
				<< ", My NickName : " << MyPlayer.nickname << endl;

			continue;
		}

		cout << "OtherPlayer Id : "      << playerIdBuff.playerId 
			 << ", Faction : " << (int)playerIdBuff.playerFaction
			 << ", NickName : " << playerNickName << endl;
	}

	std::cout << "===============================" << endl;
}

void ServerPacketHandler::Handle_S_PICK_FACTION(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	cout << "S_PICK_FACTION Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_PICK_FACTION* pkt = reinterpret_cast<PKT_S_PICK_FACTION*>(buffer);

	if (pkt->Validate() == false)
		return;

	bool _Success = pkt->success;
	WaitingStatus waiting = pkt->waiting;
	
	if(_Success)
		cout << "S_PICK_FACTION Success" << endl;
	else
		cout << "S_PICK_FACTION Fail" << endl;

	std::cout << "===============================" << endl;
}

void ServerPacketHandler::Handle_S_PICK_CHAMPION(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	cout << "S_PICK_CHAMPION Packet" << endl;

	BufferReader br(buffer, len);

	PKT_S_PICK_CHAMPION* pkt = reinterpret_cast<PKT_S_PICK_CHAMPION*>(buffer);

	if (pkt->Validate() == false)
		return;

	bool _Success = pkt->success;

	if (_Success)
		cout << "S_PICK_CHAMPION Success" << endl;
	else
		cout << "S_PICK_CHAMPION Fail" << endl;

	// 내가 챔피언을 변경했을 시 업데이트
	if (MyPlayer.id == pkt->PlayerID)
	{
		MyPlayer.champion = pkt->champion;

		cout << "My Pick Champion is " << (int)MyPlayer.champion << endl;
	}

	std::cout << "===============================" << endl;

}

void ServerPacketHandler::Handle_S_GAME_START(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();
	cout << "S_GAME_START Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_GAME_START* pkt = reinterpret_cast<PKT_S_GAME_START*>(buffer);

	if (pkt->Validate() == false)
	{
		m.unlock();
		return;
	}

	bool _Success = pkt->success;

	if (_Success)
	{
		cout << "S_GAME_START Success" << endl;

		// 인게임 진입

		/*RECT WindowPos;
		HWND WindowHandle = CEngine::GetInst()->GetInst()->GetMainWnd();
		GetWindowRect(WindowHandle, &WindowPos);

		int topMenuBarHeight = GetSystemMetrics(SM_CYCAPTION);


		Vec2 WindowResolution = CEngine::GetInst()->GetWindowResolution();

		RECT rect;
		long dx = 10;
		rect.left = WindowPos.left + dx;
		rect.top = WindowPos.top + topMenuBarHeight + dx;

		rect.right = WindowPos.left + WindowResolution.x;
		rect.bottom = WindowPos.top + WindowResolution.y + dx;

		ClipCursor(&rect);*/

		// 맵 불러옴
		//CreateTestLevel();

		//////=========json level load===========================================
		CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevelFromJson(L"level\\BushAndWall.json");
		CLevelMgr::GetInst()->SetCurLevel(pLoadedLevel);

		// inspector  UI update
		InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
		inspector->SetTargetObject(nullptr);
		
		// if curState is stop,  next level state is also stop
		CLevel* level = CUR_LEVEL;
		if (level->GetState() == LEVEL_STATE::STOP) {
		    CTimeMgr::GetInst()->SetTimeScale(0.f);
		}
		level->ChangeState(LEVEL_STATE::PLAY);
		CTimeMgr::GetInst()->SetTimeScale(1.f);
		////===================================================================


		PKT_S_GAME_START::PlayerInfoList playerInfoBuffs = pkt->GetPlayerInfoList();
		for (auto& playerInfoBuff : playerInfoBuffs)
		{

			PKT_S_GAME_START::NickNameList playerNickNamebuffs = pkt->GetNickNameList(&playerInfoBuff);

			// 전체 플레이어 이름
			wstring playerNickName = L"";
			for (auto& playerNameBuff : playerNickNamebuffs)
			{
				playerNickName.push_back(playerNameBuff.nickname);
			}


			// 내 id일시
			if (MyPlayer.id == playerInfoBuff.id)
			{
				MyPlayer.champion = playerInfoBuff.champion;
				MyPlayer.posInfo = playerInfoBuff.posInfo;

				// 플레이어 생성
				GameObjMgr::GetInst()->AddPlayer(MyPlayer, true);
				
				cout << "My Champion : " << (int)MyPlayer.champion
					<< ", My PosInfo : " << (float)MyPlayer.posInfo.pos.x
					<< ", " << (float)MyPlayer.posInfo.pos.y
					<< ", " << (float)MyPlayer.posInfo.pos.z
					<< endl;

				continue;
			}

			// 다른 플레이어 생성
			PlayerInfo otherPlayer = {};
			otherPlayer.champion = playerInfoBuff.champion;
			otherPlayer.faction = playerInfoBuff.faction;
			otherPlayer.id = playerInfoBuff.id;
			otherPlayer.nickname = playerNickName;
			otherPlayer.posInfo = playerInfoBuff.posInfo;

			cout << "OtherPlayer Champion : " << (int)playerInfoBuff.champion
				<< ", OtherPlayer PosInfo : " << (float)playerInfoBuff.posInfo.pos.x
				<< ", " << (float)playerInfoBuff.posInfo.pos.y
				<< ", " << (float)playerInfoBuff.posInfo.pos.z
				<< endl;

			GameObjMgr::GetInst()->AddPlayer(otherPlayer, false);
		}
		IsInGame = true;
	}
	else
	{
		cout << "S_GAME_START Fail" << endl;
		
		IsInGame = false;
		// 다시 진영 선택 레벨로 간다.
		//CreateFactionLevel();
	}


	// 모든 플레이어 생성후, UI를위한 오브젝트, 플레이어 vector 생성
	map<uint64, CGameObject*> mapPlayers = GameObjMgr::GetInst()->GetPlayers();
	vector<CGameObject*> vecAllPlayer;
	for (const auto& pair : mapPlayers) {
		vecAllPlayer.push_back(pair.second);
	}
	CSendServerEventMgr::GetInst()->SetVecAllPlyer(vecAllPlayer);
	

	CGameObject* UIObj = new CGameObject; //각종 스크립트에서 처리할 것들
	UIObj->SetName(L"UIObj");
	UIObj->AddComponent(new CTransform);
	UIObj->AddComponent(new CCharacterUIScript);
	UIObj->AddComponent(new CWorldHPSpawnScript);
	UIObj->AddComponent(new CInventoryUIScript);
	UIObj->AddComponent(new CMinimapUIScript);
	UIObj->AddComponent(new CMouseCursorUIScript);
	UIObj->AddComponent(new CFontUIScript);
	UIObj->AddComponent(new CScorePanelScript);
	UIObj->AddComponent(new CKillLogUIScript);
	SpawnGameObject(UIObj, Vec3(0.f, 0.f, 0.f), L"ViewPort UI");

	std::cout << "===============================" << endl;

	m.unlock();
}

void ServerPacketHandler::Handle_S_PLAYER_MOVE(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	{
		std::lock_guard<std::mutex> lock(m);

		//cout << "S_PLAYER_MOVE Packet" << endl;
		BufferReader br(buffer, len);

		PKT_S_PLAYER_MOVE* pkt = reinterpret_cast<PKT_S_PLAYER_MOVE*>(buffer);

		if (pkt->Validate() == false)
			return;

		// 내 플레이어가 보낸 움직임은 반영하지 않아도 된다. 
		uint64 _PlayerId = pkt->playerId;
		if (_PlayerId == MyPlayer.id)
			return;
		
		ObjectMove _playerMove = pkt->playerMove;
		CGameObject* obj = GameObjMgr::GetInst()->FindPlayer(_PlayerId);

		tServerEvent evn = {};
		evn.Type = SERVER_EVENT_TYPE::MOVE_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		ObjectMove* objMove = new ObjectMove();
		objMove->LV = _playerMove.LV;
		objMove->HP = _playerMove.HP;
		objMove->MP = _playerMove.MP;
		objMove->MaxHP = _playerMove.MaxHP;
		objMove->MaxMP = _playerMove.MaxMP;
		objMove->AttackPower = _playerMove.AttackPower;
		objMove->DefencePower = _playerMove.DefencePower;
		objMove->CC = _playerMove.CC;
		objMove->bUnitDead = _playerMove.bUnitDead;
		objMove->iDropGold = _playerMove.iDropGold;
		objMove->fDropExp = _playerMove.fDropExp;

		objMove->pos = _playerMove.pos;
		objMove->moveDir = _playerMove.moveDir;

		evn.lParam = (DWORD_PTR)objMove;

		ServerEventMgr::GetInst()->AddEvent(evn);

		//std::cout << "===============================" << endl;
	}
}

void ServerPacketHandler::Handle_S_OBJECT_ANIM(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_OBJECT_ANIM Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_OBJECT_ANIM* pkt = reinterpret_cast<PKT_S_OBJECT_ANIM*>(buffer);

	if (pkt->Validate() == false)
	{
		m.unlock();
		return;
	}

	// 보낸이가 본인일 경우 처리 x
	uint64 _sendId = pkt->sendId;
	if (_sendId != MyPlayer.id)
	{
		AnimInfoPacket _AnimInfoPacket = pkt->animInfo;

		// 애니메이션 이름
		PKT_S_OBJECT_ANIM::AnimNameList AnimNameBuffs = pkt->GetAnimNameList();
		wstring _animName = L"";
		for (auto& AnimNameBuff : AnimNameBuffs)
		{
			_animName.push_back(AnimNameBuff.animName);
		}

		AnimInfo _AnimInfo = {};
		_AnimInfo.targetId = _AnimInfoPacket.targetId;
		_AnimInfo.animName = _animName;
		_AnimInfo.bRepeat = _AnimInfoPacket.bRepeat;
		_AnimInfo.blend = _AnimInfoPacket.blend;
		_AnimInfo.blendTime = _AnimInfoPacket.blendTime;
		_AnimInfo.animSpeed = _AnimInfoPacket.animSpeed;

		CGameObject* obj = GameObjMgr::GetInst()->FindAllObject(_AnimInfo.targetId);

		tServerEvent evn = {};
		evn.Type = SERVER_EVENT_TYPE::ANIM_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// AnimInfo 구조체의 포인터를 DWORD_PTR로 캐스팅하여 lParam에 저장
		AnimInfo* animInfo = new AnimInfo(_AnimInfo);
		evn.lParam = (DWORD_PTR)animInfo;

		ServerEventMgr::GetInst()->AddEvent(evn);
	}

	std::cout << "===============================" << endl;

	m.unlock();
}

void ServerPacketHandler::Handle_S_SPAWN_OBJECT(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_SPAWN_OBJECT Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_SPAWN_OBJECT* pkt = reinterpret_cast<PKT_S_SPAWN_OBJECT*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_SPAWN_OBJECT Validate Fail" << endl;
		m.unlock();
		return;
	}

	ObjectInfo _objectInfo = pkt->objectInfo;
	uint64 _objectId = pkt->objectInfo.objectId;

	cout << "Id : " << _objectId << endl;

	GameObjMgr::GetInst()->AddObject(_objectId, _objectInfo);

	std::cout << "===============================" << endl;

	m.unlock();
}

void ServerPacketHandler::Handle_S_OBJECT_MOVE(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	//cout << "S_OBJECT_MOVE Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_OBJECT_MOVE* pkt = reinterpret_cast<PKT_S_OBJECT_MOVE*>(buffer);

	if (pkt->Validate() == false)
	{
		m.unlock();
		cout << "S_OBJECT_MOVE Packet Validate Fail " << endl;
		return;
	}

	// 해당 Id 오브젝트가 움직임.
	uint64 _objectId = pkt->objectId;

	// 방장을 제외한 클라이언트만 해당 움직임을 받는다.
	if (!MyPlayer.host)
	{
		ObjectMove _objectMove = pkt->objectMove;

		CGameObject* obj = GameObjMgr::GetInst()->FindAllObject(_objectId);

		tServerEvent evn = {};
		evn.Type = SERVER_EVENT_TYPE::MOVE_PACKET;
		evn.wParam = (DWORD_PTR)obj;

		// ObjectMove 구조체의 포인터를 DWORD_PTR로 캐스팅하여 lParam에 저장
		ObjectMove* objMove = new ObjectMove();
		objMove->LV = _objectMove.LV;
		objMove->HP = _objectMove.HP;
		objMove->MP = _objectMove.MP;
		objMove->MaxHP = _objectMove.MaxHP;
		objMove->MaxMP = _objectMove.MaxMP;
		objMove->AttackPower = _objectMove.AttackPower;
		objMove->DefencePower = _objectMove.DefencePower;

		objMove->CC = _objectMove.CC;
		objMove->bUnitDead = _objectMove.bUnitDead;
		objMove->iDropGold = _objectMove.iDropGold;
		objMove->fDropExp = _objectMove.fDropExp;

		objMove->moveDir = _objectMove.moveDir;
		objMove->pos = _objectMove.pos;

		evn.lParam = (DWORD_PTR)objMove;

		ServerEventMgr::GetInst()->AddEvent(evn);
	}

	//std::cout << "===============================" << endl;

	m.unlock();
}

void ServerPacketHandler::Handle_S_SKILL_PROJECTILE(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	BufferReader br(buffer, len);

	PKT_S_SKILL_PROJECTILE* pkt = reinterpret_cast<PKT_S_SKILL_PROJECTILE*>(buffer);

	cout << "S_SKILL_PROJECTILE Packet. projectileId : " << pkt->projectileId << endl;
	if (pkt->Validate() == false)
	{
		cout << "S_SKILL_PROJECTILE Packet Validate False" << endl;
		m.unlock();
		return;
	}

	// 투사체 objectId Id
	uint64 _objectId = pkt->projectileId;
	SkillInfo _skillInfo = pkt->skillInfo;

	GameObjMgr::GetInst()->AddSkillProjectile(_objectId, _skillInfo);

	std::cout << "===============================" << endl;

	m.unlock();
}

void ServerPacketHandler::Handle_S_SKILL_HIT(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_SKILL_HIT Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_SKILL_HIT* pkt = reinterpret_cast<PKT_S_SKILL_HIT*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_SKILL_HIT Packet Validate False" << endl;
		m.unlock();
		return;
	}

	uint64	hitObjId = pkt->objecId;       // 스킬을 맞은 오브젝트 id
	SkillInfo skillInfo = pkt->skillInfo;  // 맞은 스킬 정보


	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::SKILL_HIT_PACKET;
	evn.wParam = (DWORD_PTR)hitObjId;

	SkillInfo* _skillInfo = new SkillInfo(skillInfo);
	evn.lParam = (DWORD_PTR)_skillInfo;

	ServerEventMgr::GetInst()->AddEvent(evn);

	std::cout << "===============================" << endl;
	m.unlock();
}

void ServerPacketHandler::Handle_S_DESPAWN_OBJECT(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_DESPAWN_OBJECT Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_DESPAWN_OBJECT* pkt = reinterpret_cast<PKT_S_DESPAWN_OBJECT*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_DESPAWN_OBJECT Validate False" << endl;
		m.unlock();
		return;
	}

	uint64	despawnId = pkt->objId;    
	float   lifespan = pkt->time;  

	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::DESPAWN_PACKET;
	evn.wParam = despawnId;
	evn.lParam = lifespan;

	ServerEventMgr::GetInst()->AddEvent(evn);

	std::cout << "===============================" << endl;
	m.unlock();
}

void ServerPacketHandler::Handle_S_KDA_CS(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_KDA_CS Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_KDA_CS* pkt = reinterpret_cast<PKT_S_KDA_CS*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_KDA_CS Validate False" << endl;
		m.unlock();
		return;
	}
	
	KDACSInfo _kdacsInfo = pkt->kdacsInfo;

	KDACSInfo* info = new KDACSInfo();
	info->killerId = _kdacsInfo.killerId;
	info->victimId = _kdacsInfo.victimId;
	info->deadObjUnitType = _kdacsInfo.deadObjUnitType;

	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::KDA_CS_PACKET;
	evn.wParam = (DWORD_PTR)info;

	ServerEventMgr::GetInst()->AddEvent(evn);

	std::cout << "===============================" << endl;
	m.unlock();
}

void ServerPacketHandler::Handle_S_SOUND(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_SOUND Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_SOUND* pkt = reinterpret_cast<PKT_S_SOUND*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_SOUND Validate False" << endl;
		m.unlock();
		return;
	}
	UINT64 soundId = pkt->soundId;
	SoundInfoPacket	 _soundInfoPacket = pkt->soundInfo;
	PKT_S_SOUND::SoundNameList soundNameBuffs = pkt->GetSoundNameList();
	
	// sound 이름
	wstring _soundName = L"";
	for (auto& soundNameBuff : soundNameBuffs)
	{
		_soundName.push_back(soundNameBuff.soundName);
	}

	SoundInfo* soundInfo = new SoundInfo();
	soundInfo->soundName	 = _soundName;
	soundInfo->dimensionType = _soundInfoPacket.dimensionType;
	soundInfo->faction		 = _soundInfoPacket.faction;
	soundInfo->iLoopCount	 = _soundInfoPacket.iLoopCount;
	soundInfo->fVolume		 = _soundInfoPacket.fVolume;
	soundInfo->bOverlap		 = _soundInfoPacket.bOverlap;
	soundInfo->fRange		 = _soundInfoPacket.fRange;
	soundInfo->soundPos.x    = _soundInfoPacket.soundPos.x;
	soundInfo->soundPos.y    = _soundInfoPacket.soundPos.y;
	soundInfo->soundPos.z    = _soundInfoPacket.soundPos.z;


	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::SOUND_PACKET;
	evn.wParam = (DWORD_PTR)soundId;
	evn.lParam = (DWORD_PTR)soundInfo;


	ServerEventMgr::GetInst()->AddEvent(evn);

	std::cout << "===============================" << endl;
	m.unlock();
}

void ServerPacketHandler::Handle_S_TIME(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_TIME Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_TIME* pkt = reinterpret_cast<PKT_S_TIME*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_TIME Validate False" << endl;
		m.unlock();
		return;
	}

	// 모두가 본인의 시간을 업데이트 하는 코드 추가 필요
	float	fTime = pkt->second;

	Vec3 _2dPos = Vec3(0.f, 0.f, 0.f);

	if (1.f == fTime)
	{
		CSound* newSound = new CSound;
		wstring filepath = CPathMgr::GetInst()->GetContentPath();
		filepath += L"sound2d\\bgm_update_summoners_rift.mp3";
		newSound->Load(filepath);
		CSoundMgr::GetInst()->AddSound(newSound);
		int soundId = newSound->GetSoundIndex();
		CSoundMgr::GetInst()->Play(soundId, 5, 0.18f, true, 0.f, _2dPos);
		CSoundMgr::GetInst()->Stop(soundId);
		CSoundMgr::GetInst()->Play(soundId, 5, 0.18f, true, 0.f, _2dPos);
		
	}
	
	// 소환사의 협곡에 오신것을 환영합니다.
	if (3.f == fTime)
	{
		CSound* newSound = new CSound;
		wstring filepath = CPathMgr::GetInst()->GetContentPath();
		filepath += L"sound2d\\announce_welcome_rift.mp3";
		newSound->Load(filepath);
		CSoundMgr::GetInst()->AddSound(newSound);
		int soundId = newSound->GetSoundIndex();
		CSoundMgr::GetInst()->Play(soundId, 1, 0.5f, true, 0.f, _2dPos);
		CSoundMgr::GetInst()->Stop(soundId);
		CSoundMgr::GetInst()->Play(soundId, 1, 0.5f, true, 0.f, _2dPos);
		//CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\welcome_rift.mp3", 1, 0.5f, true, 0.f, _2dPos, Faction::NONE);
	}
	
	// 30초뒤 미니언이 생성됩니다.
	if (30.f == fTime)
	{
		CSound* newSound = new CSound;
		wstring filepath = CPathMgr::GetInst()->GetContentPath();
		filepath += L"sound2d\\announce_minion_spawn_remain_30.mp3";
		newSound->Load(filepath);
		CSoundMgr::GetInst()->AddSound(newSound);
		int soundId = newSound->GetSoundIndex();
		CSoundMgr::GetInst()->Play(soundId, 1, 0.5f, true, 0.f, _2dPos);
		CSoundMgr::GetInst()->Stop(soundId);
		CSoundMgr::GetInst()->Play(soundId, 1, 0.5f, true, 0.f, _2dPos);
		//CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\minion_spawn_remain_30.mp3", 1, 0.5f, true, 0.f, _2dPos, Faction::NONE);
	}
		

	CSendServerEventMgr::GetInst()->SetPlayTime(fTime);

	std::cout << "===============================" << endl;
	m.unlock();
}

void ServerPacketHandler::Handle_S_OBJECT_MTRL(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_OBJECT_MTRL Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_OBJECT_MTRL* pkt = reinterpret_cast<PKT_S_OBJECT_MTRL*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_OBJECT_MTRL Validate False" << endl;
		m.unlock();
		return;
	}

	MtrlInfoPacket	 _mtrlInfoPacket = pkt->mtrlInfo;

	PKT_S_OBJECT_MTRL::TexNameList texNameBuffs = pkt->GetTexNameList();
	// tex 이름
	wstring _TexName = L"";
	for (auto& texNameBuff : texNameBuffs)
	{
		_TexName.push_back(texNameBuff.texName);
	}
	
	PKT_S_OBJECT_MTRL::MtrlNameList mtrlNameBuffs = pkt->GetMtrlNameList();
	// mtrl 이름
	wstring _MtrlName = L"";
	for (auto& mtrlNameBuff : mtrlNameBuffs)
	{
		_MtrlName.push_back(mtrlNameBuff.mtrlName);
	}


	MtrlInfo* mtrlInfo = new MtrlInfo();
	mtrlInfo->IsSetTexParamUsage = _mtrlInfoPacket.IsSetTexParamUsage;
	mtrlInfo->targetId  = _mtrlInfoPacket.targetId;
	mtrlInfo->iMtrlIndex = _mtrlInfoPacket.iMtrlIndex;
	mtrlInfo->tex_param = _mtrlInfoPacket.tex_param;
	mtrlInfo->wTexName = _TexName;
	mtrlInfo->wMtrlName = _MtrlName;

	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::MTRL_PACKET;
	evn.wParam = (DWORD_PTR)mtrlInfo;

	ServerEventMgr::GetInst()->AddEvent(evn);


	std::cout << "===============================" << endl;
	m.unlock();
}

void ServerPacketHandler::Handle_S_CHAT(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	cout << "S_CHAT Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_CHAT* pkt = reinterpret_cast<PKT_S_CHAT*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_CHAT Validate False" << endl;
		return;
	}

	uint64 id = pkt->ownerId;

	PKT_S_CHAT::ChatLog chatLogBuffs = pkt->GetChatLog();
	
	// chatlog wstring 
	wstring _ChatLog = L"";
	for (auto& chatLogBuff : chatLogBuffs)
	{
		_ChatLog.push_back(chatLogBuff.chatLog);
	}
	
	std::wstring* pNickName = new std::wstring(GameObjMgr::GetInst()->FindPlayer(id)->GetScript<CUnitScript>()->GetNickname());
	std::wstring* pChatLog = new std::wstring(_ChatLog);

	tServerEvent evn = {};
	evn.Type = SERVER_EVENT_TYPE::CHAT_PACKET;
	evn.wParam = reinterpret_cast<DWORD_PTR>(pNickName);
	evn.lParam = reinterpret_cast<DWORD_PTR>(pChatLog);

	ServerEventMgr::GetInst()->AddEvent(evn);

	std::cout << "===============================" << endl;
}

void ServerPacketHandler::Handle_S_EFFECT(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	cout << "S_EFFECT Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_EFFECT* pkt = reinterpret_cast<PKT_S_EFFECT*>(buffer);

	if (pkt->Validate() == false)
	{
		cout << "S_EFFECT Validate False" << endl;
		return;
	}

	PKT_S_EFFECT::PrefabName prefabNameBuffs = pkt->GetPrefabName();

	// prefabNaem wstring 
	wstring _PrefabName = L"";
	for (auto& prefabNameBuff : prefabNameBuffs)
	{
		_PrefabName.push_back(prefabNameBuff.prefabName);
	}



	// 이팩트 패킷이 오면, Prefab을 찾아 바로 생성한다. 
	wstring prefabKey = _PrefabName;
	Ptr<CPrefab> EffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(prefabKey);
	CGameObject* Effect = EffectPrefab->Instantiate();

	SpawnGameObject(Effect, Vec3(pkt->Pos.x, pkt->Pos.y, pkt->Pos.z), L"Effect");
	Effect->SetLifeSpan(pkt->Lifespan);

	// 추후 pkt->Dir 필요할 시 사용.

	std::cout << "===============================" << endl;
}
