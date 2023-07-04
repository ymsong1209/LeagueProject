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
    map<wstring, CAnim3D*>  m_mapAnim;  // Animation ���
    CAnim3D* m_pCurAnim; // ���� ������� Animation
    bool                    m_bRepeat;  // �ݺ�

    CStructuredBuffer* m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���


    float			f_IsAnimBlendState; // true: 1.f false: 0.f
    float			f_BlendRatio; //�������� ���� ������(���� ������)
    float			f_BlendTime;  //������ ������ �ð�

    bool            b_HavePrevAnim;
    int             i_PrevFrameIdx; // ������ ����ϴ� �ִ��� ������������

    bool            b_UseMotionBlend;
public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void ClearData();

    void Play(const wstring& _strName, bool _bRepeat, bool _ReSetPlay = 0, bool _StartEndLerp = 0);
    //_ReSetPlay�� Play�� ȣ��ɶ����� �ִ��� ó������ �ٽ� ����Ҳ���, �ƴϸ� �̾ ����Ҳ����� ���� ���̿���
    // _bRepeat�� �ݺ������Դϴ�.
    //_StartEndLerp �� �ϳ��� �ִ��� ���ۺκа� ���κ��� ���� ������ ���� ���ο��� (�ݺ��Ǵ� �ִ��� ��� ���۰��� ����)

    CAnim3D* FindAnim(const wstring& _strName);
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip, wstring _strAnimName, const vector<tMTBone>* _vecBones);
    CAnim3D* GetCurAnim3D() { return m_pCurAnim; } //���� �ִϸ��̼� ��������

    void CreateClipToAnimF(int _StartFrame, int _EndFrame, wstring _ClipName, wstring _SetAnimName); //�����Ӱ����� �ִϸ��̼� �����
    void CreateClipToAnimT(double _StartTime, double _EndTime, wstring _ClipName, wstring _SetAnimName); // �ð������� �ִϸ��̼� �����

    void AnimBlend(); //���ε��� �ִϸ��̼� ���� �� ���

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
