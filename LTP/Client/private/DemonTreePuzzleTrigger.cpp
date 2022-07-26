#include "stdafx.h"
#include "..\public\DemonTreePuzzleTrigger.h"
#include "Player.h"
#include "Demon_Tree.h"
#include "Camera_Main.h"

CDemonTreePuzzleTrigger::CDemonTreePuzzleTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CDemonTreePuzzleTrigger::CDemonTreePuzzleTrigger(const CDemonTreePuzzleTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CDemonTreePuzzleTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CDemonTreePuzzleTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

HRESULT CDemonTreePuzzleTrigger::After_Initialize()
{
	return S_OK;
}

_int CDemonTreePuzzleTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CDemonTreePuzzleTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CDemonTreePuzzleTrigger::Render()
{
	return _int();
}

_int CDemonTreePuzzleTrigger::LateRender()
{
	return _int();
}

_int CDemonTreePuzzleTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CDemonTreePuzzleTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CDemonTreePuzzleTrigger::EParkourTriggerType CDemonTreePuzzleTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::INTERACT_DEMONTREE;
}

void CDemonTreePuzzleTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		
		CDemon_Tree* pPuzzle = static_cast<CDemon_Tree*>(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject), 0));
		if (g_pGameInstance->Get_DIKeyState(DIK_E) & DIS_Down)
		{
			if (false == pPuzzle->IsClear())
			{
				CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
				pPlayer->Set_State_StopActionStart();

				pPuzzle->Active_Puzzle(true);

				m_pMainCamera->Set_FocusTarget(pPuzzle);
				m_pMainCamera->Lock_CamLook(true, XMVectorSet(2.2f, 0.1f, 1.5f, 0.f));
			}
		}
		else if (true == pPuzzle->IsClear())
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
			pPlayer->Set_State_StopActionEnd();
			m_pMainCamera->Set_FocusTarget(pPlayer);
			m_pMainCamera->Lock_CamLook(false, XMVectorSet(0.f, 0.f, 0.f, 0.f));
		}
	}
}

HRESULT CDemonTreePuzzleTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CDemonTreePuzzleTrigger::SetUp_EtcInfo()
{
	m_pMainCamera = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	return S_OK;
}

CDemonTreePuzzleTrigger * CDemonTreePuzzleTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CDemonTreePuzzleTrigger*	pInstance = NEW CDemonTreePuzzleTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CDemonTreePuzzleTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDemonTreePuzzleTrigger::Clone(void * pArg)
{
	CDemonTreePuzzleTrigger*	pInstance = NEW CDemonTreePuzzleTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CDemonTreePuzzleTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDemonTreePuzzleTrigger::Free()
{
	__super::Free();
}
