#include "stdafx.h"
#include "..\public\Trigger_ChangeCameraView.h"
#include "Player.h"
#include "Camera_Main.h"

CTrigger_ChangeCameraView::CTrigger_ChangeCameraView(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CTrigger_ChangeCameraView::CTrigger_ChangeCameraView(const CTrigger_ChangeCameraView & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CTrigger_ChangeCameraView::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CTrigger_ChangeCameraView::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_PlayerAndCameraInfo());

	return S_OK;
}

_int CTrigger_ChangeCameraView::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;
	

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CTrigger_ChangeCameraView::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
	//FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pCollider_DEBUG)); 
#endif // _DEBUG

	return _int();
}

_int CTrigger_ChangeCameraView::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CTrigger_ChangeCameraView::LateRender()
{
	return _int();
}

_int CTrigger_ChangeCameraView::Active_Trigger(CGameObject* pTarget, _double fDeltaTime)
{
	switch (m_tChangeCameraViewDesc.eChangeCameraViewType)
	{
	case EChangeCameraViewType::TYPE_FIX:
		Change_CameraView_Fix(fDeltaTime);
		break;
	case EChangeCameraViewType::TYPE_FIX_SWITCH:
		Change_CameraView_FixSwitch(fDeltaTime);
		break;
	case EChangeCameraViewType::TYPE_TWO_INTERP:
		Change_CameraView_TwoPoint_Interp(fDeltaTime);
		break;
	}
	return _int();
}

_int CTrigger_ChangeCameraView::DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime)
{
	switch (m_tChangeCameraViewDesc.eChangeCameraViewType)
	{
	case EChangeCameraViewType::TYPE_FIX:
		break;
	case EChangeCameraViewType::TYPE_FIX_SWITCH:
		ChangeBack_CameraView(fDeltaTime);
		break;
	case EChangeCameraViewType::TYPE_TWO_INTERP:
		break;
	}
	return _int();
}

HRESULT CTrigger_ChangeCameraView::After_Initialize()
{
	m_tChangeCameraViewDesc.eChangeCameraViewType = static_cast<EChangeCameraViewType>((_int)m_fValueMat._11);
	if (EChangeCameraViewType::TYPE_TWO_INTERP == m_tChangeCameraViewDesc.eChangeCameraViewType)
	{
		m_tChangeCameraViewDesc.fMain_Pos = _float3(m_fValueMat._12, m_fValueMat._13, m_fValueMat._14);
		m_tChangeCameraViewDesc.fMain_CamPos = _float3(m_fValueMat._21, m_fValueMat._22, m_fValueMat._23);
		m_tChangeCameraViewDesc.fMain_CamLook = _float3(m_fValueMat._24, m_fValueMat._31, m_fValueMat._32);
		m_tChangeCameraViewDesc.fMain_CamMoveWeight = m_fValueMat._33;
		m_tChangeCameraViewDesc.fMain_CamLookWeight = m_fValueMat._34;
		m_tChangeCameraViewDesc.bMainLockCamLook = (m_fValueMat._41 > 0 ? true : false);
		m_tChangeCameraViewDesc.fMainMaxTargetArmLength = m_fValueMat._42;
		m_tChangeCameraViewDesc.fMainMinTargetArmLength = m_fValueMat._43;

		m_tChangeCameraViewDesc.fSub_Pos = _float3(m_fSubValueMat._11, m_fSubValueMat._12, m_fSubValueMat._13);
		m_tChangeCameraViewDesc.fSub_CamPos = _float3(m_fSubValueMat._14, m_fSubValueMat._21, m_fSubValueMat._22);
		m_tChangeCameraViewDesc.fSub_CamLook = _float3(m_fSubValueMat._23, m_fSubValueMat._24, m_fSubValueMat._31);
		m_tChangeCameraViewDesc.fSub_CamMoveWeight = m_fSubValueMat._32;
		m_tChangeCameraViewDesc.fSub_CamLookWeight = m_fSubValueMat._33;
		m_tChangeCameraViewDesc.bSubLockCamLook = (m_fSubValueMat._34 > 0 ? true : false);
		m_tChangeCameraViewDesc.fSubMaxTargetArmLength = m_fSubValueMat._41;
		m_tChangeCameraViewDesc.fSubMinTargetArmLength = m_fSubValueMat._42;
		FAILED_CHECK(SetUp_EtcInfo());
	}


	return S_OK;
}

