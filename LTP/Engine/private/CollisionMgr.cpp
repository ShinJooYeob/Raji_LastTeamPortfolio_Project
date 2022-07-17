#include "..\public\CollisionMgr.h"
#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "Navigation.h"

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

HRESULT CCollisionMgr::Add_RepelGroup(CTransform * pTransform, _float fRadious, CNavigation* pNavigation)
{
	NULL_CHECK_RETURN(pTransform,E_FAIL);
	if (fRadious < 0) fRadious = 0;


	REPELELEMENT tDesc;
	ZeroMemory(&tDesc, sizeof(REPELELEMENT));

	tDesc.pRepelObjTransform = pTransform;
	tDesc.fRadious			= fRadious;
	tDesc.pNavigation	= pNavigation;

	Safe_AddRef(tDesc.pRepelObjTransform);

	m_RepelObjectList.push_back(tDesc);

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

	FAILED_CHECK(Inspect_RepelGroup());

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

	for (auto& RepelElement : m_RepelObjectList)
		Safe_Release(RepelElement.pRepelObjTransform);
	m_RepelObjectList.clear();

}

HRESULT CCollisionMgr::Add_NaviPointCollider(EDITPOINTCOLLIDER Collider)
{
	m_EditPointCollider.push_back(Collider);

	return S_OK;
}

CGameObject * CCollisionMgr::NaviPointCollision(_Vector pos, _Vector dir)
{
	CGameObject* Obj = nullptr;

	for (auto& Point : m_EditPointCollider)
	{
		BoundingSphere* ThisSphereCol = Point.vCollider->Get_Sphere_Transform();

		if (nullptr == ThisSphereCol)
			continue;

		_float	fDist = 0.f;
		_Vector RayPos = pos;
		_Vector RayDir = dir;

		RayPos = XMVectorSetW(RayPos, 1.f);
		RayDir = XMVectorSetW(RayDir, 1.f);
		RayDir = XMVector3Normalize(RayDir);

		if (true == ThisSphereCol->Intersects(RayPos, RayDir, fDist))
		{
			Obj = Point.GameObject;
			m_EditPointCollider.clear();
			return Obj;
		}
	}
	m_EditPointCollider.clear();
	return nullptr;
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

HRESULT CCollisionMgr::Inspect_RepelGroup()
{

#define PushingSpeed 5.f 

	if (m_RepelObjectList.size() < 2)
	{
		for (auto& iter : m_RepelObjectList)
			Safe_Release(iter.pRepelObjTransform);
		m_RepelObjectList.clear();

		return S_FALSE;
	}

	_Vector SourPos = XMVectorSet(0, 0, 0, 0);
	_Vector DestPos = XMVectorSet(0, 0, 0, 0);
	_Vector D2SDir = XMVectorSet(0, 0, 0, 0);

	auto SourIter = m_RepelObjectList.begin();
	

	for (;SourIter != m_RepelObjectList.end(); )
	{
		if (SourIter->pRepelObjTransform->Get_IsOwnerDead())
		{
			Safe_Release(SourIter->pRepelObjTransform);
			SourIter =  m_RepelObjectList.erase(SourIter);
			continue;
		}


		auto DestIter = SourIter;
		DestIter++;

		for (; DestIter != m_RepelObjectList.end(); )
		{
			if (DestIter->pRepelObjTransform->Get_IsOwnerDead())
			{
				Safe_Release(DestIter->pRepelObjTransform);
				DestIter =  m_RepelObjectList.erase(DestIter);
				continue;
			}
			
			SourPos = SourIter->pRepelObjTransform->Get_MatrixState(CTransform::STATE_POS);
			DestPos = DestIter->pRepelObjTransform->Get_MatrixState(CTransform::STATE_POS);
			D2SDir = SourPos - DestPos;
			if ( XMVectorGetX(XMVector3Length(D2SDir)) < SourIter->fRadious + DestIter->fRadious)
			{
				D2SDir = XMVector3Normalize(XMVectorSetY(D2SDir,0));

				if (SourIter->fRadious > DestIter->fRadious)
				{
					DestIter->pRepelObjTransform->MovetoDir_bySpeed(-D2SDir, PushingSpeed, 0.0166667f, DestIter->pNavigation);
				}
				else
				{
					SourIter->pRepelObjTransform->MovetoDir_bySpeed(D2SDir, PushingSpeed, 0.0166667f, SourIter->pNavigation);
				}

			}

			DestIter++;
		}

		SourIter++;
	}

	for (auto iter : m_RepelObjectList)
		Safe_Release(iter.pRepelObjTransform);
	m_RepelObjectList.clear();
	




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


	for (auto& RepelElement : m_RepelObjectList)	
		Safe_Release(RepelElement.pRepelObjTransform);
	m_RepelObjectList.clear();


	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
