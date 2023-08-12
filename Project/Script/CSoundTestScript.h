#pragma once
#include <Engine\CScript.h>

class CSound;
class CSoundTestScript :
    public CScript
{
private:

    //원래 script에서 사운드를 이렇게 포인터로 가지지 못함
    //무조건 new를 해서 생성하는 방식으로 가야함
    //테스트를 위해 sound*를 남겨뒀음
    CSound* m_BGM;
    CSound* m_BGM2;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CSoundTestScript);
public:
    CSoundTestScript();
    ~CSoundTestScript();
};