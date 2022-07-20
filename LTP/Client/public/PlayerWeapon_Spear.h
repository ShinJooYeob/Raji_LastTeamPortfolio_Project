#pragma once

#include "PlayerWeapon.h"

BEGIN(Client)

class CPlayerWeapon_Spear final : public CPlayerWeapon
{
public:
	enum ESpearPivot { SPEAR_PIVOT_NORMAL, SPEAR_PIVOT_THROW, SPEAR_PIVOT_TAKEDOWN, SPEAR_PIVOT_END };
	
private:
	CPlayerWeapon_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon_Spear(const CPlayerWeapon_Spear& rhs);
	virtual ~CPlayerWeapon_Spear() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

	virtual void		Active_Trail(_bool bActivate) override;

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _bool		AbleToChangeWeapon() override;

public:
	virtual void		Dissolve_In(_double fTargetTime) override;
	virtual void		Dissolve_Out(_double fTargetTime) override;

private:
	virtual _fVector	Get_BonePos(const char* pBoneName) override;
	virtual _fMatrix	Get_BoneMatrix(const char* pBoneName) override;
	virtual void		Update_AttachCamPos() override;


public:				
	void				Change_Pivot(ESpearPivot ePitvot);
	void				Throw_Start(_fVector vThrowDir);
	void				Throw_End();
	void				Throw(_double fDeltaTimer);

private:
	_int				Update_Structure(_double fDeltaTime);
	_int				Update_Equip(_double fDeltaTime);
	_int				Update_NoEquip(_double fDeltaTime);

	_int				LateUpdate_Structure(_double fDeltaTime);
	_int				LateUpdate_Equip(_double fDeltaTime);
	_int				LateUpdate_NoEquip(_double fDeltaTime);

	void				Update_AttachMatrix();
	void				Update_Trail(_fMatrix* pMat, _double fDeltaTime);
	void				Update_Colliders();
	void				Update_Colliders_1();
	void				Update_ParticleTransform();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();
	HRESULT				SetUp_Collider();

	HRESULT				Ready_ParticleDesc();

private:/*For Particle*/
	CTransform*						m_pTextureParticleTransform = nullptr;
	vector<INSTPARTICLEDESC>		m_vecTextureParticleDesc;
	CTransform*						m_pMeshParticleTransform = nullptr;
	vector<INSTMESHDESC>			m_vecMeshParticleDesc;

private:
	_float4x4				m_fAttachedMatrix;
	CHierarchyNode*			m_pAttachedNode = nullptr;
	_bool					m_bThrowing = false;
	_float3					m_bThrowDir;
	_uint					m_iPassNum;
	_uint					m_iCurAnim;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CSwordTrail*			m_pSwordTrail = nullptr;
	
	CModel*					m_pModel_Skill = nullptr;
	CTransform*				m_pTransformCom_Skill = nullptr;

	CCollider*				m_pCollider = nullptr;
	CCollider*				m_pCollider_Range = nullptr;


	CDissolve*				m_pDissolveCom = nullptr;

public:
	static CPlayerWeapon_Spear*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
