#pragma once
#include <Engine\CScript.h>

class CSound;
class CSoundTestScript :
    public CScript
{
private:

    //���� script���� ���带 �̷��� �����ͷ� ������ ����
    //������ new�� �ؼ� �����ϴ� ������� ������
    //�׽�Ʈ�� ���� sound*�� ���ܵ���
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