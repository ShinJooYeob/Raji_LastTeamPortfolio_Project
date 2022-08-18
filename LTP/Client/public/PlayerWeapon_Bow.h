#pragma once

#include "PlayerWeapon.h"

BEGIN(Client)

class CPlayerWeapon_Bow final : public CPlayerWeapon
{
public:
	enum EBowPivot { BOW_PIVOT_NORMAL, BOW_PIVOT_THROW, BOW_PIVOT_TAKEDOWN, BOW_PIVOT_END };

	enum EBowAnim {
		BOW_ANIM_IDLE, BOW_ANIM_NORMAL_READY, BOW_ANIM_NORMAL_LOOP, BOW_ANIM_NORMAL_SHOT, 
		BOW_ANIM_UTILITY_LOOP, BOW_ANIM_UTILITY_READY, BOW_ANIM_UTILITY_SHOT, BOW_ANIM_END
	};

private:
	CPlayerWeapon_Bow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon_Bow(const CPlayerWeapon_Bow& rhs);
	virtual ~CPlayerWeapon_Bow() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

public:
	virtual void		CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _bool		AbleToChangeWeapon() override;

public:
	virtual void		Dissolve_In(_double fTargetTime) override;
	virtual void		Dissolve_Out(_double fTargetTime) override;

public:
	void				Set_UltimateAttackPos(_fVector fPos);

public:
	void				PlayAnim_Idle();

	void				PlayAnim_NormalAttack_Ready();
	void				PlayAnim_NormalAttack_Shot();

	void				PlayAnim_UtilityAttack_Ready();
	void				PlayAnim_UtilityAttack_Loop();
	void				PlayAnim_UtilityAttack_Shot();

public:
	void				Set_AnimSpeed(_float fAnimSpeed);
	CTransform*			Get_EffectTransform()
	{
		return m_pTextureParticleTransform;
	}



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

	void				Set_Pivot();

public:
	void				Change_Pivot(EBowPivot ePitvot);

private:
	void				Update_Colliders_Ultimate();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();
	HRESULT				SetUp_Collider();


private:/*For Particle*/

	/* Particle */
	HRESULT Ready_ParticleDesc();
	HRESULT Update_Particle(_double timer);
	// HRESULT PlayParticle(_uint index);

	CTransform*						m_pTextureParticleTransform = nullptr;
	CTransform*						m_pTextureParticleTransform_BowUp = nullptr;
	CTransform*						m_pTextureParticleTransform_BowBack = nullptr;
	


private:
	_float4x4			m_fAttachedMatrix;
	CHierarchyNode*		m_pAttachedNode = nullptr;
	_uint				m_iCurAnim = 0;
	_bool				m_bOnceAnim = false;
	_float				m_fAnimSpeed = 1.f;
	_float3				m_fUltimateAttackPos = _float3(0.f, 0.f, 0.f);
	_float				m_fMaxTime_UltimateAttackDuration = 5.f;
	_float				m_fCurTime_UltimateAttackDuration = 5.f;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CDissolve*			m_pDissolveCom = nullptr;

	CCollider*			m_pCollider_Ultimate = nullptr;

public:
	static CPlayerWeapon_Bow*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END
