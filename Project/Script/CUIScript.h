#pragma once
#include <Engine\CScript.h>
#include <Engine\define.h>

class CUIScript :
    public CScript
{
private:
    CGameObject* m_UIFrontPanel;
    CGameObject* m_UIBackPanel;
    ChampionType CharacterType;
    SUMMONERS_SPELL SpellType_D;
    SUMMONERS_SPELL SpellType_F;

public:
    virtual void begin() override;
    virtual void tick() override;

public:

    CGameObject* GetUIFrontPanel() { return m_UIFrontPanel; }
    CGameObject* GetUIBackPanel() { return m_UIBackPanel; }

    ChampionType GetCharacterType() { return CharacterType; }
    void SetCharacterType(ChampionType _CharacterType) { CharacterType = _CharacterType; }

    SUMMONERS_SPELL GetSpellDType() { return SpellType_D; }
    void SetSpellDType(SUMMONERS_SPELL _SpellType) { SpellType_D = _SpellType; }
    SUMMONERS_SPELL GetSpellFType() { return SpellType_F; }
    void SetSpellFType(SUMMONERS_SPELL _SpellType) { SpellType_F = _SpellType; }

    void SetChampInFo(ChampionType _ChampType, SUMMONERS_SPELL _DSpell, SUMMONERS_SPELL _FSpell)
    { CharacterType = _ChampType; SpellType_D = _DSpell; SpellType_F = _FSpell; }

    CLONE(CUIScript);
public:
    CUIScript(SCRIPT_TYPE _Type);
    CUIScript();
    ~CUIScript();
};

