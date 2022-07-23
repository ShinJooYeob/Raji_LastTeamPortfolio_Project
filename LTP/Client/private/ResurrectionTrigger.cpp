#include "stdafx.h"
#include "..\public\ResurrectionTrigger.h"
#include "Player.h"

CResurrectionTrigger::CResurrectionTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CResurrectionTrigger::CResurrectionTrigger(const CResurrectionTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CResurrectionTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CResurrectionTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());


	return S_OK;
}

HRESULT CResurrectionTrigger::After_Initialize()
{
	m_fResurrectionPos = _float3(m_fValueMat._11, m_fValueMat._12, m_fValueMat._13);
	m_iNavIndex = (_uint)m_fValueMat._14;

	return S_OK;
}

_int CResurrectionTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CResurrectionTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CResurrectionTrigger::Render()
{
	return _int();
}

_int CResurrectionTrigger::LateRender()
{
	return _int();
}

_int CResurrectionTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CResurrectionTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CResurrectionTrigger::EParkourTriggerType CResurrectionTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::CHECK_POINT;
}

void CResurrectionTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
		pPlayer->SetUp_CheckPoint(m_fResurrectionPos.XMVector(), m_iNavIndex);
	}
}

HRESULT CResurrectionTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CResurrectionTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CResurrectionTrigger * CResurrectionTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CResurrectionTrigger*	pInstance = NEW CResurrectionTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CResurrectionTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CResurrectionTrigger::Clone(void * pArg)
{
	CResurrectionTrigger*	pInstance = NEW CResurrectionTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CResurrectionTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CResurrectionTrigger::Free()
{
	__super::Free();
}

