#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos; //UI���� �θ�, �ڽ� ��ü���� ������� ��ǥ
    Vec3    m_vRelativeScale; //�θ��� ũ�⿡�� �� ũ�Ⱑ ��Ƣ��ɼ��ֱ� ����
    Vec3    m_vRelativeRot; //ȸ��

    bool    m_bAbsolute;    // ��� �̵�, ũ�⸦ ���밪���� ����  

    Vec3    m_vRelativeDir[3]; //���⺤��
    Vec3    m_vWorldDir[3];

    Matrix  m_matWorldRot;      // ���� ȸ�� ���
    Matrix  m_matWorldScale;    // ���� ũ�� ���
    Matrix  m_matWorldPos;      // ���� ��ġ(�̵�) ���

    Matrix  m_matWorld; // ũ��, ȸ��, �̵� ������ ���ĳ���
    Matrix  m_matWorldInv; // ���� �����

    float   m_fGizmoBounding_Radius; //��������� Ŭ���� �ٿ�� ����(��ü ��������)
    bool    m_bGizmoObjExcept; // ��������� ���õ��� �ʾƾ��ϴ� ������Ʈ�� ��� true
    bool    m_bUseMouseOutLine; //���콺 ��ŷ �ƿ����� ����� ����Ϸ��� true


    bool    m_bIsShootingRay;  // �͵�,Player�� Ray�� ���.
    float   m_fRayRange;
    float   m_fOutlinethickness;

    bool    m_bBillBoard;
    bool    m_bCustomBillBoard;
    Vec3    m_vCustomBillBoardAngle;

    bool    m_bNoParentaffected; //�ƿ� �θ������Ʈ�� ����,������,�����̼� ������ �ȹް� ������� true 
                                //=> �ڽĿ�����Ʈ UIī�޶� ����, �θ� ����ī�޶� ���� �̶� �����ڽ��� ���ٺθ� ������ �޾� �̻��ϰ� ��ǥ���� ���ͼ� ��������

public:
    void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
    void SetRelativeScale(Vec3 _vScale) { m_vRelativeScale = _vScale; }
    void SetRelativeRot(Vec3 _vRot) { m_vRelativeRot = _vRot; }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); }

    // ��� �̵�, ũ�⸦ ���밪���� ����  
    void SetAbsolute(bool _Set) { m_bAbsolute = _Set; }
    const bool& GetAbsolute() { return m_bAbsolute; }

    Vec3 GetRelativePos() const { return m_vRelativePos; }
    Vec3 GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetRelativeRot() const { return m_vRelativeRot; }

    Vec3 GetRelativeDir(DIR_TYPE _type) const { return m_vRelativeDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { { return m_vWorldDir[(UINT)_type]; } }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    const Matrix& GetWorldInvMat() const { return m_matWorldInv; }

    const Matrix& GetWorldRotMat() { return m_matWorldRot; }
    const Matrix& GetWorldScaleMat() { return m_matWorldScale; }
    const Matrix& GetWorldPosMat() { return m_matWorldPos; }
    const Matrix& GetWorldMat() const { return m_matWorld; }

    const Matrix& GetWorldMatInv() const { return m_matWorldInv; }

    void SetWorldMat(const Matrix& _mat) { m_matWorld = _mat; }

    void SetGizmoBounding(float _Bounding) { m_fGizmoBounding_Radius = _Bounding; }
    float GetGizmoBounding() { return m_fGizmoBounding_Radius; }
    //��������� Ŭ���� �ٿ�� ����(��ü ��������)

    void SetGizmoObjExcept(bool _IsNoGizmoObj) { m_bGizmoObjExcept = _IsNoGizmoObj; }
    bool GetGizmoObjExcept() { return m_bGizmoObjExcept; }
    // ��������� ���õ��� �ʾƾ��ϴ� ������Ʈ�� ��� true


    void SetIsShootingRay(bool _ray) { m_bIsShootingRay = _ray; }
    bool GetIsShootingRay() { return m_bIsShootingRay; }

    void SetRayRange(float _Range) { m_fRayRange = _Range; }
    int  GetRayRange() { return m_fRayRange; }

    void SetUseMouseOutline(bool _IsUse) { m_bUseMouseOutLine = _IsUse; }
    bool GetUseMouseOutline() { return m_bUseMouseOutLine; }

    void SetOutlineThickness(float _Thickenss) { m_fOutlinethickness = _Thickenss; }
    float GetOutlineThickness() { return m_fOutlinethickness; }

    bool  IsBillBoard() { return m_bBillBoard; }
    void  SetBillBoard(bool _state) { m_bBillBoard = _state; }

    bool  IsUsingCustomBillBoard() { return m_bCustomBillBoard; }
    void  SetUsingCustomBillBoard(bool _state) { m_bCustomBillBoard = _state; }
    void  SetCustomBillBoardAngle(Vec3 _Angle) { m_vCustomBillBoardAngle = _Angle; }
    Vec3  GetCustomBillBoardAngle() { return m_vCustomBillBoardAngle; }

    void SetNoParentaffected(bool _IsNoAffected) { m_bNoParentaffected = _IsNoAffected; }
    bool GetNoParentaffected() { return m_bNoParentaffected; }

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CTransform);
public:
    CTransform();
    CTransform(const CTransform& _other);
    ~CTransform();
};


