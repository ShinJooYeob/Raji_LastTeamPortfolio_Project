#pragma once
#include "Monster.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)
class CMonster_Vayusura_Leader final : public CMonster
{
private:
	explicit CMonster_Vayusura_Leader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Vayusura_Leader(const CMonster_Vayusura_Leader& rhs);
	virtual ~CMonster_Vayusura_Leader() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

private:
	HRESULT				SetUp_Info();

	HRESULT				SetUp_Fight(_double dDeltaTime);

private: //애니메이션
	HRESULT				PlayAnim(_double dDeltaTime);
	HRESULT				CoolTime_Manager(_double dDeltaTime);
	HRESULT				Once_AnimMotion(_double	dDeltaTime);
	HRESULT				Pattern_Change();
	HRESULT				Infinity_AnimMotion(_double	dDeltaTime);

private:
	/* Particle */
	virtual HRESULT Ready_ParticleDesc() override;
	virtual HRESULT Update_Particle(_double timer)override;

public: //ETC
	_bool				Get_AttackCanceOn() { return m_bAttackCancelOn; }
	_bool				Get_AttackAttackFrieOn() { return m_bAttackFrieOn; }

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	CTransform*			m_pPlayerTransform = nullptr; //플레이어 트랜스폼 정보

private://애니메이션 동작 및 이벤트
		//Anim Once Pattern
	_double				m_dOnceCoolTime = 0;
	_uint				m_iOncePattern = 0;
	_uint				m_iOnceAnimNumber = 0;

	_bool				m_bIOnceAnimSwitch = false;

	//Anim Infinity Pattern
	_double				m_dInfinity_CoolTime = 0;
	_uint				m_iInfinityPattern = 0;
	_uint				m_iInfinityAnimNumber = 0;

	//Anim Combo Pattern
	_bool				m_bComboAnimSwitch = false;

	_double				m_dAcceleration = 1;

private:
	_float				m_fDistance = 0;
	_bool				m_bLookAtOn = true; //LookAt On/Off

	_float3				m_TempLook;

	_float3				m_fPlayerPos;

	//Distance
	_float4				m_fDirection;

	
	//Attack
	_bool				m_bAttackCancelOn = false;
	_bool				m_bAttackFrieOn = false;

private: // particle	
	CTransform*						m_pTextureParticleTransform_Demo1 = nullptr;
	CTransform*						m_pTextureParticleTransform_Demo2 = nullptr;
	CTransform*						m_pTextureParticleTransform_Demo3 = nullptr;
	CTransform*						m_pTextureParticleTransform_Demo4 = nullptr;

	CGameObject*					m_BulletObj = nullptr;


private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

public:
	static CMonster_Vayusura_Leader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END