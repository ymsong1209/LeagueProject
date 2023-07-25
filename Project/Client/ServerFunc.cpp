#include "pch.h"
#include "ServerFunc.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"


void Send_CLogin(ClientServiceRef _service, wstring _userName)
{
    std::cout << "C_LOGIN Pakcet" << endl;
    std::cout << _userName << std::endl;

    PKT_C_LOGIN_WRITE pktWriter;

    //문자 개수만큼 함수에 파라미터로 넣어주세요.
    PKT_C_LOGIN_WRITE::NickName nickNamePacket = pktWriter.ReserveNickName(_userName.size());
    for (int i = 0; i < _userName.size(); i++) {
        nickNamePacket[i] = { _userName[i] };
    }

    SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
    _service->Broadcast(sendBuffer);

    std::cout << "===============================" << endl;
}

void Send_CPickFaction(ClientServiceRef _service)
{
    std::cout << "C_PICK_FACTION Pakcet" << endl;

    PKT_C_PICK_FACTION_WRITE pktWriter;

    std::cout << "   Empty     " << endl;
    SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
    _service->Broadcast(sendBuffer);

    std::cout << "===============================" << endl;
}

void Send_CPickChampionAndStart(ClientServiceRef _service, ChampionType _championType)
{
    std::cout << "C_PICK_CHAMPION_AND_START Pakcet" << endl;

    PKT_C_PICK_CHAMPION_AND_START_WRITE pktWriter(_championType);
    
    SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
    
    _service->Broadcast(sendBuffer);

    std::cout << "===============================" << endl;
}

void Send_CMove(ClientServiceRef _service, PlayerMove _move)
{
    std::cout << "C_MOVE Pakcet" << endl;

    PKT_C_MOVE_WRITE pktWriter(_move);

    SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
    _service->Broadcast(sendBuffer);

    std::cout << "===============================" << endl;
}

bool IsInGame = false;
PlayerInfo MyPlayer = {};
wstring MyPlayerNickName = L"";