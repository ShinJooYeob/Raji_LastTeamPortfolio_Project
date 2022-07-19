#include "stdafx.h"
#include "..\Public\Scene_Stage5.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "PlayerWeapon_Spear.h"
#include "MapObject.h"
#include "Trigger_ChangeCameraView.h"
#include "TestLedgeTrigger.h"
#include "Transform.h"

CScene_Stage5::CScene_Stage5(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice,pDeviceContext)
{
}





HRESULT CScene_Stage5::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	FAILED_CHECK(Ready_Light());

	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_SkyBox(TAG_LAY(Layer_SkyBox)));
	FAILED_CHECK(Ready_Layer_Terrain(TAG_LAY(Layer_Terrain)));
	FAILED_CHECK(Ready_Layer_TestMapObject(TAG_LAY(Layer_StaticMapObj)));
	//FAILED_CHECK(Ready_Layer_Boss(TAG_LAY(Layer_Boss)));
	FAILED_CHECK(Ready_Layer_Trigger(TAG_LAY(Layer_ColTrigger))); 
	
	return S_OK;
}

_int CScene_Stage5::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	return 0;
}

_int CScene_Stage5::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	return 0;
}

_int CScene_Stage5::Render()
{
	if (__super::Render() < 0)
		return -1;


	return 0;
}

_int CScene_Stage5::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

_int CScene_Stage5::Change_to_NextScene()
{

	FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, (SCENEID)m_eNextScene), SCENEID::SCENE_LOADING));

	return _int();
}



HRESULT CScene_Stage5::Ready_Light()
{

	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);

	if (pLightDesc == nullptr)
	{

		LIGHTDESC LightDesc;

		LightDesc.eLightType = tagLightDesc::TYPE_DIRECTIONAL;
		LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.vAmbient = _float4(1.0f);
		LightDesc.vSpecular = _float4(1);
		LightDesc.vVector = DefalutSunPosition;

		g_pGameInstance->Add_Light(LightDesc);
	}
	else
	{
		g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, DefalutSunPosition.XMVector());
	}


	return S_OK;
}

HRESULT CScene_Stage5::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 10.f, -5.f);
	CameraDesc.vAt = _float3(0, 0.f, 0);
	CameraDesc.vAxisY = _float3(0, 1, 0);

	CameraDesc.fFovy = XMConvertToRadians(60.f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = CAMERAFAR;

	CameraDesc.iWinCX = g_iWinCX;
	CameraDesc.iWinCY = g_iWinCY;

	CameraDesc.TransformDesc.fMovePerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.0f);
	CameraDesc.TransformDesc.fScalingPerSec = 1.f;


	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));

	if (m_pMainCam == nullptr)
	{
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Camera_Main), &CameraDesc));

		m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));

		NULL_CHECK_RETURN(m_pMainCam, E_FAIL);

	}
	else 
	{
		m_pMainCam->Set_NowSceneNum(SCENE_STAGE5);
	}
	
	return S_OK;
}

HRESULT CScene_Stage5::Ready_Layer_Player(const _tchar * pLayerTag)
{
	
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Player)));
	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE5, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	CTransform* PlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
	CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));

	static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(30.f, 37.460f, 60.f));
	//static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(157.422f, 23.7f, 75.991f));

	PlayerNavi->FindCellIndex(PlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));

	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));


	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);

	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
	m_pMainCam->Set_FocusTarget(pPlayer);
	m_pMainCam->Set_TargetArmLength(0.f);



	return S_OK;

}

HRESULT CScene_Stage5::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_SkyBox)));

	return S_OK;
}

HRESULT CScene_Stage5::Ready_Layer_TestMapObject(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_StaticMapObject)));
	CTransform* pTransform = (CTransform*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE5, pLayerTag)->Get_Component(TAG_COM(Com_Transform)));
	NULL_CHECK_RETURN(pTransform, E_FAIL);
	_Matrix tt = XMMatrixScaling(20, 1, 20) * XMMatrixTranslation(0, -3, 0);
	pTransform->Set_Matrix(tt);
	((CMapObject*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE5, pLayerTag))->Set_FrustumSize(99999999.f);






	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE5, pLayerTag, L"TestNonAnimInstance"));

	





	return S_OK;
}

HRESULT CScene_Stage5::Ready_Layer_Boss(const _tchar * pLayerTag)
{
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE4, pLayerTag, TAG_OP(Prototype_Object_Boss_Chiedtian)));

	return S_OK;
}

HRESULT CScene_Stage5::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE3, pLayerTag, TAG_OP(Prototype_Terrain)));



	return S_OK;
}

