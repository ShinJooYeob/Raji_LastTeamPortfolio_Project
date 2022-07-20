#include "..\public\CollisionMgr.h"
#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "Navigation.h"
#include "ThreadMgr.h"
#include "TimeMgr.h"

#define CollisionFPS 0.01333333333

_uint CALLBACK RepelCollisionThread(void* _Prameter)
{
	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;


	CCollisionMgr* pCollisionMgr = (CCollisionMgr*)tThreadArg.pArg;
	FAILED_CHECK(pCollisionMgr->Processing_RepelCollision(tThreadArg.IsClientQuit, tThreadArg.CriSec));

	return 0;
}


_uint CALLBACK MainCollisionThread(void* _Prameter)
{
	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;


	CCollisionMgr* pCollisionMgr = (CCollisionMgr*)tThreadArg.pArg;
	FAILED_CHECK(pCollisionMgr->Processing_MainCollision(tThreadArg.IsClientQuit, tThreadArg.CriSec));

	return 0;
}


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


	FAILED_CHECK(GetSingle(CTimeMgr)->Add_Timer(TEXT("Timer_MainTimer")));
	FAILED_CHECK(GetSingle(CTimeMgr)->Add_Timer(TEXT("Timer_RepelTimer")));

	GetSingle(CThreadMgr)->PlayThread(MainCollisionThread, this, nullptr);
	GetSingle(CThreadMgr)->PlayThread(RepelCollisionThread, this, nullptr);

	return S_OK;
}

HRESULT CCollisionMgr::Add_CollisionGroup(CollisionTypeID eType, CGameObject * pCollisionObject, CCollider * pCollider)
{
	if (m_eCollisionThreadState != CCollisionMgr::CTS_ENTER) return S_FALSE;
	
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
	if (m_eRepelCollisionThreadState != CollsionThreadStateID::CTS_ENTER) return S_FALSE;


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
	Start_InspectRepelCollision();
	Start_InspectMainCollision();



	//FAILED_CHECK(Inspect_RepelGroup());

	return S_OK;
}

void CCollisionMgr::Clear_CollisionGroup()
{
	EnterCriticalSection(m_pCriSec);
	m_eCollisionThreadState = CollsionThreadStateID::CTS_SCENECHANGING;
	m_eRepelCollisionThreadState = CCollisionMgr::CTS_SCENECHANGING;
	LeaveCriticalSection(m_pCriSec);

}

void CCollisionMgr::Start_InspectMainCollision()
{
	if (m_eCollisionThreadState == CollsionThreadStateID::CTS_ENTER)
	{
		EnterCriticalSection(m_pCriSec);
		m_eCollisionThreadState = CollsionThreadStateID::CTS_PROCESSING;
		LeaveCriticalSection(m_pCriSec);
	}

	if (m_eCollisionThreadState == CCollisionMgr::CTS_SCENECHANGING)
	{
		EnterCriticalSection(m_pCriSec);
		m_eCollisionThreadState = CollsionThreadStateID::CTS_ENTER;
		LeaveCriticalSection(m_pCriSec);
	}
}

void CCollisionMgr::Start_InspectRepelCollision()
{
	if (m_eRepelCollisionThreadState == CollsionThreadStateID::CTS_ENTER)
	{
		EnterCriticalSection(m_pCriSec);
		m_eRepelCollisionThreadState = CollsionThreadStateID::CTS_PROCESSING;
		LeaveCriticalSection(m_pCriSec);
	}

	if (m_eRepelCollisionThreadState == CCollisionMgr::CTS_SCENECHANGING)
	{
		EnterCriticalSection(m_pCriSec);
		m_eRepelCollisionThreadState = CollsionThreadStateID::CTS_ENTER;
		LeaveCriticalSection(m_pCriSec);
	}
}

HRESULT CCollisionMgr::Inspect_PlayerParkur_To_ParkurObj()
{
	_uint2 ConflictedIndex;

	for (auto& SrcElement : m_CollisionGroupList[CollisionType_PlayerParkur])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_NPC])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(SrcElement.pCollider, ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_NPC);
				DestElemet.pCollisionObject->CollisionTriger(DestElemet.pCollider, ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_PlayerParkur);
			}

		}
	}

	return S_OK;
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


