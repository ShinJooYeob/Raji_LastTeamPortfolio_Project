#include "stdafx.h"
#include "..\public\FogTrigger.h"
#include "Player.h"

CFogTrigger::CFogTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CFogTrigger::CFogTrigger(const CFogTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CFogTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CFogTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_PlayerAndCameraInfo());

	return S_OK;
}

HRESULT CFogTrigger::After_Initialize()
{
	m_tFogTriggerDesc.fMain_Pos = _float3(m_fValueMat._11, m_fValueMat._12, m_fValueMat._13);
	m_tFogTriggerDesc.fSub_Pos = _float3(m_fValueMat._14, m_fValueMat._21, m_fValueMat._22);

	m_tFogTriggerDesc.fMain_FogDensity = _float(m_fValueMat._23);
	m_tFogTriggerDesc.fSub_FogDensity = _float(m_fValueMat._24);

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

_int CFogTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CFogTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CFogTrigger::Render()
{
	return _int();
}

_int CFogTrigger::LateRender()
{
	return _int();
}

_int CFogTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CFogTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CFogTrigger::EParkourTriggerType CFogTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType();
}

void CFogTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);

		_float fDist_MainToPlayer = XMVectorGetX(XMVector3Length(m_tFogTriggerDesc.fMain_Pos.XMVector() - vPlayerPos));
		_float fWeight_MainToPlayer = fDist_MainToPlayer / m_fDist_MainToSub;

		if (1.f < fWeight_MainToPlayer)
		{
			fWeight_MainToPlayer = 1.f; 
		}
		
		_float fResultFogDensity = m_tFogTriggerDesc.fMain_FogDensity * (1.f - fWeight_MainToPlayer) + m_tFogTriggerDesc.fSub_FogDensity * fWeight_MainToPlayer;
		
		GetSingle(CUtilityMgr)->Get_Renderer()->Set_FogGlobalDensity(fResultFogDensity);
	}
}

HRESULT CFogTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CFogTrigger::SetUp_PlayerAndCameraInfo()
{
	if (SCENE_EDIT != g_pGameInstance->Get_NowSceneNum())
	{
		m_pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player)));
		m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(TAG_COM(Com_Transform)));
		m_pMainCamera = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	}

	return S_OK;
}

HRESULT CFogTrigger::SetUp_EtcInfo()
{
	m_fDist_MainToSub = XMVectorGetX(XMVector3Length(m_tFogTriggerDesc.fMain_Pos.XMVector() - m_tFogTriggerDesc.fSub_Pos.XMVector()));

	return S_OK;
}

CFogTrigger * CFogTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CFogTrigger*	pInstance = NEW CFogTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CFogTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFogTrigger::Clone(void * pArg)
{
	CFogTrigger*	pInstance = NEW CFogTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CFogTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFogTrigger::Free()
{
	__super::Free();
}
