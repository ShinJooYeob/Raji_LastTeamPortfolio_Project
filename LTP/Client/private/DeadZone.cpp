#include "stdafx.h"
#include "..\public\DeadZone.h"
#include "Player.h"


CDeadZone::CDeadZone(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CDeadZone::CDeadZone(const CDeadZone & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CDeadZone::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CDeadZone::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());


	return S_OK;
}

HRESULT CDeadZone::After_Initialize()
{
	return S_OK;
}

_int CDeadZone::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CDeadZone::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CDeadZone::Render()
{
	return _int();
}

_int CDeadZone::LateRender()
{
	return _int();
}

_int CDeadZone::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CDeadZone::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CDeadZone::EParkourTriggerType CDeadZone::Get_ParkourTriggerType()
{
	return EParkourTriggerType::DEAD_ZONE;
}

void CDeadZone::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
		pPlayer->Take_Damage(nullptr, 999.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pPlayer->Set_FallingDead(true);
	}
}

HRESULT CDeadZone::SetUp_Components()
{
	return S_OK;
}

HRESULT CDeadZone::SetUp_EtcInfo()
{
	return S_OK;
}

CDeadZone * CDeadZone::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CDeadZone*	pInstance = NEW CDeadZone(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CDeadZone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDeadZone::Clone(void * pArg)
{
	CDeadZone*	pInstance = NEW CDeadZone(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CDeadZone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDeadZone::Free()
{
	__super::Free();
}
