#pragma once
#include <Engine\CScript.h>

class dtNavMesh;

class CPlayerScript :
    public CScript
{
private:
    UINT64      m_id;
    wstring     m_nickname;
    FactionType m_faction;
    
    float       m_fSpeed;
    Vec3        m_vTargetPosTest;
    Vec3        m_vPrevPos;
    PLAYER_STATE    CurState;
    PLAYER_STATE    PrevState;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;


    UINT64 GetPlayerID() { return m_id; }
    wstring GetNickName() {return m_nickname;}
    FactionType GetFaction() {return m_faction;}

    void SetPlayerID(UINT64 _id) { m_id = _id; }
    void SetNickName(wstring _nickname) { m_nickname = _nickname; }
    void SetFaction(FactionType _fection) { m_faction = _fection; }

private:
    void Shoot();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;


    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

