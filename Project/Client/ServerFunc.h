#pragma once



enum class FactionType
{
    BLUE = 0,
    RED = 1,
};

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

struct PositionInfo
{
    // State
    // Direction

    int32 posX;
    int32 posY;
    int32 posZ;
};

struct PlayerInfo
{
    wstring nickname;
    uint64  id;
    FactionType faction;
    ChampionType champion;

    PositionInfo posInfo;

    // Vec3 Pos;
};

extern PlayerInfo MyPlayer;

void SendCLogin(ClientServiceRef _service, wstring _userName);

