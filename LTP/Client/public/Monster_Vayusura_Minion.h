#pragma once
#include "Monster.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CMonster_Vayusura_Minion final : public CMonster
{
private:
	explicit CMonster_Vayusura_Minion(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Vayusura_Minion(const CMonster_Vayusura_Minion& rhs);
	virtual ~CMonster_Vayusura_Minion() = default;

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
	HRESULT				SetUp_Collider();

	HRESULT				SetUp_Fight(_double dDeltaTime);
	HRESULT				Update_Collider(_double dDeltaTime);

private: //애니메이션
	HRESULT				PlayAnim(_double dDeltaTime);
	HRESULT				CoolTime_Manager(_double dDeltaTime);
	HRESULT				Once_AnimMotion(_double	dDeltaTime);
	HRESULT				Infinity_AnimMotion(_double	dDeltaTime);

private:
	/* Particle */
	virtual HRESULT Ready_ParticleDesc() override;
	virtual HRESULT Update_Particle(_double timer)override;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;
	CMotionTrail*		m_pMotionTrail = nullptr;

private:
	CCollider*			m_pColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttachedDesc;

	CCollider*			m_pAttackColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttackAttachedDesc;

	_bool				m_bColliderAttackOn = false;

private:
	class CHpUI*		m_pHPUI = nullptr;

private:
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
	_int				m_iInfinityPattern = -1;
	_uint				m_iInfinityAnimNumber = 0;

	_double				m_dAcceleration = 1;

private:
	_float				m_fDistance = 0;
	_bool				m_bLookAtOn = true; //LookAt On/Off

	_float3				m_fPlayerPos;

	_float3				m_TempPlayerPos; //Old Player Position

	//Distance
	_float4				m_fDirection;


	_bool				m_dAttackOn = true;

	_double				m_dSpeedTime = 0;

private://Sound
	_uint				m_iSoundIndex = 0;
	_double				m_dSoundTime = 0;
	_bool				m_bDieSound = false;

private://Dissolve
	_double				m_dDissolveTime = 0;

private: // particle	
	CTransform*						m_pTextureParticleTransform_HEAD = nullptr;
	CTransform*						m_pTextureParticleTransform_WingR = nullptr;
	CTransform*						m_pTextureParticleTransform_WingL = nullptr;
	CTransform*						m_pTextureParticleTransform_Foot = nullptr;
	_double							mPlaneTimer = 0;
	

private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

public:
	static CMonster_Vayusura_Minion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END