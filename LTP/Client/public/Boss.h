#pragma once
#include "GameObject.h"
#include "PartilceCreateMgr.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CBoss abstract : public CGameObject
{
protected:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;



public:
	//	 #Particle Func
	virtual HRESULT		Ready_ParticleDesc();
	virtual HRESULT		Set_Play_Particle(_uint ParticleIndex, _fVector FixVec = _fVector(), _float3 offset = _float3::Zero(), _float Timer = -1);
	virtual HRESULT		Set_Play_Particle_Must(_uint ParticleIndex, _float3 FixPos = _float3::Zero(), _float3 offset = _float3::Zero(), _float Timer = -1);
	virtual HRESULT		Set_Dead_Transform(_uint ParticleIndex);

	virtual HRESULT		Update_Particle(_double timer);

	virtual HRESULT		Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ type, CTransform* trans, bool * pb = nullptr);
	INSTPARTICLEDESC&	Get_VecParticle(_uint index);
	void				Set_VecParticle(_uint index, INSTPARTICLEDESC& d);


protected:
//	CTransform*						m_PartilceTransform = nullptr;
	vector<INSTPARTICLEDESC>		m_vecTextureParticleDesc;
	vector<INSTMESHDESC>			m_vecMeshParticleDesc;
	_float							m_fPlayParticleTimer[PARTILCECOUNT] = { 0, };

protected:
	_bool		m_bIsHit = false;
	_bool		m_bIsAttack = false;
	_float		m_fAttackCoolTime = 0.f;
	_float		m_fSkillCoolTime = 0.f;
	_float		m_fAnimmultiple = 1.f; // AnimationPlaySpeed
	_float		m_fRange = 0.f; // 오브젝트간의 거리변수

public:
	virtual void Free() override;
};

END
