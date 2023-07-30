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
    int             m_iUserID;          // 시전자
    CGameObject*    m_UserObj;

    int             m_iTargetID;        // 피격자
    CGameObject*    m_TargetObj;

    float           m_fProjectileSpeed;         // 투사체 속도

    float           m_fDamage;                  // 데미지

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

    void SetUserID(int _i) { m_iUserID = _i; };
    void SetTargetID(int _i) { m_iTargetID = _i; }
};

