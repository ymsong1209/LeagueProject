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
    float               m_fAggroRange;      // 

    CGameObject*        m_iAttackTargetObj;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _collider) override;

public:
    CGameObject* GetTargetObj() { return m_iAttackTargetObj; }
    void SetAggroRange(float _range) { m_fAggroRange = _range; }
    float GetAggroRange() { return m_fAggroRange; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator) override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue) override;
};