HRESULT CCollisionMgr::Processing_MainCollision(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	m_pCriSec = _CriSec;

	CTimeMgr* pTimerMgr = GetSingle(CTimeMgr);
	_double			fTimeAcc = 0.f;


	while (true)
	{
		if (*_IsClientQuit == true)
			return S_OK;

		fTimeAcc += pTimerMgr->Get_DeltaTime(L"Timer_MainTimer");

		if (fTimeAcc > CollisionFPS)
		{
			fTimeAcc = 0;

			if (m_eCollisionThreadState == CCollisionMgr::CTS_SCENECHANGING)
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

				continue;
			}


			if (m_eCollisionThreadState != CCollisionMgr::CTS_PROCESSING) continue;

			FAILED_CHECK(Inspect_MainCollision());

			if (m_eCollisionThreadState != CCollisionMgr::CTS_SCENECHANGING)
			{
				EnterCriticalSection(_CriSec);
				m_eCollisionThreadState = CCollisionMgr::CTS_ENTER;
				LeaveCriticalSection(_CriSec);
			}

		}
		else
		{
			Sleep(ThreadSleepRate);
		}
	}

	return S_OK;
}


HRESULT CCollisionMgr::Processing_RepelCollision(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CTimeMgr* pTimerMgr = GetSingle(CTimeMgr);

	_double			fTimeAcc = 0.f;

	while (true)
	{
		if (*_IsClientQuit == true)
			return S_OK;

		fTimeAcc += pTimerMgr->Get_DeltaTime(L"Timer_RepelTimer");

		if (fTimeAcc > CollisionFPS)
		{
			fTimeAcc = 0;


			if (m_eRepelCollisionThreadState == CCollisionMgr::CTS_SCENECHANGING)
			{
				if (m_RepelObjectList.size() > 0)
				{
					for (auto& RepelElement : m_RepelObjectList)
						Safe_Release(RepelElement.pRepelObjTransform);
					m_RepelObjectList.clear();
				}

				continue;
			}


			if (m_eRepelCollisionThreadState != CCollisionMgr::CTS_PROCESSING) continue;
			FAILED_CHECK(Inspect_RepelGroup());

			if (m_eRepelCollisionThreadState != CCollisionMgr::CTS_SCENECHANGING)
			{
				EnterCriticalSection(_CriSec);
				m_eRepelCollisionThreadState = CCollisionMgr::CTS_ENTER;
				LeaveCriticalSection(_CriSec);
			}
		}
		else
		{
			Sleep(ThreadSleepRate);
		}
	}



	return S_OK;
}

