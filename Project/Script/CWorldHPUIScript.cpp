#include "pch.h"
#include "CWorldHPUIScript.h"

void CWorldHPUIScript::begin()
{
}


void CWorldHPUIScript::tick()
{
	//------디버깅용-------------

	if (KEY_PRESSED(KEY::B))
		CurrentHP += DT * 50.f;

	if (KEY_PRESSED(KEY::V))
		CurrentHP -= DT * 50.f;

	if (KEY_PRESSED(KEY::G))
		CurrentMP += DT * 50.f;

	if (KEY_PRESSED(KEY::F))
		CurrentMP -= DT * 50.f;
	//--------------------------------

	//CurrentExpRatio = CurrentExp / LevelTotalExp;
	//if (CurrentExp >= LevelTotalExp)
	//	CurrentExp = LevelTotalExp;
	//if (CurrentExp <= 0.f)
	//	CurrentExp = 0.f;

	//----------HP------------
	CurrentHPRatio = CurrentHP / TotalHP;
	if (CurrentHP >= TotalHP)
		CurrentHP = TotalHP;

	if (CurrentHP <= 0.f)
		CurrentHP = 0.f;
	//-------------------------

	//----------MP------------
	CurrentMPRatio = CurrentMP / TotalMP;
	if (CurrentMP >= TotalMP)
		CurrentMP = TotalMP;

	if (CurrentMP <= 0.f)
		CurrentMP = 0.f;
	//-------------------------

	//디버깅용
	CurrentHPRatio = 0.6f;
	CurrentMPRatio = 0.3f;
	//MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &CurrentHPRatio);
	//MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, &CurrentMPRatio);

	
	// 하나의 렉트메쉬, 하나의 머터리얼로 hp,mp같이 처리할수가 없음 hp,mp이미지 모두 하나의 렉트메쉬에 맵핑되기때문
	// 그래서 머터리얼을 여러개 들고있을수 있어야 하나의 오브젝트로 각각의 셰이더로 여러개를 처리하는데,
	//그게 안되면 하나의 오브젝트가 하나의 머터리얼을 처리하도록 만들어야함 (hp오브젝트, mp오브젝트 각각..)
	
	// mp가 닳는거 따로, hp가 닳는거 따로 해줘야 하는데(hp나 mp소진되게하는걸 픽셀을 지우는걸로 처리하기때문)
	//지금 렉트메쉬 + 머터리얼 3개 이런식으로 SetMaterial인덱스를 다르게해서 머터리얼을 각각설정해주었으나 그게 안되는상태임
	//그렇다고 같은 머터리얼에 텍스쳐HP,MP 두개, 현재 HP,MP ratio변수 두개 이렇게 넘겨도 결국 하나의 렉트메쉬에 맵핑되는거라 위치 조절이 힘듬.
	//또한.. 닳게 하려면 hp,mp개별로 닳아야 하는데 그것도 힘듬. (hp바만큼 닳게 하면 mp바 부분도 같이 닳게될것임)

	// --> 해결? : 결국렉트메쉬의 인덱스버퍼가 하나라서.. 여러개의 머터리얼을 장착할수없음 그래서 결국 개별 오브젝트로 처리해줘야함.
}

void CWorldHPUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CWorldHPUIScript::CWorldHPUIScript()
	:CScript((UINT)SCRIPT_TYPE::WORLDHPUISCRIPT)
	, TotalMP(100.f)
	, TotalHP(100.f)
{
}

CWorldHPUIScript::~CWorldHPUIScript()
{
}
