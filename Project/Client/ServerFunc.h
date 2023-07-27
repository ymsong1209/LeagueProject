#pragma once
#include <Engine\serverpacket.h>


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
    미니언타입 : 근접, 원거리, 대포, 슈퍼
    진영
    id
    피통
    공격력


    ProjectileInfo
    Owner : 누가 쏜건지
    Dest : 누가 맞는지 (여기 비었으면 논타겟팅  0초기화)
    time : 투사체 생존시간

    1. a가 스킬버튼을 누름
        쏘는 사람(클라 a)이 서버한테 허락을 맡음.
        서버가 ok 패킷에 모든 클라이언트에게 쏘는 사람(클라 a)이 투사체를 쏜다고 알림.
        방장도, 클라도 투사체 생성
        투사체는 Owner -> Dest 까지 이동한다. (movepacket X ,클라 알아서)
        모든 클라는 쏘는 사람(클라 a)이 투사체를 쏘는 것들을 실행함.(애니메이션 등)

    2. 방장은 투사체가 맞았는지 판별
         스크립트에서 이벤트 매니저에 투사체가 맞았는지를 들고 있음.(투사체가 가다가 collision했는데 player면 어디 스크립트 매니저에서 보관?) 
        (투사체 죽는건 서버에 안보냄 - 서버두 알아서 시간뒤 서버가 관리하는 오브젝트에서 투사체를 없앰) 
        안맞았으면 그냥 그대로 출력.(아무도 안 맞는다면, 투사체는 모든 클라가 알아서 소멸할 거임)
        맞았으면 방장은 누가(클라 b) 투사체에게 맞았다는걸 서버한테 허락을 구함. 
        서버가 ok판정을 하면 모든 클라한테 클라b가 투사체한테 맞았다는 것을 보냄.(이 때, 방장한테 받은 클라b가 변동된 사항을 다 보냄)
        이 때, 클라b가 체력도 깎이고 등등 수정사항을 같이 보냄)
        방장도 클라도 반영
        

    Skill Type
    : 투사체, 버프, 광역, 덫

    평타는 투사체 타겟팅 스킬
*/ 

extern bool IsInGame;
extern PlayerInfo MyPlayer;

void Send_CLogin(ClientServiceRef _service, wstring _userName);
void Send_CPickFaction(ClientServiceRef _service);
void Send_CPickChampion(ClientServiceRef _service, ChampionType _championType);
