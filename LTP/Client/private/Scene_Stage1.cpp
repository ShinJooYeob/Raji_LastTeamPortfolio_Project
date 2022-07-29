#include "stdafx.h"
#include "..\Public\Scene_Stage1.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "TriggerObject.h"
#include "Player.h"
#include "StaticInstanceMapObject.h"
#include "MonsterBatchTrigger.h"

CScene_Stage1::CScene_Stage1(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice,pDeviceContext)
{
}





HRESULT CScene_Stage1::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pUtilMgr = GetSingle(CUtilityMgr);

	FAILED_CHECK(Ready_Light());

	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_SkyBox(TAG_LAY(Layer_SkyBox)));
	FAILED_CHECK(Ready_Layer_Terrain(TAG_LAY(Layer_Terrain)));
	
	FAILED_CHECK(Ready_MapData(L"Stage_1.dat", SCENE_STAGE1, TAG_LAY(Layer_StaticMapObj)));
	FAILED_CHECK(Ready_TriggerObject(L"Stage1Trigger.dat", SCENE_STAGE1, TAG_LAY(Layer_ColTrigger)));
			

	FAILED_CHECK(Ready_TriggerObject(L"TestMeshInstance2.dat", SCENE_STAGE1, TAG_LAY(Layer_ColTrigger)));

	FAILED_CHECK(Ready_TriggerObject(L"Stage1_0.dat", SCENE_STAGE1, TAG_LAY(Layer_ColTrigger)));

	//TestMeshInstance2
	//L"Stage1_Instance.dat"


	
	////_float Length = _float3(-64, 64, -64).Get_Distance(_float3(128.f, -128.f, -256.f).XMVector());
	//
	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage1_MonsterTrigger_1.dat", SCENE_STAGE1, TAG_LAY(Layer_BatchMonsterTrigger)));
	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage1_MonsterTrigger_2.dat", SCENE_STAGE1, TAG_LAY(Layer_BatchMonsterTrigger)));
	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage1_MonsterTrigger_3.dat", SCENE_STAGE1, TAG_LAY(Layer_BatchMonsterTrigger)));
	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage1_MonsterTrigger_4.dat", SCENE_STAGE1, TAG_LAY(Layer_BatchMonsterTrigger)));
	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage1_MonsterTrigger_5.dat", SCENE_STAGE1, TAG_LAY(Layer_BatchMonsterTrigger)));
	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage1_MonsterTrigger_6.dat", SCENE_STAGE1, TAG_LAY(Layer_BatchMonsterTrigger)));
	//

	FAILED_CHECK(Ready_PostPorcessing());
	return S_OK;
}

_int CScene_Stage1::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	if (g_pGameInstance->Get_DIKeyState(DIK_RETURN)&DIS_Down)
	{
		FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE2), SCENEID::SCENE_LOADING));
		return 0;
	}


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

_int CScene_Stage1::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	return 0;
}

_int CScene_Stage1::Render()
{
	if (__super::Render() < 0)
		return -1;
	if (m_fSceneStartTimer < 0.5f)
	{
		FAILED_CHECK(m_pUtilMgr->SCD_Rendering_Rolling(((_float)m_fSceneStartTimer), 0.5f, L"Target_ToonDeferredSceneChaging2"));
	}
	else if (m_fSceneStartTimer < 2.5f)
	{

		FAILED_CHECK(m_pUtilMgr->SCD_Rendering_FadeOut(((_float)m_fSceneStartTimer - 0.5f), 2.f, L"Target_ToonDeferredSceneChaging2"));
	}

	return 0;
}

_int CScene_Stage1::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

_int CScene_Stage1::Change_to_NextScene()
{

	FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, (SCENEID) m_eNextScene), SCENEID::SCENE_LOADING));


	return _int();

}



HRESULT CScene_Stage1::Ready_Light()
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

HRESULT CScene_Stage1::Ready_Layer_MainCamera(const _tchar * pLayerTag)
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
		m_pMainCam->Set_NowSceneNum(SCENE_STAGE1);
	}
	
	m_pMainCam->Set_TargetArmLength(8.f);

	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_Player(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TAG_OP(Prototype_Player), &_float3(31.773f, 37.5f, 64.801f)));
	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE1, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	static_cast<CPlayer*>(pPlayer)->Set_State_FirstStart();

	m_pPlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pPlayerTransform, E_FAIL);

	CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));
	PlayerNavi->FindCellIndex(m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));


	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);
	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
	m_pMainCam->Set_FocusTarget(pPlayer);
	m_pMainCam->Set_CameraInitState(XMVectorSet(29.8301334f, 43.5636597f, 56.5242004f, 1.f), XMVectorSet(0.242860109f, -0.570451736f, 0.784604549f, 0.f));
	pPlayer->Update_AttachCamPos();

	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE1, pLayerTag, TAG_OP(Prototype_SkyBox)));

	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE1, pLayerTag, TAG_OP(Prototype_Terrain)));



	return S_OK;
}

HRESULT CScene_Stage1::Ready_MapData(const _tchar * szMapDataFileName, SCENEID eSceneID, const _tchar * pLayerTag)
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

	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CScene_Stage1::Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag)
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

HRESULT CScene_Stage1::Ready_PostPorcessing()
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

#endif // !_DEBUG
	return S_OK;
}
	
HRESULT CScene_Stage1::Ready_MonsterBatchTrigger(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag)
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



	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(eSceneID, pLayerTag, TAG_OP(Prototype_MonsterBatchTrigger)));
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

CScene_Stage1 * CScene_Stage1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Stage1* pTempLoby = NEW CScene_Stage1(pDevice,pDeviceContext);

	if (FAILED(pTempLoby->Initialize())) 
	{
		Safe_Release(pTempLoby); 
		MSGBOX("Failed to Creating CScene_Stage1");
		return nullptr;
	}

	return pTempLoby;

}

void CScene_Stage1::Free()
{
	__super::Free();
}
