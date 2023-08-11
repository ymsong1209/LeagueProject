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
	vector<CSound*> m_vSounds;
public:
	void AddSound(CSound* _sound) { m_vSounds.push_back(_sound); }


};

