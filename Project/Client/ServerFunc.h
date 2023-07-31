#pragma once

//#include <Script/CServerTypes.h>


enum class ObjectType {
    PLAYER,

    // 여기 TOP,BOTTOM, MID 미니언 별로 나눠야할듯
    MELEE_MINION,
    RANGED_MINION,
    SIEGE_MINION,
    SUPER_MINION,

    RAPTORS,
    WOLF,
    KRUG,
    DRAGON,
    BARON,

    TOWER,
    INHIBITOR,
    NEXUS,

    PROJECTILE,
    EFFECT,

    END,
};

enum ChampionType
{
    NONE,
    BLITZCRANK,
    JINX,
    AMUMU,
    MALPHITE,
};


struct AnimInfoPacket {
    uint16  targetId;
    bool    bRepeat;
    bool    blend;
    float   blendTime;

    uint16  animNameOffset;
    uint16  animNameCount;

    struct animNameItem {
        wchar_t animName;
    };

    bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size) {
        if (animNameOffset + animNameCount * sizeof(animNameItem) > packetSize)
            return false;

        size += animNameCount * sizeof(animNameItem);
        return true;
    }
};

struct ObjectMove
{
public:
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
public:
    ObjectMove() {}
    ObjectMove(int _LV, float _HP, float _MP, float _AttackPower, float _DefencePower, ObjectMove::MoveDir _moveDir, ObjectMove::Pos _pos, CC _CC)
        : LV(_LV)
        , HP(_HP)
        , MP(_MP)
        , AttackPower(_AttackPower)
        , DefencePower(_DefencePower)
        , moveDir(_moveDir)
        , pos(_pos)
        , CC(_CC)
    {}
    ~ObjectMove() {}

    int   LV;
    float HP;
    float MP;
    float AttackPower;
    float DefencePower;

    MoveDir moveDir;
    Pos pos;
    CC  CC;
};

struct ObjectInfo {
    ObjectInfo() {}
    ObjectInfo(uint64 _objectId, ObjectType _objectType, Faction _faction, Lane _lane, ObjectMove _objectMove)
        : objectId(_objectId)
        , objectType(_objectType)
        , faction(_faction)
        , lane(_lane)
        , objectMove(_objectMove)
    {}
    ~ObjectInfo() {}

    uint64 objectId;
    ObjectType objectType;
    Faction    faction;
    Lane       lane;
    ObjectMove objectMove;
};

struct PlayerInfo
{
    uint64  id;
    wstring nickname;
    Faction faction;
    ChampionType champion;
    bool host;

    ObjectMove posInfo;
};

struct PlayerInfoPacket
{
    uint64  id;
    Faction faction;
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

enum WaitingStatus
{
    WAITING = 0,
    RUN = 1,
};

extern bool IsInGame;
extern PlayerInfo MyPlayer;

void Send_CLogin(ClientServiceRef _service, wstring _userName);
void Send_CPickFaction(ClientServiceRef _service);
void Send_CPickChampion(ClientServiceRef _service, ChampionType _championType);

