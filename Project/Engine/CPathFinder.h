#pragma once
#include "CComponent.h"

class CPathFinder :
    public CComponent
{
public:
    CPathFinder();
    ~CPathFinder();

public:
    CLONE(CPathFinder);

private:
    vector<Vec3>        m_vecPath;
    int                 m_iPathCount;
    int                 m_iCurPathIdx;

    Vec3                m_vNextPos;      // 향해야 할 다음 위치

public: 
    virtual void tick() override;
    virtual void finaltick() override {};

public:
    void FindPath(Vec3 endPos);
    void FindNextPath();

    void FindPathMousePicking(); //현재 맵콜리전과 마우스 피킹지점의 교차점을 return

public:
    Vec3 GetNextPos() { return m_vNextPos; }


public:
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _File) override {};

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override {};
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override {};

};

