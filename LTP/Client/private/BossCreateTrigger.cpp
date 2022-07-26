#include "stdafx.h"
#include "..\public\BossCreateTrigger.h"

CBossCreateTrigger::CBossCreateTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CBossCreateTrigger::CBossCreateTrigger(const CBossCreateTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CBossCreateTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CBossCreateTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());


	return S_OK;
}

HRESULT CBossCreateTrigger::After_Initialize()
{
	if (m_fValueMat._11 >= 1.f)
	{
		m_eBossType = CBossCreateTrigger::BOSS_RANGDA;
	}
	else if (m_fValueMat._12 >= 1.f)
	{
		m_eBossType = CBossCreateTrigger::BOSS_SNAKE;
	}
	else if (m_fValueMat._13 >= 1.f)
	{
		m_eBossType = CBossCreateTrigger::BOSS_MAHABASURA;
	}

	return S_OK;
}

_int CBossCreateTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (!m_bBool)
	{

		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));
	}

	return _int();
}

_int CBossCreateTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CBossCreateTrigger::Render()
{
	return _int();
}

_int CBossCreateTrigger::LateRender()
{
	return _int();
}

_int CBossCreateTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CBossCreateTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CBossCreateTrigger::EParkourTriggerType CBossCreateTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::CREATE_BOSS;
}

void CBossCreateTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (!m_bBool)
	{
		if (m_eBossType == CBossCreateTrigger::BOSS_RANGDA)
		{
			g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, TAG_LAY(Layer_Boss), TAG_OP(Prototype_Object_Boss_Rangda), &_float3(154.189f, 96.400f, 240.976f));
		}
		else if (m_eBossType == CBossCreateTrigger::BOSS_SNAKE)
		{
			//g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE4, TAG_LAY(Layer_Boss), TAG_OP(Prototype_Object_Boss_Snake), &_float3(19.472f, -190.f, 93.197f));5
			g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE4, TAG_LAY(Layer_Boss), TAG_OP(Prototype_Object_Boss_Snake), &_float3(50.f, -150.f, 93.197f));
		}
		else if (m_eBossType == CBossCreateTrigger::BOSS_MAHABASURA)
		{
			g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE5, TAG_LAY(Layer_Boss), TAG_OP(Prototype_Object_Boss_Mahabalasura), &_float3(100.f, 34.3f, 322.283f));
		}
	}
	m_bBool = true;
	Set_IsDead();

}

HRESULT CBossCreateTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CBossCreateTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CBossCreateTrigger * CBossCreateTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CBossCreateTrigger*	pInstance = NEW CBossCreateTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CBossCreateTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBossCreateTrigger::Clone(void * pArg)
{
	CBossCreateTrigger*	pInstance = NEW CBossCreateTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CBossCreateTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossCreateTrigger::Free()
{
	__super::Free();
}
