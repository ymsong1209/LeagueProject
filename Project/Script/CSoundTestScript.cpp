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
	//new�� ������ m_bgm�� soundmgr���� �ڵ����� ��������
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
		//bgm�� �ʱ� ��ġ�� 1000.f�� ���� ��Ÿ��� 1000(500~1000���� �ް��ϰ� ������)
		int bgmindex = m_BGM->GetSoundIndex();
		CSoundMgr::GetInst()->Play(bgmindex, 0, 1, true, 500.f, Vec3(1000.f, 0.f, 0.f));
		CSoundMgr::GetInst()->Stop(bgmindex);
		CSoundMgr::GetInst()->Play(bgmindex, 0, 1, true, 500.f, Vec3(1000.f, 0.f, 0.f));
	}
	if (m_BGM) {
		// �Ҹ� ��ġ�� ����Ǿ��� ���, �� ��ġ�� update
		int bgmindex = m_BGM->GetSoundIndex();
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
		CSoundMgr::GetInst()->Update3DAttributes(bgmindex, Vec3(1000.f, 0.f, 0.f));
	}

	if (KEY_TAP(KEY::O)) {
		int bgmindex2 = m_BGM2->GetSoundIndex();
		CSoundMgr::GetInst()->Play(bgmindex2, 0, 1, true, 500.f, Vec3(0.f, 0.f, 1000.f));
		CSoundMgr::GetInst()->Stop(bgmindex2);
		CSoundMgr::GetInst()->Play(bgmindex2, 0, 1, true, 500.f, Vec3(0.f, 0.f, 1000.f));
	}
	if (m_BGM2) {
		// �Ҹ� ��ġ�� ����Ǿ��� ���, �� ��ġ�� update

		int bgmindex = m_BGM2->GetSoundIndex();
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
		CSoundMgr::GetInst()->Update3DAttributes(bgmindex, Vec3(0.f, 0.f, 1000.f));
	}

	if (KEY_TAP(KEY::K)) {
		/*const map<wstring, Ptr<CRes>> SoundRes = CResMgr::GetInst()->GetResources(RES_TYPE::SOUND);
		for (const auto& kv : SoundRes) {
			Ptr<CRes> Res = kv.second;
			((CSound*)Res.Get())->Stop();
		}*/
		m_BGM->StopAllSound();
	}
}
