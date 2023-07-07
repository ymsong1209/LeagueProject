#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_RWBuffer(nullptr)
	, m_ModuleData{}
	, m_AccTime(0.f)
{
	m_ModuleData.iMaxParticleCount = 1000;
	
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
	m_ModuleData.SpawnRate = 20;
	m_ModuleData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);
	m_ModuleData.vSpawnScaleMin = Vec3(15.f, 15.f, 1.f);
	m_ModuleData.vSpawnScaleMax = Vec3(20.f, 20.f, 1.f);

	m_ModuleData.SpawnShapeType = 0;
	m_ModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);	
	m_ModuleData.Space = 0; // �ùķ��̼� ��ǥ��

	m_ModuleData.MinLifeTime = 3.f;
	m_ModuleData.MaxLifeTime = 5.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
	m_ModuleData.StartScale = 1.5f;
	m_ModuleData.EndScale = 0.2f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	m_ModuleData.vStartColor = Vec4(0.2f, 0.3f, 1.0f,1.f);
	m_ModuleData.vEndColor = Vec4(0.4f, 1.f, 0.4f,1.f);

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_ModuleData.AddVelocityType = 0; // From Center
	m_ModuleData.Speed = 700.f;
	m_ModuleData.vVelocityDir;
	m_ModuleData.OffsetAngle;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	m_ModuleData.StartDrag = 500.f;
	m_ModuleData.EndDrag = -500.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_ModuleData.fNoiseTerm = 0.3f;
	m_ModuleData.fNoiseForce = 50.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_ModuleData.VelocityAlignment = true;
	m_ModuleData.VelocityScale = true;
	m_ModuleData.vMaxVelocityScale = Vec3(15.f, 1.f, 1.f);
	m_ModuleData.vMaxSpeed = 500.f;


	// ���� �޽�
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// ��ƼŬ ���� ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"),0);

	pParticleTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\HardCircle.png");

	// ��ƼŬ ������Ʈ ��ǻƮ ���̴�	
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);

	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _other)
	: CRenderComponent(_other)
	, m_ModuleData(_other.m_ModuleData)
	, m_UpdateCS(_other.m_UpdateCS)
	, pParticleTex(_other.pParticleTex)
	, m_AccTime(_other.m_AccTime)

{
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);

	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	m_ModuleDataBuffer = new CStructuredBuffer;

	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);

	// ���� �޽�
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// ��ƼŬ ���� ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"),0);

	GetMaterial(0)->SetTexParam(TEX_0, pParticleTex);

}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_RWBuffer)
		delete m_RWBuffer;

	if (nullptr != m_ModuleDataBuffer)
		delete m_ModuleDataBuffer;
}

//��ƼŬ ����ȭ���� ũ�� �缳�� �Լ�
//���� ��ƼŬ ����ȭ���� ũ�⺸�� �� ���� ��ƼŬ�� ����� ���� ��� ������������
//Imgui���� MaxParticleCount�������ٶ� �־������
void CParticleSystem::SetParticleBufferSize()
{
	if (m_ParticleBuffer->GetElementCount() < m_ModuleData.iMaxParticleCount)
	{
		m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);
	}
}

void CParticleSystem::finaltick()
{
	// ���� ����Ʈ ���
	// 1�� ���� �ð�
	float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
	m_AccTime += DT;

	// �����ð��� ���� �����ð��� �Ѿ��
	if (fTimePerCount < m_AccTime)
	{
		// �ʰ� ���� ==> ���� ����
		float fData = m_AccTime / fTimePerCount;

		// �������� ���� �ð�
		m_AccTime = fTimePerCount * (fData - floor(fData));

		// ���ۿ� ���� ī��Ʈ ����
		tRWParticleBuffer rwbuffer = { (int)fData, };		
		m_RWBuffer->SetData(&rwbuffer);
	}


	// ��ƼŬ ������Ʈ ��ǻƮ ���̴�
	m_ModuleDataBuffer->SetData(&m_ModuleData);

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetRWParticleBuffer(m_RWBuffer);
	m_UpdateCS->SetModuleData(m_ModuleDataBuffer);
	m_UpdateCS->SetNoiseTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_01.png"));
	m_UpdateCS->SetParticleObjectPos(Transform()->GetWorldPos());

	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	// ��ƼŬ���� t20 �� ���ε�
	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL_STAGES);

	// ��� ������ t21 �� ���ε�
	m_ModuleDataBuffer->UpdateData(21, PIPELINE_STAGE::PS_ALL_STAGES);

	// Particle Render	
	
	GetMaterial(0)->SetTexParam(TEX_0, pParticleTex);

	GetMaterial(0)->UpdateData();
	GetMesh()->render_particle(m_ModuleData.iMaxParticleCount);

	// ��ƼŬ ���� ���ε� ����
	m_ParticleBuffer->Clear();
	m_ModuleDataBuffer->Clear();
}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	
	fwrite(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	SaveResRef(m_UpdateCS.Get(), _File);
}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_ModuleData, sizeof(tParticleModule), 1, _File);

	int i = 0;
	fread(&i, sizeof(i), 1, _File);

	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();
	}
}

