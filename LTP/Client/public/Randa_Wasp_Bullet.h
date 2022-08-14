#pragma once
#include "GameObject.h"


class CRanda_Wasp_Bullet final : public CGameObject
{
	typedef struct tagRangdaWaspBulletDesc
	{
		CTransform*		pTransform = nullptr;
		_float			bEachLifeTime = 0;
		_bool			bIsDead = false;
	}RWBDESC;

private:
	CRanda_Wasp_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CRanda_Wasp_Bullet(const CRanda_Wasp_Bullet& rhs);
	virtual ~CRanda_Wasp_Bullet() = default;

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
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CModelInstance*		m_pModelInstance = nullptr;
	vector<RWBDESC>		m_vecRWB;
	vector<CTransform*>		m_vecRenderVector;
	vector<_float4>			m_vecLimLight;
	vector<_float4>			m_vecEmissive;
	CCollider*			m_pCollider = nullptr;

	_float3				m_vStartPosition;
private:
	HRESULT SetUp_Components();

public:
	static CRanda_Wasp_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

