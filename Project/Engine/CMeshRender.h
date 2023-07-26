#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

enum class MovTexMoveType
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

enum class MovTexType
{
    OUTPUT,
    PUNCTURE,
    ADDITIVE,
    END,
};


class CMeshRender :
    public CRenderComponent
{
private:
    tMeshMoveData           m_tMeshMoveData;
    Ptr<CTexture>           m_arrMeshTex[MESH_TEX_PARAM::MESH_TEX_END];

public:
    void SetOutputTexture(Ptr<CTexture> _Tex);
    void SetPunctureTexture(Ptr<CTexture> _Tex) { m_arrMeshTex[MESH_TEX_PARAM::PUNCTURE] = _Tex; }
    void SetAdditiveTexture(Ptr<CTexture> _Tex) { m_arrMeshTex[MESH_TEX_PARAM::ADDITIVE] = _Tex; }

    Ptr<CTexture> GetOutputTexture(Ptr<CTexture> _Tex);
    Ptr<CTexture> GetPunctureTex() { return m_arrMeshTex[MESH_TEX_PARAM::PUNCTURE]; }
    Ptr<CTexture> GetAdditiveTex() { return m_arrMeshTex[MESH_TEX_PARAM::ADDITIVE]; }

    void SetFuncValue(MovTexType _TexType, Vec4 _Value);
    void SetTexMovingStyle(MovTexType _TexType, MovTexMoveType _Type);
    void SetAdditiveTexColor(Vec4 _Color) { m_tMeshMoveData.AdditiveColor = _Color; }
    void SetOffsetValue(MovTexType _TexType, Vec2 _OffsetValue);

    Vec4 GetFuncValue(MovTexType _TexType);
    MovTexMoveType GetTexMovingStyle(MovTexType _TexType);
    Vec4 GetAdditiveTexColor() { return m_tMeshMoveData.AdditiveColor; }
    Vec2 GetOffsetValue(MovTexType _TexType);

public:
    void CalculateNextOffset(int  _MoveStyle, Vec2& _PreviousPos, Vec4 _FunctionValue, float _DT);


public:
    virtual void finaltick() override;
    virtual void render() override;


public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;


    CLONE(CMeshRender);
public:
    CMeshRender();
    CMeshRender(const CMeshRender& _other);
    ~CMeshRender();
};
