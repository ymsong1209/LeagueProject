#pragma once



//enum class FactionType
//{
//    BLUE = 0,
//    RED = 1,
//};

enum WaitingStatus
{
    WAITING = 0,
    RUN = 1,
};
enum ChampionType
{
    BLITZCRANK,
    JINX,
    AMUMU,
    MALPHITE,
};

struct PlayerMove
{
    enum PlayerState
    {
        IDLE = 0,
        MOVE = 1,
    };

    struct MoveDir
    {
        float x;
        float y;
        float z;
    };

    struct Pos
    {
        float x;
        float y;
        float z;
    };

    PlayerState state;
    MoveDir moveDir;
    Pos pos;
};

struct PlayerInfo
{
    uint64  id;
    wstring nickname;
    FactionType faction;
    ChampionType champion;

    PlayerMove posInfo;
};
struct PlayerInfoPacket
{
    uint64  id;
    FactionType faction;
    ChampionType champion;

    PlayerMove posInfo;

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




extern PlayerInfo MyPlayer;

void Send_CLogin(ClientServiceRef _service, wstring _userName);
void Send_CPickFaction(ClientServiceRef _service);
void Send_CPickChampionAndStart(ClientServiceRef _service, ChampionType _championType);
void Send_CMove(ClientServiceRef _service, PlayerMove _move);