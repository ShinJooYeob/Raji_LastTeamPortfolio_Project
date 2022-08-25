#include "stdafx.h"
#include "..\public\MandalaPuzzleTrigger.h"
#include "Player.h"
#include "MandalaPuzzle.h"
#include "Camera_Main.h"

CMandalaPuzzleTrigger::CMandalaPuzzleTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CMandalaPuzzleTrigger::CMandalaPuzzleTrigger(const CMandalaPuzzleTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CMandalaPuzzleTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMandalaPuzzleTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

HRESULT CMandalaPuzzleTrigger::After_Initialize()
{
	return S_OK;
}

_int CMandalaPuzzleTrigger::Update(_double fDeltaTime)
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

_int CMandalaPuzzleTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CMandalaPuzzleTrigger::Render()
{
	return _int();
}

_int CMandalaPuzzleTrigger::LateRender()
{
	return _int();
}

_int CMandalaPuzzleTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CMandalaPuzzleTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CMandalaPuzzleTrigger::EParkourTriggerType CMandalaPuzzleTrigger::Get_ParkourTriggerType()
{
	return CTriggerObject::EParkourTriggerType::INTERACT_MANDALAPUZZLE;
}

void CMandalaPuzzleTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		CMandalaPuzzle* pPuzzle = static_cast<CMandalaPuzzle*>(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject), 1));
		if (g_pGameInstance->Get_DIKeyState(DIK_E) & DIS_Down)
		{
			if (false == pPuzzle->IsClear())
			{
				CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
				pPlayer->Set_State_StopActionStart();

				pPuzzle->Active_Puzzle(true);

				m_pMainCamera->Set_FocusTarget(pPuzzle);
				m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.1f, -1.f, 0.1f, 0.f));
			}
		}
		else if(true == pPuzzle->IsClear())
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
			pPlayer->Set_State_StopActionEnd();
			m_pMainCamera->Set_FocusTarget(pPlayer);
			m_pMainCamera->Lock_CamLook(false, XMVectorSet(0.f, 0.f, 0.f, 0.f));
			//

			m_bClearChecker = true;
			m_fPassedTime = 0;

		}
	}
}

HRESULT CMandalaPuzzleTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CMandalaPuzzleTrigger::SetUp_EtcInfo()
{
	m_pMainCamera = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	return S_OK;
}

CMandalaPuzzleTrigger * CMandalaPuzzleTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMandalaPuzzleTrigger*	pInstance = NEW CMandalaPuzzleTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMandalaPuzzleTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMandalaPuzzleTrigger::Clone(void * pArg)
{
	CMandalaPuzzleTrigger*	pInstance = NEW CMandalaPuzzleTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMandalaPuzzleTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMandalaPuzzleTrigger::Free()
{
	__super::Free();
}
