#pragma once
#include "CUnitScript.h"

class CStructureScript :
    public CUnitScript
{
protected:
    CStructureScript(UINT ScriptType);

public:
    CStructureScript();
    virtual ~CStructureScript();

    CLONE(CStructureScript);

protected:
    Lane        m_eLane;
    
    bool        m_bAttackable;      // 공격 가능한지  


public:
    virtual void begin() override;
    virtual void tick() override;

    bool        IsAttackable() { return m_bAttackable; }

    Lane        GetLane() { return m_eLane; }
};