HRESULT CCollisionMgr::Inspect_MainCollision()
{
	FAILED_CHECK(Inspect_Player_To_MonsterWeapon());

	FAILED_CHECK(Inspect_PlayerWeapon_To_Monster());

	FAILED_CHECK(Inspect_DynamicObject_To_PlayerNPlayerWeapon());

	FAILED_CHECK(Inspect_NPC_To_Player());

	FAILED_CHECK(Inspect_Terrain_To_All());
	//
	FAILED_CHECK(Inspect_PlayerParkur_To_ParkurObj());
	//

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


	for (; SourIter != m_RepelObjectList.end(); )
	{
		if (SourIter->pRepelObjTransform->Get_IsOwnerDead())
		{
			Safe_Release(SourIter->pRepelObjTransform);
			SourIter = m_RepelObjectList.erase(SourIter);
			continue;
		}


		auto DestIter = SourIter;
		DestIter++;

		for (; DestIter != m_RepelObjectList.end(); )
		{
			if (DestIter->pRepelObjTransform->Get_IsOwnerDead())
			{
				Safe_Release(DestIter->pRepelObjTransform);
				DestIter = m_RepelObjectList.erase(DestIter);
				continue;
			}

			SourPos = SourIter->pRepelObjTransform->Get_MatrixState(CTransform::STATE_POS);
			DestPos = DestIter->pRepelObjTransform->Get_MatrixState(CTransform::STATE_POS);
			D2SDir = SourPos - DestPos;
			if (XMVectorGetX(XMVector3Length(D2SDir)) < SourIter->fRadious + DestIter->fRadious)
			{
				D2SDir = XMVector3Normalize(XMVectorSetY(D2SDir, 0));

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


HRESULT CCollisionMgr::Inspect_Player_To_MonsterWeapon()
{
	//if (m_CollisionGroupList[CollisionType_Player].size() == 0 ||
	//	m_CollisionGroupList[CollisionType_MonsterWeapon].size() == 0) return S_FALSE;

	_uint2 ConflictedIndex;
	for (auto& SrcElement : m_CollisionGroupList[CollisionType_Player])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_MonsterWeapon])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(SrcElement.pCollider, ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_MonsterWeapon);
				DestElemet.pCollisionObject->CollisionTriger(DestElemet.pCollider, ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_Player);
			}

		}
	}

	return S_OK;
}
HRESULT CCollisionMgr::Inspect_PlayerWeapon_To_Monster()
{

	//if (m_CollisionGroupList[CollisionType_PlayerWeapon].size() == 0 ||
	//	m_CollisionGroupList[CollisionType_Monster].size() == 0) return S_FALSE;

	_uint2 ConflictedIndex;
	for (auto& SrcElement : m_CollisionGroupList[CollisionType_PlayerWeapon])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_Monster])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(SrcElement.pCollider,ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_Monster);
				DestElemet.pCollisionObject->CollisionTriger(DestElemet.pCollider,ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_PlayerWeapon);
			}

		}
	}

	return S_OK;
}
HRESULT CCollisionMgr::Inspect_NPC_To_Player()
{

	//if (m_CollisionGroupList[CollisionType_NPC].size() == 0 ||
	//	m_CollisionGroupList[CollisionType_Player].size() == 0) return S_FALSE;

	_uint2 ConflictedIndex;

	for (auto& SrcElement : m_CollisionGroupList[CollisionType_NPC])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_Player])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(SrcElement.pCollider, ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_Player);
				DestElemet.pCollisionObject->CollisionTriger(DestElemet.pCollider, ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_NPC);
			}

		}
	}

	return S_OK;
}
HRESULT CCollisionMgr::Inspect_DynamicObject_To_PlayerNPlayerWeapon()
{
	_uint2 ConflictedIndex;

	//if (m_CollisionGroupList[CollisionType_DynaicObject].size() == 0 ) return S_FALSE;


	for (auto& SrcElement : m_CollisionGroupList[CollisionType_DynaicObject])
	{
		for (auto& DestElemet : m_CollisionGroupList[CollisionType_Player])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(SrcElement.pCollider, ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_Player);
				DestElemet.pCollisionObject->CollisionTriger(DestElemet.pCollider, ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_DynaicObject);
			}

		}

		for (auto& DestElemet : m_CollisionGroupList[CollisionType_PlayerWeapon])
		{
			if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
			{
				SrcElement.pCollisionObject->CollisionTriger(SrcElement.pCollider, ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionType_PlayerWeapon);
				DestElemet.pCollisionObject->CollisionTriger(DestElemet.pCollider, ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_DynaicObject);
			}

		}
	}

	return S_OK;
}
HRESULT CCollisionMgr::Inspect_Terrain_To_All()
{
	return S_FALSE;

	_uint2 ConflictedIndex;
	for (auto& SrcElement : m_CollisionGroupList[CollisionType_Terrain])
	{
		for (_uint i = 0; i < CollisionType_Terrain; i++)
		{
			for (auto& DestElemet : m_CollisionGroupList[i])
			{
				if (SrcElement.pCollider->Inspect_Collision(DestElemet.pCollider, 0, 0, &ConflictedIndex))
				{
					SrcElement.pCollisionObject->CollisionTriger(SrcElement.pCollider, ConflictedIndex.x, DestElemet.pCollisionObject, DestElemet.pCollider, ConflictedIndex.y, CollisionTypeID(i));
					DestElemet.pCollisionObject->CollisionTriger(DestElemet.pCollider, ConflictedIndex.y, SrcElement.pCollisionObject, SrcElement.pCollider, ConflictedIndex.x, CollisionType_MonsterWeapon);
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


	for (auto& RepelElement : m_RepelObjectList)	
		Safe_Release(RepelElement.pRepelObjTransform);
	m_RepelObjectList.clear();


	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
