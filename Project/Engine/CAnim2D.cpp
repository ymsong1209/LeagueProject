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

	// �������� Duration���� �����ð��� Ŀ����
	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		// �����ð��� Duration��ŭ ���־� �ʱ�ȭ�ϰ� �������� �ѱ��.
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;

		// ������ �������� �Ѿ�� ������ ���������� �����ϰ� Finish�ɼ��� �Ҵ�
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
	// ��Ʋ�� �ؽ�ó ������ Assert
	assert(_AtlasTex.Get());

	// �ִϸ��̼� �̸� ����
	SetName(_strAnimName);

	// ��Ʋ�� �̹���, �ػ� ����
	m_AtlasTex = _AtlasTex;
	m_vAtlasResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	// FPS ����
	m_iFPS = _FPS;

	// BackSize ����
	m_vBackSizeUV = _vBackSizeUV;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		// ��Ʋ�� �ؽ�ó�� �ִϸ��̼� �������� �Ϸķ� ���ĵǾ����� �� ����
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / m_vAtlasResolution;
		frm.SliceUV = _vSlice / m_vAtlasResolution;

		// DynamicTransform �ʱⰪ
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