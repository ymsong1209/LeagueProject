#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;
class CAnim3D;

class CAnimator3D :
    public CComponent
{
private:
    map<wstring, CAnim3D*>      m_mapAnim;          // Animation 목록
    CAnim3D* m_pCurAnim;         // 현재 재생중인 Animation
    CAnim3D* m_pAnimAfterBlend;  // Blend완료후 CurAnim으로 세팅되어야함.
    bool                        m_bRepeat;          // 반복
    bool                        m_bBlend;           // 애니메이션 사이 Blend 옵션
    bool                        m_bRepeatBlend;     // 반복재생할시 처음 프레임으로 blend할것인지 옵션
    float                       m_fCurBlendTime;
    float                       m_fMaxBlendTime;
    int                         m_iBlendStartFrm;
    int                         m_iBlendEndFrm;
    float                       m_fBlendRatio;
    bool                        m_bDebugAnimator;    //True일 경우, level이 stop이여도 애니메이션 재생됨.dt가 아니라 editdt사용, Anim3dEditorUI에서 만든 object만 사용됨

    const vector<tMTBone>* m_pVecBones;

    vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보

    CStructuredBuffer* m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬

    wstring                     m_MeshDataRelativePath; //애니메이션은 mesh랑 material이 세팅되어야함. 


public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetMeshDataRelativePath(wstring _path) { m_MeshDataRelativePath = _path; }
    wstring GetMeshDataRelativePath() { return m_MeshDataRelativePath; }
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    void ClearData();

public:


    //반복재생 사용하고 싶을 시
    //repeatblend : 현재 애니메이션 반복재생할때 마지막에 blend 옵션 줄 것인지
    //blend : 이전 애니메이션이 남아있을때 애니메이션 전환할때 blend옵션을 줄 것인지
    void Play(const wstring& _strName, bool _bRepeat, bool _RepeatBlend, bool _blend = true, float _blendtime = 0.f);
    //단일재생 사용하고 싶을 시
    //blend : 이전 애니메이션이 남아있을때 애니메이션 전환할때 blend옵션을 줄 것인지
    void Play(const wstring& _strName, bool _blend = false, float _blendtime = 0.f);


    void Pause();
    void Reset();
    void Stop();
    //반복재생 사용중인지 확인
    bool IsRepeat() { return m_bRepeat; }
    void SetRepeat(bool _bRepeat) { m_bRepeat = _bRepeat; }


    //애니메이션 사이 blend 옵션 제어
    void SetBlend(bool _blend, float _time = 0.f) { m_bBlend = _blend; m_fMaxBlendTime = _time; m_fCurBlendTime = 0.f; }
    //반복재생 애니메이션 제어
    void SetRepeatBlend(bool _Repeatblend, float _time = 0.f) { m_bRepeatBlend = _Repeatblend; m_fMaxBlendTime = _time; m_fCurBlendTime = 0.f; }
    bool IsBlend() { return m_bBlend; }
    bool IsRepeatBlend() { return m_bRepeatBlend; }
    float GetBlendTime() { return m_fMaxBlendTime; }
    void SetBlendTime(float _time) { m_fMaxBlendTime = _time; }

    void SetDebugAnimator(bool _bool) { m_bDebugAnimator = _bool; }
    bool IsDebugAnimator() { return m_bDebugAnimator; }
    void SetStartBlendFrm(int _frm) { m_iBlendStartFrm = _frm; }
    void SetEndBlendFrm(int _frm) { m_iBlendEndFrm = _frm; }
    void SetBlendRatio(float _ratio) { m_fBlendRatio = _ratio; }

    CAnim3D* FindAnim(const wstring& _strName);
    CAnim3D* LoadAnim(const wstring& _strRelativePath);
    CAnim3D* CreateAnimation(const tMTAnimClip& _OriginalVecClip, const wstring& _AnimName = L"");
    CAnim3D* GetCurAnim() { return m_pCurAnim; }
    map<wstring, CAnim3D*>& GetMapAnim() { return m_mapAnim; }
    void DeleteCurrentAnim();
    void DeleteEveryAnim();
    void LoadEveryAnimFromFolder(const wstring& _strRelativePath);

private:
    void check_mesh(Ptr<CMesh> _pMesh);

public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;
    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};