void CTrigger_ChangeCameraView::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	// Change_CameraView_TwoPoint_Interp
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		if (true == static_cast<CPlayer*>(pConflictedObj)->Get_IsBlockCamTriggerState())
		{
			return;
		}

		switch (m_tChangeCameraViewDesc.eChangeCameraViewType)
		{
		case EChangeCameraViewType::TYPE_FIX:
			Change_CameraView_Stop(g_fDeltaTime);
			break;
		case EChangeCameraViewType::TYPE_FIX_SWITCH:
			break;
		case EChangeCameraViewType::TYPE_TWO_INTERP:
			Change_CameraView_TwoPoint_Interp(g_fDeltaTime);
			break;
		case EChangeCameraViewType::TYPE_STOP:
			Change_CameraView_Stop(g_fDeltaTime);
			break;
		case EChangeCameraViewType::TYPE_FIRSTPERSON_VIEW:
			Change_CameraView_FirstPerson_View(g_fDeltaTime);
			break;
		}
	}
}

_bool CTrigger_ChangeCameraView::Check_CollisionToPlayer()
{

	return true;
}

_int CTrigger_ChangeCameraView::Change_CameraView_Fix(_double fDeltaTime)
{
//	m_pMainCamera->Lock_CamLook(m_tChangeCameraViewDesc.bLockCamLook, m_tChangeCameraViewDesc.fMain_CamLook.XMVector());
	m_pMainCamera->Set_CameraMoveWeight(m_tChangeCameraViewDesc.fMain_CamMoveWeight);
	m_pMainCamera->Set_CameraLookWeight(m_tChangeCameraViewDesc.fMain_CamLookWeight);
	m_pPlayer->Set_AttachCamPosOffset(m_tChangeCameraViewDesc.fMain_CamPos);

	return _int();
}

_int CTrigger_ChangeCameraView::Change_CameraView_FixSwitch(_double fDeltaTime)
{
	if (false == m_bStoredPreCameraInfo)
	{
		m_bStoredPreCameraInfo = true;
		m_bPreCameraLookLockState = m_pMainCamera->Get_CamLock();
		m_tChangeCameraViewDesc.fSub_CamLook = m_pMainCamera->Get_FixLookDir();
		m_tChangeCameraViewDesc.fSub_CamPos = m_pPlayer->Get_AttachCamPosOffset();
		m_fPreCameraMoveWeight = m_pMainCamera->Get_CameraMoveWeight();
		m_fPreCameraLookWeight = m_pMainCamera->Get_CameraLookWeight();
	}

	//m_pMainCamera->Lock_CamLook(m_tChangeCameraViewDesc.bLockCamLook, m_tChangeCameraViewDesc.fMain_CamLook.XMVector());
	m_pMainCamera->Set_CameraMoveWeight(m_tChangeCameraViewDesc.fMain_CamMoveWeight);
	m_pMainCamera->Set_CameraLookWeight(m_tChangeCameraViewDesc.fMain_CamLookWeight);
	m_pPlayer->Set_AttachCamPosOffset(m_tChangeCameraViewDesc.fMain_CamPos);
	return _int();
}

_int CTrigger_ChangeCameraView::Change_CameraView_TwoPoint_Interp(_double fDeltaTime)
{
	_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);


	_float fDist_MainToPlayer = XMVectorGetX(XMVector3Length(m_tChangeCameraViewDesc.fMain_Pos.XMVector() - vPlayerPos));
	_float fWeight_MainToPlayer = fDist_MainToPlayer / m_fDist_MainToSub;

	if (1.f < fWeight_MainToPlayer)
		fWeight_MainToPlayer = 1.f;
	//_float3 fResult_CamPos = g_pGameInstance->Easing_Vector(TYPE_Linear, m_tChangeCameraViewDesc.fMain_CamPos.XMVector(), m_tChangeCameraViewDesc.fSub_CamPos.XMVector(), fWeight_MainToPlayer, 1.f);
	_float3 fResult_CamPos = (m_tChangeCameraViewDesc.fMain_CamPos.XMVector() * (1.f - fWeight_MainToPlayer)) + (m_tChangeCameraViewDesc.fSub_CamPos.XMVector() * fWeight_MainToPlayer);
	_float3 fResult_CamLook = (m_tChangeCameraViewDesc.fMain_CamLook.XMVector() * (1.f - fWeight_MainToPlayer)) + (m_tChangeCameraViewDesc.fSub_CamLook.XMVector() * fWeight_MainToPlayer);
	_float fResult_MoveWeight = (m_tChangeCameraViewDesc.fMain_CamMoveWeight * (1.f - fWeight_MainToPlayer)) + (m_tChangeCameraViewDesc.fSub_CamMoveWeight * fWeight_MainToPlayer);
	_float fResult_LookWeight = (m_tChangeCameraViewDesc.fMain_CamLookWeight * (1.f - fWeight_MainToPlayer)) + (m_tChangeCameraViewDesc.fSub_CamLookWeight * fWeight_MainToPlayer);

	_bool bCamLookLock = false;


	if (fWeight_MainToPlayer < 0.5f)
	{
		if (false == m_tChangeCameraViewDesc.bMainLockCamLook && fWeight_MainToPlayer > 0.1f)
		{
			if(true == m_tChangeCameraViewDesc.bSubLockCamLook)
				m_pMainCamera->Lock_CamLook(true, fResult_CamLook.XMVector());
		}
		else
		{
			m_pMainCamera->Lock_CamLook(m_tChangeCameraViewDesc.bMainLockCamLook, fResult_CamLook.XMVector());
		}
		m_pMainCamera->Set_MaxTargetArmLength(m_tChangeCameraViewDesc.fMainMaxTargetArmLength);
		m_pMainCamera->Set_MinTargetArmLength(m_tChangeCameraViewDesc.fMainMinTargetArmLength);
	}
	else
	{
		if (false == m_tChangeCameraViewDesc.bSubLockCamLook && fWeight_MainToPlayer < 0.9f)
		{
			if (true == m_tChangeCameraViewDesc.bMainLockCamLook)
				m_pMainCamera->Lock_CamLook(true, fResult_CamLook.XMVector());
		}
		else
		{
			m_pMainCamera->Lock_CamLook(m_tChangeCameraViewDesc.bSubLockCamLook, fResult_CamLook.XMVector());
		}
		m_pMainCamera->Set_MaxTargetArmLength(m_tChangeCameraViewDesc.fSubMaxTargetArmLength);
		m_pMainCamera->Set_MinTargetArmLength(m_tChangeCameraViewDesc.fSubMinTargetArmLength);
	}

	m_pMainCamera->Set_CameraMoveWeight(0.91f); 
	m_pMainCamera->Set_CameraLookWeight(0.95f);

	m_pPlayer->Set_AttachCamPosOffset(fResult_CamPos);
	
	return _int();
}

