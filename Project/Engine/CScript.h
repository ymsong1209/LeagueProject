#pragma once
#include "CComponent.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CGameObject.h"
#include "components.h"


enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC4,
};

struct tScriptParam
{
    SCRIPT_PARAM    eParam;
    void*           pData;    
    string          strDesc;
};

struct tScriptTexParam
{
    Ptr<CTexture>& tex;
    string strDesc;
};

class CCollider2D;

class CScript :
    public CComponent
{
private:  
    UINT                    m_iScriptType;
    vector<tScriptParam>    m_vecParam;
    vector<tScriptTexParam> m_vecTexParam;

public:
    void Destroy() { DestroyObject(GetOwner()); }
    void SetLifeSpan(float _Time) { GetOwner()->SetLifeSpan(_Time); }
    UINT GetScriptType() { return m_iScriptType; }
    const vector<tScriptParam>& GetScritpParam() { return m_vecParam; }
    vector<tScriptTexParam>& GetScriptTexParam() { return m_vecTexParam; }
    void SetScriptTexParam(Ptr<CTexture> pCurTex, const string& _Desc);


public:   
    virtual void finaltick() final {};

    virtual void BeginOverlap(CCollider2D* _Other) {}
    virtual void OnOverlap(CCollider2D* _Other) {}
    virtual void EndOverlap(CCollider2D* _Other) {}

    virtual void BeginOverlap(CCollider3D* _Other) {}
    virtual void OnOverlap(CCollider3D* _Other) {}
    virtual void EndOverlap(CCollider3D* _Other) {}

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _FILE) override {}

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override {}
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override {}

protected:
    void AddScriptParam(SCRIPT_PARAM eParam, void* _pData, const string& _Desc);
    void AddScriptTexParam(Ptr<CTexture>& Tex, const string& _Desc);


public:
    CScript(UINT _iScriptType);
    ~CScript();
};

