#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"

class CAnim2D;

struct tOriginalTransform
{
    Vec3 OriginalPos;
    Vec3 OriginalScale;
    Vec3 OriginalRot;
};

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim2D*>  m_mapAnim;  // Animation 목록
    CAnim2D*                m_pCurAnim; // 현재 재생중인 Animation
    bool                    m_bRepeat;  // 반복

    bool                    m_bPause;

    tOriginalTransform      m_tOriginalTransform;   // 오브젝트의 원본 Transform


public:
    virtual void finaltick() override;
    void         UpdateData();
    void         Clear();

public:
    void Play(const wstring& _strName, bool _bRepeat);

    bool IsPaused() { return m_bPause; }
    void SetPause(bool _b) { m_bPause = _b; }

    bool IsRepeating() { return m_bRepeat; }
    void SetRepeat(bool _b) { m_bRepeat = _b; }

    void CreateAnimation(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS);

    const map<wstring, CAnim2D*>& GetAnimMap() { return m_mapAnim; }
    CAnim2D* FindAnim(const wstring& _strName);
    void AddAnim(CAnim2D* _CurAnim);

    CAnim2D* GetCurAnim() { return m_pCurAnim; }
    void SetCurAnim(CAnim2D* _CurAnim) { m_pCurAnim = _CurAnim; }
    void RemoveAnim(const wstring _AnimName) { m_mapAnim.erase(_AnimName); }

    void SetOriginalTransform();
    tOriginalTransform GetOriginalTransform() { return m_tOriginalTransform; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CAnimator2D);
public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _Other);
    ~CAnimator2D();
};

