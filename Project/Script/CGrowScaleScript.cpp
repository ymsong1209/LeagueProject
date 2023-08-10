#include "pch.h"
#include "CGrowScaleScript.h"
#include <Engine/CKeyMgr.h>

CGrowScaleScript::CGrowScaleScript()
	: CScript((UINT)SCRIPT_TYPE::GROWSCALESCRIPT)
	, m_fScaleSpeed(100.f)
	, m_vDirection(1.f, 0.f, 0.f)
{
}

CGrowScaleScript::~CGrowScaleScript()
{
}

void CGrowScaleScript::begin()
{
}

void CGrowScaleScript::tick()
{
	// 커지고 싶은 방향
	m_vDirection;

	// speed per sec (커져야할)
	m_fScaleSpeed;

	if (CKeyMgr::GetInst()->GetKeyState(KEY::X) == KEY_STATE::PRESSED)
	{
		Vec3 OriginalScale = GetOwner()->Transform()->GetRelativeScale();
		Vec3 OriginalPos = GetOwner()->Transform()->GetRelativePos();

		Vec3 NextScale = Vec3(OriginalScale.x + abs(m_vDirection.x) * m_fScaleSpeed * DT
			, OriginalScale.y + abs(m_vDirection.y) * m_fScaleSpeed * DT
			, OriginalScale.z + abs(m_vDirection.z) * m_fScaleSpeed * DT);


		Vec3 NextPos = Vec3(OriginalPos.x + abs(m_vDirection.y) * m_fScaleSpeed * DT / 2.f
			, OriginalPos.y + abs(m_vDirection.x) * m_fScaleSpeed * DT / 2.f
			, OriginalPos.z + abs(m_vDirection.z) * m_fScaleSpeed * DT / 2.f);


		GetOwner()->Transform()->SetRelativeScale(NextScale);
		GetOwner()->Transform()->SetRelativePos(NextPos);
	}
}




