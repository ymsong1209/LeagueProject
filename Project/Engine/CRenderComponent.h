#pragma once
#include "CComponent.h"


#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;

    Ptr<CMaterial>          m_pSharedMtrl;      // ���� ���׸���
    Ptr<CMaterial>          m_pDynamicMtrl;     // SharedMaterial ���纻
    Ptr<CMaterial>          m_pCurrentMtrl;     // ���� ��� ���� ����
    float                   m_fBounding;        // FrustumCheck �뵵 ������
    bool                    m_bFrustumCheck;    // ����ü �ø� üũ ����
    bool                    m_bDynamicShadow;   // ���� �׸��� ��� ����


public:
    virtual void render() = 0;
    virtual void render_depthmap();

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pCurrentMtrl; }
    Ptr<CMaterial> GetSharedMaterial() { return m_pSharedMtrl; }
    Ptr<CMaterial> GetDynamicMaterial();

    void SetFrustumCheck(bool _bUse) { m_bFrustumCheck = _bUse; }
    bool IsUseFrustumCheck() { return m_bFrustumCheck; }
    void SetBounding(float _fBounding) { m_fBounding = _fBounding; }
    float GetBounding() { return m_fBounding; }
    void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
    bool IsDynamicShadow() { return m_bDynamicShadow; }

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


public:
    CRenderComponent(COMPONENT_TYPE _type);
    ~CRenderComponent();
};

