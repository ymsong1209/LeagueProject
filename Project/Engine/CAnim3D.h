#pragma once
#include "CEntity.h"

#include "ptr.h"
class CAnimator3D;
class CStructuredBuffer;

class CAnim3D :
    public CEntity
{
private:
    CAnimator3D* m_pOwner;

    tMTAnimClip                 m_pClip;

    float       				m_fClipUpdateTime;
    int							m_iFrameRate;
    double						m_dCurTime;

    int							m_iFrameIdx;            // 클립의 현재 프레임
    int							m_iNextFrameIdx;        // 클립의 다음 프레임
    float						m_fRatio;	            // 프레임 사이 비율

    bool                        m_bFinish;
    bool                        m_bPause;

    wstring                     m_RelativePath;

public:
    void finaltick();

    void Create(const tMTAnimClip& _OriginalVecClip, const wstring& _AnimName = L"");
    //animator3d를 통해 자동저장
    //CreateAnimFromTxt일때만 bool값 true임
    void Save(bool _nameincludesmeshpath = false);
    //수동저장
    void Save(const wstring& _strRelativePath);
    void Load(const wstring& _strRelativePath);

public:
    const tMTAnimClip& GetClipList() { return m_pClip; }
    void SetClipList(const tMTAnimClip& clip) { m_pClip = clip; }

    int GetCurFrameIdx() { return m_iFrameIdx; }
    int GetNextFrameIdx() { return m_iNextFrameIdx; }
    void SetFrameRate(int _rate) { m_iFrameRate = _rate; }
    int GetFrameRate() { return m_iFrameRate; }
    void SetUpdateTime(float _time) { m_fClipUpdateTime = _time; }
    float GetRatio() { return m_fRatio; }
    wstring GetAnimRelativePath() { return m_RelativePath; }

    //blend 옵션을 줄때 강제로 적용할 것
    void SetFrameIdx(int _idx) { m_iFrameIdx = _idx; }
    void SetNextFrameIdx(int _idx) { m_iNextFrameIdx = _idx; }
    void SetRatio(float _ratio) { m_fRatio = _ratio; }

public:

    bool IsFinish() { return m_bFinish; }
    bool IsPause() { return m_bPause; }
    void Reset()
    {
        m_fClipUpdateTime = 0.f;
        m_bFinish = false;
    }

    void Pause() { m_bPause = true; }
    void Play() { m_bPause = false; }

    CLONE(CAnim3D);
public:
    CAnim3D();
    CAnim3D(const CAnim3D& _other);
    ~CAnim3D();

    friend class CAnimator3D;
};