#pragma once
#include "CEntity.h"

#include "ptr.h"
#include "CTexture.h"

class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D*        m_pOwner;               // �ش� �ִϸ��̼��� �����ϴ� �ִϸ�����
    vector<tAnim2DFrm>  m_vecFrm;               // ������ ����

    Ptr<CTexture>       m_AtlasTex;             // Atlas �̹��� 
    Vec2                m_vAtlasResolution;     // Atlas �̹��� �ػ�
    wstring             m_strRelativePath;         // �ش� �ִϸ��̼� ���� ���

    int                 m_iCurFrm;              // ���� ������
    float               m_fTime;                // ���� �ð�

    int                 m_iFPS;                 // FPS
    Vec2                m_vBackSizeUV;          // ���� Slice���� �˳��ϰ� ���� �޹�� ũ��

    bool                m_bFinish;              // �ִϸ��̼� ���� ����

    bool                m_bDynamicTransform;    // �����Ӹ��� Transform ����Ǵ� �ɼ� ��뿩��

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

    void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator);
    void LoadFromLevelJsonFile(const Value& _componentValue);
    void Save();
    void Load();
    void Load(const wstring& _strFilePath);


    CLONE(CAnim2D);
public:
    CAnim2D();
    ~CAnim2D();

    friend class CAnimator2D;
};

