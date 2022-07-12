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

	memcpy(&m_tChangeCameraViewDesc, pArg, sizeof(CHANGECAMERAVIEWDESC));

	FAILED_CHECK(SetUp_EtcInfo());

	FAILED_CHECK(SetUp_PlayerAndCameraInfo());

	return S_OK;
}

_int CTrigger_ChangeCameraView::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;
	
	if (true == Check_CollisionToPlayer())
	{
		Active_Trigger(fDeltaTime);
	}
	else
	{
		DeActive_Trigger(fDeltaTime);
	}
	
	//Active_Trigger(fDeltaTime);
	return _int();
}

_int CTrigger_ChangeCameraView::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this));
	return _int();
}

_int CTrigger_ChangeCameraView::Render()
{
	if (__super::Render() < 0)		return -1;

	switch (m_tChangeCameraViewDesc.eChangeCameraViewType)
	{
	case EChangeCameraViewType::TYPE_FIX:
	case EChangeCameraViewType::TYPE_FIX_SWITCH:
	{
		CGameInstance* pInstance = GetSingle(CGameInstance);
		NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

		FAILED_CHECK(m_pTransformCom_Main->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

		if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;

		m_pVIBufferCom->Render(m_pShaderCom, 3);
	}
		break;
	case EChangeCameraViewType::TYPE_TWO_INTERP:
		CGameInstance* pInstance = GetSingle(CGameInstance);
		NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);
		FAILED_CHECK(m_pTransformCom_Main->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
		if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;
		m_pVIBufferCom->Render(m_pShaderCom, 3);


		FAILED_CHECK(m_pTransformCom_Sub->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
		m_pVIBufferCom->Render(m_pShaderCom, 3);
		break;
	}

	return _int();
}

_int CTrigger_ChangeCameraView::LateRender()
{
	return _int();
}

_int CTrigger_ChangeCameraView::Active_Trigger(_double fDeltaTime)
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

_int CTrigger_ChangeCameraView::DeActive_Trigger(_double fDeltaTime)
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

_bool CTrigger_ChangeCameraView::Check_CollisionToPlayer()
{
	// Temperate ( Later Change to Collider )
	_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vTriggerPos = m_pTransformCom_Main->Get_MatrixState(CTransform::TransformState::STATE_POS);

	_float fDist = XMVectorGetX(XMVector3Length(vTriggerPos - vPlayerPos));
	if (fDist <= 3.f)
	{
		return true;
	}
	//
	return false;
}

_int CTrigger_ChangeCameraView::Change_CameraView_Fix(_double fDeltaTime)
{
	m_pMainCamera->Lock_CamLook(m_tChangeCameraViewDesc.bLockCamLook, m_tChangeCameraViewDesc.fMain_CamLook.XMVector());
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

	m_pMainCamera->Lock_CamLook(m_tChangeCameraViewDesc.bLockCamLook, m_tChangeCameraViewDesc.fMain_CamLook.XMVector());
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

	_float3 fResult_CamPos = (m_tChangeCameraViewDesc.fMain_CamPos.XMVector() * fWeight_MainToPlayer) + (m_tChangeCameraViewDesc.fSub_CamPos.XMVector() * (1.f - fWeight_MainToPlayer));
	_float3 fResult_CamLook = (m_tChangeCameraViewDesc.fMain_CamLook.XMVector() * fWeight_MainToPlayer) + (m_tChangeCameraViewDesc.fSub_CamLook.XMVector() * (1.f - fWeight_MainToPlayer));
	_float fResult_MoveWeight = (m_tChangeCameraViewDesc.fMain_CamMoveWeight * fWeight_MainToPlayer) + (m_tChangeCameraViewDesc.fSub_CamMoveWeight * (1.f - fWeight_MainToPlayer));
	_float fResult_LookWeight = (m_tChangeCameraViewDesc.fMain_CamLookWeight * fWeight_MainToPlayer) + (m_tChangeCameraViewDesc.fSub_CamLookWeight * (1.f - fWeight_MainToPlayer));

	m_pMainCamera->Lock_CamLook(m_tChangeCameraViewDesc.bLockCamLook, fResult_CamLook.XMVector());
	m_pMainCamera->Set_CameraMoveWeight(fResult_MoveWeight);
	m_pMainCamera->Set_CameraLookWeight(fResult_LookWeight);

	m_pPlayer->Set_AttachCamPosOffset(fResult_CamPos);
	
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
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom_Main, &tDesc));


	// Sub Transform
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_SubTransform), (CComponent**)&m_pTransformCom_Sub, &tDesc));


	// For Debug
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_ShellingPoint), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));
	//

	return S_OK;
}

HRESULT CTrigger_ChangeCameraView::SetUp_PlayerAndCameraInfo()
{
	m_pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player)));
	m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(TAG_COM(Com_Transform)));
	m_pMainCamera = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));

	return S_OK;
}

HRESULT CTrigger_ChangeCameraView::SetUp_EtcInfo()
{
	m_pTransformCom_Main->Set_MatrixState(CTransform::TransformState::STATE_POS, m_tChangeCameraViewDesc.fMain_Pos);
	m_pTransformCom_Main->Scaled_All(m_tChangeCameraViewDesc.fMain_Scale);
	m_pTransformCom_Main->Rotation_CW(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_pTransformCom_Sub->Set_MatrixState(CTransform::TransformState::STATE_POS, m_tChangeCameraViewDesc.fSub_Pos);
	m_pTransformCom_Sub->Scaled_All(m_tChangeCameraViewDesc.fSub_Scale);
	m_pTransformCom_Sub->Rotation_CW(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));


	if (EChangeCameraViewType::TYPE_TWO_INTERP == m_tChangeCameraViewDesc.eChangeCameraViewType)
	{
		m_fDist_MainToSub = XMVectorGetX(XMVector3Length(m_tChangeCameraViewDesc.fMain_Pos.XMVector() - m_tChangeCameraViewDesc.fSub_Pos.XMVector()));
	}

	return S_OK;
}

CTrigger_ChangeCameraView * CTrigger_ChangeCameraView::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTrigger_ChangeCameraView*	pInstance = new CTrigger_ChangeCameraView(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTrigger_ChangeCameraView");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTrigger_ChangeCameraView::Clone(void * pArg)
{
	CTrigger_ChangeCameraView*	pInstance = new CTrigger_ChangeCameraView(*this);

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

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
