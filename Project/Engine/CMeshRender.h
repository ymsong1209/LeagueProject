#pragma once
#include "CRenderComponent.h"

// Texture가 움직여야한다면 어떤 형태로 움직이는가
enum class eTexMovingStyle
{
    NONE,
    HORIZONTAL,
    VERTICAL,
    LINEAR,
    PARABOLA,
    SIN,
    COS,
    END,
};

// 대상이 되는 Texture는 어떤 용도로 사용되는 것인가
enum class eTargetTexture
{
    OUTPUT,                 // 수업때 했던 Fighter.bmp처럼 출력이 목적
    PUNCTURE,               // 출력된 사진에 대해서 구멍을 뚫는 용도
    END,
};

struct MovingStruct
{
    eTargetTexture   TargetTex;
    eTexMovingStyle  MovingStyle;
    Vec4            FuncValue;                  // Texture가 움직일때 각각의 함수에 맞는 계수값
    Vec2            PreviousPos;
};


class CMeshRender :
    public CRenderComponent
{
private:
    vector<MovingStruct>    m_vMovingVec;
    bool                    m_bIsUsingMovingVec;
 

public:
    virtual void finaltick() override;
    virtual void render() override;


public:
    void SetOutputTexture(Ptr<CTexture> _Tex) { GetMaterial(0)->SetTexParam(TEX_0, _Tex); }
    void SetPunctureTexture(Ptr<CTexture> _Tex) { GetMaterial(0)->SetTexParam(TEX_1, _Tex); }
    void SetAdditiveTexture(Ptr<CTexture> _Tex) { GetMaterial(0)->SetTexParam(TEX_2, _Tex); }

    const vector<MovingStruct>& GetMovingStruct() { return m_vMovingVec; }
    void SetMovingStruct(int _index, MovingStruct _info) { m_vMovingVec[_index] = _info; }

    bool IsUsingMovingVec() { return m_bIsUsingMovingVec; }
    void SetUsingMovingVec(bool _use); 

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    

    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();
};

