#pragma once
#include <Engine\serverpacket.h>


//enum class FactionType
//{
//    BLUE = 0,
//    RED = 1,
//    NONE = 2, // �񼱰�����(��θ� ������. ����x)
//    END = 3,
//};

enum WaitingStatus
{
    WAITING = 0,
    RUN = 1,
};

enum ChampionType
{
    NONE,
    BLITZCRANK,
    JINX,
    AMUMU,
    MALPHITE,
};

enum class ObjectType {
    PLAYER,
    MELEEMINION,
    RANGEMINION,
    TOWER,
    MONSTER,
    END,
};


struct PlayerInfo
{
    uint64  id;
    wstring nickname;
    FactionType faction;
    ChampionType champion;
    bool host;

    ObjectMove posInfo;
};

struct PlayerInfoPacket
{
    uint64  id;
    FactionType faction;
    ChampionType champion;
    bool host;

    ObjectMove posInfo;

    uint16 nickNameOffset;
    uint16 nickNameCount;

    struct NickNameItem {
        wchar_t nickname;
    };

    bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size) {
        if (nickNameOffset + nickNameCount * sizeof(NickNameItem) > packetSize)
            return false;

        size += nickNameCount * sizeof(NickNameItem);
        return true;
    }
};

/*
    MinionInfo
    �̴Ͼ�Ÿ�� : ����, ���Ÿ�, ����, ����
    ����
    id
    ����
    ���ݷ�


    ProjectileInfo
    Owner : ���� �����
    Dest : ���� �´��� (���� ������� ��Ÿ����  0�ʱ�ȭ)
    time : ����ü �����ð�

    1. a�� ��ų��ư�� ����
        ��� ���(Ŭ�� a)�� �������� ����� ����.
        ������ ok ��Ŷ�� ��� Ŭ���̾�Ʈ���� ��� ���(Ŭ�� a)�� ����ü�� ��ٰ� �˸�.
        ���嵵, Ŭ�� ����ü ����
        ����ü�� Owner -> Dest ���� �̵��Ѵ�. (movepacket X ,Ŭ�� �˾Ƽ�)
        ��� Ŭ��� ��� ���(Ŭ�� a)�� ����ü�� ��� �͵��� ������.(�ִϸ��̼� ��)

    2. ������ ����ü�� �¾Ҵ��� �Ǻ�
         ��ũ��Ʈ���� �̺�Ʈ �Ŵ����� ����ü�� �¾Ҵ����� ��� ����.(����ü�� ���ٰ� collision�ߴµ� player�� ��� ��ũ��Ʈ �Ŵ������� ����?) 
        (����ü �״°� ������ �Ⱥ��� - ������ �˾Ƽ� �ð��� ������ �����ϴ� ������Ʈ���� ����ü�� ����) 
        �ȸ¾����� �׳� �״�� ���.(�ƹ��� �� �´´ٸ�, ����ü�� ��� Ŭ�� �˾Ƽ� �Ҹ��� ����)
        �¾����� ������ ����(Ŭ�� b) ����ü���� �¾Ҵٴ°� �������� ����� ����. 
        ������ ok������ �ϸ� ��� Ŭ������ Ŭ��b�� ����ü���� �¾Ҵٴ� ���� ����.(�� ��, �������� ���� Ŭ��b�� ������ ������ �� ����)
        �� ��, Ŭ��b�� ü�µ� ���̰� ��� ���������� ���� ����)
        ���嵵 Ŭ�� �ݿ�
        

    Skill Type
    : ����ü, ����, ����, ��

    ��Ÿ�� ����ü Ÿ���� ��ų
*/ 

extern bool IsInGame;
extern PlayerInfo MyPlayer;

void Send_CLogin(ClientServiceRef _service, wstring _userName);
void Send_CPickFaction(ClientServiceRef _service);
void Send_CPickChampion(ClientServiceRef _service, ChampionType _championType);
