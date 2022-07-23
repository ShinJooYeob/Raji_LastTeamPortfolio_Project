#include "stdafx.h"
#include "..\public\TestLedgeTrigger.h"
#include "Player.h"

CTestLedgeTrigger::CTestLedgeTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CTestLedgeTrigger::CTestLedgeTrigger(const CTestLedgeTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CTestLedgeTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CTestLedgeTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());
	
	

	return S_OK;
}

HRESULT CTestLedgeTrigger::After_Initialize()
{
	m_eLedgeTriggerType = static_cast<ELedgeTriggerState>((int)m_fValueMat._11);
	switch (m_eLedgeTriggerType)
	{
	case ELedgeTriggerState::STATE_LAST_LEDGE:
	{
		m_fLookDir = _float3(m_fValueMat._12, m_fValueMat._13, m_fValueMat._14);
	}
		break;
	}

	return S_OK;
}

_int CTestLedgeTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));
	
	return _int();
}

_int CTestLedgeTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;


#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CTestLedgeTrigger::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CTestLedgeTrigger::LateRender()
{
	return _int();
}

_int CTestLedgeTrigger::Active_Trigger(CGameObject* pTarget, _double fDeltaTime)
{
	return _int();
}

_int CTestLedgeTrigger::DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime)
{


	return _int();
}

CTestLedgeTrigger::EParkourTriggerType  CTestLedgeTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::PACUR_LEDGE;
}

void CTestLedgeTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_PlayerParkur == eConflictedObjCollisionType || CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
		if (true == pPlayer->Get_IsLedgeReachBackState())
		{
			return;
		}

		switch (m_eLedgeTriggerType)
		{
		case ELedgeTriggerState::STATE_START:
		{
			if (g_pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
			{
				//if (CPlayer::STATE_IDLE == pPlayer->Get_PlayerState())
				//{
					pPlayer->Set_State_WallJump();
					pPlayer->Set_CurParkurLedge(this);
				//}
			}
		}
			break;
		case ELedgeTriggerState::STATE_LEDGE:
		{
			if ((CPlayer::EPARKOUR_LEDGESTATE::LEDGE_JUMP == pPlayer->Get_LedgeState() || CPlayer::EPARKOUR_LEDGESTATE::LEDGE_HANGING_FALLINGDOWN == pPlayer->Get_LedgeState())
				&& CollisionTypeID::CollisionType_PlayerParkur == eConflictedObjCollisionType 
				&& pPlayer->Get_CurParkurTriger() != this)
			{
				pPlayer->Set_State_ParkourStart(g_fDeltaTime);
				pPlayer->Set_CurParkurLedge(this);
			}
		}
			break;
		case ELedgeTriggerState::STATE_LAST_LEDGE:
		{
			if (CPlayer::STATE_IDLE == pPlayer->Get_PlayerState())
			{
				if (g_pGameInstance->Get_DIKeyState(DIK_E) & DIS_Down)
				{
					if (CPlayer::STATE_IDLE == pPlayer->Get_PlayerState())
					{
						pPlayer->Set_State_LedgeClimbDownStart(m_fLookDir, g_fDeltaTime);
						pPlayer->Set_CurParkurLedge(this);
					}
				}
			}
			else if (CPlayer::EPARKOUR_LEDGESTATE::LEDGE_HANGING_IDLE == pPlayer->Get_LedgeState())
			{
				if (g_pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
				{
					pPlayer->Set_State_LedgeClimbUpStart(g_fDeltaTime);
					pPlayer->Set_CurParkurLedge(this);
				}
			}
			else if (CPlayer::EPARKOUR_LEDGESTATE::LEDGE_HANGING_JUMPUP == pPlayer->Get_LedgeState() &&
					 pPlayer->Get_CurParkurTriger() != this &&
					 CollisionTypeID::CollisionType_PlayerParkur == eConflictedObjCollisionType)
			{
				pPlayer->Set_State_ParkourStart(g_fDeltaTime);
				pPlayer->Set_CurParkurLedge(this);
			}
			else if (CPlayer::EPARKOUR_LEDGESTATE::LEDGE_JUMP == pPlayer->Get_LedgeState() &&
				pPlayer->Get_CurParkurTriger() != this &&
				CollisionTypeID::CollisionType_PlayerParkur == eConflictedObjCollisionType)
			{
				pPlayer->Set_State_ParkourStart(g_fDeltaTime);
				pPlayer->Set_CurParkurLedge(this);
			}
		}
			break; 
		}
	}
	
}

CTestLedgeTrigger::ELedgeTriggerState CTestLedgeTrigger::Get_LedgeType()
{
	return m_eLedgeTriggerType;
}

_bool CTestLedgeTrigger::Is_Cornor()
{
	return m_bCornor;
}

CTestLedgeTrigger::ELedgeCornorType CTestLedgeTrigger::Get_CornorType()
{
	return m_eConorType;
}

void CTestLedgeTrigger::Set_Pos(_float3 fPos)
{
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, fPos);
}

void CTestLedgeTrigger::Set_LookDir(_float3 fLookDir)
{
	m_fLookDir = fLookDir;
}

void CTestLedgeTrigger::Set_LedgeType(ELedgeTriggerState eType)
{
	m_eLedgeTriggerType = eType;
}

_bool CTestLedgeTrigger::Check_CollisionToPlayer()
{
	return _bool();
}

HRESULT CTestLedgeTrigger::SetUp_Components()
{

	return S_OK;
}

HRESULT CTestLedgeTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CTestLedgeTrigger * CTestLedgeTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestLedgeTrigger*	pInstance = NEW CTestLedgeTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTestLedgeTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTestLedgeTrigger::Clone(void * pArg)
{
	CTestLedgeTrigger*	pInstance = NEW CTestLedgeTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CTestLedgeTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestLedgeTrigger::Free()
{
	__super::Free();

}
