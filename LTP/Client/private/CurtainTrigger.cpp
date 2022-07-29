#include "stdafx.h"
#include "..\public\CurtainTrigger.h"


CCurtainTrigger::CCurtainTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CCurtainTrigger::CCurtainTrigger(const CCurtainTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CCurtainTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CCurtainTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

HRESULT CCurtainTrigger::After_Initialize()
{
	return S_OK;
}

_int CCurtainTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CCurtainTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CCurtainTrigger::Render()
{
	return _int();
}

_int CCurtainTrigger::LateRender()
{
	return _int();
}

_int CCurtainTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CCurtainTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CCurtainTrigger::EParkourTriggerType CCurtainTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::PACUR_CURTAIN;
}

void CCurtainTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		switch (m_eCurtainType)
		{
		case ECURTAINTYPE::TYPE_START:
		{
			// if player pressed 'E' then change player state to curtain start	
			Set_IsDead();
		}
			break;
		case ECURTAINTYPE::TYPE_END:
		{
			// Change player state to falling
			Set_IsDead();
		}
			break;
		}
	}
}

HRESULT CCurtainTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CCurtainTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CCurtainTrigger * CCurtainTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CCurtainTrigger*	pInstance = NEW CCurtainTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CCurtainTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCurtainTrigger::Clone(void * pArg)
{
	CCurtainTrigger*	pInstance = NEW CCurtainTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CCurtainTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCurtainTrigger::Free()
{
	__super::Free();
}
