#pragma once

#include "Base.h"
#include "NonInstanceMeshEffect.h"


BEGIN(Client)

class CParticleCreater final :public CBase
{
private:
	explicit CParticleCreater(){}
	virtual ~CParticleCreater() = default;


public:
	HRESULT Initialize_ParticleMgr();

private:


public:
	virtual void Free()override;

};

END