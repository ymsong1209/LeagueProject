#pragma once
#include <Engine\CScript.h>

class CItem 
{
private:
    wstring m_name;

public:
    wstring GetName() { return m_name; }
    void SetName(wstring _Name) {  m_name = _Name; }

    // Other item properties go here, such as icon texture, etc.

public:
    CItem() {};
    ~CItem() {};
};


class CInventoryUIScript :
    public CScript
{
private:
    CItem* m_Inventory[6];
    vector<CGameObject*> m_vecItemObj;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void PosSetting(int _i);

    CLONE(CInventoryUIScript);

public:
    CInventoryUIScript();
    ~CInventoryUIScript();
};




