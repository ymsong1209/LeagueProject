#pragma once
#include "CComponent.h"


#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"

struct tMtrlSet
{
    Ptr<CMaterial>  pSharedMtrl;    // 공유 메테리얼
    Ptr<CMaterial>  pDynamicMtrl;   // 공유 메테리얼의 복사본    
    Ptr<CMaterial>  pCurMtrl;       // 현재 사용 할 메테리얼
};


class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;
    vector<tMtrlSet>        m_vecMtrls;             // 재질    

    float                   m_fBounding;            // FrustumCheck 용도 경계범위
    bool                    m_bFrustumCheck;        // 절두체 컬링 체크 유무
    bool                    m_bDynamicShadow;       // 동적 그림자 사용 유무
    bool                    m_bShowDebugBoundShape; // bounding 범위 선 보기 유무

    bool                    m_bRaySightCulling;     // Ray 광원에 의한 Culling 체크
    bool                    m_bCulled;              // Camera의 SortObject에서 Culling 되었으면 true로 바뀜

    // Camera에 의해 출력되고싶은지.
    // true면 아예 출력이 안되고, false면 정상적으로 Camera의 SortObject를 거침.
    // Level의 RegisterObject단계에서 자동으로 false로 세팅되고,
    // Script의 tick단계에서 true 바뀔수가 있음. 계속 출력안하고 싶으면 매 tick마다 true로 바꿔줘야함
    bool                    m_bSortExcept;          

    bool                    m_bUseBoundingOffset;   // BoundingBox Offset 사용
    Vec3                    m_vBoundingBoxOffset;   // BoundingBox Offset


public:
    virtual void render() = 0;
    virtual void render(UINT _iSubset) = 0;
    virtual void render_depthmap();

public:
    void SetMesh(Ptr<CMesh> _Mesh);
    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx);

    Ptr<CMesh> GetMesh() { return m_pMesh; }

    Ptr<CMaterial> GetMaterial(UINT _idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);
    Ptr<CMaterial> GetDynamicMaterial(UINT _idx);

    UINT GetMtrlCount() { return (UINT)m_vecMtrls.size(); }
    ULONG64 GetInstID(UINT _iMtrlIdx);

    void SetFrustumCheck(bool _bUse) { m_bFrustumCheck = _bUse; }
    bool IsUseFrustumCheck() { return m_bFrustumCheck; }
    void SetBounding(float _fBounding) { m_fBounding = _fBounding; }
    float GetBounding() { return m_fBounding; }
    bool IsShowDebugBound() { return m_bShowDebugBoundShape; }
    void SetShowDebugBound(bool _bShow) { m_bShowDebugBoundShape = _bShow; }

    void SetRaySightCulling(bool _bUse) { m_bRaySightCulling = _bUse; }
    bool IsUsingRaySightCulling() { return m_bRaySightCulling; }

    //컬링되었는지를 확인. true면 컬링되었음
    bool IsCulled() { return m_bCulled; } 
    //컬링되었는지를 확인. true면 컬링되었음
    void SetCulled(bool _cull) { m_bCulled = _cull; }

    // Camera에 의해 출력되고싶은지.
    // true면 아예 출력이 안되고, false면 정상적으로 Camera의 SortObject를 거침.
    // Level의 RegisterObject단계에서 자동으로 false로 세팅되고,
    // Script의 tick단계에서 true 바뀔수가 있음. 계속 출력안하고 싶으면 매 tick마다 true로 바꿔줘야함
    bool IsSortExcept() { return m_bSortExcept; }
    // Camera에 의해 출력되고싶은지.
    // true면 아예 출력이 안되고, false면 정상적으로 Camera의 SortObject를 거침.
    // Level의 RegisterObject단계에서 자동으로 false로 세팅되고,
    // Script의 tick단계에서 true 바뀔수가 있음. 계속 출력안하고 싶으면 매 tick마다 true로 바꿔줘야함
    void SetSortExcept(bool _b) { m_bSortExcept = _b; }

    //true로 설정되면 Offset 위치에 BoundingBox가 생김
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

