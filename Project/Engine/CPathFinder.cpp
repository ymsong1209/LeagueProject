#include "pch.h"
#include "CPathFinder.h"
#include "CPathFindMgr.h"

#include "CTransform.h"
#include "CCamera.h"
#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

CPathFinder::CPathFinder()
	: CComponent(COMPONENT_TYPE::PATHFINDER)
	, m_vecPath{}
	, m_iPathCount(0)
	, m_iCurPathIdx(-1)
	, m_vNextPos(NaN, NaN, NaN)
{
}

CPathFinder::~CPathFinder()
{
}

void CPathFinder::tick()
{


	//m_vCurPos = Transform()->GetRelativePos();
}

void CPathFinder::FindPath(Vec3 endPos)
{
	// 새 경로를 찾을 때마다 새로운 경로로 대체함
	float dist = (endPos - m_vPrevEndPos).Length();

	if (dist > 10.f) //이전에 찍어둔 목적지와 현재 요청한 목적지의 차이가 10픽셀밖에안된다면 길찾기를 진행하지 않음 : 매번 비슷한곳을찍었을때 캐릭터가 덜덜떠는 현상 방지하기 위함!
	{
		m_vecPath.clear();
		// 오브젝트의 현재 위치
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

		// 현재 경로를 시작으로 경로를 받아옴
		m_vecPath = CPathFindMgr::GetInst()->FindPath(CurPos, endPos);
		m_iPathCount = m_vecPath.size();
		m_iCurPathIdx = 0;

		m_vNextPos = Vec3(m_vecPath[0].x, m_vecPath[0].y, m_vecPath[0].z);

		m_vPrevEndPos = endPos;
	}

}

void CPathFinder::FindNextPath()
{
	m_iCurPathIdx++;

	// 다음 경로가 있다면 그 위치를 반환하고, 경로 인덱스 증가시킴
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount)
	{
		m_vNextPos = Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);
	}
	// 다음 경로가 없다면 갈 수 없는 위치 반환
	else
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
	}
}

//void CPathFinder::FindPathMousePicking()  //현재 카메라에서 레이를 가져오고, 맵콜리전과의 교차점을 endPos로 지정해서 FindPath 호출.(마우스 피킹 이동)
//{
//	CGameObject* LoLMap = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
//	/*if (!LayerObj.empty())
//	{
//		if (LayerObj[0] && !LayerObj[0]->IsDead())
//			LoLMap = LayerObj[0];
//	}*/
//
//	if (LoLMap != nullptr && !LoLMap->IsDead())  //맵 콜리전이 정상적으로 존재할경우만 호출
//	{
//		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
//		tRay ray = MainCam->GetRay();
//		IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, LoLMap);
//		Vec3 EndPos = result.vCrossPoint;
//		FindPath(EndPos);
//	}
//}

void CPathFinder::ClearPath()
{
	m_vNextPos = Vec3(NaN, NaN, NaN);
}