_int CTrigger_ChangeCameraView::Change_CameraView_Stop(_double fDeltaTime)
{
	m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_FIX);

	return _int();
}

_int CTrigger_ChangeCameraView::Change_CameraView_FirstPerson_View(_double fDeltaTime)
{
	m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_FIRSTPERSONVIEW);
	m_pPlayer->Set_PlayerState(CPlayer::EPLAYER_STATE::STATE_FIRSTPERSONVIEW);

	return _int();
}

_int CTrigger_ChangeCameraView::ChangeBack_CameraView(_double fDeltaTime)
{
	if (true == m_bStoredPreCameraInfo)
	{
		m_bStoredPreCameraInfo = false;
		m_pMainCamera->Lock_CamLook(m_bPreCameraLookLockState, m_tChangeCameraViewDesc.fMain_CamLook.XMVector());
		m_pMainCamera->Set_CameraMoveWeight(m_fPreCameraMoveWeight);
		m_pMainCamera->Set_CameraLookWeight(m_fPreCameraLookWeight);
		m_pPlayer->Set_AttachCamPosOffset(m_tChangeCameraViewDesc.fMain_CamPos);
	}

	return _int();
}

HRESULT CTrigger_ChangeCameraView::SetUp_Components()
{
	// Main Transform
	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_SubTransform), (CComponent**)&m_pTransformCom_Main, &tDesc));


	// Sub Transform
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_SubTransform2), (CComponent**)&m_pTransformCom_Sub, &tDesc));

	return S_OK;
}

HRESULT CTrigger_ChangeCameraView::SetUp_PlayerAndCameraInfo()
{
	if (SCENE_EDIT != g_pGameInstance->Get_NowSceneNum())
	{
		m_pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player)));
		m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(TAG_COM(Com_Transform)));
		m_pMainCamera = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	}

	return S_OK;
}

HRESULT CTrigger_ChangeCameraView::SetUp_EtcInfo()
{
	m_pTransformCom_Main->Set_MatrixState(CTransform::TransformState::STATE_POS, m_tChangeCameraViewDesc.fMain_Pos);
	m_pTransformCom_Main->Rotation_CW(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_pTransformCom_Sub->Set_MatrixState(CTransform::TransformState::STATE_POS, m_tChangeCameraViewDesc.fSub_Pos);
	m_pTransformCom_Sub->Rotation_CW(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));


	if (EChangeCameraViewType::TYPE_TWO_INTERP == m_tChangeCameraViewDesc.eChangeCameraViewType)
	{
		m_fDist_MainToSub = XMVectorGetX(XMVector3Length(m_tChangeCameraViewDesc.fMain_Pos.XMVector() - m_tChangeCameraViewDesc.fSub_Pos.XMVector()));
	}

	return S_OK;
}

CTrigger_ChangeCameraView * CTrigger_ChangeCameraView::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTrigger_ChangeCameraView*	pInstance = NEW CTrigger_ChangeCameraView(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTrigger_ChangeCameraView");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTrigger_ChangeCameraView::Clone(void * pArg)
{
	CTrigger_ChangeCameraView*	pInstance = NEW CTrigger_ChangeCameraView(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CTrigger_ChangeCameraView");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrigger_ChangeCameraView::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom_Main);
	Safe_Release(m_pTransformCom_Sub);
	Safe_Release(m_pCollider_DEBUG);
}
