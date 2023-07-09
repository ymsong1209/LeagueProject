#pragma once
#include "CRenderComponent.h"

// Texture�� ���������Ѵٸ� � ���·� �����̴°�
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

// ����� �Ǵ� Texture�� � �뵵�� ���Ǵ� ���ΰ�
enum class eTargetTexture
{
    OUTPUT,                 // ������ �ߴ� Fighter.bmpó�� ����� ����
    PUNCTURE,               // ��µ� ������ ���ؼ� ������ �մ� �뵵
    END,
};

struct MovingStruct
{
    eTargetTexture   TargetTex;
    eTexMovingStyle  MovingStyle;
    Vec4            FuncValue;                  // Texture�� �����϶� ������ �Լ��� �´� �����
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

