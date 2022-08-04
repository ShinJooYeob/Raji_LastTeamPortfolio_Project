#include "stdafx.h"
#include "Scene_Laboratory_Jino.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Player.h"
#include "Golu.h"
#include "DynamicPlatform.h"

CScene_Laboratory_Jino::CScene_Laboratory_Jino(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CScene_Laboratory_Jino::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pUtilMgr = GetSingle(CUtilityMgr);

	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_Objects(TAG_LAY(Layer_MapObject)));

	FAILED_CHECK(Ready_PostPorcessing());

	return S_OK;
}

_int CScene_Laboratory_Jino::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	if (m_iSceneStartChecker <= 2)
	{
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}

	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	_Vector vDir = XMVector3Normalize(XMVectorSetY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS), 10) - XMVectorSet(128.f, -64.f, 256.f, 0));
	g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, XMVectorSet(128.f, -64.f, 256.f, 0) + vDir * 330.f);

	return 0;
}

_int CScene_Laboratory_Jino::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_Laboratory_Jino::Render()
{
	if (__super::Render() < 0)
		return -1;
	
	return 0;
}

_int CScene_Laboratory_Jino::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_Laboratory_Jino::Ready_Light()
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

HRESULT CScene_Laboratory_Jino::Ready_Layer_MainCamera(const _tchar * pLayerTag)
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
		m_pMainCam->Set_NowSceneNum(SCENE_LABORATORY_JINO);
	}

	m_pMainCam->Set_TargetArmLength(15.f);
	m_pMainCam->Set_MaxTargetArmLength(15.f);
	m_pMainCam->Set_MinTargetArmLength(15.f);
	return S_OK;
}

HRESULT CScene_Laboratory_Jino::Ready_Layer_Player(const _tchar * pLayerTag)
{
	// Golu
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, TAG_LAY(Layer_NPC), TAG_OP(Prototype_NPC_Golu), &_float3(10.f, 0.f, 0.f)));
	CGameObject* pGolu = (CGolu*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_LABORATORY_JINO, TAG_LAY(Layer_NPC)));
	static_cast<CGolu*>(pGolu)->Set_State_FallingStart(g_fDeltaTime);


	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);
	m_pMainCam->Set_CameraInitState(XMVectorSet(29.8301334f, 43.5636597f, 56.5242004f, 1.f), XMVectorSet(0.242860109f, -0.570451736f, 0.784604549f, 0.f));

	// Player
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Player), &_float3(-10.f, 0.f, 0.f)));
	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_LABORATORY_JINO, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	static_cast<CPlayer*>(pPlayer)->Set_State_FallingStart(g_fDeltaTime);
	m_pPlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pPlayerTransform, E_FAIL);

	CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));
	PlayerNavi->FindCellIndex(m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));
	pPlayer->Update_AttachCamPos();

	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_RAJIGOLU_MINIGAME);
	m_pMainCam->Set_FocusTarget(pPlayer);
	m_pMainCam->Set_CameraInitState(XMVectorSet(29.8301334f, 43.5636597f, 56.5242004f, 1.f), XMVectorSet(0.242860109f, -0.570451736f, 0.784604549f, 0.f));
	pPlayer->Update_AttachCamPos();

	return S_OK;
}

HRESULT CScene_Laboratory_Jino::Ready_Layer_Objects(const _tchar * pLayerTag)
{
	CDynamicPlatform::DYNAMICPLATFORMDESC tDynamicPlatSpawnDesc;
	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, 0.f, 0.f);
	tDynamicPlatSpawnDesc.eOnTarget = CDynamicPlatform::EONTARGET::TARGET_RAJI;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-6.f, -10.f, 0.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, -20.f, 0.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, -30.f, 5.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));


	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, 0.f, 0.f);
	tDynamicPlatSpawnDesc.eOnTarget = CDynamicPlatform::EONTARGET::TARGET_GOLU;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(6.f, -10.f, 0.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, -20.f, 0.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, -30.f, -5.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	return S_OK;
}

HRESULT CScene_Laboratory_Jino::Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag)
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

HRESULT CScene_Laboratory_Jino::Ready_PostPorcessing()
{
#ifndef _DEBUG

	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(128.f, -64.f, 256.f));
	pLightDesc->vDiffuse = _float4(0.78125f, 0.78125f, 1.f, 1.f);
	pLightDesc->vAmbient = _float4(0.6640625f, 0.65625f, 1.f, 1.f);
	pLightDesc->vSpecular = _float4(0.234375f, 0.234375f, 0.234375f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	pRenderer->Set_ShadowIntensive(0.3f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(1.5f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	pRenderer->Set_DofLength(30.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	pRenderer->Set_FogStartDist(5.f);
	pRenderer->Set_FogGlobalDensity(0.1f);
	pRenderer->Set_FogHeightFalloff(0.1f);

	//POSTPROCESSING_GODRAY
	//POSTPROCESSING_LENSEFLARE
	//POSTPROCESSING_CAMMOTIONBLUR
#else
#ifdef DEBUGONSHADERSETTING

	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(128.f, -64.f, 256.f));
	pLightDesc->vDiffuse = _float4(0.78125f, 0.78125f, 1.f, 1.f);
	pLightDesc->vAmbient = _float4(0.6640625f, 0.65625f, 1.f, 1.f);
	pLightDesc->vSpecular = _float4(0.234375f, 0.234375f, 0.234375f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	pRenderer->Set_ShadowIntensive(0.3f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(1.5f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	pRenderer->Set_DofLength(30.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	pRenderer->Set_FogStartDist(5.f);
	pRenderer->Set_FogGlobalDensity(0.1f);
	pRenderer->Set_FogHeightFalloff(0.1f);
#endif

#endif // !_DEBUGee
	return S_OK;
}

CScene_Laboratory_Jino * CScene_Laboratory_Jino::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Laboratory_Jino* pTempLoby = NEW CScene_Laboratory_Jino(pDevice, pDeviceContext);

	if (FAILED(pTempLoby->Initialize()))
	{
		Safe_Release(pTempLoby);
		MSGBOX("Failed to Creating CScene_Laboratory_Jino");
		return nullptr;
	}

	return pTempLoby;
}

void CScene_Laboratory_Jino::Free()
{
	__super::Free();
}
