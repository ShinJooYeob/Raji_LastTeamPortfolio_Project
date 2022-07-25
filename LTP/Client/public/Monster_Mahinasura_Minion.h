#pragma once
#include "Monster.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)
class CMonster_Mahinasura_Minion final : public CMonster
{
public:
	enum ColliderType {HANDATTACK,TAILATTACK,COLLIDER_END};
private:
	explicit CMonster_Mahinasura_Minion(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Mahinasura_Minion(const CMonster_Mahinasura_Minion& rhs);
	virtual ~CMonster_Mahinasura_Minion() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

	 
public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

public:
	/* Particle */
	virtual HRESULT Ready_ParticleDesc() override;
	virtual HRESULT Update_Particle(_double timer)override;


private:
	HRESULT				SetUp_Info();
	HRESULT				SetUp_Collider();

	HRESULT				SetUp_Fight(_double dDeltaTime);
	HRESULT				Update_Collider(_double dDeltaTime);

private: //애니메이션
	HRESULT				PlayAnim(_double dDeltaTime);
	HRESULT				CoolTime_Manager(_double dDeltaTime);
	HRESULT				Once_AnimMotion(_double	dDeltaTime);
	HRESULT				Pattern_Change();
	HRESULT				Infinity_AnimMotion(_double	dDeltaTime);

	HRESULT				Special_Trigger(_double	dDeltaTime);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;


private:
	CCollider*			m_pColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttachedDesc;

	CCollider*			m_pHandAttackColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecHandAttackAttachedDesc;

	CCollider*			m_pTailAttackColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecTailAttackAttachedDesc;

	//ColliderType
	ColliderType		m_eColliderType;
	_bool				m_bColliderAttackOn = false;

private:
	class CHpUI*		m_pHPUI = nullptr;

private:
	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	CTransform*			m_pPlayerTransform = nullptr; //플레이어 트랜스폼 정보

	// Particle
	CTransform*						m_pTextureParticleTransform_RHand = nullptr;
	CTransform*						m_pTextureParticleTransform_LHand = nullptr;
	CTransform*						m_pTextureParticleTransform_Tail = nullptr;
	_uint							m_EffectAdjust = 0;
	

	//Anim Once Pattern
	_double				m_dOnceCoolTime = 0;
	_uint				m_iOncePattern = 0;
	_uint				m_iOnceAnimNumber = 0;

	_bool				m_bIOnceAnimSwitch = false;

	//Anim Infinity Pattern
	_double				m_dInfinity_CoolTime = 0;
	_uint				m_iInfinityPattern = 0;
	_uint				m_iInfinityAnimNumber = 0;
	
	//Anim Special Pattern
	_double				m_dSpecial_CoolTime = 0;

private:
	_float				m_fDistance = 0;
	_bool				m_bLookAtOn = true; //LookAt On/Off

	_float3				m_TempLook;

	_uint				m_iBoolOnce = 0;

	_bool				m_bFastRunOn = false;

private://speed
	_double				m_dAcceleration = 1;

	///Knockback
	_float3				m_fKnockbackDir;

private://Sound
	_uint				m_iSoundIndex = 0;
	_double				m_dSoundTime = 0;

private://Dissolve
	_double				m_dDissolveTime = 0;

public:
	static CMonster_Mahinasura_Minion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END