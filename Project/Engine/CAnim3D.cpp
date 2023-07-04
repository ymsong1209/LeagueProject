#include "pch.h"
#include "CAnim3D.h"

#include "CTimeMgr.h"

CAnim3D::CAnim3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_iCurClip(0)
	, m_dClipAnimCurTime(0.)
	, m_iFrameCount(30)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_bFinish(false)
	, f_AnimSpeed(1.f)

{
}

CAnim3D::~CAnim3D()
{
}

void CAnim3D::finaltick()
{
	m_iFrameLength = m_iEndFrame - m_iStartFrame;
	m_dTimeLength = m_dEndTime - m_dStartTime;

	m_dClipAnimCurTime = 0.f;
	// 현재 재생중인 Clip 의 시간을 진행한다.
	m_vecClipUpdateTime[m_iCurClip] += EditorDT * f_AnimSpeed; //스피드 값에 따라 속도 조절도 함께함

	m_PlayTime = m_vecClipUpdateTime[m_iCurClip];

	m_dClipAnimCurTime = m_dStartTime + m_vecClipUpdateTime[m_iCurClip];

	if (m_dClipAnimCurTime >= m_dEndTime)
	{
		m_bFinish = true;
	}


	//초당 30개짜리

	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dClipAnimCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iFrameIdx >= m_pVecClip->at(0).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);


	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;
}

void CAnim3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}

void CAnim3D::SaveToLevelFile(FILE* _File)
{
}

void CAnim3D::LoadFromLevelFile(FILE* _File)
{
}


