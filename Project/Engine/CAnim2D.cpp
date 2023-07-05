#include "pch.h"
#include "CAnim2D.h"

#include "CTimeMgr.h"


CAnim2D::CAnim2D()
	: m_pOwner(nullptr)
	, m_iCurFrm(0)
	, m_fTime(0.f)
	, m_bFinish(false)
	, m_bDynamicTransform(false)
{
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{
	if (m_bFinish)
		return;

	m_vAtlasResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_fTime += DT;

	// 프레임의 Duration보다 누적시간이 커지면
	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		// 누적시간을 Duration만큼 빼주어 초기화하고 프레임을 넘긴다.
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;

		// 마지막 프레임을 넘어가면 마지막 프레임으로 고정하고 Finish옵션을 켠다
		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}
}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSizeUV, int _FrameCount, int _FPS)
{
	// 아틀라스 텍스처 없으면 Assert
	assert(_AtlasTex.Get());

	// 애니메이션 이름 설정
	SetName(_strAnimName);

	// 아틀라스 이미지, 해상도 설정
	m_AtlasTex = _AtlasTex;
	m_vAtlasResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	// FPS 대입
	m_iFPS = _FPS;

	// BackSize 설정
	m_vBackSizeUV = _vBackSizeUV;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		// 아틀라스 텍스처에 애니메이션 프레임이 일렬로 정렬되어있을 때 기준
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / m_vAtlasResolution;
		frm.SliceUV = _vSlice / m_vAtlasResolution;

		// DynamicTransform 초기값
		frm.DynamicPos = Vec3(0.f, 0.f, 0.f);
		frm.DynamicScale = Vec3(1.f, 1.f, 1.f);
		frm.DynamicRot = Vec3(0.f, 0.f, 0.f);

		m_vecFrm.push_back(frm);
	}
}

void CAnim2D::SaveToLevelFile(FILE* _File)
{
	SaveWString(GetName(), _File);

	size_t FrameCount = m_vecFrm.size();
	fwrite(&FrameCount, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), FrameCount, _File);
	fwrite(&m_vBackSizeUV, sizeof(Vec2), 1, _File);

	SaveResRef(m_AtlasTex.Get(), _File);
}

void CAnim2D::LoadFromLevelFile(FILE* _File)
{
	wstring name;
	LoadWString(name, _File);
	SetName(name);

	size_t FrameCount = 0;
	fread(&FrameCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < FrameCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, _File);
		m_vecFrm.push_back(frm);
	}

	fread(&m_vBackSizeUV, sizeof(Vec2), 1, _File);

	LoadResRef(m_AtlasTex, _File);
}