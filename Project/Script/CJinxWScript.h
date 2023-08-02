#pragma once
#include <Engine/CScript.h>

class CJinxWScript :
    public CScript
{
public:
    CJinxWScript();
    ~CJinxWScript();

    CLONE(CJinxWScript);

private:
    int             m_iUserID;          // ������
    CGameObject*    m_UserObj;
    UINT64          m_iServerUserID;

    int             m_iTargetID;        // �ǰ���
    CGameObject*    m_TargetObj;
    UINT64          m_iServerTargetID;

    // ������
    UINT64          m_iServerID;        // ����ü ������ ���� ���̵�

    Vec3            m_vSpawnPos;        // ���� ��ġ

    float           m_fFaceRot;                 // ����ü ����
    float           m_fProjectileSpeed;         // ����ü �ӵ�
    float           m_fSkillRange;

    float           m_fDamage;                  // ������

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void OnOverlap(CCollider2D* _Other)  override;

    void SetUserID(int _i) { m_iUserID = _i; };
    void SetTargetID(int _i) { m_iTargetID = _i; }
    void SetServerUserID(int _i) { m_iServerUserID = _i; }
    void SetServerTargetID(int _i) { m_iServerTargetID = _i; }
    void SetServeID(int _i) { m_iServerID = _i; }
    void SetUserObj(CGameObject* _userObj) { m_UserObj = _userObj; }
    void SetTargetObj(CGameObject* _targetObj) { m_TargetObj = _targetObj; }

    UINT64  GetServerUserID() { return m_iServerUserID; }
    UINT64  GetServerTargetID() { return m_iServerTargetID; }
    UINT64  GetServerID() { return m_iServerID; }

    Vec3    GetSpawnPos() { return m_vSpawnPos; }
    void    SetSpawnPos(Vec3 _v) { m_vSpawnPos = _v; }

    void    SetFaceRot(float _f) { m_fFaceRot = _f; }
};

