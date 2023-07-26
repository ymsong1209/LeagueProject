#pragma once
#include <Engine\CScript.h>

class CUIScript :
    public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

    CGameObject* m_UIFrontPanel;
    CGameObject* m_UIBackPanel;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CGameObject* GetUIFrontPanel() { return m_UIFrontPanel; }
    CGameObject* GetUIBackPanel() { return m_UIBackPanel; }

    CLONE(CUIScript);
public:
    CUIScript(SCRIPT_TYPE _Type);
    CUIScript();
    ~CUIScript();
};

