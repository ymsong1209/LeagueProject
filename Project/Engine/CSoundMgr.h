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
	vector<CSound*> m_vSounds;
public:
	void AddSound(CSound* _sound) { m_vSounds.push_back(_sound); }


};

