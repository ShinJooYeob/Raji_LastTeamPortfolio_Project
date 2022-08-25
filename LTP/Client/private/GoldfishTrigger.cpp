#include "stdafx.h"
#include "..\public\GoldfishTrigger.h"
#include "Monster.h"


CGoldfishTrigger::CGoldfishTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CGoldfishTrigger::CGoldfishTrigger(const CGoldfishTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CGoldfishTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CGoldfishTrigger::Initialize_Clone(void * pArg)
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

HRESULT CGoldfishTrigger::After_Initialize()
{
	return S_OK;
}

_int CGoldfishTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	if (m_eNowSceneNum != SCENEID::SCENE_EDIT && m_bOnceSwitch == true && m_bAllDieOn == false)
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

_int CGoldfishTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CGoldfishTrigger::Render()
{
	return _int();
}

_int CGoldfishTrigger::LateRender()
{
	return _int();
}

_int CGoldfishTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CGoldfishTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CGoldfishTrigger::EParkourTriggerType CGoldfishTrigger::Get_ParkourTriggerType()
{
	return CGoldfishTrigger::INSTANCE_MONSTER;
}

void CGoldfishTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	//m_fValueMat
	//m_fSubValueMat
	if (m_bOnceSwitch == false)
	{
		ZeroMemory(&m_Batch_InfoDesc, sizeof(GOLDFISH_INFO));

		m_Batch_InfoDesc.fValueMat = m_fValueMat;
		m_Batch_InfoDesc.fSubValueMat = m_fSubValueMat;
		m_Batch_InfoDesc.Object = this;

		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Npc), TAG_OP(Prototype_Object_Goldfish), &m_Batch_InfoDesc);

		m_bOnceSwitch = true;
	}
	//Set_IsDead();
}

void CGoldfishTrigger::Set_MonsterAllDie(_bool bMonsterAllDie)
{
	m_bMonsterAllDie = bMonsterAllDie;
	m_bAllDieOn = bMonsterAllDie;
}

HRESULT CGoldfishTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CGoldfishTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CGoldfishTrigger * CGoldfishTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGoldfishTrigger*	pInstance = NEW CGoldfishTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGoldfishTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGoldfishTrigger::Clone(void * pArg)
{
	CGoldfishTrigger*	pInstance = NEW CGoldfishTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGoldfishTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoldfishTrigger::Free()
{
	__super::Free();
}
