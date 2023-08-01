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

	// ����ǰ� �ִ� ä���� �ִµ�, �ߺ������ ������� �ʾҴ� -> ��� ����
	if (!_bOverlap && !m_listChannel.empty())
	{
		return -1;
	}

	_iRoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);
	// ���尡 3D�� ���
	if (m_bIs3D)
	{
		// ������ ��ġ ����
		FMOD_VECTOR position = { _vPosition.x, _vPosition.z, _vPosition.y };
		FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
		pChannel->set3DAttributes(&position, &velocity);
		// ������ ���� ����
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

	// ���� ��ο��� 'sound2d' �Ǵ� 'sound3d'�� ���ԵǾ� �ִ��� Ȯ��
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
		assert(false);  // 'sound2d' �Ǵ� 'sound3d'�� ���ԵǾ� ���� ������ ����
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
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };  // �ӵ��� �ϴ� 0���� ����. �ʿ��ϴٸ� ���� ������Ʈ�� �ӵ��� �����ؾ� ��.
	for (auto& channel : m_listChannel) {
		channel->set3DAttributes(&position, &velocity);
		// ������ ���� ����
		m_pSound->set3DMinMaxDistance(0, 1000.f);
	}
}

void CSound::UpdateListenerAttributes(Vec3 PlayerPos, Vec3 PlayerForward, Vec3 PlayerUp)
{
	FMOD_VECTOR position = { PlayerPos.x, PlayerPos.z, PlayerPos.y };  // y�� z�� ��ȯ
	FMOD_VECTOR velocity = { 0.f, 0.0f, 0.0f };  // �ӵ��� �ϴ� 0���� ����. �ʿ��ϴٸ� ���� �÷��̾��� �ӵ��� �����ؾ� ��.
	FMOD_VECTOR forward = { PlayerForward.x, PlayerForward.z, PlayerForward.y };  // �����⿡ ���ؼ��� y�� z�� ��ȯ
	FMOD_VECTOR up = { PlayerUp.x, PlayerUp.z, PlayerUp.y };  // �� ���⿡ ���ؼ��� y�� z�� ��ȯ

	CSound::g_pFMOD->set3DListenerAttributes(0, &position, &velocity, &forward, &up);

	//CSound::g_pFMOD->set3DSettings(1.0f, 0000.1f, 1.0f);
}
