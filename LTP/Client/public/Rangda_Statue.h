#pragma once
#include "GameObject.h"


class CRangda_Statue final : public CGameObject
{
private:
	CRangda_Statue(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CRangda_Statue(const CRangda_Statue& rhs);
	virtual ~CRangda_Statue() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;
	CCollider*			m_pRaserCollider = nullptr;

	CModel*				m_pSubModel = nullptr;
	CTransform*			m_pSubTransformCom = nullptr;

	CDissolve*			m_pDissolveCom = nullptr;

	_uint				m_iPassIndex = 1;

	ATTACHEDESC			m_AttacheDesc;
	_float				m_fAliveTime = 4.f;

	_bool				m_bIsDissolveStart = false;
	_bool				m_bIsCollison = true;
	NONINSTNESHEFTDESC	m_tMeshDesc;
	INSTPARTICLEDESC	m_tTexDesc;

private:
	HRESULT SetUp_Components();

public:
	static CRangda_Statue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

