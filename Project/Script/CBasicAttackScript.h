#pragma once
#include <Engine/CScript.h>

class CBasicAttackScript :
    public CScript
{
public:
    CBasicAttackScript();
    ~CBasicAttackScript();

    CLONE(CBasicAttackScript);

private:
    int             m_iUserID;          // ������
    CGameObject*    m_UserObj;

    int             m_iTargetID;        // �ǰ���
    CGameObject*    m_TargetObj;

    float           m_fProjectileSpeed;         // ����ü �ӵ�

    float           m_fDamage;                  // ������

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

    void SetUserID(int _i) { m_iUserID = _i; };
    void SetTargetID(int _i) { m_iTargetID = _i; }
};

