#pragma once


#include "Base.h"
BEGIN(Engine)

class CCollisionMgr final : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr);


	typedef struct tagColliderObject
	{
		class  CGameObject* pCollisionObject = nullptr;
		class CCollider* pCollider = nullptr;
	}COLLIDERELEMENTS;

public:
	typedef struct tagEditPointCollider
	{
		CCollider* vCollider;
		CGameObject* GameObject = nullptr;
	}EDITPOINTCOLLIDER;

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr() = default;


public:
	HRESULT Initialize_CollisionMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT Inspect_Collision();

public:
	HRESULT Add_CollisionGroup(CollisionTypeID eType, class CGameObject* pCollisionObject, class CCollider* pCollider);
	void	Clear_CollisionGroup();

public:
	//MapTool
	HRESULT		 Add_NaviPointCollider(EDITPOINTCOLLIDER Collider);
	CGameObject* NaviPointCollision(_Vector pos, _Vector dir);

private:
	HRESULT Inspect_Player_To_MonsterWeapon();
	HRESULT Inspect_PlayerWeapon_To_Monster();
	HRESULT Inspect_DynamicObject_To_PlayerNPlayerWeapon();
	HRESULT Inspect_NPC_To_Player();
	HRESULT Inspect_Terrain_To_All();

private:
	list<COLLIDERELEMENTS>		m_CollisionGroupList[CollisionType_END];

	list<EDITPOINTCOLLIDER>		m_EditPointCollider;


private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

public:
	virtual void Free() override;
};

END