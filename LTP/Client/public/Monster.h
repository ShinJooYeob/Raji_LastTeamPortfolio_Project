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
//	 #Particle Func NOUSE
//	virtual HRESULT		Set_Play_Particle(_uint ParticleIndex, _fVector FixVec = _fVector(), _float3 offset = _float3::Zero(), _float Timer = -1);
//	virtual HRESULT		Set_Play_Particle_Must(_uint ParticleIndex, _float3 FixPos = _float3::Zero(), _float3 offset = _float3::Zero(), _float Timer = -1);
//	virtual HRESULT		Set_Dead_Transform(_uint ParticleIndex);
//	INSTPARTICLEDESC&	Get_VecParticle(_uint index);
//	void				Set_VecParticle(_uint index, INSTPARTICLEDESC& d);

//	 #Particle Func
	virtual HRESULT		Ready_ParticleDesc();
	virtual HRESULT		Update_Particle(_double timer);

	virtual HRESULT		Play_SpawnEffect(); // SpawnEffect
	void				Set_DealyTimer(_double time) { m_SpawnDealytime = time; }

	virtual HRESULT		Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ type, CTransform* trans, bool * pb = nullptr);



protected:
	/* Partilce */
	_uint							m_EffectAdjust = 0;
	_bool							m_bisSpawnParticle = false;
	_double							m_SpawnDealytime = 0;

//	CModelInstance*					m_pModelInstance = nullptr;
//	vector<CTransform*>				m_vecInstancedTransform;




protected:
	CNavigation*	 m_pNavigationCom = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END