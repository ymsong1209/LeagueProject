#pragma once

//#include <Script/CServerTypes.h>


struct AnimInfoPacket {
    uint16  targetId;       // 애니메이션이 변경될 오브젝트의 id
    bool    bRepeat;        // 애니메이션 반복 여부
    bool    bRepeatBlend;   // 블렌드 반복 여부
    bool    blend;          // 블렌드 사용여부
    float   blendTime;
    float   animSpeed;      // 애니메이션 속도

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

struct SoundInfoPacket
{
    struct vec3Server
    {
        float x;
        float y;
        float z;
    };

    DimensionType   dimensionType;
    Faction         faction;
    int             iLoopCount;
    float           fVolume;
    bool            bOverlap;
    float           fRange;
    vec3Server      soundPos;

    uint16 soundNameOffset;
    uint16 soundNameCount;

    struct soundNameItem {
        wchar_t soundName;
    };

    bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size) {
        if (soundNameOffset + soundNameCount * sizeof(soundNameItem) > packetSize)
            return false;

        size += soundNameCount * sizeof(soundNameItem);
        return true;
    }
};
struct MtrlInfoPacket
{
    UINT64 targetId;
    int iMtrlIndex;
    TEX_PARAM  tex_param;
    bool IsSetTexParamUsage;

    uint16 texNameOffset;
    uint16 texNameCount;

    uint16 mtrlNameOffset;
    uint16 mtrlNameCount;

    struct texNameItem {//예시 L"texture\\FBXTexture\\alphaTex.png"
        wchar_t texName;
    };

    struct mtrlNameItem {
        wchar_t mtrlName;
    };

    bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size) {
        if (texNameOffset + texNameCount * sizeof(texNameItem) > packetSize)
            return false;

        size += texNameCount * sizeof(texNameItem);

        if (mtrlNameOffset + mtrlNameCount * sizeof(mtrlNameItem) > packetSize)
            return false;

        size += mtrlNameCount * sizeof(mtrlNameItem);
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
    ObjectMove(int _LV, float _HP, float _MP, float _AttackPower, float _DefencePower, float MaxHP, float MaxMP, bool bUnitDead, ObjectMove::MoveDir _moveDir, ObjectMove::Pos _pos, UINT _CC)
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
    float MaxHP;
    float MaxMP;
    bool  bUnitDead;

    MoveDir moveDir;
    Pos pos;
    UINT  CC;
};

struct ObjectInfo {
    ObjectInfo() {}
    ObjectInfo(uint64 _objectId, UnitType _unitType, Faction _faction, Lane _lane, ObjectMove _objectMove)
        : objectId(_objectId)
        , unitType(_unitType)
        , faction(_faction)
        , lane(_lane)
        , objectMove(_objectMove)
    {}
    ~ObjectInfo() {}

    uint64     objectId;
    UnitType   unitType;
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

