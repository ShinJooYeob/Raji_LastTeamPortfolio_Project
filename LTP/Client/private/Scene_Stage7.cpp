#include "stdafx.h"
#include "..\Public\Scene_Stage7.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Player.h"
#include "MapObject.h"
#include "StaticInstanceMapObject.h"
#include "AssimpCreateMgr.h"
#include "TriggerObject.h"
#include "TestLedgeTrigger.h"
#include "MonsterBatchTrigger.h"
#include "Trigger_ChangeCameraView.h"
#include "Elevator.h"

CScene_Stage7::CScene_Stage7(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice,pDeviceContext)
{
}





HRESULT CScene_Stage7::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pUtilMgr = GetSingle(CUtilityMgr);


	FAILED_CHECK(Ready_Light());

	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_SkyBox(TAG_LAY(Layer_SkyBox)));
	FAILED_CHECK(Ready_Layer_Terrain(TAG_LAY(Layer_Terrain)));
	FAILED_CHECK(Ready_Layer_Boss(TAG_LAY(Layer_Boss)));
	FAILED_CHECK(Ready_Layer_InteractObject(TAG_LAY(Layer_InteractObject)));


	FAILED_CHECK(Ready_MapData(L"BossStage_Chiedtian.dat", SCENE_STAGE7, TAG_LAY(Layer_StaticMapObj)));
	FAILED_CHECK(Ready_TriggerObject(L"BossStage_Chiedtian.dat",   SCENE_STAGE7, TAG_LAY(Layer_ColTrigger)));
	//FAILED_CHECK(Ready_MonsterBatchTrigger(L"JinhoBabo.dat", SCENE_STAGE7, TAG_LAY(Layer_BatchMonsterTrigger)));

	//EH
	FAILED_CHECK(Ready_TriggerObject(L"Stage7_InstanceMonsterTrigger.dat", SCENE_STAGE7, TAG_LAY(Layer_ColTrigger)));

	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage7_MonsterTrigger_1.dat", SCENE_STAGE7, TAG_LAY(Layer_BatchMonsterTrigger)));
	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage7_MonsterTrigger_2.dat", SCENE_STAGE7, TAG_LAY(Layer_BatchMonsterTrigger)));
	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage7_MonsterTrigger_3.dat", SCENE_STAGE7, TAG_LAY(Layer_BatchMonsterTrigger)));
	//

	FAILED_CHECK(Ready_PostPorcessing());
	

	//FAILED_CHECK(teST());
	m_fSceneStartTimer = 0.f;
	GetSingle(CUtilityMgr)->Get_Renderer()->Set_SunAtPoint(_float3(128.f, -64.f, 256.f));

	return S_OK;
}

_int CScene_Stage7::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	if (g_pGameInstance->Get_DIKeyState(DIK_RETURN)&DIS_Down)
	{
		FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE6), SCENEID::SCENE_LOADING));
		return 0;
	}
	if (m_iSceneStartChecker <= 2)
	{
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}
	_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
	_float PlayerZ = XMVectorGetZ(vPlayerPos);
	static _float LightFar = 1.f;


	_float fValue = max(min(PlayerZ, 128.f), 103.f);
	LightFar = fValue * 2.0f / 25.f - 7.24f;
	LightFar = max(min(LightFar, 3.f), 1.f);




	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, vPlayerPos + XMVectorSet(-64.f, 64.f, 32.f, 0) * LightFar);

	//m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(XMVectorSetY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS), -64.f));


	if (PlayerZ < 80.f)
	{
		_float fValue = max(min(PlayerZ, 80.f), 45.f);
		_float fFogHeightFalloffValue = fValue * -0.08f / 35.f + 0.262857142857142f;
		fFogHeightFalloffValue = max(min(fFogHeightFalloffValue, 0.16f), 0.08f);
		m_pUtilMgr->Get_Renderer()->Set_FogHeightFalloff(fFogHeightFalloffValue);
	}
	if (PlayerZ < 58.f)
	{
		_float fValue = max(min(PlayerZ, 58.f), 45.f);
		_float fGodRayIntensValue = fValue * -0.1f / 13.f + 0.44615384615384f;
		fGodRayIntensValue = max(min(fGodRayIntensValue, 0.1f), 0.f);

		m_pUtilMgr->Get_Renderer()->Set_GodrayIntensity(fGodRayIntensValue);
	}
	return 0;
}

