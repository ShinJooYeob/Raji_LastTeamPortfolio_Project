#include "stdafx.h"
#include "..\public\BlockZone.h"


CBlockZone::CBlockZone(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CBlockZone::CBlockZone(const CBlockZone & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CBlockZone::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CBlockZone::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());


	return S_OK;
}

HRESULT CBlockZone::After_Initialize()
{
	m_fRepelRadious = m_fValueMat._11;

	return S_OK;
}

_int CBlockZone::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;


	FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, m_fRepelRadious, nullptr, true));

	return _int();
}

_int CBlockZone::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CBlockZone::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CBlockZone::LateRender()
{
	return _int();
}

_int CBlockZone::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CBlockZone::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CBlockZone::EParkourTriggerType CBlockZone::Get_ParkourTriggerType()
{
	return EParkourTriggerType::BLOCK_ZONE;
}

void CBlockZone::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

HRESULT CBlockZone::SetUp_Components()
{
	return S_OK;
}

HRESULT CBlockZone::SetUp_EtcInfo()
{
	return S_OK;
}

CBlockZone * CBlockZone::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CBlockZone*	pInstance = NEW CBlockZone(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CBlockZone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlockZone::Clone(void * pArg)
{
	CBlockZone*	pInstance = NEW CBlockZone(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CBlockZone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlockZone::Free()
{
	__super::Free();
}
