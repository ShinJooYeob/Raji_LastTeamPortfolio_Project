#pragma once
#include "GameObject.h"

BEGIN(Client)

class CGaolCollider final : public CGameObject
{
public:
	enum ColliderType {COLLIDER_GOOD, COLLIDER_DIE, COLLIDER_END};

	typedef struct tagGoalColliderDesc
	{
		ColliderType	ColliderType = COLLIDER_END;
		_float3			vNotePosition = _float3(0.f);
	}GOALCOLLIDERDESC;

private:
	CGaolCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGaolCollider(const CGaolCollider& rhs);
	virtual ~CGaolCollider() = default;

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


private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pCollider = nullptr;

	GOALCOLLIDERDESC		m_GoalColliderDesc;

private:
	HRESULT SetUp_Components();

public:
	static CGaolCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END