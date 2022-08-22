#pragma once

BEGIN(Client)

class CMiniGame_DonkeyKong final : public CGameObject
{
private:
	explicit CMiniGame_DonkeyKong(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMiniGame_DonkeyKong(const CMiniGame_DonkeyKong& rhs);
	virtual ~CMiniGame_DonkeyKong() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void Set_IsDead();

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);

	HRESULT	SetUp_Info();

	//InGame
	HRESULT	Play_MiniGame(_double dDeltaTime);

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
	vector<ATTACHEDESC> m_vecAttachedDesc;

public:
	static CMiniGame_DonkeyKong* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END