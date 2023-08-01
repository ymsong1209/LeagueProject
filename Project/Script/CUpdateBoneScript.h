#pragma once
#include  <Engine/CScript.h>

class CUpdateBoneScript :
    public CScript
{
private:
    CGameObject* m_pTrackObj;
    Vtx*         m_pTrackVertexMem;
    int          m_iTrackIdx;


public:
    virtual void tick() override;
    virtual void begin() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    void SetTrackObject(CGameObject* _Obj);
    CGameObject* GetTrackObject() { return m_pTrackObj; }

    CLONE(CUpdateBoneScript);

 

public:
    CUpdateBoneScript();
    ~CUpdateBoneScript();
};

 