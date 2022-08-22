#pragma once

#include "PlayerWeapon.h"

BEGIN(Engine)
class CTimer;
END

BEGIN(Client)

class CPlayerWeapon_Arrow final : public CPlayerWeapon
{
public:
	enum EArrowState
	{
		Arrow_State_NormalReady, Arrow_State_NormalShot,
		Arrow_State_UtilityReady, Arrow_State_UtilityShot,
		Arrow_State_PowerShot_Combo_0, Arrow_State_PowerShot_Combo_1, Arrow_State_PowerShot_Combo_2,
		Arrow_State_UltimateReady_Pre_Ready, Arrow_State_UltimateShot_Pre_Shot,
		Arrow_State_UltimateReady_Post_Ready, Arrow_State_UltimateShot_Post_Shot,
		STATE_END
	};

private:
	CPlayerWeapon_Arrow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon_Arrow(const CPlayerWeapon_Arrow& rhs);
	virtual ~CPlayerWeapon_Arrow() = default;

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

public:
	void				LookAtDir(_fVector fDir);

public:
	void				Set_State(EArrowState eNewState, _float fSpeed = 30.f);
	void				Set_State_PowerShot_Combo_0(_fVector vShotDir, _uint iArrowIndex);
	void				Set_State_PowerShot_Combo_1(_fVector vShotDir);
	void				Set_State_PowerShot_Combo_2(_fVector vShotDir, _float fSpeed);

	void				Set_State_Ultimate_Pre_Ready();
	void				Set_State_Ultimate_Pre_Shot();

	void				Set_State_Ultimate_Post_Ready();
	void				Set_State_Ultimate_Post_Shot();

	void				Set_TargetPos(_float3 fTargetPos);

	CTransform*			Get_Transform_Hand()
	{
		return m_pTextureParticleTransform_Hand;
	}

public:
	_int				UpdateState_NormalReady(_double fDeltaTime);
	_int				UpdateState_NormalShot(_double fDeltaTime);

	_int				UpdateState_UtilityReady(_double fDeltaTime);
	_int				UpdateState_UtilityShot(_double fDeltaTime);

	_int				UpdateState_UltimateReady(_double fDeltaTime);
	_int				UpdateState_UltimateShot(_double fDeltaTime);

	_int				UpdateState_PowerShot_Conbo_0(_double fDeltaTime);
	_int				UpdateState_PowerShot_Conbo_1(_double fDeltaTime);
	_int				UpdateState_PowerShot_Conbo_2(_double fDeltaTime);

	_int				UpdateState_Ultimate_Pre_Ready(_double fDeltaTime);
	_int				UpdateState_Ultimate_Pre_Shot(_double fDeltaTime);

	_int				UpdateState_Ultimate_Post_Ready(_double fDeltaTime);
	_int				UpdateState_Ultimate_Post_Shot(_double fDeltaTime);


public:
	virtual HRESULT Ready_ParticleDesc();
	//	virtual HRESULT Set_Play_Particle(_uint ParticleIndex, _float Timer = -1, CTransform* defaultTrans = nullptr, _float3 offset = _float3::Zero());
	virtual HRESULT Update_Particle(_double fDeltaTime);

private:
	virtual _fVector	Get_BonePos(const char* pBoneName) override;
	virtual void		Update_AttachCamPos() override;


	void				Update_AttachMatrix();
	void				Update_Trail(_fMatrix* pMat, _double fDeltaTime);
	void				Update_Colliders();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();
	HRESULT				SetUp_Collider();

private:
	_float4x4			m_fAttachedMatrix;
	EArrowState			m_eCurState = Arrow_State_NormalReady;
	_uint				m_iPassNum;
	_bool				m_bFired = false;
	_float3				m_fShotDir;
	_float				m_fStartPos_y;
	_float3				m_fEndPos;
	_float				m_fTimeAcc = 0.f;
	_float3				m_fTargetPos;

private:
	CTimer*				m_pTimer_Destroy = nullptr;
	_float				m_fMaxTime_Destroy = 3.f;
	_float				m_fCurTime_Destroy = 0.f;
	_bool				m_bEffect_Head = false;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CSwordTrail*		m_pSwordTrail = nullptr;
	CSwordTrail*		m_pSwordTrail2 = nullptr;
	CCollider*			m_pCollider = nullptr;

private:/*For Particle*/
	CTransform*						m_pTextureParticleTransform = nullptr;
	CTransform*						m_pTextureParticleTransform_Hand = nullptr;


public:
	static CPlayerWeapon_Arrow*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END
