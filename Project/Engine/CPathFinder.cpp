#include "pch.h"
#include "CPathFinder.h"
#include "CPathFindMgr.h"

#include "CTransform.h"

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
	m_vecPath.clear();

	// 오브젝트의 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// 현재 경로를 시작으로 경로를 받아옴
	m_vecPath = CPathFindMgr::GetInst()->FindPath(CurPos, endPos);
	m_iPathCount = m_vecPath.size();
	m_iCurPathIdx = 0;

	m_vNextPos = Vec3(m_vecPath[0].x, m_vecPath[0].y, m_vecPath[0].z);
}

void CPathFinder::FindNextPath()
{
	m_iCurPathIdx++;

	// 다음 경로가 있다면 그 위치를 반환하고, 경로 인덱스 증가시킴
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount) 
	{	
		m_vNextPos =  Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);
	}
	// 다음 경로가 없다면 갈 수 없는 위치 반환
	else 
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
	}
}
