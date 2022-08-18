#pragma once

BEGIN(Client)
class CCamera_Main;

class CPM_Food final : public CGameObject
{
public:
	enum PMFOODID	{ FOOD_CANCOLA,FOOD_BURGER, FOOD_BIGBURGER, FOOD_END	};

	typedef struct tagPMFoodDesc
	{
		_uint iKinds = FOOD_CANCOLA;
		_float3 vPosition = _float3(0);

	}PMFOODDESC;

	typedef struct tagInstanceTransformLayer
	{
		CTransform*		pTransform = nullptr;
		_bool			bIsDead = false;

	}INSTANCELAYER;

private:
	explicit CPM_Food(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CPM_Food(const CPM_Food& rhs);
	virtual ~CPM_Food() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	HRESULT Ready_AtIndexFood(const _tchar * pLayerTag, _uint iIndex);
	HRESULT Ready_AtPosFood(const _tchar * pLayerTag, _float3 vPosition);
	HRESULT Ready_HorizFood(const _tchar * pLayerTag, _uint iStartIndex, _uint iEndIndex, _uint iMaxmumCount = 10);
	HRESULT Ready_VerticFood(const _tchar * pLayerTag, _uint iStartIndex, _uint iEndIndex, _uint iMaxmumCount = 7);

public:
	HRESULT SetUp_Components();


	HRESULT Update_Food(_double dDeltaTime);


private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	CModelInstance*			m_pModelInstance = nullptr;
	vector<INSTANCELAYER>	m_vecInstancedTransform;
	vector<CTransform*>		m_vecForRenderTransform;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	CTransform*			m_pPlayerTransfom = nullptr;


	PMFOODDESC			m_tDesc;

private://Anim Speed
	_float				m_PassedTimer = 0;




public:
	static CPM_Food* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END