_int CScene_Stage7::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	return 0;
}

_int CScene_Stage7::Render()
{
	if (__super::Render() < 0)
		return -1;

	return 0;

	if (m_bIsNeedToSceneChange) return S_FALSE;
	
	if (m_fSceneStartTimer < 0.5f)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->SCD_Rendering_Rolling(((_float)m_fSceneStartTimer), 0.5f, L"Target_ToonDeferredSceneChaging2"));
	}
	else if (m_fSceneStartTimer < 2.5f)
	{

		FAILED_CHECK(GetSingle(CUtilityMgr)->SCD_Rendering_FadeOut(((_float)m_fSceneStartTimer - 0.5f), 2.f, L"Target_ToonDeferredSceneChaging2"));
	}

	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	_Vector vDir = XMVector3Normalize(XMVectorSetY(g_pGameInstance->Get_TargetPostion_Vector(PLV_PLAYER), 10) - XMVectorSet(128.f, -64.f, 256.f, 0));
	g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, XMVectorSet(128.f, -64.f, 256.f, 0) + vDir * 330.f);

	return 0;
}

_int CScene_Stage7::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

_int CScene_Stage7::Change_to_NextScene()
{

	FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, (SCENEID)m_eNextScene), SCENEID::SCENE_LOADING));

	return _int();
}



HRESULT CScene_Stage7::Ready_Light()
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

HRESULT CScene_Stage7::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 14.0000162f, -18.2519970f);
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
		m_pMainCam->Set_NowSceneNum(SCENE_STAGE7);
	}
	m_pMainCam->Set_MaxTargetArmLength(10.f);
	m_pMainCam->Set_MinTargetArmLength(4.f);
	m_pMainCam->Set_TargetArmLength(10.f);
	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_Player(const _tchar * pLayerTag)
{
	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	m_pMainCam->Set_CameraInitState(XMVectorSet(0.f, 14.0000162f, -18.2519970f, 1.f), XMVectorSet(0.f, 0.f, 1.f, 0.f));

	// _float3(0.f, 10, 0.f) Start Pos
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Player), &_float3(0.f, 10, -6.252f)));
	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE7, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	m_pPlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pPlayerTransform, E_FAIL);

	CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));
	PlayerNavi->FindCellIndex(m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));

	m_pMainCam->Lock_CamLook(true);
	static_cast<CPlayer*>(pPlayer)->Set_AttachCamPosOffset(_float3(0.f, 4.f, -2.f));
	static_cast<CPlayer*>(pPlayer)->Update_AttachCamPos();

	
	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);
	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
	m_pMainCam->Set_FocusTarget(pPlayer);
	m_pMainCam->Set_CameraInitState(XMVectorSet(0.f, 14.0000162f, -18.2519970f, 1.f), XMVectorSet(0.f, 0.f, 1.f, 0.f));
	pPlayer->Update_AttachCamPos();



	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_SkyBox)));

	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_Terrain(const _tchar * pLayerTag)
{



	return S_OK;
}


HRESULT CScene_Stage7::Ready_Layer_Boss(const _tchar * pLayerTag)
{
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Boss_Chiedtian)));

	return S_OK;
}

