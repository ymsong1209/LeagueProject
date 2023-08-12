#pragma once

class CSound;
//사운드는 CResMgr를 통해 관리되고 있지만 동일 사운드를 여러번 재생,
//혹은 동일 사운드를 중첩해서 여러군데 실행하고 싶으면 새롭게 사운드를 만들어서 실행해야함
//DynamicSound를 관리하는 Mgr라고 생각하면 된다.
class CSoundMgr
	: public CSingleton<CSoundMgr>
{
	SINGLE(CSoundMgr);
private:
	int					m_iSoundIndex;
	map<int, CSound*>   m_vSounds;
	
public:
	void AddSound(CSound* _sound);
	
	void Play(int SoundIndex, int _iLoopCount, float _fVolume = 1.f, bool _bOverlap = false, float _fRange = 0.f, const Vec3& _vPosition = Vec3());

	void Stop(int SoundIndex);
//현재 서버에서 사운드를 조절할 수 있는 방법이 없어서 private으로 막아뒀음
//서버게임이 아니라 client게임을 사용할 경우, private을 풀고 맘껏 사용가능
private:
	// 0 ~ 1
	void SetVolume(float _Volume, int _iSoundIndex);

	void Update3DAttributes(int _isoundindex,  Vec3 _SoundPosition);

};

