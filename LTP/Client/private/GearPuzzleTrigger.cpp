#include "stdafx.h"
#include "..\public\GearPuzzleTrigger.h"
#include "Player.h"
#include "Gear_Puzzle.h"
#include "Camera_Main.h"


CGearPuzzleTrigger::CGearPuzzleTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CGearPuzzleTrigger::CGearPuzzleTrigger(const CGearPuzzleTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CGearPuzzleTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CGearPuzzleTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

HRESULT CGearPuzzleTrigger::After_Initialize()
{
	return S_OK;
}

_int CGearPuzzleTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (m_bClearChecker)
	{
		if (m_fPassedTime >= 0)
		{

			m_fPassedTime += (_float)fDeltaTime;

			if (m_fPassedTime > 0.75f)
			{
				m_fPassedTime = -999999999.f;
				GetSingle(CUtilityMgr)->Plus_SKillPoint(2);
				Set_IsDead();
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CGearPuzzleTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CGearPuzzleTrigger::Render()
{
	return _int();
}

_int CGearPuzzleTrigger::LateRender()
{
	return _int();
}

_int CGearPuzzleTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CGearPuzzleTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CGearPuzzleTrigger::EParkourTriggerType CGearPuzzleTrigger::Get_ParkourTriggerType()
{
	return CTriggerObject::EParkourTriggerType::INTERACT_MANDALAPUZZLE;
}

void CGearPuzzleTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		CGear_Puzzle* pPuzzle = static_cast<CGear_Puzzle*>(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject), 0));

		if (g_pGameInstance->Get_DIKeyState(DIK_E) & DIS_Down)
		{
			if (0 == pPuzzle->Get_PuzzleState())
			{
				CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
				pPlayer->Set_State_StopActionStart();

				pPuzzle->Set_PuzzleState(1);

				m_pMainCamera->Set_FocusTarget(pPuzzle);
				m_pMainCamera->Lock_CamLook(true, XMVectorSet(1.f, 0.f, 0.f, 0.f));
			}
		}
		else if (2 == pPuzzle->Get_PuzzleState())
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
			pPlayer->Set_State_StopActionEnd();

			m_pMainCamera->Set_FocusTarget(pPlayer);
			
			pPuzzle->Set_PuzzleState(3);


			m_bClearChecker = true;
			m_fPassedTime = 0;
		}
	}
}

HRESULT CGearPuzzleTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CGearPuzzleTrigger::SetUp_EtcInfo()
{
	m_pMainCamera = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	return S_OK;
}

CGearPuzzleTrigger * CGearPuzzleTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGearPuzzleTrigger*	pInstance = NEW CGearPuzzleTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGearPuzzleTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGearPuzzleTrigger::Clone(void * pArg)
{
	CGearPuzzleTrigger*	pInstance = NEW CGearPuzzleTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGearPuzzleTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGearPuzzleTrigger::Free()
{
	__super::Free();
}
