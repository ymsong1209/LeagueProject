#pragma once
#include <Engine\CScript.h>

class dtNavMesh;

class CPlayerScript :
    public CScript
{
private:
    float       m_fSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

private:
    void Shoot();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    dtNavMesh* loadNavMeshFromBinFile(const char* path);

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

