#pragma once
#include <Engine\CScript.h>
#include "CUIScript.h"

class CCharacterUIScript :
    public CUIScript
{
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    CGameObject* CharacterImage;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CCharacterUIScript);
public:
    CCharacterUIScript();
    ~CCharacterUIScript();
};

