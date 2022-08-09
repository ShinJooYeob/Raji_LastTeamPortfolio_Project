#include "stdafx.h"
#include "..\public\InstanceMonsterBatchTrigger.h"
#include "Monster.h"


CInstanceMonsterBatchTrigger::CInstanceMonsterBatchTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CInstanceMonsterBatchTrigger::CInstanceMonsterBatchTrigger(const CInstanceMonsterBatchTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CInstanceMonsterBatchTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CInstanceMonsterBatchTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());


	if (m_eNowSceneNum != SCENEID::SCENE_EDIT)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

		RELEASE_INSTANCE(CGameInstance);
	}


	return S_OK;
}

HRESULT CInstanceMonsterBatchTrigger::After_Initialize()
{
	return S_OK;
}

_int CInstanceMonsterBatchTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	if (m_eNowSceneNum != SCENEID::SCENE_EDIT && m_bOnceSwitch == true)
	{//그리고 인스턴스들 공격할 때 네비 Y태워서 올라가는 문제해결하자
		m_fPoint.x = m_fValueMat.m[2][0];
		m_fPoint.y = m_fValueMat.m[2][1];
		m_fPoint.z = m_fValueMat.m[2][2];
		if (m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(XMLoadFloat3(&m_fPoint)) > m_fValueMat.m[1][2])
		{
			m_bMonsterAllDie = true;
		}
		else {
			m_bMonsterAllDie = false;
		}
	}
	return _int();
}

_int CInstanceMonsterBatchTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CInstanceMonsterBatchTrigger::Render()
{
	return _int();
}

_int CInstanceMonsterBatchTrigger::LateRender()
{
	return _int();
}

_int CInstanceMonsterBatchTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CInstanceMonsterBatchTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CInstanceMonsterBatchTrigger::EParkourTriggerType CInstanceMonsterBatchTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::INSTANCE_MONSTER;
}

void CInstanceMonsterBatchTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	//m_fValueMat
	//m_fSubValueMat
	if (m_bOnceSwitch == false)
	{
		ZeroMemory(&m_Instance_Batch_InfoDesc, sizeof(INSTANCE_BATCH_INFO));

		m_Instance_Batch_InfoDesc.fValueMat = m_fValueMat;
		m_Instance_Batch_InfoDesc.fSubValueMat = m_fSubValueMat;
		m_Instance_Batch_InfoDesc.Object = this;
		switch ((_uint)m_fValueMat.m[0][0])
		{
		case WASP:
		{
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Monster), TAG_OP(Prototype_Object_Monster_Wasp), &m_Instance_Batch_InfoDesc);
			break;
		}
		case WORMGRUB:
		{
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Monster), TAG_OP(Prototype_Object_Monster_Wormgrub), &m_Instance_Batch_InfoDesc);
			break;
		}
		case SPIDER:
		{
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Monster), TAG_OP(Prototype_Object_Monster_Spider), &m_Instance_Batch_InfoDesc);
			break;
		}
		case WOLF:
		{
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Monster), TAG_OP(Prototype_Object_Monster_Wolf), &m_Instance_Batch_InfoDesc);
			break;
		}
		default:
			break;
		}

		m_bOnceSwitch = true;
	}
	//Set_IsDead();
}

HRESULT CInstanceMonsterBatchTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CInstanceMonsterBatchTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CInstanceMonsterBatchTrigger * CInstanceMonsterBatchTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CInstanceMonsterBatchTrigger*	pInstance = NEW CInstanceMonsterBatchTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CInstanceMonsterBatchTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInstanceMonsterBatchTrigger::Clone(void * pArg)
{
	CInstanceMonsterBatchTrigger*	pInstance = NEW CInstanceMonsterBatchTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CInstanceMonsterBatchTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInstanceMonsterBatchTrigger::Free()
{
	__super::Free();
}
