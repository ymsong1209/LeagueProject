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
    map<wstring, CAnim3D*>      m_mapAnim;          // Animation ���
    CAnim3D* m_pCurAnim;         // ���� ������� Animation
    CAnim3D* m_pAnimAfterBlend;  // Blend�Ϸ��� CurAnim���� ���õǾ����.
    bool                        m_bRepeat;          // �ݺ�
    bool                        m_bBlend;           // �ִϸ��̼� ���� Blend �ɼ�
    bool                        m_bRepeatBlend;     // �ݺ�����ҽ� ó�� ���������� blend�Ұ����� �ɼ�
    float                       m_fCurBlendTime;
    float                       m_fMaxBlendTime;
    int                         m_iBlendStartFrm;
    int                         m_iBlendEndFrm;
    float                       m_fBlendRatio;
    bool                        m_bDebugAnimator;    //True�� ���, level�� stop�̿��� �ִϸ��̼� �����.dt�� �ƴ϶� editdt���, Anim3dEditorUI���� ���� object�� ����

    const vector<tMTBone>* m_pVecBones;

    vector<Matrix>				m_vecFinalBoneMat; // �ؽ��Ŀ� ������ ���� �������

    CStructuredBuffer* m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���

    wstring                     m_MeshDataRelativePath; //�ִϸ��̼��� mesh�� material�� ���õǾ����. 


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


    //�ݺ���� ����ϰ� ���� ��
    //repeatblend : ���� �ִϸ��̼� �ݺ�����Ҷ� �������� blend �ɼ� �� ������
    //blend : ���� �ִϸ��̼��� ���������� �ִϸ��̼� ��ȯ�Ҷ� blend�ɼ��� �� ������
    void Play(const wstring& _strName, bool _bRepeat, bool _RepeatBlend, bool _blend = true, float _blendtime = 0.f);
    //������� ����ϰ� ���� ��
    //blend : ���� �ִϸ��̼��� ���������� �ִϸ��̼� ��ȯ�Ҷ� blend�ɼ��� �� ������
    void Play(const wstring& _strName, bool _blend = false, float _blendtime = 0.f);


    void Pause();
    void Reset();
    void Stop();
    //�ݺ���� ��������� Ȯ��
    bool IsRepeat() { return m_bRepeat; }
    void SetRepeat(bool _bRepeat) { m_bRepeat = _bRepeat; }


    //�ִϸ��̼� ���� blend �ɼ� ����
    void SetBlend(bool _blend, float _time = 0.f) { m_bBlend = _blend; m_fMaxBlendTime = _time; m_fCurBlendTime = 0.f; }
    //�ݺ���� �ִϸ��̼� ����
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
