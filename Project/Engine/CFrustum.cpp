#include "pch.h"
#include "CFrustum.h"

#include "CCamera.h"

CFrustum::CFrustum(CCamera* _pOwner)
	: m_pOwner(_pOwner)
	, m_arrFace{}
{
	// 투영공간 좌표
	//     4 ------ 5
	//     |        |  Far
	//   / |        |
	//	/  7 ------ 6	
	// /      /
	// 0 -- 1     /
	// |    |    / Near
	// 3 -- 2
	m_arrProj[0] = Vec3(-1.f, 1.f, 0.f);
	m_arrProj[1] = Vec3(1.f, 1.f, 0.f);
	m_arrProj[2] = Vec3(1.f, -1.f, 0.f);
	m_arrProj[3] = Vec3(-1.f, -1.f, 0.f);

	m_arrProj[4] = Vec3(-1.f, 1.f, 1.f);
	m_arrProj[5] = Vec3(1.f, 1.f, 1.f);
	m_arrProj[6] = Vec3(1.f, -1.f, 1.f);
	m_arrProj[7] = Vec3(-1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::finaltick()
{
	Vec3 vWorldPos[8] = {};

	Matrix matInv = m_pOwner->GetProjMatInv() * m_pOwner->GetViewMatInv();

	for (int i = 0; i < 8; ++i)
	{
		vWorldPos[i] = XMVector3TransformCoord(m_arrProj[i], matInv);
	}


	m_arrFace[FT_NEAR] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
	m_arrFace[FT_FAR] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[7], vWorldPos[6]);
	m_arrFace[FT_LEFT] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
	m_arrFace[FT_RIGHT] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
	m_arrFace[FT_TOP] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
	m_arrFace[FT_BOT] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
}