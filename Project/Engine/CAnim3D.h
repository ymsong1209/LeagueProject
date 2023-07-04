#pragma once
#include "CEntity.h"

class CAnimator3D;

class CAnim3D :
	public CEntity
{
private:
	CAnimator3D* m_pOwner;
	Vec2                m_vBackSize;
	wstring				m_wClipName;  //���� Ŭ�� �ִϸ��̼��� �̸� (������ �ڽŸ���, ��δ� �����ϴ� ����)

	bool                m_bFinish; //�ִ� ������

	const vector<tMTBone>* m_pVecBones;
	const vector<tMTAnimClip>* m_pVecClip;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // �ؽ��Ŀ� ������ ���� �������
	int							m_iFrameCount; // 30
	double						m_dClipAnimCurTime;
	int							m_iCurClip; // Ŭ�� �ε���	

	int							m_iFrameIdx; // Ŭ���� ���� ������
	int							m_iNextFrameIdx; // Ŭ���� ���� ������
	float						m_fRatio;	// ������ ���� ����

	bool						m_bFinalMatUpdate; // ������� ���� ���࿩��

	int             m_iStartFrame; //�ִ� ����������
	int             m_iEndFrame; //�ִ� ��������
	int				m_iFrameLength; //�ִ� ������ ����

	double			m_dStartTime; //�ִ� ���۽ð�
	double			m_dEndTime; //�ִ� ���ð�
	double			m_dTimeLength; //�ִ� �ð� ����

	double			m_PlayTime; //�ִ� ����� �귯�� �ð�

	bool			b_StartEndLerp; //�ִ� ���۰� �������� lerp�� �Ұ������� ���� üũ
	float			f_AnimSpeed;

public:
	void finaltick();

	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
	const vector<tMTBone>* GetBones() { return m_pVecBones; }

	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);

	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }
	UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }

	const vector<tMTAnimClip>* GetVecClip() { return m_pVecClip; }

	bool IsFinish() { return m_bFinish; }
	void Reset()
	{
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
		m_bFinish = false;
	}

	void SaveToLevelFile(FILE* _File);
	void LoadFromLevelFile(FILE* _File);

	void SetStartFrame(int _iStartFrame)
	{
		m_iStartFrame = _iStartFrame;
		m_dStartTime = static_cast<double>(_iStartFrame) / m_iFrameCount;  // iStartFrame ���� dStartTime ������ ��ȯ
	}
	int GetStartFrame() { return m_iStartFrame; }

	void SetEndFrame(int _iEndFrame)
	{
		m_iEndFrame = _iEndFrame;
		m_dEndTime = static_cast<double>(_iEndFrame) / m_iFrameCount;  //�ð����� �����Ӱ��� ���� ��ȯ�ؼ� �� ������ ����
	}

	int GetEndFrame() { return m_iEndFrame; }

	void SetFrameLength(int _iFrameLength) { m_iFrameLength = _iFrameLength; }
	int GetFrameLength() { return m_iFrameLength; }

	void SetStartTime(double _dStartTime)
	{
		m_dStartTime = _dStartTime;
		m_iStartFrame = static_cast<int>(_dStartTime * m_iFrameCount); // �ð����� �����Ӱ��� ���� ��ȯ�ؼ� �� ������ ����
	}

	double GetStartTime() { return m_dStartTime; }

	void SetEndTime(double _dEndTime)
	{
		m_dEndTime = _dEndTime;
		m_iEndFrame = static_cast<int>(_dEndTime * m_iFrameCount); // �ð����� �����Ӱ��� ���� ��ȯ�ؼ� �� ������ ����
	}
	double GetEndTime() { return m_dEndTime; }
	void SetTimeLength(double _dTimeLength) { m_dTimeLength = _dTimeLength; }
	double GetTimeLength() { return m_dTimeLength; }
	double Get_ClipAnimCurTime() { return m_dClipAnimCurTime; } // Ŭ�� �ִϸ��̼ǿ��� ��� �ð��κ��� ������ΰ�?
	double GetAnimPlayTime() { return m_PlayTime; } //���� �ִ� ����� �귯�� �ð�
	int GetFrameIdx() { return m_iFrameIdx; }

	void SetClipName(wstring _AnimFromClipName) { m_wClipName = _AnimFromClipName; }
	wstring GetClipName() { return m_wClipName; }

	void SetStartEndLerp(bool _Use) { b_StartEndLerp = _Use; }
	bool GetStartEndLerp() { return b_StartEndLerp; }

	void SetAnimSpeed(float _Speed) { f_AnimSpeed = _Speed; }
	float GetAnimSpeed() { return f_AnimSpeed; }

	CLONE(CAnim3D);
public:
	CAnim3D();
	~CAnim3D();

	friend class CAnimator3D;
};

