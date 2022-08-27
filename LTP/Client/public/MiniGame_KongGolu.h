#pragma once

BEGIN(Client)

class CMiniGame_KongGolu final : public CGameObject
{
private:
	explicit CMiniGame_KongGolu(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMiniGame_KongGolu(const CMiniGame_KongGolu& rhs);
	virtual ~CMiniGame_KongGolu() = default;

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

public:
	_bool	Get_bClear() { return m_bClear; }

	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);

	HRESULT	SetUp_Info();

	//InGame
	HRESULT	Play_MiniGame(_double dDeltaTime);

	HRESULT	Clear_Firecracker(_double dDeltaTime);

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

private:
	_bool				m_bClear = false;
	_double				m_dClearTime = 0;
	_uint				m_iClearIndex = 0;

private:
	_bool				m_bSoundIndex = 0;
	_bool				m_bIsClear = false;

public:
	static CMiniGame_KongGolu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END