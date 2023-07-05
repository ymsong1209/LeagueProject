#pragma once
#include "CEntity.h"

#include "ptr.h"
#include "CTexture.h"

class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D*        m_pOwner;               // 해당 애니메이션을 관리하는 애니메이터
    vector<tAnim2DFrm>  m_vecFrm;               // 프레임 정보

    Ptr<CTexture>       m_AtlasTex;             // Atlas 이미지 
    Vec2                m_vAtlasResolution;     // Atlas 이미지 해상도
    wstring             m_strRelativePath;         // 해당 애니메이션 저장 경로

    int                 m_iCurFrm;              // 현재 프레임
    float               m_fTime;                // 누적 시간

    int                 m_iFPS;                 // FPS
    Vec2                m_vBackSizeUV;          // 실제 Slice보다 넉넉하게 잡을 뒷배경 크기

    bool                m_bFinish;              // 애니메이션 종료 여부

    bool                m_bDynamicTransform;    // 프레임마다 Transform 변경되는 옵션 사용여부

public:
    void finaltick();
    void Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _BackSize, int _FrameCount, int _FPS);

    void SetUseDynamicTransform(bool _b) { m_bDynamicTransform = _b; }
    bool IsUsingDynamicTransform() { return m_bDynamicTransform; }

    const tAnim2DFrm& GetCurFrame() { return m_vecFrm[m_iCurFrm]; }
    const tAnim2DFrm& GetFrameByIdx(int _i) { return m_vecFrm[_i]; }

    int GetCurFrameIdx() { return m_iCurFrm; };
    void SetFrameIdx(int _i) { if (_i < 0) _i = 0; m_iCurFrm = _i; }

    int GetTotalFrameCount() { return (int)m_vecFrm.size(); }
    Vec2 GetAtlasResolution() { return m_vAtlasResolution; }

    void SetCurFrameLeftTopUV(Vec2 _UV) { m_vecFrm[m_iCurFrm].LeftTopUV = _UV; }
    void SetCurFrameSliceUV(Vec2 _UV) { m_vecFrm[m_iCurFrm].SliceUV = _UV; }
    void SetCurFrameOffsetUV(Vec2 _UV) { m_vecFrm[m_iCurFrm].OffsetUV = _UV; }
    void SetTotalDuration(float  _fDur) { for (int i = 0; i < m_vecFrm.size(); i++) m_vecFrm[i].fDuration = _fDur; }

    void SetFPS(int _i) { m_iFPS = _i; }
    int GetFPS() { return m_iFPS; }

    void SetBackSizeUV(Vec2 _UV) { m_vBackSizeUV = _UV; }
    Vec2 GetBackSizeUV() { return m_vBackSizeUV; }

    Vec3 GetCurFrameDynamicPos() { return m_vecFrm[m_iCurFrm].DynamicPos;  }
    Vec3 GetCurFrameDynamicScale() { return m_vecFrm[m_iCurFrm].DynamicScale;  }
    Vec3 GetCurFrameDynamicRot() { return m_vecFrm[m_iCurFrm].DynamicRot;  }

    void SetCurFrameDynamicPos(Vec3 _vPos) { m_vecFrm[m_iCurFrm].DynamicPos = _vPos; }
    void SetCurFrameDynamicScale(Vec3 _vScale) { m_vecFrm[m_iCurFrm].DynamicScale = _vScale; }
    void SetCurFrameDynamicRot(Vec3 _vRot) { m_vecFrm[m_iCurFrm].DynamicRot = _vRot; }


    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }

    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_iCurFrm = 0;
        m_fTime = 0.f;
        m_bFinish = false;
    }

    void Clear() { m_vecFrm.clear(); }
    bool IsClear() { return m_vecFrm.empty(); }

    void AddFrame(tAnim2DFrm _frm) { m_vecFrm.push_back(_frm); }
    void PopBackFrame() { m_vecFrm.pop_back(); }
    void DeleteFrameByIdx(int _i) { vector<tAnim2DFrm>::iterator iter = m_vecFrm.begin() + _i; m_vecFrm.erase(iter); }

    wstring GetRelativePath() { return m_strRelativePath; }
    void SetRelativePath(wstring _path) { m_strRelativePath = _path; }

    void SaveToLevelFile(FILE* _File);
    void LoadFromLevelFile(FILE* _File);

    void Save();
    void Load();
    void Load(const wstring& _strFilePath);


    CLONE(CAnim2D);
public:
    CAnim2D();
    ~CAnim2D();

    friend class CAnimator2D;
};

