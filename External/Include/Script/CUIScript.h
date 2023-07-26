#pragma once
#include <Engine\CScript.h>
#include <Engine\define.h>

class CUIScript :
    public CScript
{
private:
    CGameObject* m_UIFrontPanel;
    CGameObject* m_UIBackPanel;
    CHARACTER_TYPE CharacterType;
    SUMMONERS_SPELL SpellType_D;
    SUMMONERS_SPELL SpellType_F;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CGameObject* GetUIFrontPanel() { return m_UIFrontPanel; }
    CGameObject* GetUIBackPanel() { return m_UIBackPanel; }

    CHARACTER_TYPE GetCharacterType() { return CharacterType; }
    void SetCharacterType(CHARACTER_TYPE _CharacterType) { CharacterType = _CharacterType; }

    SUMMONERS_SPELL GetSpellDType() { return SpellType_D; }
    void SetSpellDType(SUMMONERS_SPELL _SpellType) { SpellType_D = _SpellType; }
    SUMMONERS_SPELL GetSpellFType() { return SpellType_F; }
    void SetSpellFType(SUMMONERS_SPELL _SpellType) { SpellType_F = _SpellType; }

    void SetChampInFo(CHARACTER_TYPE _ChampType, SUMMONERS_SPELL _DSpell, SUMMONERS_SPELL _FSpell) 
    { CharacterType = _ChampType; SpellType_D = _DSpell; SpellType_F = _FSpell; }

    CLONE(CUIScript);
public:
    CUIScript(SCRIPT_TYPE _Type);
    CUIScript();
    ~CUIScript();
};

