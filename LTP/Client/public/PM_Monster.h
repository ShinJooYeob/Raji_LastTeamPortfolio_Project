#pragma once

BEGIN(Client)
class CCamera_Main;

class CPM_Monster final : public CGameObject
{
public:
	typedef struct tagPMMonsterDesc
	{
		_uint iIndex = 0;
	}PMMONDESC;





private:
	explicit CPM_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CPM_Monster(const CPM_Monster& rhs);
	virtual ~CPM_Monster() = default;

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
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);


//Input
	HRESULT	Update_MonsterPattern(_double dDeltatime);
	HRESULT Find_Path_to_Player(_double dDeltatime);
	HRESULT Find_Path_to_Evading(_double dDeltatime);


	static vector<PMTP> m_vecTurningPoints;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	class CPackMen*		m_pPackMan = nullptr;
	CTransform*			m_pPlayerTransfom = nullptr;

	_float3				m_vMoveToNextPointDir;
	_uint				m_iNowPointIndex;
	_uint				m_iPassedPointIndex;

	PMMONDESC			m_tDesc;
private://Anim Speed
	_double				m_dAcceleration = 1;
	_float				m_PassedTimer = 0;

	_float3				m_vOldColor = _float3(1,0,1);
	_float3				m_vNewColor = _float3(1, 0, 1);
	_float				m_fColorPassedTimer = 0;

	_bool				m_bOld = false;

	_float				m_bDeadTimer = 0;


public:
	static CPM_Monster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END