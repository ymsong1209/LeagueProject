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
	//new로 생성된 m_bgm은 soundmgr에서 자동으로 삭제해줌
	//아래는 클라에서 실행해보기 위한 예시 코드임. script작성할때 이거 쓰면 안됨.
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
	if (KEY_TAP(KEY::I)) {
		//bgm의 초기 위치를 1000.f로 설정 사거리는 1000(500~1000까지 급격하게 떨어짐)
		int bgmindex = m_BGM->GetSoundIndex();
		CSoundMgr::GetInst()->Play(bgmindex, 0, 1, true, 500.f, Vec3(1000.f, 0.f, 0.f));
		CSoundMgr::GetInst()->Stop(bgmindex);
		CSoundMgr::GetInst()->Play(bgmindex, 0, 1, true, 500.f, Vec3(1000.f, 0.f, 0.f));
	}
	if (m_BGM) {
		// 소리 위치가 변경되었을 경우, 그 위치로 update
		int bgmindex = m_BGM->GetSoundIndex();
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
		//CSoundMgr::GetInst()->Update3DAttributes(bgmindex, Vec3(1000.f, 0.f, 0.f));
	}

	if (KEY_TAP(KEY::O)) {
		int bgmindex2 = m_BGM2->GetSoundIndex();
		CSoundMgr::GetInst()->Play(bgmindex2, 0, 1, true, 500.f, Vec3(0.f, 0.f, 1000.f));
		CSoundMgr::GetInst()->Stop(bgmindex2);
		CSoundMgr::GetInst()->Play(bgmindex2, 0, 1, true, 500.f, Vec3(0.f, 0.f, 1000.f));
	}
	if (m_BGM2) {
		// 소리 위치가 변경되었을 경우, 그 위치로 update

		int bgmindex = m_BGM2->GetSoundIndex();
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
		//CSoundMgr::GetInst()->Update3DAttributes(bgmindex, Vec3(0.f, 0.f, 1000.f));
	}

	if (KEY_TAP(KEY::K)) {
		/*const map<wstring, Ptr<CRes>> SoundRes = CResMgr::GetInst()->GetResources(RES_TYPE::SOUND);
		for (const auto& kv : SoundRes) {
			Ptr<CRes> Res = kv.second;
			((CSound*)Res.Get())->Stop();
		}*/
		m_BGM->StopAllSound();
	}

	//서버 친화적 코드
	//슈도코드이기 때문에 일부러 실행안하게 함
	if(false)
	{
		//서버에게 사운드 재생해달라고 요청하는 코드
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound3d\\Season1Rift.mp3");
		SoundInfo sound;
		sound.soundName = L"sound3d\\Season1Rift.mp3";
		sound.iLoopCount = 0;
		sound.fVolume = 1.f;
		sound.bOverlap = true;
		sound.fRange = 500.f;
		sound.soundPos = SoundInfo::vec3Server{ 0.f, 0.f, 1000.f };

		//대충 서버에 쏴주세요 하는 코드
		//server->sendinfo(sound);


		// 서버에서 사운드를 받는 코드
		// 서버에 사운드를 쏜 사람도 이 패킷을 받아야함
		//이건 script에 짜여질게 아니라 패킷 처리되는 부분에서 해야함
		{
			CSound* ServerSound = new CSound;
			//SoundInfo에는 상대경로값만 들어가있음
			wstring filepath = CPathMgr::GetInst()->GetContentPath();
			filepath += sound.soundName;
			ServerSound->Load(filepath);
			CSoundMgr::GetInst()->AddSound(ServerSound);
			Vec3 SoundPos = Vec3(sound.soundPos.x, sound.soundPos.y, sound.soundPos.z);
			CSoundMgr::GetInst()->Play(ServerSound->GetSoundIndex(), sound.iLoopCount, sound.fVolume, sound.bOverlap, sound.fRange, SoundPos);
			//정체모를 버그로 인해 stop을 한번은 해줘야함
			//서버에서 패킷을 받아 사운드를 생성하는 시점에서는 serversound*를 알고 있기 때문에 stop을 할 수가 있다.
			//대신 script에서는 이 포인터를 받아올 방법이 없어서 stop함수 실행못함
			CSoundMgr::GetInst()->Stop(ServerSound->GetSoundIndex());
			CSoundMgr::GetInst()->Play(ServerSound->GetSoundIndex(), sound.iLoopCount, sound.fVolume, sound.bOverlap, sound.fRange, SoundPos);
		}
		

	}



}

