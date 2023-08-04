#include "pch.h"
#include "CSoundTestScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CSound.h>
#include <Engine/CResMgr.h>

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
}

void CSoundTestScript::tick()
{
	if (KEY_TAP(KEY::I)) {
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound3d\\Season1Rift.mp3");
		m_BGM = pSound.Get();
		//bgm�� �ʱ� ��ġ�� 1000.f�� ���� ��Ÿ��� 1000(500~1000���� �ް��ϰ� ������)
		int a = m_BGM->Play(0, 1, true, 500.f, Vec3(1000.f, 0.f, 0.f));
		m_BGM->Stop();
		m_BGM->Play(0, 1, true, 500.f, Vec3(1000.f, 0.f, 0.f));
	}
	if (m_BGM) {
		// �Ҹ� ��ġ�� ����Ǿ��� ���, �� ��ġ�� update
		Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
		m_BGM->Update3DAttributes(CurPos);
	}
	if (KEY_TAP(KEY::K)) {
		/*const map<wstring, Ptr<CRes>> SoundRes = CResMgr::GetInst()->GetResources(RES_TYPE::SOUND);
		for (const auto& kv : SoundRes) {
			Ptr<CRes> Res = kv.second;
			((CSound*)Res.Get())->Stop();
		}*/
		m_BGM->Stop();
	}
}
