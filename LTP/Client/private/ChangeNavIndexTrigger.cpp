#include "stdafx.h"
#include "..\public\ChangeNavIndexTrigger.h"
#include "Player.h"

CChangeNavIndexTrigger::CChangeNavIndexTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CChangeNavIndexTrigger::CChangeNavIndexTrigger(const CChangeNavIndexTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CChangeNavIndexTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CChangeNavIndexTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());


	return S_OK;
}

HRESULT CChangeNavIndexTrigger::After_Initialize()
{
	m_iChangeNavIndex = (_uint)m_fValueMat._11;

	return S_OK;
}

_int CChangeNavIndexTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CChangeNavIndexTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CChangeNavIndexTrigger::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CChangeNavIndexTrigger::LateRender()
{
	return _int();
}

_int CChangeNavIndexTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CChangeNavIndexTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CChangeNavIndexTrigger::EParkourTriggerType CChangeNavIndexTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::PACUR_CHANGE_NAV_INDEX;
}

void CChangeNavIndexTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
		pPlayer->Set_PlayerNavIndex(m_iChangeNavIndex);
	}
}

_uint CChangeNavIndexTrigger::Get_ChangeNavIndex()
{
	return m_iChangeNavIndex;
}

HRESULT CChangeNavIndexTrigger::SetUp_Components()
{
	return S_OK;
} 

HRESULT CChangeNavIndexTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CChangeNavIndexTrigger * CChangeNavIndexTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CChangeNavIndexTrigger*	pInstance = NEW CChangeNavIndexTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChangeNavIndexTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChangeNavIndexTrigger::Clone(void * pArg)
{
	CChangeNavIndexTrigger*	pInstance = NEW CChangeNavIndexTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CChangeNavIndexTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChangeNavIndexTrigger::Free()
{
	__super::Free();
}
