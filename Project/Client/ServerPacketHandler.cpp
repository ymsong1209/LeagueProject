#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "ServerSession.h"

#include "ServerFunc.h"
#include "GameObjMgr.h"
#include "TestLevel.h"

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

	case S_SKILL_DAMAGE:
		Handle_S_SKILL_DAMAGE(session, buffer, len);
		break;

	case S_SKILL_CC:
		Handle_S_SKILL_CC(session, buffer, len);
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

	// 진영 선택 레벨 
	//CreateFactionLevel();

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
	// 챔피언 픽 레벨로 이동
	//CreateChampionPickLevel();

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

		// 맵 불러옴
		CreateTestLevel();

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

	std::cout << "===============================" << endl;

	m.unlock();
}

void ServerPacketHandler::Handle_S_PLAYER_MOVE(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_PLAYER_MOVE Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_PLAYER_MOVE* pkt = reinterpret_cast<PKT_S_PLAYER_MOVE*>(buffer);

	if (pkt->Validate() == false)
	{
		m.unlock();
		return;
	}

	// 해당 Id 플레이어가 움직임.
	uint64 _PlayerId = pkt->playerId;
	if (_PlayerId != MyPlayer.id)
	{
		ObjectMove playerMove = pkt->playerMove;

		GameObjMgr::GetInst()->E_MovePlayer(_PlayerId, playerMove);
	}

	std::cout << "===============================" << endl;

	m.unlock();
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
		_AnimInfo.blend = _AnimInfoPacket.blend;
		_AnimInfo.blendTime = _AnimInfoPacket.blendTime;

		GameObjMgr::GetInst()->E_ObjectAnim(_AnimInfo);
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

	cout << "S_OBJECT_MOVE Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_OBJECT_MOVE* pkt = reinterpret_cast<PKT_S_OBJECT_MOVE*>(buffer);

	if (pkt->Validate() == false)
	{
		m.unlock();
		return;
	}

	// 해당 Id 오브젝트가 움직임.
	uint64 _objectId = pkt->objectId;

	// 방장을 제외한 클라이언트만 해당 움직임을 받는다.
	if (!MyPlayer.host)
	{
		ObjectMove playerMove = pkt->objectMove;

		GameObjMgr::GetInst()->E_MoveObject(_objectId, playerMove);
	}

	std::cout << "===============================" << endl;

	m.unlock();
}

void ServerPacketHandler::Handle_S_SKILL_PROJECTILE(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_SKILL_PROJECTILE Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_SKILL_PROJECTILE* pkt = reinterpret_cast<PKT_S_SKILL_PROJECTILE*>(buffer);

	if (pkt->Validate() == false)
	{
		m.unlock();
		return;
	}

	// 투사체 objectId Id 오브젝트가 움직임.
	uint64 _objectId = pkt->projectileId;

	 // 여기 작성중
	 // 
	 // 
	 // 
	// 방장을 제외한 클라이언트만 해당 움직임을 받는다.
	if (!MyPlayer.host)
	{
		ObjectMove playerMove = pkt->objectMove;

		GameObjMgr::GetInst()->E_MoveObject(_objectId, playerMove);
	}

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
		m.unlock();
		return;
	}

	uint64 AttackedId = pkt->objecId;       // 스킬을 맞은 오브젝트 id
	SkillInfo skillInfo = pkt->skillInfo;  // 맞은 스킬 정보


	// 1. 스킬맞은 id가 플레이어 id면 걔 클라이언트에서 알아서 처리
	// 2. 스킬맞은 id가 오브젝트 id면(미니언,포탑) 방장 클라이언트에서 알아서 처리

	// 여기서 스킬 타입에 따라서 
	// CSkill curSkill = SkillMgr::GetInst()->AttackedSkill(skillInfo.skillType) 



	std::cout << "===============================" << endl;
	m.unlock();
}


// 안씀
void ServerPacketHandler::Handle_S_SKILL_DAMAGE(PacketSessionRef& session, BYTE* buffer, int32 len)
{
}

void ServerPacketHandler::Handle_S_SKILL_CC(PacketSessionRef& session, BYTE* buffer, int32 len)
{
}
