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
	case S_MOVE:
		Handle_S_MOVE(session, buffer, len);
	//case S_PLAYER_UPDATE:
	//	Handle_S_PLAYER_UPDATE(session, buffer, len);
	//	break;
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


	if(MyPlayer.id == 0)
		MyPlayer.id = _PlayerId;

	// ���� ���� ���� 
	//CreateFactionLevel();

	PKT_S_LOGIN::PlayerList playerIdBuffs = pkt->GetPlayerList();
	for (auto& playerIdBuff : playerIdBuffs)
	{
		// �ٸ� �÷��̾���� ���� ������ UI�� Ȱ��Ǿ�� �Ѵ�. 
		// ��ü �÷��̾� id
		playerIdBuff.playerId;

		PKT_S_LOGIN::NickNameList playerNickNamebuffs = pkt->GetNickNameList(&playerIdBuff);
		
		// ��ü �÷��̾� ����
		playerIdBuff.playerFaction;

		// ��ü �÷��̾� �̸�
		wstring playerNickName = L"";
		for (auto& playerNameBuff : playerNickNamebuffs)
		{
			playerNickName.push_back(playerNameBuff.nickname);
		}

		// �� �÷��̾��Ͻ�, continue;
		if (playerIdBuff.playerId == MyPlayer.id)
		{
			MyPlayer.faction = playerIdBuff.playerFaction;
			MyPlayer.nickname = playerNickName;
			MyPlayer.host = playerIdBuff.host;

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

	// è�Ǿ� �� ������ �̵�
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

	// ���� è�Ǿ��� �������� �� ������Ʈ
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
		// �ΰ��� ����

		// �� �ҷ���
		CreateIngameLevel();

		PKT_S_GAME_START::PlayerInfoList playerInfoBuffs = pkt->GetPlayerInfoList();
		for (auto& playerInfoBuff : playerInfoBuffs)
		{

			PKT_S_GAME_START::NickNameList playerNickNamebuffs = pkt->GetNickNameList(&playerInfoBuff);

			// ��ü �÷��̾� �̸�
			wstring playerNickName = L"";
			for (auto& playerNameBuff : playerNickNamebuffs)
			{
				playerNickName.push_back(playerNameBuff.nickname);
			}


			// �� id�Ͻ�
			if (MyPlayer.id == playerInfoBuff.id)
			{
				MyPlayer.champion = playerInfoBuff.champion;
				MyPlayer.posInfo = playerInfoBuff.posInfo;

				// �÷��̾� ����
				GameObjMgr::GetInst()->AddPlayer(MyPlayer, true);
				
				cout << "My Champion : " << (int)MyPlayer.champion
					<< ", My PosInfo : " << (float)MyPlayer.posInfo.pos.x
					<< ", " << (float)MyPlayer.posInfo.pos.y
					<< ", " << (float)MyPlayer.posInfo.pos.z
					<< endl;

				continue;
			}

			// �ٸ� �÷��̾� ����
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
		IsInGame = false;
		// �ٽ� ���� ���� ������ ����.
		//CreateFactionLevel();

	}

	std::cout << "===============================" << endl;

	m.unlock();
}

void ServerPacketHandler::Handle_S_MOVE(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	std::mutex m;
	m.lock();

	cout << "S_MOVE Packet" << endl;
	BufferReader br(buffer, len);

	PKT_S_MOVE* pkt = reinterpret_cast<PKT_S_MOVE*>(buffer);

	if (pkt->Validate() == false)
	{
		m.unlock();
		return;
	}

	// �ش� Id �÷��̾ ������.
	uint64 _PlayerId = pkt->playerId;
	if (_PlayerId != MyPlayer.id)
	{
		PlayerMove playerMove = pkt->playerMove;

		GameObjMgr::GetInst()->E_MovePlayer(_PlayerId, playerMove);
	}

	std::cout << "===============================" << endl;

	m.unlock();
}
