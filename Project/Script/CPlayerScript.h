#pragma once
#include <Engine\CScript.h>

class dtNavMesh;

class CPlayerScript :
    public CScript
{
private:
    UINT64      m_id;
    wstring     m_nickname;
    Faction     m_faction;
    bool        m_bHost;
    
    float       m_fSpeed;
    Vec3        m_vTargetPosTest;
    Vec3        m_vPrevPos;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

    Vec3 GetPrevPos() { return m_vPrevPos; }

    UINT64 GetPlayerID() { return m_id; }
    wstring GetNickName() {return m_nickname;}
    Faction GetFaction() {return m_faction;}
    bool   GetIsHost() { return m_bHost; }


    void SetPlayerID(UINT64 _id) { m_id = _id; }
    void SetNickName(wstring _nickname) { m_nickname = _nickname; }
    void SetFaction(Faction _fection) { m_faction = _fection; }
    void SetHost(bool _bHost) { m_bHost = _bHost; }

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

