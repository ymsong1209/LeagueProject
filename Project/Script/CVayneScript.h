#pragma once
#include "CChampionScript.h"
class CVayneScript :
    public CChampionScript
{
public:
    CVayneScript();
    ~CVayneScript();

private:

public: 
    virtual void begin() override;
    virtual void tick() override;

 
};

 

 