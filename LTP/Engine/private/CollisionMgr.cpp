#include "..\public\CollisionMgr.h"
#include "GameObject.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CCollisionMgr);

CCollisionMgr::CCollisionMgr()
{
}

HRESULT CCollisionMgr::Initialize_CollisionMgr(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CCollisionMgr::Add_CollisionGroup(CollisionTypeID eType, CGameObject * pCollisionObject, CCollider * pCollider)
{

	COLLIDERELEMENTS tElement;
	tElement.pCollider = pCollider;
	tElement.pCollisionObject = pCollisionObject;

	Safe_AddRef(tElement.pCollider);
	Safe_AddRef(tElement.pCollisionObject);

	m_CollisionGroupList[eType].push_back(tElement);


	return S_OK;
}

HRESULT CCollisionMgr::Inspect_Collision()
{

	FAILED_CHECK(Inspect_Player_To_MonsterWeapon());

	FAILED_CHECK(Inspect_PlayerWeapon_To_Monster());

	FAILED_CHECK(Inspect_DynamicObject_To_PlayerNPlayerWeapon());
	
	FAILED_CHECK(Inspect_NPC_To_Player());

	FAILED_CHECK(Inspect_Terrain_To_All());

	for (_uint i = 0; i < CollisionType_END; i++)
	{
		for (auto& ColideElements : m_CollisionGroupList[i])
		{
			Safe_Release(ColideElements.pCollider);
			Safe_Release(ColideElements.pCollisionObject);
		}
		m_CollisionGroupList[i].clear();
	}


	return S_OK;
}

void CCollisionMgr::Clear_CollisionGroup()
{
	for (_uint i = 0; i < CollisionType_END; i++)
	{
		for (auto& ColideElements : m_CollisionGroupList[i])
		{
			Safe_Release(ColideElements.pCollider);
			Safe_Release(ColideElements.pCollisionObject);
		}
		m_CollisionGroupList[i].clear();
	}
}


HRESULT CCollisionMgr::Inspect_Player_To_MonsterWeapon()
{

	_uint2 ConflictedIndex;
	for (auto& SrcElement : m_CollisionGroupList[CollisionType_Player])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_MonsterWeapon])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_MonsterWeapon);
				DestElemet.pCollisionObject->CollisionTriger(ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_Player);
			}

		}
	}

	return S_OK;
}

HRESULT CCollisionMgr::Inspect_PlayerWeapon_To_Monster()
{
	//enum CollisionTypeID { CollisionType_Player, CollisionType_PlayerWeapon, CollisionType_Monster, CollisionType_MonsterWeapon, 
	//CollisionType_NPC, CollisionType_Terrain, CollisionType_END };

	_uint2 ConflictedIndex;
	for (auto& SrcElement : m_CollisionGroupList[CollisionType_PlayerWeapon])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_Monster])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_Monster);
				DestElemet.pCollisionObject->CollisionTriger(ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_PlayerWeapon);
			}

		}
	}

	return S_OK;
}
HRESULT CCollisionMgr::Inspect_NPC_To_Player()
{
	//enum CollisionTypeID { CollisionType_Player, CollisionType_PlayerWeapon, CollisionType_Monster, CollisionType_MonsterWeapon, 
	//CollisionType_NPC, CollisionType_Terrain, CollisionType_END };
	_uint2 ConflictedIndex;

	for (auto& SrcElement : m_CollisionGroupList[CollisionType_NPC])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_Player])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_Player);
				DestElemet.pCollisionObject->CollisionTriger(ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_NPC);
			}

		}
	}

	return S_OK;
}
HRESULT CCollisionMgr::Inspect_DynamicObject_To_PlayerNPlayerWeapon()
{
	_uint2 ConflictedIndex;



	for (auto& SrcElement : m_CollisionGroupList[CollisionType_DynaicObject])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_Player])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_Player);
				DestElemet.pCollisionObject->CollisionTriger(ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_DynaicObject);
			}

		}

		for (auto& DestElemet : m_CollisionGroupList[CollisionType_PlayerWeapon])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_PlayerWeapon);
				DestElemet.pCollisionObject->CollisionTriger(ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_DynaicObject);
			}

		}
	}

	return S_OK;
}
HRESULT CCollisionMgr::Inspect_Terrain_To_All()
{

	_uint2 ConflictedIndex;
	for (auto& SrcElement : m_CollisionGroupList[CollisionType_Terrain])
	{
		for (_uint i = 0; i < CollisionType_Terrain; i++)
		{
			for (auto& DestElemet : m_CollisionGroupList[i])
			{
				if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
				{
					SrcElement.pCollisionObject->CollisionTriger(ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionTypeID(i));
					DestElemet.pCollisionObject->CollisionTriger(ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_MonsterWeapon);
				}

			}
		}
	}
	return S_OK;
}




void CCollisionMgr::Free()
{
	for (_uint i = 0 ; i < CollisionType_END; i++ )
	{
		for (auto& ColideElements : m_CollisionGroupList[i])
		{
			Safe_Release(ColideElements.pCollider);
			Safe_Release(ColideElements.pCollisionObject);
		}
		m_CollisionGroupList[i].clear();
	}


	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
