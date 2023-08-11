#pragma once
#include <Engine\CScript.h>

class CSound;
class CSoundTestScript :
    public CScript
{
private:

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