void CParticleSystem::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	CRenderComponent::SaveToLevelJsonFile(_objValue, allocator);

	// tParticleModule ����
	Value tModuelVal(kObjectType);

	// Spawn ���
	tModuelVal.AddMember("vSpawnColor",SaveVec4Json(m_ModuleData.vSpawnColor, allocator), allocator);
	tModuelVal.AddMember("vSpawnScaleMin", SaveVec4Json(m_ModuleData.vSpawnScaleMin, allocator), allocator);
	tModuelVal.AddMember("vSpawnScaleMax", SaveVec4Json(m_ModuleData.vSpawnScaleMax, allocator), allocator);
	tModuelVal.AddMember("vBoxShapeScale", SaveVec3Json(m_ModuleData.vBoxShapeScale, allocator), allocator);
	tModuelVal.AddMember("fSphereShapeRadius", m_ModuleData.fSphereShapeRadius, allocator);
	tModuelVal.AddMember("SpawnShapeType", m_ModuleData.SpawnShapeType, allocator);
	tModuelVal.AddMember("SpawnRate", m_ModuleData.SpawnRate, allocator);
	tModuelVal.AddMember("Space", m_ModuleData.Space, allocator);
	tModuelVal.AddMember("MinLifeTime", m_ModuleData.MinLifeTime, allocator);
	tModuelVal.AddMember("MaxLifeTime", m_ModuleData.MaxLifeTime, allocator);
	
	// spawnpad �迭�� ����
	Value spawnpadArray(kArrayType);
	for (int i = 0; i < 3; ++i) {
		spawnpadArray.PushBack(m_ModuleData.spawnpad[i], allocator);
	}
	tModuelVal.AddMember("spawnpad[3]", spawnpadArray, allocator);

	// Color Change ���
	tModuelVal.AddMember("vStartColor", SaveVec4Json(m_ModuleData.vStartColor, allocator), allocator);
	tModuelVal.AddMember("vEndColor", SaveVec4Json(m_ModuleData.vEndColor, allocator), allocator);

	// Scale Change ���
	tModuelVal.AddMember("StartScale", m_ModuleData.StartScale, allocator);
	tModuelVal.AddMember("EndScale", m_ModuleData.EndScale, allocator);

	// ���� �ִ�ũ��
	tModuelVal.AddMember("iMaxParticleCount", m_ModuleData.iMaxParticleCount, allocator);
	tModuelVal.AddMember("ipad", m_ModuleData.ipad, allocator);

	// Add Velocity ���
	tModuelVal.AddMember("vVelocityDir", SaveVec4Json(m_ModuleData.vVelocityDir, allocator), allocator);
	tModuelVal.AddMember("AddVelocityType", m_ModuleData.AddVelocityType, allocator);
	tModuelVal.AddMember("OffsetAngle", m_ModuleData.OffsetAngle, allocator);
	tModuelVal.AddMember("Speed", m_ModuleData.Speed, allocator);
	tModuelVal.AddMember("addvpad", m_ModuleData.addvpad, allocator);

	// Drag ��� - �ӵ� ����
	tModuelVal.AddMember("StartDrag", m_ModuleData.StartDrag, allocator);
	tModuelVal.AddMember("EndDrag", m_ModuleData.EndDrag, allocator);

	// NoiseForce ��� - ���� �� ����	
	tModuelVal.AddMember("fNoiseTerm", m_ModuleData.fNoiseTerm, allocator);
	tModuelVal.AddMember("fNoiseForce", m_ModuleData.fNoiseForce, allocator);

	// Gravity ��� - �߷� �߰�
	tModuelVal.AddMember("fGravityForce", m_ModuleData.fGravityForce, allocator);

	// GravityPad �迭�� ����
	Value GravityPadArray(kArrayType);
	for (int i = 0; i < 3; ++i) {
		GravityPadArray.PushBack(m_ModuleData.GravityPad[i], allocator);
	}
	tModuelVal.AddMember("GravityPad[3]", GravityPadArray, allocator);

	// Render ���
	tModuelVal.AddMember("VelocityAlignment", m_ModuleData.VelocityAlignment, allocator);
	tModuelVal.AddMember("VelocityScale", m_ModuleData.VelocityScale, allocator);
	tModuelVal.AddMember("AnimationUse", m_ModuleData.AnimationUse, allocator);
	tModuelVal.AddMember("AnimationLoop", m_ModuleData.AnimationLoop, allocator);
	tModuelVal.AddMember("bRotate", m_ModuleData.bRotate, allocator);

	tModuelVal.AddMember("vMaxSpeed", m_ModuleData.vMaxSpeed, allocator);
	tModuelVal.AddMember("fRotationAngle", m_ModuleData.fRotationAngle, allocator);
	tModuelVal.AddMember("fRotateSpeed", m_ModuleData.fRotateSpeed, allocator);
	tModuelVal.AddMember("vMaxVelocityScale", SaveVec4Json(m_ModuleData.vMaxVelocityScale, allocator), allocator);

	tModuelVal.AddMember("iAnimXCount", m_ModuleData.iAnimXCount, allocator);
	tModuelVal.AddMember("iAnimYCount", m_ModuleData.iAnimYCount, allocator);
	tModuelVal.AddMember("fAnimFrmTime", m_ModuleData.fAnimFrmTime, allocator);

	tModuelVal.AddMember("renderpad", m_ModuleData.renderpad, allocator);

	// Module Check  �迭�� ����
	Value ModuleCheckArray(kArrayType);
	for (int i = 0; i < (UINT)PARTICLE_MODULE::END; ++i) {
		ModuleCheckArray.PushBack(m_ModuleData.ModuleCheck[i], allocator);
	}
	tModuelVal.AddMember("ModuleCheck[PARTICLE_MODULE]", ModuleCheckArray, allocator);
	_objValue.AddMember("ParticleModule", tModuelVal, allocator);
	// tParticleModule ���� ��

	// m_UpdateCS 
	string key = "UpdateCS";
	Value keyName(kStringType);
	keyName.SetString(key.c_str(), key.length(), allocator);
	_objValue.AddMember(keyName, SaveResRefJson(m_UpdateCS.Get(), allocator), allocator);
}

