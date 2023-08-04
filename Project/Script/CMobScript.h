#pragma once
#include "CUnitScript.h"
class CMobScript :
    public CUnitScript
{
protected:
    CMobScript(UINT ScriptType);
    
public:
    CMobScript();
    virtual ~CMobScript();

    CLONE(CMobScript);

protected:
    int                 m_iDropGold;

    float               m_fAggroRange;      // 

    CGameObject*        m_iAttackTargetObj;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CGameObject* GetTargetObj() { return m_iAttackTargetObj; }
    void SetAggroRange(float _range) { m_fAggroRange = _range; }
    float GetAggroRange() { return m_fAggroRange; }
};

