#include "pch.h"
#include "COtherPlayerScript.h"
COtherPlayerScript::COtherPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::OTHERPLAYERSCRIPT)
	, m_fSpeed(200.f)			
	, m_vTargetPosTest(Vec3(0.f, 0.f, 0.f))
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_vTargetPosTest, "TargetPos");
}
COtherPlayerScript::~COtherPlayerScript()
{
}
void COtherPlayerScript::begin()
{
}
void COtherPlayerScript::tick()
{
	// 여기서 패킷 받고 세팅
	//GetOwner()->Transform()->SetRelativePos(NewPos);
}

void COtherPlayerScript::BeginOverlap(CCollider2D* _Other)
{
}
void COtherPlayerScript::Shoot()
{
}
void COtherPlayerScript::SaveToLevelFile(FILE* _File)
{
}
void COtherPlayerScript::LoadFromLevelFile(FILE* _FILE)
{
}
void COtherPlayerScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
}
void COtherPlayerScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
}