#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer*         m_arrLayer[MAX_LAYER];
    LEVEL_STATE     m_State;


    bool            m_bIsDebugOutlineShouldShow;

public:
    void begin();
    void RegisterObject();
    void tick();
    void finaltick(); 

public:
    CLayer* FindLayerByName(const wstring& _strName);
    CLayer* GetLayer(int _iLayerIdx) { assert(!(_iLayerIdx < 0)); return m_arrLayer[_iLayerIdx]; }

    CGameObject* FindObjectByID(UINT _id);

    CGameObject* FindObjectByName(const wstring& _Name);
    void FindObjectByName(const wstring& _Name, vector<CGameObject*>& _Out);

    CGameObject* FindParentObjectByName(const wstring& _Name);
    void FindParentObjectByName(const wstring& _Name, vector<CGameObject*>& _Out);

    void AddGameObject(CGameObject* _Object, int _iLayerIdx, bool _Move);
    void AddGameObject(CGameObject* _Object, const wstring& _LayerName, bool _Move);
    
    void ChangeState(LEVEL_STATE _State);
    LEVEL_STATE GetState() { return m_State; }


    bool GetIsDebugOutlineShouldShow() { return m_bIsDebugOutlineShouldShow; }

private:
    // 등록된 GameObject 제거
    void clear();

    CLONE(CLevel);
public:
    CLevel();
    ~CLevel();

    friend class CLevelMgr;
};

