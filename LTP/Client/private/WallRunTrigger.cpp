#include "stdafx.h"
#include "..\public\WallRunTrigger.h"
#include "Player.h"


CWallRunTrigger::CWallRunTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CWallRunTrigger::CWallRunTrigger(const CWallRunTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CWallRunTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CWallRunTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());


	return S_OK;
}

HRESULT CWallRunTrigger::After_Initialize()
{
	m_fStartPos = _float3(m_fValueMat._11, m_fValueMat._12, m_fValueMat._13);
	m_fEndPos = _float3(m_fValueMat._14, m_fValueMat._21, m_fValueMat._22);
	m_bAnimDir = (m_fValueMat._23 > 0 ? true : false);
	return S_OK;
}

_int CWallRunTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CWallRunTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CWallRunTrigger::Render()
{
	return _int();
}

_int CWallRunTrigger::LateRender()
{
	return _int();
}

_int CWallRunTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CWallRunTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CWallRunTrigger::EParkourTriggerType CWallRunTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::PACUR_WALLRUN;
}

void CWallRunTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
			pPlayer->Set_State_WallRunStart(m_bAnimDir, m_fStartPos, m_fEndPos, g_fDeltaTime);
		}
	}
}

HRESULT CWallRunTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CWallRunTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CWallRunTrigger * CWallRunTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CWallRunTrigger*	pInstance = NEW CWallRunTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CWallRunTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWallRunTrigger::Clone(void * pArg)
{
	CWallRunTrigger*	pInstance = NEW CWallRunTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CWallRunTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWallRunTrigger::Free()
{
	__super::Free();
}
