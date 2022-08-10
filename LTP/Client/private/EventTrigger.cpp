#include "stdafx.h"
#include "..\public\EventTrigger.h"
#include "Player.h"
#include "Camera_Main.h"
#include "Rangda.h"
#include "Chiedtian.h"
#include "Scene_Stage3.h"

CEventTrigger::CEventTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CEventTrigger::CEventTrigger(const CEventTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CEventTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CEventTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_PlayerAndCameraInfo());

	return S_OK;
}

HRESULT CEventTrigger::After_Initialize()
{
	m_eEventType = (EEventType)((_int)m_fValueMat._11);

	return S_OK;
}

_int CEventTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CEventTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CEventTrigger::Render()
{
	return _int();
}

_int CEventTrigger::LateRender()
{
	return _int();
}

_int CEventTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CEventTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CEventTrigger::EParkourTriggerType CEventTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType();
}

void CEventTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType && false == m_bActive)
	{
		switch (m_eEventType)
		{
		case TYPE_RANGDA_CUTSCENE:
			EVENT_Rangda_Cutscene();
			break;
		case TYPE_CHIEDTIAN_CUTSCENE:
			EVENT_Chiedtian_Cutscene();
			break;
		}

		m_bActive = true;
	}
}

void CEventTrigger::EVENT_Rangda_Cutscene()
{
	CRangda* pRangda = static_cast<CRangda*>(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE3, Tag_Layer(Layer_Boss), 0));
	pRangda->Change_Animation(6);
	pRangda->Set_Dissolve_In();

	m_pPlayer->Set_State_StopActionStart();

	static_cast<CScene_Stage3*>(g_pGameInstance->Get_NowScene())->Set_PlayGoluSound(false);

	g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_UI_IMG))->Set_IsDead();
}

void CEventTrigger::EVENT_Chiedtian_Cutscene()
{
	CChiedtian* pChiedtian = static_cast<CChiedtian*>(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE7, Tag_Layer(Layer_Boss), 0));
	pChiedtian->Change_Animation(14);
	m_pPlayer->Set_State_StopActionStart();

	GetSingle(CUtilityMgr)->Get_MainCamera()->Set_FocusTarget(pChiedtian);
	GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 0.f));
}

HRESULT CEventTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CEventTrigger::SetUp_PlayerAndCameraInfo()
{
	if (SCENE_EDIT != g_pGameInstance->Get_NowSceneNum())
	{
		m_pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player)));
		m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(TAG_COM(Com_Transform)));
		m_pMainCamera = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	}

	return S_OK;
}

HRESULT CEventTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CEventTrigger * CEventTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CEventTrigger*	pInstance = NEW CEventTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CEventTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEventTrigger::Clone(void * pArg)
{
	CEventTrigger*	pInstance = NEW CEventTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CEventTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEventTrigger::Free()
{
	__super::Free();
}
