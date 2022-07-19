#pragma once

#include "GameObject.h"

BEGIN(Client)

class CMonsterBatchTrigger : public CGameObject
{
	typedef struct tagMonsterBatch
	{
		_float4x4				vWorldMat = XMMatrixIdentity();
		OBJECTPROTOTYPEID		eMonsterID = Prototype_Object_Monster_Mahinasura_Minion;
		_uint					iNaviIndex = 0;
	}MONSTERBATCH;

protected:
	CMonsterBatchTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonsterBatchTrigger(const CMonsterBatchTrigger& rhs);
	virtual ~CMonsterBatchTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;

	HRESULT Add_MonsterBatch(_float4x4& WorldMatrix, OBJECTPROTOTYPEID eID);

	_float4x4 Get_ValueMat() { return m_ValueMat; };
	void Set_ValueMat(_float4x4* pMat) { memcpy(&m_ValueMat, pMat, sizeof(_float4x4)); };

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) ;


public:
	virtual _int					Update(_double fDeltaTime) override;
	virtual _int					LateUpdate(_double fDeltaTimer) override;
	virtual _int					Render() override;
	virtual _int					LateRender() override;


protected:
	CTransform*						m_pTransformCom		= nullptr;
	CCollider*						m_pColliderCom		= nullptr;
	CNavigation*					m_pNavigationCom	= nullptr;

	_float4x4						m_ValueMat = XMMatrixIdentity();

private:
	vector<MONSTERBATCH>			m_vecBatchedValue;

private:
	HRESULT Ready_NaviIndex_By_Position(_uint iIndex);
	HRESULT Spwan_Monster(_uint iIndex, const _tchar * pLayerTag);

public:
	static CMonsterBatchTrigger*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*				Clone(void* pArg)override;
	virtual void						Free() override;
};

END
