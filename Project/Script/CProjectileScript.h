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

    CGameObject*    m_UserObj;          // ������ 
    UINT64          m_iServerUserID;

    CGameObject*    m_TargetObj;        // �ǰ���
    UINT64          m_iServerTargetID;

    float           m_fProjectileSpeed; // ����ü �ӵ�
    Vec3            m_vDir;             // ����ü ����

    Vec3            m_vSpawnPos;        // ����ü ���� ��ġ
    
    float           m_fSkillRange;      // ��ų ��Ÿ�


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

    //target�� �׾��ų�, m_bunitdead�� �Ǿ����� ó��������ϴ��Լ�
    virtual void Despawn() {}; 

};