void CParticleSystem::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CRenderComponent::LoadFromLevelJsonFile(_componentValue);

	// ���� ���
	m_ModuleData.vSpawnColor = LoadVec4Json(_componentValue["ParticleModule"]["vSpawnColor"]);
	m_ModuleData.vSpawnScaleMin = LoadVec4Json(_componentValue["ParticleModule"]["vSpawnScaleMin"]);
	m_ModuleData.vSpawnScaleMax = LoadVec4Json(_componentValue["ParticleModule"]["vSpawnScaleMax"]);
	m_ModuleData.vBoxShapeScale = LoadVec3Json(_componentValue["ParticleModule"]["vBoxShapeScale"]);
	m_ModuleData.fSphereShapeRadius = _componentValue["ParticleModule"]["fSphereShapeRadius"].GetFloat();
	m_ModuleData.SpawnShapeType = _componentValue["ParticleModule"]["SpawnShapeType"].GetInt();
	m_ModuleData.SpawnRate = _componentValue["ParticleModule"]["SpawnRate"].GetInt();
	m_ModuleData.Space = _componentValue["ParticleModule"]["Space"].GetInt();
	m_ModuleData.MinLifeTime = _componentValue["ParticleModule"]["MinLifeTime"].GetFloat();
	m_ModuleData.MaxLifeTime = _componentValue["ParticleModule"]["MaxLifeTime"].GetFloat();
	
	// pad�� �Ⱦ��� Load ���ص�
	// m_ModuleData.spawnpad[3];
	
	// Color Change ���
	m_ModuleData.vStartColor = LoadVec4Json(_componentValue["ParticleModule"]["vStartColor"]);
	m_ModuleData.vEndColor = LoadVec4Json(_componentValue["ParticleModule"]["vEndColor"]);
	
	// Scale Change ���
	m_ModuleData.StartScale = _componentValue["ParticleModule"]["StartScale"].GetFloat();
	m_ModuleData.EndScale = _componentValue["ParticleModule"]["EndScale"].GetFloat();

	// ���� �ִ�ũ��
	m_ModuleData.iMaxParticleCount = _componentValue["ParticleModule"]["iMaxParticleCount"].GetInt();
	//m_ModuleData.ipad = _componentValue["ParticleModule"]["ipad"].GetInt();

	// Add Velocity ���
	m_ModuleData.vVelocityDir = LoadVec4Json(_componentValue["ParticleModule"]["vVelocityDir"]);
	m_ModuleData.AddVelocityType = _componentValue["ParticleModule"]["AddVelocityType"].GetInt();
	m_ModuleData.OffsetAngle = _componentValue["ParticleModule"]["OffsetAngle"].GetFloat();
	m_ModuleData.Speed = _componentValue["ParticleModule"]["Speed"].GetFloat();
	//m_ModuleData.addvpad = _componentValue["ParticleModule"]["addvpad"].GetInt();

	// Drag ��� - �ӵ� ����
	m_ModuleData.StartDrag = _componentValue["ParticleModule"]["StartDrag"].GetFloat();
	m_ModuleData.EndDrag = _componentValue["ParticleModule"]["EndDrag"].GetFloat();

	// NoiseForce ��� - ���� �� ����	
	m_ModuleData.fNoiseTerm = _componentValue["ParticleModule"]["fNoiseTerm"].GetFloat();
	m_ModuleData.fNoiseForce = _componentValue["ParticleModule"]["fNoiseForce"].GetFloat();

	// Gravity ��� - �߷� �߰�
	m_ModuleData.fGravityForce = _componentValue["ParticleModule"]["fGravityForce"].GetFloat();
	// pad�� �Ⱦ��� Load ���ص�
	// float	GravityPad[3];

	// Render ���
	m_ModuleData.VelocityAlignment = _componentValue["ParticleModule"]["VelocityAlignment"].GetInt();
	m_ModuleData.VelocityScale = _componentValue["ParticleModule"]["VelocityScale"].GetInt();
	m_ModuleData.AnimationUse = _componentValue["ParticleModule"]["AnimationUse"].GetInt();
	m_ModuleData.AnimationLoop = _componentValue["ParticleModule"]["AnimationLoop"].GetInt();
	m_ModuleData.bRotate = _componentValue["ParticleModule"]["bRotate"].GetInt();

	m_ModuleData.vMaxSpeed = _componentValue["ParticleModule"]["vMaxSpeed"].GetFloat();
	m_ModuleData.fRotationAngle = _componentValue["ParticleModule"]["fRotationAngle"].GetFloat();
	m_ModuleData.fRotateSpeed = _componentValue["ParticleModule"]["fRotateSpeed"].GetFloat();
	m_ModuleData.vMaxVelocityScale = LoadVec4Json(_componentValue["ParticleModule"]["vMaxVelocityScale"]);

	m_ModuleData.iAnimXCount = _componentValue["ParticleModule"]["iAnimXCount"].GetInt();
	m_ModuleData.iAnimYCount = _componentValue["ParticleModule"]["iAnimYCount"].GetInt();
	m_ModuleData.fAnimFrmTime = _componentValue["ParticleModule"]["fAnimFrmTime"].GetFloat();
	//m_ModuleData.renderpad = _componentValue["ParticleModule"]["renderpad"].GetInt();
	
	// Module Check
	const Value& moduleCheckArray = _componentValue["ParticleModule"]["ModuleCheck[PARTICLE_MODULE]"];
	size_t moduleCheckSize = moduleCheckArray.Size();
	for (size_t i = 0; i < moduleCheckSize; ++i) {
		m_ModuleData.ModuleCheck[i] = moduleCheckArray[i].GetUint();
	}
	// tParticleModule �ε� ��
	

	// m_UpdateCS
	if (_componentValue["UpdateCS"]["IsNull"].GetBool())
	{
		wstring strKey, strRelativePath;
		strKey = StrToWStr(_componentValue["UpdateCS"]["Key"].GetString());
		strRelativePath = StrToWStr(_componentValue["UpdateCS"]["RelativePath"].GetString());

		m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();
	}
}
