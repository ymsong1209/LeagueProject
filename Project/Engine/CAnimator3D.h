#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

////////////////////////////////////////////////////////
        //단일재생
        //pObj->Animator3D()->Play(L"Take 001");
        //단일재생, 이전 애니메이션 0.5초 블렌딩 후 단일재생
        //pObj->Animator3D()->Play(L"Take001", true, 0.5f);
        //반복재생, 마지막프레임후 첫프레임으로 blend안함
        //pObj->Animator3D()->Play(L"Take001", true, false);
        //반복재생, AnimA->Take001로 0.5초동안 애니메이션 blend하고 take001재생,take001마지막프레임->첫프레임 돌아오면서 blend안하기 
        //pObj->Animator3D()->Play(L"Take001", true, false, true, 0.5f);
        //반복재생, AnimA->Take001로 0.5초동안 애니메이션 blend하고 take001재생,take001마지막프레임->첫프레임 돌아오면서 blend 0.5초동안 하기
        //pObj->Animator3D()->Play(L"Take001", true, true, true, 0.5f);
        //사용 예시

        //IdleAnimation 반복재생 blend안함
        //pObj->animator3d()->play(L"IdleAnimation",true,false);
        //공격버튼 누르면 공격 애니메이션 단일재생, blending 0.2초
        //if(key_pressed(key::lbtn){
        //changestate(attack);
        //pobj->animator3d()->play(L"AttackAnimation", true,0.2f);
        //}
        //공격 애님 끝나면 다시 idle로 전환
        //if(animator3d()->getcuranim()->isfinish()){
        //changestate(idle)
        ////Attack->Idle 0.2초 blend주고, idle animation 재생, idle anim은 마지막프레임->첫프레임 이동시 blend없음 
        //pobj->animator3d()->play(L"IdleAnimation",true,false,true,0.2f); 
        //}
/////////////////////////////////////////////////
class CStructuredBuffer;
class CAnim3D;

class CAnimator3D :
    public CComponent
{
private:
    map<wstring, CAnim3D*>      m_mapAnim;          // Animation 목록
    CAnim3D* m_pCurAnim;         // 현재 재생중인 Animation
    bool                        m_bRepeat;          // 반복
    bool                        m_bBlend;           // 애니메이션 사이 Blend 옵션
    bool                        m_bRepeatBlend;     // 반복재생할시 처음 프레임으로 blend할것인지 옵션
    bool                        m_bRepeatBlending;  // 애니메이션이 blend하면서 반복재생중임
    float                       m_fCurBlendTime;
    float                       m_fMaxBlendTime;
    int                         m_iBlendStartFrm;
    int                         m_iStartFrm;
    int                         m_iNextFrm;
    float                       m_fFrameRatio;
    float                       m_fBlendRatio;       //애니메이션 전환시에 적용되는 blend값
    bool                        m_bDebugAnimator;    //True일 경우, level이 stop이여도 애니메이션 재생됨.dt가 아니라 editdt사용, Anim3dEditorUI에서 만든 object만 사용됨
    float                       m_fAnimSpeed;

    const vector<tMTBone>* m_pVecBones;

    vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보

    CStructuredBuffer* m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬

    wstring                     m_MeshDataRelativePath; //애니메이션은 mesh랑 material이 세팅되어야함. 


public:
    virtual void finaltick() override;
    bool UpdateData();

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
    void PlayRepeat(const wstring& _strName, bool _RepeatBlend, bool _blend = true, float _blendtime = 0.f);
    //단일재생 사용하고 싶을 시
    //blend : 이전 애니메이션이 남아있을때 애니메이션 전환할때 blend옵션을 줄 것인지
    void PlayOnce(const wstring& _strName, bool _blend = false, float _blendtime = 0.f);


    void Pause();
    void Reset();
    void Stop();
    //반복재생 사용중인지 확인
    bool IsRepeat() { return m_bRepeat; }
    void SetRepeat(bool _bRepeat) { m_bRepeat = _bRepeat; }

    void  SetSpeed(float _speed) { m_fAnimSpeed = _speed; }
    float GetSpeed() { return m_fAnimSpeed; }


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
    void SetCurFrm(int _frm) { m_iStartFrm = _frm; }
    void SetNextFrame(int _frm) { m_iNextFrm = _frm; }
    void SetBlendRatio(float _ratio) { m_fBlendRatio = _ratio; }
    void SetFrameRatio(float _ratio) { m_fFrameRatio = _ratio; }

    CAnim3D* FindAnim(const wstring& _strName);
    CAnim3D* LoadAnim(const wstring& _strRelativePath);
    CAnim3D* CreateAnimation(const tMTAnimClip& _OriginalVecClip, const wstring& _AnimName = L"");
    CAnim3D* GetCurAnim() { return m_pCurAnim; }
    map<wstring, CAnim3D*>& GetMapAnim() { return m_mapAnim; }
    void DeleteCurrentAnim();
    void DeleteEveryAnim();
    void LoadEveryAnimFromFolder(const wstring& _strRelativePath);
    // 애니메이션이 존재할 경우, 덮어씌워지지 않고 바로 return됨
    void CreateAnimFromText(const wstring& _strRelativePath);
    vector<wstring> split(const std::wstring& s, wchar_t delimiter);

private:
    void check_mesh(Ptr<CMesh> _pMesh);

public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};
