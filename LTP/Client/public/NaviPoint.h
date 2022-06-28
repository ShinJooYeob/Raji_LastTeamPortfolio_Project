#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "Cell.h"

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
	vector<CCell*>		Get_Cells() { return m_vThisUsedCells; }

public:
	void				Push_Cells(CCell* Cell) { m_vThisUsedCells.push_back(Cell); }

	void				ReLocationCell(_float3 PointPos, _float3 RePointPos);

public:
	CCollider*			Get_Collider() { return m_pColliderCom; }
	_float3				Get_Position() { return m_pTransformCom->Get_MatrixState(CTransform::STATE_POS); }

private:
	CCollider*			m_pColliderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	vector<CCell*>		m_vThisUsedCells;

private:
	HRESULT SetUp_Components();

public:
	static CNaviPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END