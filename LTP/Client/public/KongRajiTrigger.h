#pragma once

#include "GameObject.h"

BEGIN(Client)

class CKongRajiTrigger : public CGameObject
{
public:
	enum CColliderPosType{ COLLIDER_ONE_POINT, COLLIDER_TWO_POINT,COLLIDER_END};
private:
	explicit CKongRajiTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CKongRajiTrigger(const CKongRajiTrigger& rhs);
	virtual ~CKongRajiTrigger() = default;

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
	HRESULT SetUp_Components();
	HRESULT SetUp_Collider();
	HRESULT	SetUp_Info();

private:
	HRESULT	Update_Collider(_double dDeltaTime);

private:
	HRESULT	Once_Trigger();

private:
	HRESULT Update_Trigger(_double dDeltaTime);

private:
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	class CMiniGame_KongRaji*	m_pPlayer = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;

private:
	_float3				m_fKongRajiPos;


private:
	_bool				m_bOnce_Switch = false;

private:
	_float3				m_fColliderPos[CKongRajiTrigger::COLLIDER_END];

public:
	static CKongRajiTrigger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END