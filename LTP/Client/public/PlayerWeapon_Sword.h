#pragma once

#include "PlayerWeapon.h"

BEGIN(Client)

class CPlayerWeapon_Sword final : public CPlayerWeapon
{
public:
	enum ESwordPivot { SWORD_PIVOT_NORMAL, SWORD_PIVOT_THROW, SWORD_PIVOT_TAKEDOWN, SWORD_PIVOT_END };

private:
	CPlayerWeapon_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon_Sword(const CPlayerWeapon_Sword& rhs);
	virtual ~CPlayerWeapon_Sword() = default;

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
	virtual void		EffectParticleOn(_uint iIndex , void* pArg = nullptr) override;

	virtual void		Set_WeaponDamage(EAttackType eAttackType, _int iComboCount) override;

public:
	void				Set_ShieldBashAttack(_bool bShieldBashAttack);

private:
	virtual _fVector	Get_BonePos(const char* pBoneName) override;
	virtual void		Update_AttachCamPos() override;

	_int				Update_Structure(_double fDeltaTime);
	_int				Update_Equip(_double fDeltaTime);
	_int				Update_NoEquip(_double fDeltaTime);

	_int				LateUpdate_Structure(_double fDeltaTime);
	_int				LateUpdate_Equip(_double fDeltaTime);
	_int				LateUpdate_NoEquip(_double fDeltaTime);

	void				Update_AttachMatrix();
	void				Update_Trail(_fMatrix* pMat, _double fDeltaTime);
	void				Update_Colliders();
	void				Update_Colliders_MainAttackCombo3();
	void				Update_Colliders_PowerAttackCombo3();
	void				Update_Colliders_UltimateAttack();

	HRESULT				Ready_ParticleDesc();

public:
	void				Change_Pivot(ESwordPivot ePitvot);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();
	HRESULT				SetUp_Collider();


private:
	_float4x4			m_fAttachedMatrix;
	CHierarchyNode*		m_pAttachedNode = nullptr;
	_float				m_fMaxTime_ClearTrail = 0.f;
	_float				m_fCurTime_ClearTrail = 0.f;
	_bool				m_bShieldBashAttack = false;

	vector<_float>			m_Arr_MainAttackDamage[4];
	vector<_float>			m_Arr_StrongAttackDamage[4];
	_float					m_fSubAttackDamage = 1.f;


private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CSwordTrail*		m_pSwordTrail = nullptr;
	CCollider*			m_pCollider = nullptr;
	CCollider*			m_pCollider_MainAttack_Combo3 = nullptr;
	CCollider*			m_pCollider_PowerAttack_Combo3 = nullptr;
	CCollider*			m_pCollider_Ultimate = nullptr;

	CDissolve*			m_pDissolveCom = nullptr;

private:
	CTransform*						m_pTextureParticleTransform = nullptr;
	vector<INSTPARTICLEDESC>		m_vecTextureParticleDesc;
	CTransform*						m_pMeshParticleTransform = nullptr;
	vector<INSTMESHDESC>			m_vecMeshParticleDesc;
	_float							m_ParticlePassedTime = 0;
	_float							m_ParticleTargetTime = 0;
	_float3							m_vParticleMovingDir = _float3(0, 0, 1);

	vector<NONINSTNESHEFTDESC>								m_vecNonInstMeshDesc;
public:
	static CPlayerWeapon_Sword*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
