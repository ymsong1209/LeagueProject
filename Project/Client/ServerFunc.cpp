#include "pch.h"
#include "ServerFunc.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"


void SendCLogin(ClientServiceRef _service, wstring _userName)
{
    std::cout << "Client send Login packet to Server" << std::endl;

    PKT_C_LOGIN_WRITE pktWriter;

    //���� ������ŭ �Լ��� �Ķ���ͷ� �־��ּ���.
    PKT_C_LOGIN_WRITE::NickName nickNamePacket = pktWriter.ReserveNickName(_userName.size());
    for (int i = 0; i < _userName.size(); i++) {
        nickNamePacket[i] = { _userName[i] };
    }

    SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
    _service->Broadcast(sendBuffer);
}

PlayerInfo MyPlayer = {};
