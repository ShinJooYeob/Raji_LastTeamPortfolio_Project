#pragma once
#include "Monster.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CMonster_Gadasura_Rage final : public CMonster
{
private:
	explicit CMonster_Gadasura_Rage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Gadasura_Rage(const CMonster_Gadasura_Rage& rhs);
	virtual ~CMonster_Gadasura_Rage() = default;

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

public:
	_int				Get_OnceAnimNumber() { return m_iOnceAnimNumber; }
	_bool				Get_WeaponAttackSwitch() { return m_bWeaponAttackSwitch; }


	HRESULT				SetUp_Info();
	HRESULT				SetUp_Weapon();
	HRESULT				SetUp_Collider();

	HRESULT				SetUp_Fight(_double dDeltaTime);
	HRESULT				Update_Collider(_double dDeltaTime);

	/* Particle */
	virtual HRESULT Ready_ParticleDesc() override;
	virtual HRESULT Update_Particle(_double timer)override;

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


	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	class CHpUI*		m_pHPUI = nullptr;
	class CMonster_Weapon_Universal* m_pWeapon = nullptr;

private:
	CCollider*			m_pColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttachedDesc;

private:
	CTransform*			m_pPlayerTransform = nullptr; //플레이어 트랜스폼 정보

private://애니메이션 동작 및 이벤트
		//Anim Once Pattern
	_double				m_dOnceCoolTime = 0;
	_uint				m_iOncePattern = 0;
	_int				m_iOnceAnimNumber = -1;

	_bool				m_bIOnceAnimSwitch = false;

	//Anim Infinity Pattern
	_double				m_dInfinity_CoolTime = 0;
	_uint				m_iInfinityPattern = 0;
	_uint				m_iInfinityAnimNumber = 0;

	//Anim Combo Pattern
	_bool				m_bComboAnimSwitch = false;

	//Anim Special Pattern
	_double				m_dSpecial_CoolTime = 0;

	_double				m_dAcceleration = 1;

private:
	_float				m_fDistance = 0;
	_bool				m_bLookAtOn = true; //LookAt On/Off

	_float3				m_TempLook;

	_uint				m_iBoolOnce = 0;

	// Particle
	CTransform*						m_pTextureParticleTransform_Hand = nullptr;
	CTransform*						m_pTextureParticleTransform_Demo1 = nullptr;
	CTransform*						m_pTextureParticleTransform_Demo2 = nullptr;
	CTransform*						m_pTextureParticleTransform_Demo3 = nullptr;

	_uint							m_EffectAdjust = 0;

private:
	_bool				m_bWeaponAttackSwitch = false;

private:
	//Knockback
	_float3				m_fKnockbackDir;

private://Sound
	_uint				m_iSoundIndex = 0;
	_double				m_dSoundTime = 0;

private://Dissolve
	_double				m_dDissolveTime = 0;

private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

public:
	static CMonster_Gadasura_Rage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END