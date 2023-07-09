#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2D::CLight2D(const CLight2D& _other)
	: CComponent(_other)
	, m_LightInfo(_other.m_LightInfo)
{

}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _File);
}

void CLight2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo, sizeof(tLightInfo), 1, _File);
}

void CLight2D::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	// tLightInfo
	Value vTemp(kObjectType);

	// tLightColor
	Value lightColorValue(kObjectType);
	lightColorValue.AddMember("vDiffuse", SaveVec4Json(m_LightInfo.Color.vDiffuse, allocator), allocator);
	lightColorValue.AddMember("vAmbient", SaveVec4Json(m_LightInfo.Color.vAmbient, allocator), allocator);
	lightColorValue.AddMember("vSpecular", SaveVec4Json(m_LightInfo.Color.vSpecular, allocator), allocator);
	vTemp.AddMember("Color", lightColorValue, allocator);

	vTemp.AddMember("vWorldPos", SaveVec4Json(m_LightInfo.vWorldPos, allocator), allocator);
	vTemp.AddMember("vWorldDir", SaveVec4Json(m_LightInfo.vWorldDir, allocator), allocator);
	vTemp.AddMember("LightType", m_LightInfo.LightType, allocator);
	vTemp.AddMember("Radius", m_LightInfo.Radius, allocator);
	vTemp.AddMember("Angle", m_LightInfo.Angle, allocator);
	vTemp.AddMember("InnerAngle", m_LightInfo.InnerAngle, allocator);
	_objValue.AddMember("LightInfo", vTemp, allocator);

}

void CLight2D::LoadFromLevelJsonFile(const Value& _componentValue)
{
	// tLightInfo

	// tLightColor
	const Value& colorValue = _componentValue["LightInfo"]["Color"];
	m_LightInfo.Color.vDiffuse = LoadVec4Json(_componentValue["LightInfo"]["Color"]["vDiffuse"]);
	m_LightInfo.Color.vAmbient = LoadVec4Json(_componentValue["LightInfo"]["Color"]["vAmbient"]);
	m_LightInfo.Color.vSpecular = LoadVec4Json(_componentValue["LightInfo"]["Color"]["vSpecular"]);

	m_LightInfo.vWorldPos = LoadVec4Json(_componentValue["LightInfo"]["vWorldPos"]);
	m_LightInfo.vWorldDir = LoadVec4Json(_componentValue["LightInfo"]["vWorldDir"]);
	m_LightInfo.LightType = _componentValue["LightInfo"]["LightType"].GetUint();
	m_LightInfo.Radius = _componentValue["LightInfo"]["Radius"].GetFloat();
	m_LightInfo.Angle = _componentValue["LightInfo"]["Angle"].GetFloat();
	m_LightInfo.InnerAngle = _componentValue["LightInfo"]["InnerAngle"].GetFloat();
}
