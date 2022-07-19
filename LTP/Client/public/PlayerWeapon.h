#pragma once

#include "GameObject.h"
#include "PartilceCreateMgr.h"

BEGIN(Client)

class CPlayerWeapon abstract : public CGameObject
{
public:
	enum EWeaponState { STATE_STRUCTURE, STATE_EQUIP, STATE_NON_EQUIP, STATE_END };

	typedef struct tagPlayerWeaponDesc
	{
		EWeaponState	eWeaponState;
		ATTACHEDESC		eAttachedDesc;
		CGameObject*	pOwner;
	}PlayerWeaponDesc;

protected:
	CPlayerWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon(const CPlayerWeapon& rhs);
	virtual ~CPlayerWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

public:
	virtual void		Dissolve_In(_double fTargetTime);
	virtual void		Dissolve_Out(_double fTargetTime);

public:
	void				Active_Collision();
	void				DeActive_Collision();

	void				Active_Collision_1();
	void				DeActive_Collision_1();

	void				Active_Collision_2();
	void				DeActive_Collision_2();

public:
	virtual void		Active_Trail(_bool bActivate);
	virtual _bool		AbleToChangeWeapon();

public:
	void				Set_BlockUpdate(_bool bBlock);
	_bool				Get_BlockUpdate() { return m_bBlockUpdate; }

	// #Particle Func
	virtual HRESULT Ready_ParticleDesc();
	virtual HRESULT Set_Play_Particle(_uint ParticleIndex, _float3 FixPos = _float3::Zero(), _float3 offset = _float3::Zero(), _float Timer = -1);
	virtual HRESULT Set_Play_Particle_Must(_uint ParticleIndex, _float3 FixPos = _float3::Zero(), _float3 offset = _float3::Zero(), _float Timer = -1);
	virtual HRESULT Set_Dead_Transform(_uint ParticleIndex);
	virtual HRESULT Update_Particle(_double timer);

	virtual HRESULT Set_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ type, CTransform* trans, bool bb, bool * pb = nullptr);





private:
	virtual _fVector	Get_BonePos(const char* pBoneName) override;
	virtual void		Update_AttachCamPos() override;

protected:
	/* Partilce */
	vector<INSTPARTICLEDESC>		m_vecTextureParticleDesc;
	vector<INSTMESHDESC>			m_vecMeshParticleDesc;
	_float							m_fPlayParticleTimer[PARTILCECOUNT] = { 0, };

protected:
	PlayerWeaponDesc			m_tPlayerWeaponDesc;
	_bool						m_bBlockUpdate = false;
	_bool						m_bActiveTrail = false;
	_bool						m_bActiveCollision = false;
	_bool						m_bActiveCollision_1 = false;
	_bool						m_bActiveCollision_2 = false;


public:
	virtual CGameObject*		Clone(void* pArg)PURE;
	virtual void				Free() override;
};
END
