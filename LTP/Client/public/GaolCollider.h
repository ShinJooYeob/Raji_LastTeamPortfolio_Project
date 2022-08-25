#pragma once
#include "GameObject.h"

BEGIN(Client)

class CGaolCollider final : public CGameObject
{
public:
	enum ColliderType {COLLIDER_GOOD, COLLIDER_DIE, COLLIDER_END};

	typedef struct tagGoalColliderDesc
	{
		ColliderType	ColliderType = COLLIDER_END;
		_float3			vNotePosition = _float3(0.f);
	}GOALCOLLIDERDESC;

private:
	CGaolCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGaolCollider(const CGaolCollider& rhs);
	virtual ~CGaolCollider() = default;

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

public:
	_bool	Get_GoodColliderSmallRedCrashing() {return m_bIsSmallRedCrashing;}
	_bool	Get_GoodColliderBigRedCrashing() {return m_bIsBigRedCrashing;}
	_bool	Get_GoodColliderSmallBlueCrashing() {return m_bIsSmallBlueCrashing;}
	_bool	Get_GoodColliderBigBlueCrashing() {return m_bIsBigBlueCrashing;}

	void	Set_GoodColliderSmallRedCrashing(_bool State) { m_bIsSmallRedCrashing = State; }
	void	Set_GoodColliderBigRedCrashing(_bool State) { m_bIsSmallRedCrashing = State; }
	void	Set_GoodColliderSmallBlueCrashing(_bool State) { m_bIsSmallRedCrashing = State; }
	void	Set_GoodColliderBigBlueCrashing(_bool State) { m_bIsSmallRedCrashing = State; }






	class CTaiko_Monster* Get_GoodCollisionMonster() { return m_pMonster; }

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pCollider = nullptr;

	GOALCOLLIDERDESC		m_GoalColliderDesc;

	class CTaiko_Monster*	m_pMonster = nullptr;

	_bool					m_bIsSmallRedCrashing = false;
	_bool					m_bIsBigRedCrashing = false;
	_bool					m_bIsSmallBlueCrashing = false;
	_bool					m_bIsBigBlueCrashing = false;


private:
	HRESULT SetUp_Components();

public:
	static CGaolCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END