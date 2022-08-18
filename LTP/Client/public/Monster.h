#pragma once
#include "GameObject.h"
#include "PartilceCreateMgr.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CMonster abstract : public CGameObject
{
protected:
	explicit CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT	Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;




protected:
//	 #Particle Func
	virtual HRESULT		Ready_ParticleDesc();
	virtual HRESULT		Update_Particle(_double timer);

	virtual HRESULT		Play_SpawnEffect(); // SpawnEffect
	void				Set_DealyDIssolveTime(_double time, _double disTIme = 0.5f)
	{
		m_SpawnDealytime = time; 
		m_SpawnDissolveTime = disTIme;
	}

	virtual HRESULT		Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ type, CTransform* trans, bool * pb = nullptr);



protected:
	/* Partilce */
	_uint							m_EffectAdjust = 0;
	_bool							m_bisSpawnParticle = false;
	_bool							m_bIsSpawnPartilceOne = false;
	_bool							m_bIsSpawnDissolove = false;
	_uint							m_SpawnEffectAdjust = 0;
	_double							m_SpawnDealytime = 0;
	_double							m_SpawnDissolveTime= 0.5f;

	// BaseParticle
	_bool							m_bIsInitParticle = false;

	//	CModelInstance*					m_pModelInstance = nullptr;
//	vector<CTransform*>				m_vecInstancedTransform;




protected:
	CNavigation*	 m_pNavigationCom = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END