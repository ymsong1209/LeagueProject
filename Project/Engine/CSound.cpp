#include "pch.h"
#include "CSound.h"

#include "CResMgr.h"

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2);

FMOD::System* CSound::g_pFMOD = nullptr;

CSound::CSound()
	: CRes(RES_TYPE::SOUND)
	, m_pSound(nullptr)
	, m_bIs3D(false)
{
}

CSound::~CSound()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}


int CSound::Play(int _iRoopCount, float _fVolume, bool _bOverlap, float _fRange, const Vec3& _vPosition)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
	if (!_bOverlap && !m_listChannel.empty())
	{
		return -1;
	}

	_iRoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);
	// 사운드가 3D인 경우
	if (m_bIs3D)
	{
		// 사운드의 위치 설정
		FMOD_VECTOR position = { _vPosition.x, _vPosition.z, _vPosition.y };
		FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
		pChannel->set3DAttributes(&position, &velocity);
		// 사운드의 범위 설정
		m_pSound->set3DMinMaxDistance(_fRange, _fRange*2.f);
	}

	pChannel->setVolume(_fVolume);

	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	
	m_listChannel.push_back(pChannel);
	

	int iIdx = -1;
	pChannel->getIndex(&iIdx);

	return iIdx;
}

void CSound::Stop()
{
	list<FMOD::Channel*>::iterator iter;

	while (!m_listChannel.empty())
	{
		iter = m_listChannel.begin();
		(*iter)->stop();
	}
}

void CSound::SetVolume(float _f, int _iChannelIdx)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

	int iIdx = -1;
	for (; iter != m_listChannel.end(); ++iter)
	{
		(*iter)->getIndex(&iIdx);
		if (_iChannelIdx == iIdx)
		{
			(*iter)->setVolume(_f);
			return;
		}
	}
}

void CSound::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		if (*iter == _pTargetChannel)
		{
			m_listChannel.erase(iter);
			return;
		}
	}
}

int CSound::Load(const wstring& _strFilePath)
{
	string path(_strFilePath.begin(), _strFilePath.end());

	// 파일 경로에서 'sound2d' 또는 'sound3d'가 포함되어 있는지 확인
	FMOD_MODE mode;
	if (path.find("sound2d") != string::npos)
	{
		mode = FMOD_2D| FMOD_3D_LINEARROLLOFF;
	}
	else if (path.find("sound3d") != string::npos)
	{
		m_bIs3D = true;
		mode = FMOD_3D| FMOD_3D_LINEARROLLOFF|FMOD_3D_WORLDRELATIVE;
	}
	else
	{
		assert(false);  // 'sound2d' 또는 'sound3d'가 포함되어 있지 않으면 에러
	}

	if (FMOD_OK != g_pFMOD->createSound(path.c_str(), mode, nullptr, &m_pSound))
	{
		assert(nullptr);
	}

	return S_OK;
}



// =========
// Call Back
// =========
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	CSound* pSound = nullptr;

	switch (controltype)
	{
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}

void CSound::Update3DAttributes(Vec3 _SoundPosition)
{
	FMOD_VECTOR position = { _SoundPosition.x, _SoundPosition.z, _SoundPosition.y };
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };  // 속도는 일단 0으로 설정. 필요하다면 실제 오브젝트의 속도로 설정해야 함.
	for (auto& channel : m_listChannel) {
		channel->set3DAttributes(&position, &velocity);
		// 사운드의 범위 설정
		m_pSound->set3DMinMaxDistance(0, 1000.f);
	}
}

void CSound::UpdateListenerAttributes(Vec3 PlayerPos, Vec3 PlayerForward, Vec3 PlayerUp)
{
	FMOD_VECTOR position = { PlayerPos.x, PlayerPos.z, PlayerPos.y };  // y와 z를 교환
	FMOD_VECTOR velocity = { 0.f, 0.0f, 0.0f };  // 속도는 일단 0으로 설정. 필요하다면 실제 플레이어의 속도로 설정해야 함.
	FMOD_VECTOR forward = { PlayerForward.x, PlayerForward.z, PlayerForward.y };  // 전방향에 대해서도 y와 z를 교환
	FMOD_VECTOR up = { PlayerUp.x, PlayerUp.z, PlayerUp.y };  // 위 방향에 대해서도 y와 z를 교환

	CSound::g_pFMOD->set3DListenerAttributes(0, &position, &velocity, &forward, &up);

	//CSound::g_pFMOD->set3DSettings(1.0f, 0000.1f, 1.0f);
}
