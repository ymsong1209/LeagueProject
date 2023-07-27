#pragma once
#include <Engine\serverpacket.h>


enum SkillType {
    AUTO_ATTACK,
    JINX_W,
    JINX_E,
    JINX_R,
};

struct SkillInfo {
    uint64 OwnerId;
    uint64 TargetId;
    SkillType skillType;
};

struct AnimInfo {
    uint16 animIdx;
    bool blend;
    float blendTime;

    uint16 animNameOffset;
    uint16 animNameCount;

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

//enum class FactionType
//{
//    BLUE = 0,
//    RED = 1,
//    NONE = 2, // 비선공몬스터(모두를 적대함. 선빵x)
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
    MELEE_MINION,
    CASTER_MINION,
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

    END,
};

enum class CC_TYPE
{
    STUN, // 기절
    SLOW, // 둔화
    SILENCE, // 침묵
    SNARE, // 속박
    BLEED, // 출혈
    AIRBORNE, // 에어본
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

extern bool IsInGame;
extern PlayerInfo MyPlayer;

void Send_CLogin(ClientServiceRef _service, wstring _userName);
void Send_CPickFaction(ClientServiceRef _service);
void Send_CPickChampion(ClientServiceRef _service, ChampionType _championType);

