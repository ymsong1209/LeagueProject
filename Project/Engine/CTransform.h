#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos; //UI같은 부모, 자식 객체간의 상대적인 좌표
    Vec3    m_vRelativeScale; //부모의 크기에다 내 크기가 뻥튀기될수있기 때문
    Vec3    m_vRelativeRot; //회전

    bool    m_bAbsolute;    // 상대 이동, 크기를 절대값으로 지정  

    Vec3    m_vRelativeDir[3]; //방향벡터
    Vec3    m_vWorldDir[3];

    Matrix  m_matWorldRot;      // 월드 회전 행렬
    Matrix  m_matWorldScale;    // 월드 크기 행렬
    Matrix  m_matWorldPos;      // 월드 위치(이동) 행렬

    Matrix  m_matWorld; // 크기, 회전, 이동 정보를 합쳐놓음
    Matrix  m_matWorldInv; // 월드 역행렬

    float   m_fGizmoBounding_Radius; //기즈모한테 클릭될 바운딩 범위(구체 반지름값)
    bool    m_bGizmoObjExcept; // 기즈모한테 선택되지 않아야하는 오브젝트일 경우 true
    bool    m_bUseMouseOutLine; //마우스 피킹 아웃라인 출력을 사용하려면 true


    bool    m_bIsShootingRay;  // 와드,Player는 Ray를 쏜다.
    float   m_fRayRange;
    float   m_fOutlinethickness;

    bool    m_bBillBoard;
    bool    m_bCustomBillBoard;
    Vec3    m_vCustomBillBoardAngle;

    bool    m_bNoParentaffected; //아예 부모오브젝트의 포즈,스케일,로테이션 영향을 안받고 싶은경우 true 
                                //=> 자식오브젝트 UI카메라가 직교, 부모가 메인카메라 원근 이라서 직교자식이 원근부모 영향을 받아 이상하게 좌표값이 나와서 만들었어요

public:
    void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
    void SetRelativeScale(Vec3 _vScale) { m_vRelativeScale = _vScale; }
    void SetRelativeRot(Vec3 _vRot) { m_vRelativeRot = _vRot; }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); }

    // 상대 이동, 크기를 절대값으로 지정  
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
    //기즈모한테 클릭될 바운딩 범위(구체 반지름값)

    void SetGizmoObjExcept(bool _IsNoGizmoObj) { m_bGizmoObjExcept = _IsNoGizmoObj; }
    bool GetGizmoObjExcept() { return m_bGizmoObjExcept; }
    // 기즈모한테 선택되지 않아야하는 오브젝트일 경우 true


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


