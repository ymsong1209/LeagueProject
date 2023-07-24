#pragma once
#include "CComponent.h"


#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"

struct tMtrlSet
{
    Ptr<CMaterial>  pSharedMtrl;    // ���� ���׸���
    Ptr<CMaterial>  pDynamicMtrl;   // ���� ���׸����� ���纻    
    Ptr<CMaterial>  pCurMtrl;       // ���� ��� �� ���׸���
};


class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;
    vector<tMtrlSet>        m_vecMtrls;             // ����    


    float                   m_fBounding;            // FrustumCheck �뵵 ������
    bool                    m_bFrustumCheck;        // ����ü �ø� üũ ����
    bool                    m_bDynamicShadow;       // ���� �׸��� ��� ����
    bool                    m_bShowDebugBoundShape; /// bounding ���� �� ���� ����


    bool                    m_bRaySightCulling;     // Ray ������ ���� Culling üũ

    bool                    m_bUseBoundingOffset;   // BoundingBox Offset ���
    Vec3                    m_vBoundingBoxOffset;   // BoundingBox Offset


public:
    virtual void render() = 0;
    virtual void render_depthmap();

public:
    void SetMesh(Ptr<CMesh> _Mesh);
    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx);

    Ptr<CMesh> GetMesh() { return m_pMesh; }

    Ptr<CMaterial> GetMaterial(UINT _idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);
    Ptr<CMaterial> GetDynamicMaterial(UINT _idx);

    UINT GetMtrlCount() { return (UINT)m_vecMtrls.size(); }

    void SetFrustumCheck(bool _bUse) { m_bFrustumCheck = _bUse; }
    bool IsUseFrustumCheck() { return m_bFrustumCheck; }
    void SetBounding(float _fBounding) { m_fBounding = _fBounding; }
    float GetBounding() { return m_fBounding; }
    bool IsShowDebugBound() { return m_bShowDebugBoundShape; }
    void SetShowDebugBound(bool _bShow) { m_bShowDebugBoundShape = _bShow; }

    void SetRaySightCulling(bool _bUse) { m_bRaySightCulling = _bUse; }
    bool IsUsingRaySightCulling() { return m_bRaySightCulling; }

    //true�� �����Ǹ� Offset ��ġ�� BoundingBox�� ����
    void SetBoundingBoxOffsetUse(bool _Use) { m_bUseBoundingOffset = _Use; }
    bool GetBoundingBoxOffsetUse() { return m_bUseBoundingOffset; }

    void SetBoundingBoxOffset(Vec3 _offset) { m_vBoundingBoxOffset = _offset; }
    Vec3 GetBoundingBoxOffset() { return m_vBoundingBoxOffset; }

    bool IsDynamicMtrlEmpty(UINT _idx);
    void ClearDynamicMtrl(UINT _idx);

 
    void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
    bool IsDynamicShadow() { return m_bDynamicShadow; }
 

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;


public:
    CRenderComponent(COMPONENT_TYPE _type);
    CRenderComponent(const CRenderComponent& _other);
    ~CRenderComponent();
};

