#pragma once
#include "CRes.h"

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

class CSound :
	public CRes
{
public:
	static FMOD::System* g_pFMOD;

private:
	int						m_iSoundIndex;//SoundMgr���Լ� �ο����� Number;
	int						m_iChannelIndex; //play�� �������� channel index??
	
	FMOD::Sound* m_pSound;
	list<FMOD::Channel*>	m_listChannel;
	bool					m_bIs3D;

private:
	// 0 (���ѹݺ�) 0 ~ 1(Volume)
	int Play(int _iLoopCount, float _fVolume = 1.f, bool _bOverlap = false, float _fRange = 0.f, const Vec3& _vPosition = Vec3());
	void Stop(int _channelIndex);

	// ������ ��ġ�� �Ű��������� ȣ��Ǿ����
	void Update3DAttributes(Vec3 _SoundPosition);

	void SetSoundIndex(int _index) { m_iSoundIndex = _index; }

public:
	void StopAllSound();

	// 0 ~ 1
	void SetVolume(float _Volume, int _iChannelIdx);

	void AssignNum(int _num) { m_iSoundIndex = _num; }
	int  GetSoundIndex() { return m_iSoundIndex; }
	int  GetChannelIndex() { return m_iChannelIndex; }

private:
	void RemoveChannel(FMOD::Channel* _pTargetChannel);
	friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2);

public:
	virtual void UpdateData() {}

	// ���Ϸ� ����
	virtual int Save(const wstring&) override { return S_OK; }

	// ���Ϸκ��� �ε�
	virtual int Load(const wstring& _strFilePath) override;

	

	// ���带 ��� listener�� ��ġ�� �ű�. �̰� script�ʿ��� �� tick���� ȣ���ؾ���
	static void UpdateListenerAttributes(Vec3 PlayerPos, Vec3 PlayerForward, Vec3 PlayerUp);
		
public:
	CSound();
	virtual ~CSound();

	friend class CSoundMgr;
};

