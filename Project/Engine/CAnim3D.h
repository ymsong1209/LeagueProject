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

    int							m_iFrameIdx;            // Ŭ���� ���� ������
    int							m_iNextFrameIdx;        // Ŭ���� ���� ������
    float						m_fRatio;	            // ������ ���� ����

    bool                        m_bFinish;
    bool                        m_bPause;

    wstring                     m_RelativePath;

public:
    void finaltick();

    void Create(const tMTAnimClip& _OriginalVecClip, const wstring& _AnimName = L"");
    //animator3d�� ���� �ڵ�����
    //CreateAnimFromTxt�϶��� bool�� true��
    void Save(bool _nameincludesmeshpath = false);
    //��������
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

    //blend �ɼ��� �ٶ� ������ ������ ��
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