HRESULT CScene_Stage7::Ready_PostPorcessing()
{

#ifndef _DEBUG



	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(1000.f, -128.f, 1000.f));
	pLightDesc->vDiffuse = _float4(0.94140625f, 0.5703125f, 0.23046875f, 1.f);
	pLightDesc->vAmbient = _float4(1.f, 0.83203125f, 0.68359375f, 1.f);
	pLightDesc->vSpecular = _float4(0.40625f, 0.1875f, 0.05078125f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	pRenderer->Set_ShadowIntensive(0.35f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, true);
	pRenderer->Set_SunSize(0.5f);
	pRenderer->Set_GodrayLength(64.f);
	pRenderer->Set_GodrayIntensity(0.1f);
	pRenderer->Set_StartDecay(0.25f);
	pRenderer->Set_DistDecay(1.6f);
	pRenderer->Set_MaxDeltaLen(0.006f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, true);
	_float Value = (1 - 0.98f) * 344.f + 16.f;
	pRenderer->Set_LensfalreSupportSunSize(Value);
	pRenderer->Set_LensefalreNoiseTexIndex(245);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(2.5f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	pRenderer->Set_DofLength(45.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	pRenderer->Set_FogColor(_float3(0.94921875f, 0.4296875f, 0.328125f));
	pRenderer->Set_FogStartDist(0.1f);
	pRenderer->Set_FogGlobalDensity(0.06f);
	pRenderer->Set_FogHeightFalloff(0.16f);

	//POSTPROCESSING_GODRAY
	//POSTPROCESSING_LENSEFLARE
	//POSTPROCESSING_CAMMOTIONBLUR
#else

#ifdef DEBUGONSHADERSETTING




	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(1000.f, -128.f, 1000.f));
	pLightDesc->vDiffuse = _float4(0.94140625f, 0.5703125f, 0.23046875f, 1.f);
	pLightDesc->vAmbient = _float4(1.f, 0.83203125f, 0.68359375f, 1.f);
	pLightDesc->vSpecular = _float4(0.40625f, 0.1875f, 0.05078125f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	pRenderer->Set_ShadowIntensive(0.35f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, true);
	pRenderer->Set_SunSize(0.5f);
	pRenderer->Set_GodrayLength(64.f);
	pRenderer->Set_GodrayIntensity(0.1f);
	pRenderer->Set_StartDecay(0.25f);
	pRenderer->Set_DistDecay(1.6f);
	pRenderer->Set_MaxDeltaLen(0.006f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, true);
	_float Value = (1 - 0.98f) * 344.f + 16.f;
	pRenderer->Set_LensfalreSupportSunSize(Value);
	pRenderer->Set_LensefalreNoiseTexIndex(245);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(2.5f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	pRenderer->Set_DofLength(45.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	pRenderer->Set_FogColor(_float3(0.94921875f, 0.4296875f, 0.328125f));
	pRenderer->Set_FogStartDist(0.1f);
	pRenderer->Set_FogGlobalDensity(0.06f);
	pRenderer->Set_FogHeightFalloff(0.16f);


#endif
#endif // !_DEBUG

	return S_OK;
}

HRESULT CScene_Stage7::Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag )
{


	{

		CGameInstance* pInstance = g_pGameInstance;

		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/Trigger/";
		lstrcat(szFullPath, szTriggerDataName);


		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
		{
			__debugbreak();
			return E_FAIL;
		}

		DWORD	dwByte = 0;
		_int iIDLength = 0;




		while (true)
		{



			_uint eNumber = 0;
			_tchar eObjectID[MAX_PATH];
			_float4x4 WorldMat = XMMatrixIdentity();
			_float4x4 ValueData = XMMatrixIdentity();
			_float4x4 SubValueData = XMMatrixIdentity();

			ZeroMemory(eObjectID, sizeof(_tchar) * MAX_PATH);

			ReadFile(hFile, &(eNumber), sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &(eObjectID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);

			ReadFile(hFile, &(WorldMat), sizeof(_float4x4), &dwByte, nullptr);
			ReadFile(hFile, &(ValueData), sizeof(_float4x4), &dwByte, nullptr);
			ReadFile(hFile, &(SubValueData), sizeof(_float4x4), &dwByte, nullptr);
			if (0 == dwByte) break;



			FAILED_CHECK(pInstance->Add_GameObject_To_Layer(eSceneID, pLayerTag, eObjectID, &eNumber));

			CTriggerObject* pObject = (CTriggerObject*)(pInstance->Get_GameObject_By_LayerLastIndex(eSceneID, pLayerTag));

			NULL_CHECK_RETURN(pObject, E_FAIL);

			pObject->Set_eNumberNObjectID(eNumber, eObjectID);

			((CTransform*)pObject->Get_Component(TAG_COM(Com_Transform)))->Set_Matrix(WorldMat);

			pObject->Set_ValueMat(&ValueData);
			pObject->Set_SubValueMat(&SubValueData);

			pObject->After_Initialize();

		}

		CloseHandle(hFile);
	}








	return S_OK;
}

HRESULT CScene_Stage7::Ready_MonsterBatchTrigger(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag)
{


	//../bin/Resources/Data/ParicleData/TextureParticle/
	_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/MonsterBatchData/";
	lstrcat(szFullPath, szTriggerDataName);
	
	HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);
	
	
	if (INVALID_HANDLE_VALUE == hFile)
	{
		__debugbreak();
		return E_FAIL;
	}
	
	DWORD	dwByte = 0;
	
	_int iIDLength = 0;
	
	
	_float4x4 WorldMat = XMMatrixIdentity();
	_float4x4 ValueMat = XMMatrixIdentity();
	_tchar	 eObjectID[MAX_PATH] = L"";
	
	ReadFile(hFile, &(WorldMat), sizeof(_float4x4), &dwByte, nullptr);
	ReadFile(hFile, &(ValueMat), sizeof(_float4x4), &dwByte, nullptr);
	
	
	
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(eSceneID, pLayerTag,TAG_OP(Prototype_MonsterBatchTrigger)));
	CMonsterBatchTrigger* pMonsterBatchTrigger = (CMonsterBatchTrigger*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(eSceneID, pLayerTag));

	NULL_CHECK_RETURN(pMonsterBatchTrigger, E_FAIL);
	
	CTransform * pTrigTransform = (CTransform*)pMonsterBatchTrigger->Get_Component(TAG_COM(Com_Transform));
	
	//////////////////////////////////////////////////////////////////////////메트릭스 넣기
	
	pTrigTransform->Set_Matrix(WorldMat);
	pMonsterBatchTrigger->Set_ValueMat(&ValueMat);
	
	
	while (true)
	{
		ZeroMemory(eObjectID, sizeof(_tchar) * MAX_PATH);

		ReadFile(hFile, &(WorldMat), sizeof(_float4x4), &dwByte, nullptr);
		
		ReadFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &(eObjectID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);


		if (0 == dwByte) break;
		pMonsterBatchTrigger->Add_MonsterBatch(WorldMat, eObjectID);
	}
	
	
	CloseHandle(hFile);




	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_CameraTrigger(const _tchar * pLayerTag)
{
	//CTrigger_ChangeCameraView::CHANGECAMERAVIEWDESC ChangeCameraViewDesc;
	//ChangeCameraViewDesc.eChangeCameraViewType = CTrigger_ChangeCameraView::EChangeCameraViewType::TYPE_TWO_INTERP;
	//ChangeCameraViewDesc.fMain_CamPos = _float3(0.f, 6.f, -2.f);
	//ChangeCameraViewDesc.fMain_CamLook = _float3(0.f, -1.f, 1.f);
	//ChangeCameraViewDesc.fMain_Pos = _float3(5.f, 0.01f, 5.f);
	////ChangeCameraViewDesc.fMain_Scale = _float3(3.f, 3.f, 3.f); 
	// 
	//ChangeCameraViewDesc.fSub_CamPos = _float3(-2.f, 6.f, 0.f);
	//ChangeCameraViewDesc.fSub_CamLook = _float3(1.f, -1.f, 0.f);
	//ChangeCameraViewDesc.fSub_Pos = _float3(-10.f, 0.01f, 5.f);
	////ChangeCameraViewDesc.fSub_Scale = _float3(3.f, 3.f, 3.f);
	//ChangeCameraViewDesc.bLockCamLook = true;
	//ChangeCameraViewDesc.fMain_CamMoveWeight = 0.9f;
	//ChangeCameraViewDesc.fMain_CamLookWeight = 0.9f;
	//ChangeCameraViewDesc.fSub_CamMoveWeight = 0.5f;
	//ChangeCameraViewDesc.fSub_CamLookWeight = 0.5f;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Trigger_ChangeCameraView), &ChangeCameraViewDesc));



	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_InteractObject(const _tchar * pLayerTag)
{
	CElevator::ELEVATORDESC tElevatorDesc;
	tElevatorDesc.fStartPos = _float3(-0.032f, 58.1f, 145.368f);
	tElevatorDesc.fDestPos = _float3(-0.032f, 30.1f, 145.368f);
	tElevatorDesc.fRotation = _float3(0.f, XMConvertToRadians(90.f), 0.f);
	tElevatorDesc.fScale = _float3(0.05f);
	tElevatorDesc.fMoveSpeed = 5.f;
	tElevatorDesc.fColliderOffset_Y = 0.f;
	tElevatorDesc.iMeshType = 1;
	tElevatorDesc.fColliderScale = 2.f;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_InteractObj_Elevator), &tElevatorDesc));

	return S_OK;
}

HRESULT CScene_Stage7::Ready_MapData(const _tchar* szMapDataFileName, SCENEID eSceneID, const _tchar * pLayerTag)
{

	//../bin/Resources/Data/Map/
	_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/Map/";

	lstrcat(szFullPath, szMapDataFileName);



	CGameInstance* pInstance = g_pGameInstance;

	pInstance->Add_GameObject_To_Layer(eSceneID, pLayerTag, TAG_OP(Prototype_InstanceStaticMapObject));
	CStaticInstanceMapObject* pInstanceMapObject = (CStaticInstanceMapObject*)pInstance->Get_GameObject_By_LayerLastIndex(eSceneID, pLayerTag);
	NULL_CHECK_RETURN(pInstanceMapObject, E_FAIL);


	//HANDLE hFile = CreateFileW(szFullPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);


	HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;


	_uint iIDLength = 0;

	// 유니코드임을 알리는 BOM
	//DWORD wc = 0xFF;
	//ReadFile(hFile, &wc, 3, &dwByte, NULL);

	while (true)
	{


		/**/
		OBJELEMENT	tData{};
		_tchar szBuffer[MAX_PATH] = L"";
		// key 값 로드
		ReadFile(hFile, &(iIDLength), sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, (tData.ObjectID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);
		//lstrcpy(tData.ObjectID, szBuffer);

		ReadFile(hFile, &(iIDLength), sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, (tData.MeshID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);
		//lstrcpy(tData.MeshID, szBuffer);

		ReadFile(hFile, &(tData.PassIndex), sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &(tData.FrustumRange), sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &(tData.bIsOcllsuion), sizeof(_bool), &dwByte, nullptr);

		ReadFile(hFile, &(tData.matSRT.m[0][0]), sizeof(_float) * 16, &dwByte, nullptr);
		ReadFile(hFile, &(tData.matTransform.m[0][0]), sizeof(_float) * 16, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (!lstrcmp(L"Prototype_EditorCursor", tData.ObjectID)) continue;

		FAILED_CHECK(pInstanceMapObject->Add_InstanceMapObject(tData));



/*

		//객채 생성해주기
		//&(tData.pObject),
		pInstance->Add_GameObject_To_Layer(eSceneID, pLayerTag ,tData.ObjectID);

		CGameObject* pObject = pInstance->Get_GameObject_By_LayerLastIndex(eSceneID, pLayerTag);
		NULL_CHECK_RETURN(pObject, E_FAIL);


		if (lstrcmp(tData.MeshID, TAG_CP(Prototype_Mesh_None)))
		{
			//매쉬 바꿔주기 

			pObject->Change_Component_by_NewAssign(eSceneID, tData.MeshID, TAG_COM(Com_Model));

			((CMapObject*)pObject)->Set_FrustumSize(tData.FrustumRange);
			((CMapObject*)pObject)->Set_IsOcllusion(tData.bIsOcllsuion);
			((CMapObject*)pObject)->Set_PassIndex(tData.PassIndex);




			CDissolve::DISSOLVEDESC	tDissolveDesc;
			dw
			tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
			tDissolveDesc.pModel = (CModel*)pObject->Get_Component(TAG_COM(Com_Model));
			tDissolveDesc.pShader = (CShader*)pObject->Get_Component(TAG_COM(Com_Shader));
			tDissolveDesc.RampTextureIndex = 1;

			pObject->Change_Component_by_NewAssign(eSceneID, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), &tDissolveDesc);


		}

		//트렌스폼
		CTransform* pTrans = (CTransform*)(pObject->Get_Component(TAG_COM(Com_Transform)));
		NULL_CHECK_RETURN(pTrans, E_FAIL);
		//tData.matTransform._42 -= 20;
		pTrans->Set_Matrix(tData.matTransform);
*/

	}




	CloseHandle(hFile);





	return S_OK;
}

CScene_Stage7 * CScene_Stage7::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Stage7* pTempLoby = NEW CScene_Stage7(pDevice,pDeviceContext);

	if (FAILED(pTempLoby->Initialize())) 
	{
		Safe_Release(pTempLoby); 
		MSGBOX("Failed to Creating CScene_Stage7");
		return nullptr;
	}

	return pTempLoby;

}

void CScene_Stage7::Free()
{
	__super::Free();
}
