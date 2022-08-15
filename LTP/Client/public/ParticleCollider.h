#pragma once

#include "GameObject.h"

BEGIN(Client)
class CParticleCollider : public CGameObject
{
public:
	typedef struct tagSettingCollider
	{
		COLLIDERTYPE	ColliderType = COLLIDERTYPE_END;
		COLLIDERDESC	ColliderDesc;
		CTransform*		pTargetTransform = nullptr;
		_float			fWaitingTime = 0.f;
	}SETTINGCOLLIDER;

private:
	CParticleCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CParticleCollider(const CParticleCollider& rhs);
	virtual ~CParticleCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	void	Set_WaitingTime(_float Time) { m_fWaitingTime = Time; }


private:
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pCollider = nullptr;

	_float					m_fWaitingTime = 0.f;

	SETTINGCOLLIDER			m_pColliderDesc;
private:
	HRESULT SetUp_Components();


public:
	static CParticleCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END