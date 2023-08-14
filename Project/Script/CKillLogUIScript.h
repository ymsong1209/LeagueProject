#pragma once
#include <Engine\CScript.h>

#include <deque>
#include <string>
#include <chrono>
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>

struct KillLog
{
    ChampionType killer;
    ChampionType victim;
    CGameObject* KillLogObj;
    Vec3 position;
    float remainingTime;
    float targetY; // �߰��� targetY �Ӽ�
    float initialX = 1.f; // ���� X ��ǥ
    float moveSpeed = 370.0f; // ������ �ӵ� (�ʿ信 ���� ����)
};

class KillLogManager
{
public:
    float displayTime; //  8.0f
    float shiftSpeed;  //  50.0f
    float lineHeight;  //  20.0f
    vector<KillLog> killLogs;
    bool needToShift = false;
    Vec3 initialPosition = Vec3(700, 100, 327); // Default position

    void SpawnChampRect(ChampionType _Killer, CGameObject* _Parent, Vec3 _Pos);

    void Update(float deltaTime)
    {
        if (killLogs.empty()) // ų�αװ� ��������� �ƹ� �͵� ���� �ʰ� ��ȯ
            return;

        MoveKillLogsHorizontally(deltaTime);
        ShiftKillLogsDown(deltaTime);
        RemoveExpiredKillLogs();
    }

    const vector<KillLog>& GetKillLogs() const { return killLogs; }
    void AddKillLog(ChampionType _killer, ChampionType _victim, Faction _Faction);

private:
    void ShiftKillLogsDown(float deltaTime)
    {
        if (needToShift)
        {
            float shiftAmount = shiftSpeed * deltaTime;
            for (auto& log : killLogs)
            {
                log.position.y -= shiftAmount; // �Ʒ��� �̵�
                if (log.position.y <= log.targetY)
                    log.position.y = log.targetY;
            }

            if (killLogs.front().position.y == killLogs.front().targetY)
                needToShift = false;
        }
    }

    void MoveKillLogsHorizontally(float deltaTime)
    {
        float moveAmount = 400.f * deltaTime;
        for (auto& log : killLogs)
        {
            log.position.x += moveAmount;
            if (log.position.x >= initialPosition.x)
                log.position.x = initialPosition.x;
        }
    }

    void UpdateTargetPositions()
    {
        float targetY = initialPosition.y; // Default position
        for (auto& log : killLogs)
        {
            log.targetY = targetY;
            targetY += lineHeight; // ���� ��ġ
        }
    }

    void RemoveExpiredKillLogs()
    {
        for (auto it = killLogs.begin(); it != killLogs.end();)
        {
            it->remainingTime -= DT;
            if (it->remainingTime <= 0)
            {
                //������Ʈ ����
                DestroyObject(it->KillLogObj);
                it = killLogs.erase(it);
                needToShift = true;
                UpdateTargetPositions();
            }
            else
                ++it;
        }
    }

};

enum class SpecificType 
{
    CHAMPION,
    JUNGLE_MOB,
    MINION,
    AllTURRET,
    Other
};

enum class AnnounceType
{
    GOTKILLED, //óġ���߽��ϴ�.
    KILLEDENEMY, //���� óġ�߽��ϴ�.
    ALLY_HASBEENSLAIN, //�Ʊ��� óġ���߽��ϴ�.
    TURRET_DESTROY,
    INHIBITOR_DESTROY,
};


struct AnnounceMessage
{
    AnnounceType type;
    Faction KillerFaction;

    ChampionType KillerChampType;
    ChampionType VictimChampType;

    UnitType KillerUnitType;
    UnitType VictimUnitType;
};


class CKillLogUIScript :
    public CScript
{
private:
    ChampionType m_KillChamp;
    ChampionType m_DeathChamp;
    KillLogManager killLogManager;

    CGameObject* CurAnnounce;
    float       m_fAnnounceTimer;

    queue<AnnounceMessage> announcementsQueue;

    float displayTimer = 0.0f;
    const float displayDuration = 3.0f; // ������Ʈ�� ȭ�鿡 ǥ�õǴ� �ð� (3��)

    CGameObject* EndOfGameObj;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void IconSetting(CGameObject* _Obj, ChampionType ChampType);
    void DisplayAnnounce(ChampionType _Killer, ChampionType _Victim, AnnounceType _Type);

    SpecificType GetSpecificType(UnitType type);
    void AnnouncePlayerIcon(CGameObject* _Parent, ChampionType _Killer, ChampionType _Victim, AnnounceType _Type);

    void AddAnnouncement(AnnounceType type, Faction _KillerFaction , 
        ChampionType _KillerChamp = ChampionType::NONE , ChampionType _VictimChamp = ChampionType::NONE,
        UnitType _KillerUnitType = UnitType::END, UnitType _VictimUnitType = UnitType::END)
    {
        AnnounceMessage message{ type , _KillerFaction, _KillerChamp, _VictimChamp, _KillerUnitType, _VictimUnitType};
        announcementsQueue.push(message);
    }

    void AnnounceLogUpdate(float deltaTime);
    void DisplayAnnounceAll(AnnounceType type, Faction _KillerFaction, ChampionType _KillerChamp, ChampionType _VictimChamp, UnitType _KillerUnitType, UnitType _VictimUnitType);
    void DisplayAnnounceTurret(ChampionType _Killer, Faction _KillerFaction, AnnounceType _Type);

    void DisplayCS(CGameObject* _KillerChamp, UnitType _UnitType);

    CGameObject* SpawnAnnouncePanel(Vec3 _Scale, wstring _Name);
    CGameObject* SpawnIconPanel(Vec3 _Scale, wstring _Name);


    void Announce_EndofGame(Faction _VictimFaction);

public:
    CLONE(CKillLogUIScript);

public:
    CKillLogUIScript();
    ~CKillLogUIScript();
};

