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
    float targetY; // 추가된 targetY 속성
    float initialX = 1.f; // 시작 X 좌표
    float moveSpeed = 370.0f; // 움직임 속도 (필요에 따라 수정)
};

class KillLogManager
{
public:
    float displayTime; // e.g., 8.0f
    float shiftSpeed;  // e.g., 50.0f
    float lineHeight;  // e.g., 20.0f
    vector<KillLog> killLogs;
    bool needToShift = false;
    Vec3 initialPosition = Vec3(700, 100, 327); // Default position

    void SpawnChampRect(ChampionType _Killer, CGameObject* _Parent, Vec3 _Pos);

    void Update(float deltaTime)
    {
        if (killLogs.empty()) // 킬로그가 비어있으면 아무 것도 하지 않고 반환
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
                log.position.y -= shiftAmount; // 아래로 이동
                if (log.position.y <= log.targetY)
                    log.position.y = log.targetY;
            }

            if (killLogs.front().position.y == killLogs.front().targetY)
                needToShift = false;
        }
    }

    void MoveKillLogsHorizontally(float deltaTime)
    {
        float moveAmount = 300.f * deltaTime;
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
            targetY += lineHeight; // 위로 배치
        }
    }

    void RemoveExpiredKillLogs()
    {
        for (auto it = killLogs.begin(); it != killLogs.end();)
        {
            it->remainingTime -= DT;
            if (it->remainingTime <= 0)
            {
                //오브젝트 삭제
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


class CKillLogUIScript :
    public CScript
{
private:
    ChampionType m_KillChamp;
    ChampionType m_DeathChamp;
    KillLogManager killLogManager;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


    SpecificType GetSpecificType(UnitType type);



public:
    CLONE(CKillLogUIScript);

public:
    CKillLogUIScript();
    ~CKillLogUIScript();
};
