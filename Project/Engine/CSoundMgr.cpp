#include "pch.h"
#include "CSoundMgr.h"
#include "CSound.h"


CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
	Safe_Del_Map(m_vSounds);
}

void CSoundMgr::AddSound(CSound* _sound)
{
	m_vSounds.insert(make_pair(m_iSoundIndex, _sound));
	_sound->SetSoundIndex(m_iSoundIndex);
	++m_iSoundIndex;
}

void CSoundMgr::Stop(int SoundIndex)
{
	if (SoundIndex == -1) return;
	CSound* sound = m_vSounds.find(SoundIndex)->second;
	sound->Stop(sound->GetChannelIndex());
}

void CSoundMgr::Play(int SoundIndex, int _iLoopCount, float _fVolume, bool _bOverlap, float _fRange, const Vec3& _vPosition)
{
	if (SoundIndex == -1) return;
	CSound* Sound = m_vSounds.find(SoundIndex)->second;
	if (Sound == nullptr) assert(nullptr);

	Sound->Play(_iLoopCount, _fVolume, _bOverlap, _fRange, _vPosition);
}

void CSoundMgr::SetVolume(float _f, int _iSoundIndex)
{
	if (_iSoundIndex == -1) return;
	CSound* sound = m_vSounds.find(_iSoundIndex)->second;
	int soundchannel = sound->GetChannelIndex();
	sound->SetVolume(_f, soundchannel);

}

void CSoundMgr::Update3DAttributes(int _isoundindex, Vec3 _SoundPosition)
{
	if (_isoundindex == -1) return;
	CSound* sound = m_vSounds.find(_isoundindex)->second;
	sound->Update3DAttributes(_SoundPosition);
}
