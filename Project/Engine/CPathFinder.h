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

    Vec3                m_vNextPos;      // ���ؾ� �� ���� ��ġ
    Vec3                m_vPrevEndPos;   //���� ������(�������� ������ Ŭ���ҽ� �߻��ϴ� ������ �����ϱ� ���� : ���� �������� �������� ���� ��ġ�ų� ������ �����ٸ�, �ٽ� ��ã�⸦ �������� ����)

public:
    virtual void tick() override;
    virtual void finaltick() override {};

public:
    void FindPath(Vec3 endPos);
    void FindNextPath();

    //void FindPathMousePicking(); //���� ���ݸ����� ���콺 ��ŷ������ �������� return

public:
    Vec3 GetNextPos() { return m_vNextPos; }
    void ClearPath();


public:
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _File) override {};

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override {};
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override {};

};

