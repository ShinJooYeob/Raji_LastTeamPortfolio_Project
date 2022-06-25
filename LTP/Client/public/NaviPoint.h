#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CNaviPoint final : public CGameObject
{
public:
	CNaviPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNaviPoint(const CNaviPoint& rhs);
	virtual ~CNaviPoint() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg);
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual _int Update(_double TimeDelta);
	virtual _int LateUpdate(_double TimeDelta);
	virtual _int Render();
	virtual _int LateRender();
	virtual void CollisionTriger(_uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType);

public:
	CCollider* Get_Collider() { return m_pSphereCom; }

private:
	CCollider*			m_pSphereCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	HRESULT SetUp_Components();

public:
	static CNaviPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END