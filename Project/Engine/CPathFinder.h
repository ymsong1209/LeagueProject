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
    Vec3                m_vPrevEndPos;   //이전 목적지(같은지점 여러번 클릭할시 발생하는 떨림을 개선하기 위함 : 현재 목적지와 비교했을때 같은 위치거나 굉장히 가깝다면, 다시 길찾기를 진행하지 않음)

public:
    virtual void tick() override;
    virtual void finaltick() override {};

public:
    void FindPath(Vec3 endPos);
    void FindNextPath();

    //void FindPathMousePicking(); //현재 맵콜리전과 마우스 피킹지점의 교차점을 return

public:
    Vec3 GetNextPos() { return m_vNextPos; }
    void ClearPath();


public:
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _File) override {};

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override {};
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override {};

};

