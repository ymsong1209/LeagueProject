#include "pch.h"
#include "CSoundTestScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CSoundMgr.h>
#include <Engine/CSound.h>
#include <Engine/CResMgr.h>
#include <Engine/CPathMgr.h>

CSoundTestScript::CSoundTestScript()
	: CScript((UINT)SCRIPT_TYPE::SOUNDTESTSCRIPT)
	, m_BGM(nullptr)
{
}

CSoundTestScript::~CSoundTestScript()
{
}

void CSoundTestScript::begin()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound3d\\Season1Rift.mp3");
	m_BGM = new CSound;
	m_BGM2 = new CSound;
	wstring filepath = CPathMgr::GetInst()->GetContentPath();
	filepath += pSound.Get()->GetRelativePath();
	CSoundMgr::GetInst()->AddSound(m_BGM);
	CSoundMgr::GetInst()->AddSound(m_BGM2);
	m_BGM->Load(filepath);
	m_BGM2->Load(filepath);
}

void CSoundTestScript::tick()
{
	//if (KEY_TAP(KEY::I)) {
	//	//bgm의 초기 위치를 1000.f로 설정 사거리는 1000(500~1000까지 급격하게 떨어짐)
	//	int a = m_BGM->Play(0, 1, true, 500.f, Vec3(1000.f, 0.f, 0.f));
	//	m_BGM->Stop();
	//	m_BGM->Play(0, 1, true, 500.f, Vec3(1000.f, 0.f, 0.f));
	//}
	//if (m_BGM) {
	//	// 소리 위치가 변경되었을 경우, 그 위치로 update
	//	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	//	m_BGM->Update3DAttributes(Vec3(1000.f, 0.f, 0.f));
	//}

	//if (KEY_TAP(KEY::O)) {
	//	int a = m_BGM2->Play(0, 1, true, 500.f, Vec3(0.f, 0.f, 1000.f));
	//	m_BGM2->Stop();
	//	m_BGM2->Play(0, 1, true, 500.f, Vec3(0.f, 0.f, 1000.f));
	//}
	//if (m_BGM2) {
	//	// 소리 위치가 변경되었을 경우, 그 위치로 update
	//	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	//	m_BGM2->Update3DAttributes(Vec3(0.f, 0.f, 1000.f));
	//}

	//if (KEY_TAP(KEY::K)) {
	//	/*const map<wstring, Ptr<CRes>> SoundRes = CResMgr::GetInst()->GetResources(RES_TYPE::SOUND);
	//	for (const auto& kv : SoundRes) {
	//		Ptr<CRes> Res = kv.second;
	//		((CSound*)Res.Get())->Stop();
	//	}*/
	//	m_BGM->Stop();
	//}
}
