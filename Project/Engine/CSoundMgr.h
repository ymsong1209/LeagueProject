#pragma once

class CSound;
//����� CResMgr�� ���� �����ǰ� ������ ���� ���带 ������ ���,
//Ȥ�� ���� ���带 ��ø�ؼ� �������� �����ϰ� ������ ���Ӱ� ���带 ���� �����ؾ���
//DynamicSound�� �����ϴ� Mgr��� �����ϸ� �ȴ�.
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
//���� �������� ���带 ������ �� �ִ� ����� ��� private���� ���Ƶ���
//���������� �ƴ϶� client������ ����� ���, private�� Ǯ�� ���� ��밡��
private:
	// 0 ~ 1
	void SetVolume(float _Volume, int _iSoundIndex);

	void Update3DAttributes(int _isoundindex,  Vec3 _SoundPosition);

};

