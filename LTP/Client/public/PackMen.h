#pragma once

BEGIN(Client)
class CCamera_Main;

class CPackMen final : public CGameObject
{
private:
	explicit CPackMen(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CPackMen(const CPackMen& rhs);
	virtual ~CPackMen() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	HRESULT Added_Damaged();

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	_float Get_IsBustTimer() {	return m_BustTimer;	}
	void Set_IsBustTimer(_float fValue) { m_BustTimer = fValue; }

private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);


//Input
	HRESULT	Keyboard_Input(_double dDeltatime);




private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	CCollider*			m_pColliderCom = nullptr;


private://Anim Speed
	_double				m_dAcceleration = 1;
	_float				m_BustTimer = 0;

private:
	CTransform*			m_pSubTransform = nullptr;
	_uint				m_iLifeCount = 3;

public:
	static CPackMen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END