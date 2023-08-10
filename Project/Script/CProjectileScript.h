#pragma once
#include "CUnitScript.h"

class CProjectileScript :
    public CUnitScript
{
protected:
    CProjectileScript(UINT ScriptType);

public:
    CProjectileScript();
    virtual ~CProjectileScript();

    CLONE(CProjectileScript);

protected:

    CGameObject*    m_UserObj;          // 시전자 
    UINT64          m_iServerUserID;

    CGameObject*    m_TargetObj;        // 피격자
    UINT64          m_iServerTargetID;

    float           m_fProjectileSpeed; // 투사체 속도
    Vec3            m_vDir;             // 투사체 방향

    Vec3            m_vSpawnPos;        // 투사체 생성 위치
    
    float           m_fSkillRange;      // 스킬 사거리


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other)  override;

    void SetServerUserID(int _i) { m_iServerUserID = _i; }
    void SetServerTargetID(int _i) { m_iServerTargetID = _i; }
    void SetUserObj(CGameObject* _userObj) { m_UserObj = _userObj; }
    void SetTargetObj(CGameObject* _targetObj) { m_TargetObj = _targetObj; }

    UINT64  GetServerUserID() { return m_iServerUserID; }
    UINT64  GetServerTargetID() { return m_iServerTargetID; }
    CGameObject* GetUserObj() { return m_UserObj; }
    CGameObject* GetTargetObj() { return m_TargetObj; }

    void SetProjectileSpeed(float _f) { m_fProjectileSpeed = _f; }
    void SetDir(Vec3 _v) { m_vDir = _v; }
    void SetSpawnPos(Vec3 _v) { m_vSpawnPos = _v; }
    void SetSkillRange(float _f) { m_fSkillRange = _f; }

    //target이 죽었거나, m_bunitdead가 되었을때 처리해줘야하는함수
    virtual void Despawn() {}; 

};
