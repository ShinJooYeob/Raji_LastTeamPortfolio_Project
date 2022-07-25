#include "stdafx.h"
#include "..\public\SceneChangeTrigger.h"
#include "Scene_Loading.h"

CSceneChangeTrigger::CSceneChangeTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CSceneChangeTrigger::CSceneChangeTrigger(const CSceneChangeTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CSceneChangeTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CSceneChangeTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	return S_OK;
}

HRESULT CSceneChangeTrigger::After_Initialize()
{
	m_eNextSceneID = static_cast<SCENEID>((_int)m_fValueMat._11);
	return S_OK;
}

_int CSceneChangeTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (!m_bTriggerChecker)
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CSceneChangeTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CSceneChangeTrigger::Render()
{
	return _int();
}

_int CSceneChangeTrigger::LateRender()
{
	return _int();
}

_int CSceneChangeTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CSceneChangeTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CSceneChangeTrigger::EParkourTriggerType CSceneChangeTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::TRIGGER_SCENECHANGE;
}

void CSceneChangeTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		if (!m_bTriggerChecker)
		{
			m_bTriggerChecker = true;
			g_pGameInstance->Get_NowScene()->Set_SceneChanging(m_eNextSceneID);
		}
	}
}

HRESULT CSceneChangeTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CSceneChangeTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CSceneChangeTrigger * CSceneChangeTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSceneChangeTrigger*	pInstance = NEW CSceneChangeTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSceneChangeTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSceneChangeTrigger::Clone(void * pArg)
{
	CSceneChangeTrigger*	pInstance = NEW CSceneChangeTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CSceneChangeTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneChangeTrigger::Free()
{
	__super::Free();
}
