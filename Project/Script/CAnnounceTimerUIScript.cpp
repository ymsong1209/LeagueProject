#include "pch.h"
#include "CAnnounceTimerUIScript.h"

void CAnnounceTimerUIScript::begin()
{
}

void CAnnounceTimerUIScript::tick()
{
	m_fCurTimer += DT;;

	if (m_fCurTimer >= 2.5f && !GetOwner()->IsDead())
	{
		DestroyObject(GetOwner());
	}
}

CAnnounceTimerUIScript::CAnnounceTimerUIScript()
	:CScript((UINT)SCRIPT_TYPE::ANNOUNCETIMERUISCRIPT)
	, m_fCurTimer(0.f)
{
}

CAnnounceTimerUIScript::~CAnnounceTimerUIScript()
{
}
