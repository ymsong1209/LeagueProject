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
    map<wstring, CAnim3D*>  m_mapAnim;  // Animation 목록
    CAnim3D* m_pCurAnim; // 현재 재생중인 Animation
    bool                    m_bRepeat;  // 반복

    CStructuredBuffer* m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬


    float			f_IsAnimBlendState; // true: 1.f false: 0.f
    float			f_BlendRatio; //전달해줄 블렌딩 비율값(러프 비율값)
    float			f_BlendTime;  //블렌딩을 진행할 시간

    bool            b_HavePrevAnim;
    int             i_PrevFrameIdx; // 이전에 재생하던 애님의 마지막프레임

    bool            b_UseMotionBlend;
public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void ClearData();

    void Play(const wstring& _strName, bool _bRepeat, bool _ReSetPlay = 0, bool _StartEndLerp = 0);
    //_ReSetPlay는 Play가 호출될때마다 애님을 처음부터 다시 재생할껀지, 아니면 이어서 재생할껀지에 대한 값이에요
    // _bRepeat은 반복여부입니다.
    //_StartEndLerp 는 하나의 애님의 시작부분과 끝부분을 블렌딩 할지에 대한 여부에요 (반복되는 애님의 경우 시작과끝 블렌딩)

    CAnim3D* FindAnim(const wstring& _strName);
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip, wstring _strAnimName, const vector<tMTBone>* _vecBones);
    CAnim3D* GetCurAnim3D() { return m_pCurAnim; } //현재 애니메이션 가져오기

    void CreateClipToAnimF(int _StartFrame, int _EndFrame, wstring _ClipName, wstring _SetAnimName); //프레임값으로 애니메이션 만들기
    void CreateClipToAnimT(double _StartTime, double _EndTime, wstring _ClipName, wstring _SetAnimName); // 시간값으로 애니메이션 만들기

    void AnimBlend(); //바인딩될 애니메이션 블렌드 값 계산

    float GetBlendTime() { return f_BlendTime; }
    void SetBlendTime(float _Time) { f_BlendTime = _Time; }

    bool IsUseMotionBlend() { return b_UseMotionBlend; }
    void SetUseMotionBlend(bool _Use) { b_UseMotionBlend = _Use; }

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
