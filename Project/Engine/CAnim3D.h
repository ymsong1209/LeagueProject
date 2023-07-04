#pragma once
#include "CEntity.h"

class CAnimator3D;

class CAnim3D :
	public CEntity
{
private:
	CAnimator3D* m_pOwner;
	Vec2                m_vBackSize;
	wstring				m_wClipName;  //원본 클립 애니메이션의 이름 (개별의 자신말고, 모두다 포함하는 원본)

	bool                m_bFinish; //애님 끝여부

	const vector<tMTBone>* m_pVecBones;
	const vector<tMTAnimClip>* m_pVecClip;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
	int							m_iFrameCount; // 30
	double						m_dClipAnimCurTime;
	int							m_iCurClip; // 클립 인덱스	

	int							m_iFrameIdx; // 클립의 현재 프레임
	int							m_iNextFrameIdx; // 클립의 다음 프레임
	float						m_fRatio;	// 프레임 사이 비율

	bool						m_bFinalMatUpdate; // 최종행렬 연산 수행여부

	int             m_iStartFrame; //애님 시작프레임
	int             m_iEndFrame; //애님 끝프레임
	int				m_iFrameLength; //애님 프레임 길이

	double			m_dStartTime; //애님 시작시간
	double			m_dEndTime; //애님 끝시간
	double			m_dTimeLength; //애님 시간 길이

	double			m_PlayTime; //애님 재생이 흘러간 시간

	bool			b_StartEndLerp; //애님 시작과 끝사이의 lerp를 할것인지에 대한 체크
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
		m_dStartTime = static_cast<double>(_iStartFrame) / m_iFrameCount;  // iStartFrame 값을 dStartTime 값으로 변환
	}
	int GetStartFrame() { return m_iStartFrame; }

	void SetEndFrame(int _iEndFrame)
	{
		m_iEndFrame = _iEndFrame;
		m_dEndTime = static_cast<double>(_iEndFrame) / m_iFrameCount;  //시간값과 프레임값을 서로 변환해서 각 변수에 세팅
	}

	int GetEndFrame() { return m_iEndFrame; }

	void SetFrameLength(int _iFrameLength) { m_iFrameLength = _iFrameLength; }
	int GetFrameLength() { return m_iFrameLength; }

	void SetStartTime(double _dStartTime)
	{
		m_dStartTime = _dStartTime;
		m_iStartFrame = static_cast<int>(_dStartTime * m_iFrameCount); // 시간값과 프레임값을 서로 변환해서 각 변수에 세팅
	}

	double GetStartTime() { return m_dStartTime; }

	void SetEndTime(double _dEndTime)
	{
		m_dEndTime = _dEndTime;
		m_iEndFrame = static_cast<int>(_dEndTime * m_iFrameCount); // 시간값과 프레임값을 서로 변환해서 각 변수에 세팅
	}
	double GetEndTime() { return m_dEndTime; }
	void SetTimeLength(double _dTimeLength) { m_dTimeLength = _dTimeLength; }
	double GetTimeLength() { return m_dTimeLength; }
	double Get_ClipAnimCurTime() { return m_dClipAnimCurTime; } // 클립 애니메이션에서 어느 시간부분을 재생중인가?
	double GetAnimPlayTime() { return m_PlayTime; } //개별 애님 재생이 흘러간 시간
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