HRESULT CScene_Stage5::Ready_Layer_Trigger(const _tchar * pLayerTag)
{
	//CTrigger_ChangeCameraView::CHANGECAMERAVIEWDESC ChangeCameraViewDesc;
	//ChangeCameraViewDesc.eChangeCameraViewType = CTrigger_ChangeCameraView::EChangeCameraViewType::TYPE_FIX;
	//ChangeCameraViewDesc.fMain_CamPos = _float3(0.f, 2.f, -2.f);
	//ChangeCameraViewDesc.fMain_CamLook = _float3(0.f, 0.f, 1.f);
	//ChangeCameraViewDesc.fMain_Pos = _float3(5.f, 0.01f, 5.f);
	//ChangeCameraViewDesc.fMain_Scale = _float3(3.f, 3.f, 3.f);
	//ChangeCameraViewDesc.bLockCamLook = true;
	//ChangeCameraViewDesc.fMain_CamMoveWeight = 0.98f;
	//ChangeCameraViewDesc.fMain_CamLookWeight = 0.98f;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_ChangeCameraView), &ChangeCameraViewDesc));


	//ChangeCameraViewDesc.eChangeCameraViewType = CTrigger_ChangeCameraView::EChangeCameraViewType::TYPE_FIX;
	//ChangeCameraViewDesc.fMain_CamPos = _float3(0.f, 1.5f, -2.f);
	//ChangeCameraViewDesc.fMain_CamLook = _float3(0.f, 0.f, 1.f);
	//ChangeCameraViewDesc.fMain_Pos = _float3(0.f, 0.01f, 5.f);
	//ChangeCameraViewDesc.fMain_Scale = _float3(3.f, 3.f, 3.f);
	//ChangeCameraViewDesc.bLockCamLook = false;
	//ChangeCameraViewDesc.fMain_CamMoveWeight = 0.9f;
	//ChangeCameraViewDesc.fMain_CamLookWeight = 0.9f;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_ChangeCameraView), &ChangeCameraViewDesc));


	//ChangeCameraViewDesc.eChangeCameraViewType = CTrigger_ChangeCameraView::EChangeCameraViewType::TYPE_FIX_SWITCH;
	//ChangeCameraViewDesc.fMain_CamPos = _float3(0.f, 2.f, -2.f);
	//ChangeCameraViewDesc.fMain_CamLook = _float3(0.f, 0.f, 1.f);
	//ChangeCameraViewDesc.fMain_Pos = _float3(-5.f, 0.01f, 5.f);
	//ChangeCameraViewDesc.fMain_Scale = _float3(3.f, 3.f, 3.f);
	//ChangeCameraViewDesc.bLockCamLook = true;
	//ChangeCameraViewDesc.fMain_CamMoveWeight = 0.99f;
	//ChangeCameraViewDesc.fMain_CamLookWeight = 0.99f;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_ChangeCameraView), &ChangeCameraViewDesc));


	//ChangeCameraViewDesc.eChangeCameraViewType = CTrigger_ChangeCameraView::EChangeCameraViewType::TYPE_TWO_INTERP;
	//ChangeCameraViewDesc.fMain_CamPos = _float3(0.f, 6.f, -2.f);
	//ChangeCameraViewDesc.fMain_CamLook = _float3(0.f, -1.f, 1.f);
	//ChangeCameraViewDesc.fMain_Pos = _float3(5.f, 0.01f, 5.f);
	//ChangeCameraViewDesc.fMain_Scale = _float3(3.f, 3.f, 3.f); 
	// 
	//ChangeCameraViewDesc.fSub_CamPos = _float3(-2.f, 6.f, 0.f);
	//ChangeCameraViewDesc.fSub_CamLook = _float3(1.f, -1.f, 0.f);
	//ChangeCameraViewDesc.fSub_Pos = _float3(-10.f, 0.01f, 5.f);
	//ChangeCameraViewDesc.fSub_Scale = _float3(3.f, 3.f, 3.f);
	//ChangeCameraViewDesc.bLockCamLook = true;
	//ChangeCameraViewDesc.fMain_CamMoveWeight = 0.9f;
	//ChangeCameraViewDesc.fMain_CamLookWeight = 0.9f;
	//ChangeCameraViewDesc.fSub_CamMoveWeight = 0.5f;
	//ChangeCameraViewDesc.fSub_CamLookWeight = 0.5f;

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_ChangeCameraView), &ChangeCameraViewDesc));

	//CTestLedgeTrigger::LEDGETRIGGERDESC tLedgeTriggerDesc;
	//tLedgeTriggerDesc.fSpawnPos = _float3(8.f, 0.f, 13.f);
	//tLedgeTriggerDesc.eLedgeTriggerState = CTestLedgeTrigger::ELedgeTriggerState::STATE_START;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_TestLedgeTrigger),&tLedgeTriggerDesc));
	//
	//tLedgeTriggerDesc.fSpawnPos = _float3(8.f, 3.f, 13.f);
	//tLedgeTriggerDesc.eLedgeTriggerState = CTestLedgeTrigger::ELedgeTriggerState::STATE_LEDGE;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_TestLedgeTrigger), &tLedgeTriggerDesc));
	//
	//tLedgeTriggerDesc.fSpawnPos = _float3(8.f, 6.f, 13.f);
	//tLedgeTriggerDesc.eLedgeTriggerState = CTestLedgeTrigger::ELedgeTriggerState::STATE_LEDGE;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_TestLedgeTrigger), &tLedgeTriggerDesc));
	//
	//tLedgeTriggerDesc.fSpawnPos = _float3(8.f, 9.f, 13.f);
	//tLedgeTriggerDesc.eLedgeTriggerState = CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_TestLedgeTrigger), &tLedgeTriggerDesc));
	
	/*tLedgeTriggerDesc.fSpawnPos = _float3(5.f, 11.f, 13.f);
	tLedgeTriggerDesc.eLedgeTriggerState = CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE5, pLayerTag, TAG_OP(Prototype_Trigger_TestLedgeTrigger), &tLedgeTriggerDesc));*/

	return S_OK;
}



CScene_Stage5 * CScene_Stage5::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Stage5* pTempLoby = NEW CScene_Stage5(pDevice,pDeviceContext);

	if (FAILED(pTempLoby->Initialize())) 
	{
		Safe_Release(pTempLoby); 
		MSGBOX("Failed to Creating CScene_Stage5");
		return nullptr;
	}

	return pTempLoby;

}

void CScene_Stage5::Free()
{
	__super::Free();
}
