#pragma once
#include <Engine\CScript.h>

class CCoolDownUIScript :
    public CScript
{
private:
    float m_dCoolDown;


public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CCoolDownUIScript);

public:
    CCoolDownUIScript();
    ~CCoolDownUIScript();
};

