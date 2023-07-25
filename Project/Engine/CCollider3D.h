#pragma once
#include "CComponent.h"


struct FogOfWall {

    Matrix          m_matCollider3D;

};

class CCollider3D :
    public CComponent
{
private:
    Vec3            m_vOffsetPos;
    Vec3            m_vOffsetScale;
    bool            m_bAbsolute;
    COLLIDER3D_TYPE m_Shape;

    Matrix          m_matColliderScale;
    Matrix          m_matColliderPos;

    Matrix          m_matCollider3D;    // Collider �� �������
    int             m_iCollisionCount;    // �浹 Ƚ��

    bool            m_bIsCollidedFromRay; // Camera���� Mouse��ŷ�� Ray�� �浹 ��������

    bool            m_bIsWall;           // Wall ����� �޾Ƽ� ������ �Ȱ� ����� ������

    bool            m_bDrawCollision;   //�ݸ����� ���̰����� ����

public:
    virtual void finaltick() override;

public:
    void SetOffsetPos(Vec3 _vOffsetPos) { m_vOffsetPos = Vec3(_vOffsetPos.x, _vOffsetPos.y, _vOffsetPos.z); }
    void SetOffsetScale(Vec3 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, _vOffsetScale.z); }
    void SetAbsolute(bool _bSet) { m_bAbsolute = _bSet; }
    void SetCollider3DType(COLLIDER3D_TYPE _Type) { m_Shape = _Type; }

    Vec3 GetOffsetPos() { return m_vOffsetPos; }
    Vec3 GetOffsetScale() { return m_vOffsetScale; }
    bool GetColliderAbsolute() { return m_bAbsolute; }
    COLLIDER3D_TYPE GetColliderShape() { return m_Shape; }

    const Matrix& GetColliderPosMat() { return m_matColliderPos; }
    const Matrix& GetColliderScaleMat() { return m_matColliderScale; }
    const Matrix& GetColliderWorldMat() { return m_matCollider3D; }
    const COLLIDER3D_TYPE& GetCollider3DType() { return m_Shape; }

    const int& GetCollisionCount() { return m_iCollisionCount; }

    bool  IsCollidedFromRay() { return m_bIsCollidedFromRay; }
    void  SetCollidedFromRay(bool _state) { m_bIsCollidedFromRay = _state; }

    bool  IsWall() { return m_bIsWall; }
    void  SetIsWall(bool _wall) { m_bIsWall = _wall; }

    bool GetDrawCollision() { return m_bDrawCollision; }
    void SetDrawCollision(bool _IsDraw) { m_bDrawCollision = _IsDraw; }
 

public:
    void BeginOverlap(CCollider3D* _Other);
    void OnOverlap(CCollider3D* _Other);
    void EndOverlap(CCollider3D* _Other);

    void BeginRayOverlap();
    void OnRayOverlap();
    void EndRayOverlap();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
   
    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;


    CLONE(CCollider3D);
public:
    CCollider3D();
    CCollider3D(const CCollider3D& _other);
    ~CCollider3D();
};