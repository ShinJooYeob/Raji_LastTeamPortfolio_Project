#pragma once

BEGIN(Client)

class CDonkeyKong_Bullet final : public CGameObject
{
public:
	enum DirectionType { RIGHT, LEFT, DIR_END };
private:
	explicit CDonkeyKong_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDonkeyKong_Bullet(const CDonkeyKong_Bullet& rhs);
	virtual ~CDonkeyKong_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void Set_IsDead()override;

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

	void	Set_NaviIndex(_uint iNaviIndex) { m_iNaviIndex = iNaviIndex; }
	void	Set_MoveToDirIndex(_uint iMoveToDirIndex) { m_iMoveToDirIndex = iMoveToDirIndex; }
	void	Set_MovoToHeightOn(_bool bMovoToHeightOn) { m_bMovoToHeightOn = bMovoToHeightOn; }
	void	Set_MovoToWidthOn(_bool bMovoToWidthOn) { m_bMoveToWidthOn = bMovoToWidthOn; }
	void	Set_NewColliderIndex(_uint bNewColliderIndex) { m_iNewColliderIndex = bNewColliderIndex; }

	_bool	Get_bDescent() { return m_bDescent; }

	HRESULT SetUp_Components();

	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);

	HRESULT	SetUp_Info();

	//InGame
	HRESULT	Play_MiniGame(_double dDeltaTime);

	HRESULT	Play_Bullet(_double dDeltaTime);
	HRESULT Play_Collider(_double dDeltaTime);
	HRESULT Play_Random(_double dDeltaTime);

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

	//Navigation
private:
	_uint				m_iNaviIndex = 11;
	_float				m_fNaviHeight = 0;

private:
	_float3				m_fMyPos;
	_float3				m_fPivotPos;


private://Move
	_uint				m_iMoveToDirIndex = 0;

private://Height && 
	_bool				m_bMovoToHeightOn = false;
	_bool				m_bMoveToWidthOn = true;

private://Collider
	_uint				m_iCurColliderIndex = 0;
	_uint				m_iNewColliderIndex = 0;
	_bool				m_bDescent = false;

private:
	_uint				m_iSoundIndex = 0;

public:
	static CDonkeyKong_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END