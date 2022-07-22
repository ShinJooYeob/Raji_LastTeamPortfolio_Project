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

CScene_Stage7::CScene_Stage7(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice,pDeviceContext)
{
}





HRESULT CScene_Stage7::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	FAILED_CHECK(Ready_Light());

	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_SkyBox(TAG_LAY(Layer_SkyBox)));
	FAILED_CHECK(Ready_Layer_Terrain(TAG_LAY(Layer_Terrain)));
	FAILED_CHECK(Ready_Layer_Monster(TAG_LAY(Layer_Monster)));
	//FAILED_CHECK(Ready_Layer_Boss(TAG_LAY(Layer_Boss)));
	

	FAILED_CHECK(Ready_MapData(L"Stage_1.dat", SCENE_STAGE7, TAG_LAY(Layer_StaticMapObj)));
	FAILED_CHECK(Ready_TriggerObject(L"Stage1Trigger.dat",   SCENE_STAGE7, TAG_LAY(Layer_ColTrigger)));

	//FAILED_CHECK(Ready_MonsterBatchTrigger(L"JinhoBabo.dat", SCENE_STAGE7, TAG_LAY(Layer_BatchMonsterTrigger)));
	
	

	//FAILED_CHECK(teST());
	
	return S_OK;
}

_int CScene_Stage7::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (g_pGameInstance->Get_DIKeyState(DIK_N)&DIS_Down)
	{


		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, TAG_LAY(Layer_Monster), TAG_OP(Prototype_Object_Monster_Wasp)));
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, TAG_LAY(Layer_Monster), TAG_OP(Prototype_Object_Monster_Wormgrub)));
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, TAG_LAY(Layer_Monster), TAG_OP(Prototype_Object_Monster_Spider)));
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, TAG_LAY(Layer_Monster), TAG_OP(Prototype_Object_Monster_Wolf)));
	}

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

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
}

_int CScene_Stage7::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

_int CScene_Stage7::Change_to_NextScene()
{

	FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, (SCENEID)m_eNextScene), SCENEID::SCENE_LOADING));

	return _int();
}



HRESULT CScene_Stage7::Ready_Light()
{

	//const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);

	//if (pLightDesc == nullptr)
	//{

	//	LIGHTDESC LightDesc;

	//	LightDesc.eLightType = tagLightDesc::TYPE_DIRECTIONAL;
	//	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//	LightDesc.vAmbient = _float4(1.0f);
	//	LightDesc.vSpecular = _float4(1);
	//	LightDesc.vVector = DefalutSunPosition;

	//	g_pGameInstance->Add_Light(LightDesc);
	//}
	//else
	//{
	//	g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, DefalutSunPosition.XMVector());
	//}


	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_MainCamera(const _tchar * pLayerTag)
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
		m_pMainCam->Set_NowSceneNum(SCENE_STAGE7);
	}
	
	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_Player(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Player)));

	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE7, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	CTransform* PlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
	CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));

	//static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(30.f, 37.460f, 60.f));
	//static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(157.422f, 23.7f, 75.991f));
	static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(216.357f, 29.2f, 185.583f));
	//static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(242.479f, 21.198f, 178.668f));
	//static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(280.874f, 22.85f, 179.978f));
	//static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(296.946f, 38.713f, 214.058f));

	PlayerNavi->FindCellIndex(PlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));
	
	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));


	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);

	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
	m_pMainCam->Set_FocusTarget(pPlayer);
	m_pMainCam->Set_TargetArmLength(0.f);

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_StaticMapObject)));

	//CTransform* pTransform = (CTransform*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE7, pLayerTag)->Get_Component(TAG_COM(Com_Transform)));

	//NULL_CHECK_RETURN(pTransform, E_FAIL);


	//_Matrix tt = XMMatrixScaling(20, 1, 20) * XMMatrixTranslation(0, -2, 0);

	//pTransform->Set_Matrix(tt);

	//((CMapObject*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE7, pLayerTag))->Set_FrustumSize(99999999.f);



	//CTestLedgeTrigger::LEDGETRIGGERDESC tLedgeTriggerDesc;
	//tLedgeTriggerDesc.fSpawnPos = _float3(242.284f, 27.910f, 183.879f);
	//tLedgeTriggerDesc.eLedgeTriggerState = CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Trigger_TestLedgeTrigger), &tLedgeTriggerDesc));
	//CTestLedgeTrigger* pTrigger = (CTestLedgeTrigger*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENE_STAGE7, TAG_LAY(Layer_Player)));
	//pTrigger->Set_Pos(_float3(242.f, 28.f, 181.f));
	//pTrigger->Set_LookDir(_float3(0.f, 0.f, 1.f));
	//pTrigger->Set_LedgeType(CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE);

	//tLedgeTriggerDesc.fSpawnPos = _float3(242.284f, 27.910f, 183.879f);
	//tLedgeTriggerDesc.eLedgeTriggerState = CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Trigger_TestLedgeTrigger), &tLedgeTriggerDesc));
	//pTrigger = (CTestLedgeTrigger*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENE_STAGE7, TAG_LAY(Layer_Player)));
	//pTrigger->Set_Pos(_float3(241.95f, 25.f, 181.65f));
	//pTrigger->Set_LookDir(_float3(0.f, 0.f, 1.f));
	//pTrigger->Set_LedgeType(CTestLedgeTrigger::ELedgeTriggerState::STATE_LEDGE);

	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_SkyBox)));

	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE3, pLayerTag, TAG_OP(Prototype_Terrain)));



	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Minion)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Leader)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Minion)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Leader)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Ninjasura_Minion)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Ninjasura)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Black)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Rage)));
	
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Rage_Hollogram)));



	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Tezabsura_Minion)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Tezabsura_Purple)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Tezabsura_Bomber)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Tezabsura_Landmine)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Jalsura)));

	//Mesh Instance
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Wasp)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Wormgrub)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Wolf)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Spider)));



	return S_OK;
}

HRESULT CScene_Stage7::Ready_Layer_Boss(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Boss_Chiedtian)));
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

HRESULT CScene_Stage7::teST()
{
	vector<wstring> wwString;
	vector<wstring> wwPro;


	{
		wwString.push_back(L"ENV_BLD_Palace_02.FBX");
		wwString.push_back(L"ENV_BLD_Palace_05.fbx");
		wwString.push_back(L"ENV_BLD_Palace_08.fbx");
		wwString.push_back(L"ENV_BLD_Palace_11.fbx");
		wwString.push_back(L"ENV_BLD_Palace_16.fbx");
		wwString.push_back(L"ENV_BLD_Window_Pane_02.fbx");
		wwString.push_back(L"ENV_F_DecorativePillar_02.fbx");
		wwString.push_back(L"F_Vinestest.fbx");
		wwString.push_back(L"PillarPlatforming.fbx");
		wwString.push_back(L"PillarPlatforming_DM.fbx");
		wwString.push_back(L"SM_Arrow.fbx");
		wwString.push_back(L"SM_Arrow1.fbx");
		wwString.push_back(L"SM_BG_Mountain1.fbx");
		wwString.push_back(L"SM_BG_Mountain2.fbx");
		wwString.push_back(L"SM_BG_Mountain3.fbx");
		wwString.push_back(L"SM_BLD_Arch_Base.fbx");
		wwString.push_back(L"SM_BLD_Arch_Stairs.fbx");
		wwString.push_back(L"SM_BLD_ArchStairs_03.fbx");
		wwString.push_back(L"SM_BLD_BridgeMiddle01.fbx");
		wwString.push_back(L"SM_BLD_BridgeMiddle03.fbx");
		wwString.push_back(L"SM_BLD_BrokenArch_01.fbx");
		wwString.push_back(L"SM_BLD_BrokenArch_03.fbx");
		wwString.push_back(L"SM_BLD_BrokenArch_04.fbx");
		wwString.push_back(L"SM_BLD_CC_TemplePainting_01.fbx");
		wwString.push_back(L"SM_BLD_CC_TemplePainting_03.fbx");
		wwString.push_back(L"SM_BLD_CC_TemplePainting_04.fbx");
		wwString.push_back(L"SM_BLD_CombatArena01.fbx");
		wwString.push_back(L"SM_BLD_DB_WoodenPlank.fbx");
		wwString.push_back(L"SM_BLD_DBPillar01.fbx");
		wwString.push_back(L"SM_BLD_DBPillar02.fbx");
		wwString.push_back(L"SM_BLD_DBPillar04.fbx");
		wwString.push_back(L"SM_BLD_DBSculpture.fbx");
		wwString.push_back(L"SM_BLD_DiyaBridge.fbx");
		wwString.push_back(L"SM_BLD_Drawbridge.fbx");
		wwString.push_back(L"SM_BLD_DS_BrokenArch_01.fbx");
		wwString.push_back(L"SM_BLD_DS_BrokenArch_02.fbx");
		wwString.push_back(L"SM_BLD_DS_BrokenArch_03.fbx");
		wwString.push_back(L"SM_BLD_DS_SpiralPlatform_01.fbx");
		wwString.push_back(L"SM_BLD_F_Column_B_02.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Arch_01.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Block_01.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Block_02.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Column_01.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Column_02.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Column_03.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Column_04.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Door_02.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Door_Arch_01.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_STAIRS_01.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_TILE_01.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_WALL_01.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Window_LG_01.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Window_LG_02.fbx");
		wwString.push_back(L"SM_BLD_F_Palace_Window_SM_01.fbx");
		wwString.push_back(L"SM_BLD_Lion_Statue.fbx");
		wwString.push_back(L"SM_BLD_StraightWall02.fbx");
		wwString.push_back(L"SM_BLD_StraightWall06.fbx");
		wwString.push_back(L"SM_BLD_SV_PalaceBig_02.fbx");
		wwString.push_back(L"SM_BLD_Temple_Arch.fbx");
		wwString.push_back(L"SM_BLD_TempleEnt.fbx");
		wwString.push_back(L"SM_BLD_Tower02.fbx");
		wwString.push_back(L"SM_BLD_TrishulGrant03.fbx");
		wwString.push_back(L"SM_BouVines_01.fbx");
		wwString.push_back(L"SM_boxbush_01.fbx");
		wwString.push_back(L"SM_Branch_Face_01.fbx");
		wwString.push_back(L"SM_Branch_Face_02.fbx");
		wwString.push_back(L"SM_Branch_LG_01.fbx");
		wwString.push_back(L"SM_Branch_LG_02.fbx");
		wwString.push_back(L"SM_Branch_LG_03.fbx");
		wwString.push_back(L"SM_Branch_LG_04.fbx");
		wwString.push_back(L"SM_Branch_LG_05.fbx");
		wwString.push_back(L"SM_Branch_Piece_02.fbx");
		wwString.push_back(L"SM_Branch_SM_01.fbx");
		wwString.push_back(L"SM_bri_decal_08.fbx");
		wwString.push_back(L"SM_Bridge_Chainhook.fbx");
		wwString.push_back(L"SM_Broken_Branch_01.fbx");
		wwString.push_back(L"SM_Broken_Branch_02.fbx");
		wwString.push_back(L"SM_Broken_Branch_03.fbx");
		wwString.push_back(L"SM_Broken_Branch_04.fbx");
		wwString.push_back(L"SM_CC_archi_merg_01.fbx");
		wwString.push_back(L"SM_CC_archi_merg_02.fbx");
		wwString.push_back(L"SM_CC_straightboxwall_01.fbx");
		wwString.push_back(L"SM_CC_straightboxwall_02.fbx");
		wwString.push_back(L"SM_CH_Gadasura_Black_Mask_01.fbx");
		wwString.push_back(L"SM_CH_Gadasura_Rage_Mask_01.fbx");
		wwString.push_back(L"SM_CH_Mahinasura_Leader_Mask_01.fbx");
		wwString.push_back(L"SM_CH_Mahinasura_Leader_Stinger_01.fbx");
		wwString.push_back(L"SM_CH_Mahinasura_Leader_Tail_01.fbx");
		wwString.push_back(L"SM_CH_Mahinasura_Minion_Mask_01.fbx");
		wwString.push_back(L"SM_CH_Tezabsura_Kamikaze_Mask_01.fbx");
		wwString.push_back(L"SM_CH_Tezabsura_Landmine_Mask_01.fbx");
		wwString.push_back(L"SM_CH_Vayusura_Mask_01.fbx");
		wwString.push_back(L"SM_CH_Vayusura_Mask_02.fbx");
		wwString.push_back(L"SM_circular_plane.fbx");
		wwString.push_back(L"SM_Crack_Decal_Ground_01.fbx");
		wwString.push_back(L"SM_Crack_Decal_Ground_03.fbx");
		wwString.push_back(L"SM_Crack_Decal_Ground_05.fbx");
		wwString.push_back(L"SM_Crack_Decal_Ground_06.fbx");
		wwString.push_back(L"SM_D_BLD_PalaceArch_01.fbx");
		wwString.push_back(L"SM_D_BLD_PalaceCOL_01.fbx");
		wwString.push_back(L"SM_D_BLD_PalaceCOL_02.fbx");
		wwString.push_back(L"SM_D_BLD_PalaceENT_01.fbx");
		wwString.push_back(L"SM_D_BLD_PalaceRFCORNER_01.fbx");
		wwString.push_back(L"SM_D_BLD_PalaceRFLEDGE_01.fbx");
		wwString.push_back(L"SM_D_BLD_PalaceRFSEG_01.fbx");
		wwString.push_back(L"SM_D_BLD_PalaceWIN_01.fbx");
		wwString.push_back(L"SM_Damage_Decal_01.fbx");
		wwString.push_back(L"SM_Damage_Decal_02.fbx");
		wwString.push_back(L"SM_Damage_Decal_03.fbx");
		wwString.push_back(L"SM_Debris_01.fbx");
		wwString.push_back(L"SM_Debris_02.fbx");
		wwString.push_back(L"SM_Debris_03.fbx");
		wwString.push_back(L"SM_Debris_04.fbx");
		wwString.push_back(L"SM_Demon_Tree_Seg_01.fbx");
		wwString.push_back(L"SM_Demon_Tree_Seg_02.fbx");
		wwString.push_back(L"SM_Demon_Tree_Seg_03.fbx");
		wwString.push_back(L"SM_Demon_Tree_Seg_04.fbx");
		wwString.push_back(L"SM_Demon_Tree_Seg_05.fbx");
		wwString.push_back(L"SM_DemonT_Creeper_01.fbx");
		wwString.push_back(L"SM_DemonT_Creeper_02.fbx");
		wwString.push_back(L"SM_DemonT_Creeper_03.fbx");
		wwString.push_back(L"SM_DiyaSmall_01.fbx");
		wwString.push_back(L"SM_Dry_Green_Bush_01.fbx");
		wwString.push_back(L"SM_Dry_Green_Bush_02.fbx");
		wwString.push_back(L"SM_Dry_Green_Bush_03.fbx");
		wwString.push_back(L"SM_Dry_Stack_01.fbx");
		wwString.push_back(L"SM_Dry_Stack_02.fbx");
		wwString.push_back(L"SM_Dry_Stack_03.fbx");
		wwString.push_back(L"SM_DryLeaves_Clutter_01.fbx");
		wwString.push_back(L"SM_DryLeaves_Clutter_02.fbx");
		wwString.push_back(L"SM_DryLeaves_Clutter_03.fbx");
		wwString.push_back(L"SM_DryLeaves_Clutter_04.fbx");
		wwString.push_back(L"SM_DryLeaves_Clutter_05.fbx");
		wwString.push_back(L"SM_Dune_01.fbx");
		wwString.push_back(L"SM_Dune_02.fbx");
		wwString.push_back(L"SM_Dune_03.fbx");
		wwString.push_back(L"SM_ENV_BLD_TopFloor_Wall_01.fbx");
		wwString.push_back(L"SM_ENV_BLD_TopFloor_Wall_02.fbx");
		wwString.push_back(L"SM_ENV_CC_Arch_B.fbx");
		wwString.push_back(L"SM_ENV_CC_Arch_Base_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Arch_C.fbx");
		wwString.push_back(L"SM_ENV_CC_Arch_D.fbx");
		wwString.push_back(L"SM_ENV_CC_Boat_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Border_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Border_02.fbx");
		wwString.push_back(L"SM_ENV_CC_Border_03.fbx");
		wwString.push_back(L"SM_ENV_CC_Border_05.fbx");
		wwString.push_back(L"SM_ENV_CC_Bridge_Arch.fbx");
		wwString.push_back(L"SM_ENV_CC_Bridge_Pillar_03.fbx");
		wwString.push_back(L"SM_ENV_CC_CogLarge_01.fbx");
		wwString.push_back(L"SM_ENV_CC_CogLarge_02.fbx");
		wwString.push_back(L"SM_ENV_CC_CogLarge_03.fbx");
		wwString.push_back(L"SM_ENV_CC_CogLarge_04.fbx");
		wwString.push_back(L"SM_ENV_CC_CogMedium_01.fbx");
		wwString.push_back(L"SM_ENV_CC_CogMedium_02.fbx");
		wwString.push_back(L"SM_ENV_CC_CogMedium_03.fbx");
		wwString.push_back(L"SM_ENV_CC_CogMedium_04.fbx");
		wwString.push_back(L"SM_ENV_CC_CogMedium_05.fbx");
		wwString.push_back(L"SM_ENV_CC_CogSmall_01.fbx");
		wwString.push_back(L"SM_ENV_CC_CogTiny_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Diya_02.fbx");
		wwString.push_back(L"SM_ENV_CC_DockPlatform_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Dome_A.fbx");
		wwString.push_back(L"SM_ENV_CC_Dome_B.fbx");
		wwString.push_back(L"SM_ENV_CC_Dome_C.fbx");
		wwString.push_back(L"SM_ENV_CC_Dome_D.fbx");
		wwString.push_back(L"SM_ENV_CC_Dome_E.fbx");
		wwString.push_back(L"SM_ENV_CC_DoorWay_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Elevator_Horizontal_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Elevator_Platform_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Elevator_Platform_02.fbx");
		wwString.push_back(L"SM_ENV_CC_Elevator_Vertical_01.fbx");
		wwString.push_back(L"SM_ENV_CC_ElevatorHorizontal_CogTR_01.fbx");
		wwString.push_back(L"SM_ENV_CC_ElevatorHousing_01.fbx");
		wwString.push_back(L"SM_ENV_CC_ElevatorLever_01.fbx");
		wwString.push_back(L"SM_ENV_CC_ElevatorLeverBase_01.fbx");
		wwString.push_back(L"SM_ENV_CC_ElevatorRailing_Horizontal_01.fbx");
		wwString.push_back(L"SM_ENV_CC_ElevatorRailing_Vertical_01.fbx");
		wwString.push_back(L"SM_ENV_CC_ElevatorVertical_Cog_Bottom_01.fbx");
		wwString.push_back(L"SM_ENV_CC_ElevatorVertical_Cog_Top_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Garuda_Gate.fbx");
		wwString.push_back(L"SM_ENV_CC_Garuda_Tower.fbx");
		wwString.push_back(L"SM_ENV_CC_Garuda_Tower_Balcony.fbx");
		wwString.push_back(L"SM_ENV_CC_MandalaCircle_01.fbx");
		wwString.push_back(L"SM_ENV_CC_MandalaCircle_02.fbx");
		wwString.push_back(L"SM_ENV_CC_MandalaCircle_03.fbx");
		wwString.push_back(L"SM_ENV_CC_MandalaCircle_04.fbx");
		wwString.push_back(L"SM_ENV_CC_PaddyField_01.fbx");
		wwString.push_back(L"SM_ENV_CC_PaddyField_02.fbx");
		wwString.push_back(L"SM_ENV_CC_PaddyField_03.fbx");
		wwString.push_back(L"SM_ENV_CC_PaddyField_04.fbx");
		wwString.push_back(L"SM_ENV_CC_PaddyField_05.fbx");
		wwString.push_back(L"SM_ENV_CC_PaddyField_06.fbx");
		wwString.push_back(L"SM_ENV_CC_PaddyField_07.fbx");
		wwString.push_back(L"SM_ENV_CC_Panel_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Panel_03.fbx");
		wwString.push_back(L"SM_ENV_CC_Panel_04.fbx");
		wwString.push_back(L"SM_ENV_CC_Panel_05.fbx");
		wwString.push_back(L"SM_ENV_CC_Pillar_03.fbx");
		wwString.push_back(L"SM_ENV_CC_Pillar_05.fbx");
		wwString.push_back(L"SM_ENV_CC_Pillar_043.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_02.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_03.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_04.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_06.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_054_SM_ENV_CC_Platform_05.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_Circular_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_Circular_02.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_Circular_low.fbx");
		wwString.push_back(L"SM_ENV_CC_Platform_Circular_low01.fbx");
		wwString.push_back(L"SM_ENV_CC_Puzzle_Water.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_01.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_02.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_03.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_04.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_05.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_08.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_10.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_11.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_12.fbx");
		wwString.push_back(L"SM_ENV_CC_PuzzleAssembly_Right_15.fbx");
		wwString.push_back(L"SM_ENV_CC_Railing_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Railing_02.fbx");
		wwString.push_back(L"SM_ENV_CC_Railing_03.fbx");
		wwString.push_back(L"SM_ENV_CC_RangdaArena_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Stairs_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Stairs_02.fbx");
		wwString.push_back(L"SM_ENV_CC_Stairs_Railing.fbx");
		wwString.push_back(L"SM_ENV_CC_Statue.fbx");
		wwString.push_back(L"SM_ENV_CC_StoryPanel_02.fbx");
		wwString.push_back(L"SM_ENV_CC_StraightWall_01.fbx");
		wwString.push_back(L"SM_ENV_CC_StraightWall_02.fbx");
		wwString.push_back(L"SM_ENV_CC_StraightWall_02_SM_ENV_CC_StraightWall_02.fbx");
		wwString.push_back(L"SM_ENV_CC_StraightWall_03_SM_ENV_CC_StraightWall_03.fbx");
		wwString.push_back(L"SM_ENV_CC_StraightWall_04.fbx");
		wwString.push_back(L"SM_ENV_CC_StraightWall_Withwindow_SM_ENV_CC_StraightWall_WithWindow.fbx");
		wwString.push_back(L"SM_ENV_CC_StraighWall_Withwindow1.fbx");
		wwString.push_back(L"SM_ENV_CC_TempleBase_01.fbx");
		wwString.push_back(L"SM_ENV_CC_TempleTop_01.fbx");
		wwString.push_back(L"SM_ENV_CC_TempleWalls_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Vishnu_Statue.fbx");
		wwString.push_back(L"SM_ENV_CC_VishnuTemple_01.fbx");
		wwString.push_back(L"SM_ENV_CC_VishnuTemple_Base_01.fbx");
		wwString.push_back(L"SM_ENV_CC_VishnuTemple_DoorAssembly_Left.fbx");
		wwString.push_back(L"SM_ENV_CC_VishnuTemple_DoorAssembly_Right.fbx");
		wwString.push_back(L"SM_ENV_CC_Water_Platform_01.fbx");
		wwString.push_back(L"SM_ENV_CC_Water_Platform_02a.fbx");
		wwString.push_back(L"SM_ENV_CC_Water_Platform_03.fbx");
		wwString.push_back(L"SM_ENV_CC_Water_Platform_04.fbx");
		wwString.push_back(L"SM_ENV_CC_Water_Platform_06.fbx");
		wwString.push_back(L"SM_ENV_CC_Water_Platform_07.fbx");
		wwString.push_back(L"SM_ENV_CC_Water_Platform_10.fbx");
		wwString.push_back(L"SM_ENV_CC_WaterFountain_01.fbx");
		wwString.push_back(L"SM_ENV_CC_WaterFountain_02.fbx");
		wwString.push_back(L"SM_ENV_CC_WaterPuzzle_Base.fbx");
		wwString.push_back(L"SM_ENV_CC_WaterPuzzle_Boundary_01.fbx");
		wwString.push_back(L"SM_ENV_CC_WaterPuzzle_Boundary_02.fbx");
		wwString.push_back(L"SM_ENV_CC_WaterWheel_01.fbx");
		wwString.push_back(L"SM_ENV_CC_WaterWheel_02.fbx");
		wwString.push_back(L"SM_ENV_CC_Window_A.fbx");
		wwString.push_back(L"SM_ENV_CC_Window_A1.fbx");
		wwString.push_back(L"SM_ENV_CC_Window_Ba.fbx");
		wwString.push_back(L"SM_ENV_CC_Window_D.fbx");
		wwString.push_back(L"SM_ENV_CC_Window1.fbx");
		wwString.push_back(L"SM_ENV_CC_Wooden_Planks_03.fbx");
		wwString.push_back(L"SM_ENV_CC_Wooden_Planks_08.fbx");
		wwString.push_back(L"SM_ENV_D_BasePlatform_01.fbx");
		wwString.push_back(L"SM_ENV_D_BossArenaArch_01.fbx");
		wwString.push_back(L"SM_ENV_D_BossArenaArch_02.fbx");
		wwString.push_back(L"SM_ENV_D_CenterPlatform_01.fbx");
		wwString.push_back(L"SM_ENV_D_ExitDoor_01.fbx");
		wwString.push_back(L"SM_ENV_D_ExitPlatformTop_01.fbx");
		wwString.push_back(L"SM_ENV_D_FemaleStatue_01.fbx");
		wwString.push_back(L"SM_ENV_D_FemaleStatue_02.fbx");
		wwString.push_back(L"SM_ENV_D_FrontPlatformBottom_01.fbx");
		wwString.push_back(L"SM_ENV_D_FrontPlatformTop_01.fbx");
		wwString.push_back(L"SM_ENV_D_GodHand_01.fbx");
		wwString.push_back(L"SM_ENV_D_GroundArch_01.fbx");
		wwString.push_back(L"SM_ENV_D_GroundColumn_01.fbx");
		wwString.push_back(L"SM_ENV_D_GroundFloorPlatform_01.fbx");
		wwString.push_back(L"SM_ENV_D_GroundFloorPlatform_02.fbx");
		wwString.push_back(L"SM_ENV_D_MandalaCircle_01.fbx");
		wwString.push_back(L"SM_ENV_D_MandalaCircle_02.fbx");
		wwString.push_back(L"SM_ENV_D_MandalaCircle_03.fbx");
		wwString.push_back(L"SM_ENV_D_MandalaCircle_04.fbx");
		wwString.push_back(L"SM_ENV_D_MandalaCircle_05.fbx");
		wwString.push_back(L"SM_ENV_D_OuterWall_01.fbx");
		wwString.push_back(L"SM_ENV_D_OuterWall_02.fbx");
		wwString.push_back(L"SM_ENV_D_OuterWall_03.fbx");
		wwString.push_back(L"SM_ENV_D_OuterWall_04.fbx");
		wwString.push_back(L"SM_ENV_D_OuterWall_05.fbx");
		wwString.push_back(L"SM_ENV_D_OuterWall_06.fbx");
		wwString.push_back(L"SM_ENV_D_OuterWall_07.fbx");
		wwString.push_back(L"SM_ENV_D_OuterWall_08.fbx");
		wwString.push_back(L"SM_ENV_D_Roof_01.fbx");
		wwString.push_back(L"SM_ENV_D_SidePlatformTop_01.fbx");
		wwString.push_back(L"SM_ENV_D_SoldierStatue_01.fbx");
		wwString.push_back(L"SM_ENV_D_SoldierStatue_02.fbx");
		wwString.push_back(L"SM_ENV_D_StepwellStairs_01.fbx");
		wwString.push_back(L"SM_ENV_D_StepwellStairs_Half_01.fbx");
		wwString.push_back(L"SM_ENV_D_StepwellWater_01.fbx");
		wwString.push_back(L"SM_ENV_D_TombBase_01.fbx");
		wwString.push_back(L"SM_ENV_D_Window_01.fbx");
		wwString.push_back(L"SM_ENV_DS_Platform_28.fbx");
		wwString.push_back(L"SM_ENV_DT_BackdropTemple_01.fbx");
		wwString.push_back(L"SM_ENV_DT_BellSpline_01.fbx");
		wwString.push_back(L"SM_ENV_DT_Bull.fbx");
		wwString.push_back(L"SM_ENV_DT_BullPlatform_01.fbx");
		wwString.push_back(L"SM_ENV_DT_BullPlatform_Broken_01.fbx");
		wwString.push_back(L"SM_ENV_DT_Diya_02_01.fbx");
		wwString.push_back(L"SM_ENV_DT_DurgaBase_01.fbx");
		wwString.push_back(L"SM_ENV_DT_DurgaBase_02.fbx");
		wwString.push_back(L"SM_ENV_DT_DurgaBaseStep_01.fbx");
		wwString.push_back(L"SM_ENV_DT_DurgaBaseStep_02.fbx");
		wwString.push_back(L"SM_ENV_DT_LionEntrance_01.fbx");
		wwString.push_back(L"SM_ENV_DT_LionEntranceSplit_01.fbx");
		wwString.push_back(L"SM_ENV_DT_Mountain_02.fbx");
		wwString.push_back(L"SM_ENV_DT_Mountain_06.fbx");
		wwString.push_back(L"SM_ENV_DT_Mountain_08.fbx");
		wwString.push_back(L"SM_ENV_DT_PillarLarge_01.fbx");
		wwString.push_back(L"SM_ENV_DT_PillarLarge_02.fbx");
		wwString.push_back(L"SM_ENV_DT_Platform01.fbx");
		wwString.push_back(L"SM_ENV_DT_Platform02_2.fbx");
		wwString.push_back(L"SM_ENV_DT_Platform03.fbx");
		wwString.push_back(L"SM_ENV_DT_Platform04_2.fbx");
		wwString.push_back(L"SM_ENV_DT_Platform05.fbx");
		wwString.push_back(L"SM_ENV_DT_Platform06_Base.fbx");
		wwString.push_back(L"SM_ENV_DT_Platform06_StatueBase.fbx");
		wwString.push_back(L"SM_ENV_DT_Stairs_01_2.fbx");
		wwString.push_back(L"SM_ENV_DT_StatueBase_02.fbx");
		wwString.push_back(L"SM_ENV_DT_StatueBase_03.fbx");
		wwString.push_back(L"SM_ENV_DT_StatueFemale_Pose01.fbx");
		wwString.push_back(L"SM_ENV_DT_StatueFemale_Pose02.fbx");
		wwString.push_back(L"SM_ENV_DT_StatueFemale_Pose03.fbx");
		wwString.push_back(L"SM_ENV_DT_StatueFemale_Pose04.fbx");
		wwString.push_back(L"SM_ENV_DT_StatuePillar_01.fbx");
		wwString.push_back(L"SM_ENV_DT_StatuePillar_02.fbx");
		wwString.push_back(L"SM_ENV_DT_TempleBase_01.fbx");
		wwString.push_back(L"SM_ENV_DT_TempleBase_02.fbx");
		wwString.push_back(L"SM_ENV_DT_TempleBell_03.fbx");
		wwString.push_back(L"SM_ENV_F_3_Decorative_Piece_01.fbx");
		wwString.push_back(L"SM_ENV_F_3_Window_02.fbx");
		wwString.push_back(L"SM_ENV_F_3_Window_04.fbx");
		wwString.push_back(L"SM_ENV_F_3_Window_06.fbx");
		wwString.push_back(L"SM_ENV_F_3_WindowArch_01.fbx");
		wwString.push_back(L"SM_ENV_F_3_WindowArch_02.fbx");
		wwString.push_back(L"SM_ENV_F_Arch_01.fbx");
		wwString.push_back(L"SM_ENV_F_Arch_02.fbx");
		wwString.push_back(L"SM_ENV_F_Balcony_01.fbx");
		wwString.push_back(L"SM_ENV_F_Balcony_1B.fbx");
		wwString.push_back(L"SM_ENV_F_Balcony_1C.fbx");
		wwString.push_back(L"SM_ENV_F_Balcony_1D.fbx");
		wwString.push_back(L"SM_ENV_F_Balcony_1E.fbx");
		wwString.push_back(L"SM_ENV_F_Balcony_02.fbx");
		wwString.push_back(L"SM_ENV_F_Balcony_03.fbx");
		wwString.push_back(L"SM_ENV_F_Bow_Platform_03.fbx");
		wwString.push_back(L"SM_ENV_F_Bow_Platform_04.fbx");
		wwString.push_back(L"SM_ENV_F_Building_01.fbx");
		wwString.push_back(L"SM_ENV_F_Chatri_Base.fbx");
		wwString.push_back(L"SM_ENV_F_Corner_Wall_01.fbx");
		wwString.push_back(L"SM_ENV_F_Corner_Wall_02.fbx");
		wwString.push_back(L"SM_ENV_F_Corner_Wall_05.fbx");
		wwString.push_back(L"SM_ENV_F_Defences_01.fbx");
		wwString.push_back(L"SM_ENV_F_Defences_02.fbx");
		wwString.push_back(L"SM_ENV_F_Defences_02B.fbx");
		wwString.push_back(L"SM_ENV_F_Dome_05.fbx");
		wwString.push_back(L"SM_ENV_F_Dome_06.fbx");
		wwString.push_back(L"SM_ENV_F_Dome_08.fbx");
		wwString.push_back(L"SM_ENV_F_Dome_09.fbx");
		wwString.push_back(L"SM_ENV_F_Dome_014.fbx");
		wwString.push_back(L"SM_ENV_F_Door_Damaged_S_01.fbx");
		wwString.push_back(L"SM_ENV_F_Door_Damaged_S_02.fbx");
		wwString.push_back(L"SM_ENV_F_Door_NS_01.fbx");
		wwString.push_back(L"SM_ENV_F_Door_NS_02.fbx");
		wwString.push_back(L"SM_ENV_F_Door_S_01.fbx");
		wwString.push_back(L"SM_ENV_F_Door_S_02.fbx");
		wwString.push_back(L"SM_ENV_F_DoorWay_01.fbx");
		wwString.push_back(L"SM_ENV_F_DoorWayAbandoned_02.fbx");
		wwString.push_back(L"SM_ENV_F_DurgaAvatar_Temple_01.fbx");
		wwString.push_back(L"SM_ENV_F_DurgaAvatarTemple_Floor_01.fbx");
		wwString.push_back(L"SM_ENV_F_FirstFloorEntrance_01.fbx");
		wwString.push_back(L"SM_ENV_F_FirstFloorTile_01.fbx");
		wwString.push_back(L"SM_ENV_F_FirstFloorTile_02.fbx");
		wwString.push_back(L"SM_ENV_F_FirstFloorTile_03.fbx");
		wwString.push_back(L"SM_ENV_F_FirstFloorTile_04.fbx");
		wwString.push_back(L"SM_ENV_F_FirstFloorTile_05.fbx");
		wwString.push_back(L"SM_ENV_F_FirstFloorTile_06.fbx");
		wwString.push_back(L"SM_ENV_F_FloatingWall_01.fbx");
		wwString.push_back(L"SM_ENV_F_FloatingWall_02.fbx");
		wwString.push_back(L"SM_ENV_F_FloatingWall_03.fbx");
		wwString.push_back(L"SM_ENV_F_FloatingWall_Trim_07.fbx");
		wwString.push_back(L"SM_ENV_F_Floor_01_2.fbx");
		wwString.push_back(L"SM_ENV_F_Fort_Pathway_01.fbx");
		wwString.push_back(L"SM_ENV_F_Fort_Pathway_02.fbx");
		wwString.push_back(L"SM_ENV_F_Fort_Pathway_03.fbx");
		wwString.push_back(L"SM_ENV_F_Fort_Pathway_04.fbx");
		wwString.push_back(L"SM_ENV_F_FortBridge_Broken_01.fbx");
		wwString.push_back(L"SM_ENV_F_FortBridge_Broken_02.fbx");
		wwString.push_back(L"SM_ENV_F_FortBridge_Broken_03.fbx");
		wwString.push_back(L"SM_ENV_F_FortBridge_Broken_04.fbx");
		wwString.push_back(L"SM_ENV_F_FortCobbleArena_01.fbx");
		wwString.push_back(L"SM_ENV_F_FortWALL_01b.fbx");
		wwString.push_back(L"SM_ENV_F_FortWALL_01d.fbx");
		wwString.push_back(L"SM_ENV_F_FortWALL_05.fbx");
		wwString.push_back(L"SM_ENV_F_Frame_01_a_int.fbx");
		wwString.push_back(L"SM_ENV_F_Frame_01_int.fbx");
		wwString.push_back(L"SM_ENV_F_FrameClosed_01.fbx");
		wwString.push_back(L"SM_ENV_F_HighWall_01.fbx");
		wwString.push_back(L"SM_ENV_F_Mandala02_01.fbx");
		wwString.push_back(L"SM_ENV_F_Mandala02_02.fbx");
		wwString.push_back(L"SM_ENV_F_Mandala02_03.fbx");
		wwString.push_back(L"SM_ENV_F_Mandala02_04.fbx");
		wwString.push_back(L"SM_ENV_F_Mandala02_05.fbx");
		wwString.push_back(L"SM_ENV_F_MandalaCircle_01.fbx");
		wwString.push_back(L"SM_ENV_F_MandalaCircle_02.fbx");
		wwString.push_back(L"SM_ENV_F_MandalaCircle_03.fbx");
		wwString.push_back(L"SM_ENV_F_MandalaCircle_04.fbx");
		wwString.push_back(L"SM_ENV_F_MandalaCircle_05.fbx");
		wwString.push_back(L"SM_ENV_F_Mystic_Creeper_06.fbx");
		wwString.push_back(L"SM_ENV_F_Mystic_Creeper_07.fbx");
		wwString.push_back(L"SM_ENV_F_Mystic_Creeper_09.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_02.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_03.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_05.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_07.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_08.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_09A.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_09B.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_09C.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_09D.fbx");
		wwString.push_back(L"SM_ENV_F_PlatFORM_09E.fbx");
		wwString.push_back(L"SM_ENV_F_Platform_Wall_01.fbx");
		wwString.push_back(L"SM_ENV_F_Rani_Platform_05.fbx");
		wwString.push_back(L"SM_ENV_F_Roof_01.fbx");
		wwString.push_back(L"SM_ENV_F_RoofWall_Assembled_01.fbx");
		wwString.push_back(L"SM_ENV_F_RoofWall_Assembled_02a.fbx");
		wwString.push_back(L"SM_ENV_F_RoofWall_Single.fbx");
		wwString.push_back(L"SM_ENV_F_RoofWall_Single_02.fbx");
		wwString.push_back(L"SM_ENV_F_Stairs_02.fbx");
		wwString.push_back(L"SM_ENV_F_Stairs_04.fbx");
		wwString.push_back(L"SM_ENV_F_Stairs_Railing_01.fbx");
		wwString.push_back(L"SM_ENV_F_StatueBase_03.fbx");
		wwString.push_back(L"SM_ENV_F_StoryPanel_01.fbx");
		wwString.push_back(L"SM_ENV_F_Tez_Balcony.fbx");
		wwString.push_back(L"SM_ENV_F_Tower_01.fbx");
		wwString.push_back(L"SM_ENV_F_Tower_02.fbx");
		wwString.push_back(L"SM_ENV_F_Tower_06.fbx");
		wwString.push_back(L"SM_ENV_F_Tower_07.fbx");
		wwString.push_back(L"SM_ENV_F_Tower_08.fbx");
		wwString.push_back(L"SM_ENV_F_ViewPlatform_01.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_01.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Corner_Holes_02.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Holes_01.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Holes_02.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Holes_Half_01.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Holes_Turret_01.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Holes_Turret_02.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Solid_01.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Solid_02.fbx");
		wwString.push_back(L"SM_ENV_F_Wall_Step_01.fbx");
		wwString.push_back(L"SM_ENV_F_WallTrim_02.fbx");
		wwString.push_back(L"SM_ENV_F_Window_Pane_White.fbx");
		wwString.push_back(L"SM_ENV_FemaleStatue_Pose_04.fbx");
		wwString.push_back(L"SM_ENV_FemaleStatue_Pose_06.fbx");
		wwString.push_back(L"SM_ENV_PRP_PillarStatuesBroken_01.fbx");
		wwString.push_back(L"SM_ENV_PRP_PillarStatuesBroken_02.fbx");
		wwString.push_back(L"SM_ENV_PRP_PillarStatuesBroken_04.fbx");
		wwString.push_back(L"SM_ENV_PRP_PillarStatuesBroken_05.fbx");
		wwString.push_back(L"SM_ENV_PRP_PillarStatuesBroken_06.fbx");
		wwString.push_back(L"SM_ENV_T_Creeper_01.fbx");
		wwString.push_back(L"SM_ENV_T_DemonT_Roots_04.fbx");
		wwString.push_back(L"SM_ENV_T_DemonT_Roots_05.fbx");
		wwString.push_back(L"SM_ENV_T_HangingPlant_01.fbx");
		wwString.push_back(L"SM_ENV_T_TetrisBase_01.fbx");
		wwString.push_back(L"SM_ENV_T_TetrisBase_Trim_01.fbx");
		wwString.push_back(L"SM_ENV_WallBasic_01.fbx");
		wwString.push_back(L"SM_ENV_WallBasic_02.fbx");
		wwString.push_back(L"SM_ENV_WallBasic_04.fbx");
		wwString.push_back(L"SM_ENV_WallBasic_05.fbx");
		wwString.push_back(L"SM_ENV_WallBasic_08.fbx");
		wwString.push_back(L"SM_ENV_WallBasic_09.fbx");
		wwString.push_back(L"SM_ENV_WallBasic_11.fbx");
		wwString.push_back(L"SM_ENV_Window_A_RED_BLOCK_01.fbx");
		wwString.push_back(L"SM_F_ArchIvy_01.fbx");
		wwString.push_back(L"SM_F_ArchIvy_03.fbx");
		wwString.push_back(L"SM_F_BalconyPlant01.fbx");
		wwString.push_back(L"SM_F_BalconyPlant03.fbx");
		wwString.push_back(L"SM_F_Creepers_V01.fbx");
		wwString.push_back(L"SM_F_Flower02.fbx");
		wwString.push_back(L"SM_F_HangingIvy_01.fbx");
		wwString.push_back(L"SM_F_Mystic_Bridge_Creeper_01.fbx");
		wwString.push_back(L"SM_F_Mystic_Bridge_Creeper_02.fbx");
		wwString.push_back(L"SM_F_Mystic_Bridge_Creeper_03.fbx");
		wwString.push_back(L"SM_F_Mystic_Cliff_Creeper_01.fbx");
		wwString.push_back(L"SM_F_Mystic_Cliff_Creeper_02.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_01.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_02.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_03.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_05.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_06.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_07.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_08.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_09.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_10.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_11.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_12.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_13.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_14.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_15.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_16.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_17.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_18.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_19.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_20.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_21.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_22.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_24.fbx");
		wwString.push_back(L"SM_F_Mystic_Creeper_25.fbx");
		wwString.push_back(L"SM_F_Mystic_MergedRoots_04.fbx");
		wwString.push_back(L"SM_F_Mystic_Roots_02.fbx");
		wwString.push_back(L"SM_F_Mystic_S_Creeper_01.fbx");
		wwString.push_back(L"SM_F_Mystic_S_Creeper_02.fbx");
		wwString.push_back(L"SM_F_Mystic_Shiv_Creeper_01.fbx");
		wwString.push_back(L"SM_F_Mystic_Snake_Creeper_0.fbx");
		wwString.push_back(L"SM_F_Mystic_Snake_Creeper_01.fbx");
		wwString.push_back(L"SM_F_Mystic_Snake_Creeper_03.fbx");
		wwString.push_back(L"SM_F_Mystic_Snake_Creeper_05.fbx");
		wwString.push_back(L"SM_F_Mystic_Snake_Creeper_06.fbx");
		wwString.push_back(L"SM_F_Mystic_Snake_Creeper_07.fbx");
		wwString.push_back(L"SM_F_Mystics_Creeper_04.fbx");
		wwString.push_back(L"SM_F_Mystics_Creeper_05.fbx");
		wwString.push_back(L"SM_F_Mystics_MergedRoots_01.fbx");
		wwString.push_back(L"SM_F_Mystics_MergedRoots_02.fbx");
		wwString.push_back(L"SM_F_Mystics_MergedRoots_03.fbx");
		wwString.push_back(L"SM_F_Mystics_MergedRoots_05.fbx");
		wwString.push_back(L"SM_F_Mystics_MergedRoots_06.fbx");
		wwString.push_back(L"SM_F_Mystics_Roots_01.fbx");
		wwString.push_back(L"SM_F_PurpleFlower_Fort_01.fbx");
		wwString.push_back(L"SM_F_PurpleFlower_Fort_02.fbx");
		wwString.push_back(L"SM_F_PurpleFlower_Opt02.fbx");
		wwString.push_back(L"SM_F_PurpleFlowerOpt_01.fbx");
		wwString.push_back(L"SM_F_PurpleFlowerOpt_04.fbx");
		wwString.push_back(L"SM_F_Roots_01.fbx");
		wwString.push_back(L"SM_F_Roots_03.fbx");
		wwString.push_back(L"SM_F_Roots_05.fbx");
		wwString.push_back(L"SM_F_Roots01.fbx");
		wwString.push_back(L"SM_F_Statue_Creeper_01.fbx");
		wwString.push_back(L"SM_F_Statue_Creeper_02.fbx");
		wwString.push_back(L"SM_F_Statue_Creeper_03.fbx");
		wwString.push_back(L"SM_F_Statue_Creeper_04.fbx");
		wwString.push_back(L"SM_F_Vine06.fbx");
		wwString.push_back(L"SM_F_Vine07.fbx");
		wwString.push_back(L"SM_F_Vine08.fbx");
		wwString.push_back(L"SM_F_Vine09.fbx");
		wwString.push_back(L"SM_F_Vine10.fbx");
		wwString.push_back(L"SM_F_Vine12.fbx");
		wwString.push_back(L"SM_F_Vine14.fbx");
		wwString.push_back(L"SM_F_Vine15.fbx");
		wwString.push_back(L"SM_F_Vine16.fbx");
		wwString.push_back(L"SM_F_Vine18.fbx");
		wwString.push_back(L"SM_F_Vine19.fbx");
		wwString.push_back(L"SM_F_Vine22.fbx");
		wwString.push_back(L"SM_F_Vine23.fbx");
		wwString.push_back(L"SM_F_Vine24.fbx");
		wwString.push_back(L"SM_F_Vine25.fbx");
		wwString.push_back(L"SM_F_Vine26.fbx");
		wwString.push_back(L"SM_F_Vine27.fbx");
		wwString.push_back(L"SM_F_Vine28.fbx");
		wwString.push_back(L"SM_F_Vine31.fbx");
		wwString.push_back(L"SM_F_Vine32.fbx");
		wwString.push_back(L"SM_F_Vine33.fbx");
		wwString.push_back(L"SM_F_Vine36.fbx");
		wwString.push_back(L"SM_F_Vines40.fbx");
		wwString.push_back(L"SM_Fern_01.fbx");
		wwString.push_back(L"SM_Fern_02.fbx");
		wwString.push_back(L"SM_Fern_03.fbx");
		wwString.push_back(L"SM_FlowerBush_01.fbx");
		wwString.push_back(L"SM_FlowerBush_02.fbx");
		wwString.push_back(L"SM_FlowerBush_03.fbx");
		wwString.push_back(L"SM_FLowerTest_06.fbx");
		wwString.push_back(L"SM_FlowerVine_01.fbx");
		wwString.push_back(L"SM_FlowerVine01.fbx");
		wwString.push_back(L"SM_FlowerVines_02.fbx");
		wwString.push_back(L"SM_FlowerVines_03.fbx");
		wwString.push_back(L"SM_FlowerVines_04.fbx");
		wwString.push_back(L"SM_FlowerVines_05.fbx");
		wwString.push_back(L"SM_FlowerVines_07.fbx");
		wwString.push_back(L"SM_FlowerVines_08.fbx");
		wwString.push_back(L"SM_FlowerVines_09.fbx");
		wwString.push_back(L"SM_Fort_Bow_Creeper_0.fbx");
		wwString.push_back(L"SM_Fort_Bow_Creeper_03.fbx");
		wwString.push_back(L"SM_Fort_Bow_Creeper_04.fbx");
		wwString.push_back(L"SM_Fort_Courtyard_02.fbx");
		wwString.push_back(L"SM_Fort_Courtyard_Creeper_03.fbx");
		wwString.push_back(L"SM_Fort_Courtyard_Creeper_04.fbx");
		wwString.push_back(L"SM_Fort_Creeper_01.fbx");
		wwString.push_back(L"SM_Fort_Creeper_02.fbx");
		wwString.push_back(L"SM_Fort_Creeper_03.fbx");
		wwString.push_back(L"SM_Fort_Creeper_04.fbx");
		wwString.push_back(L"SM_Fort_Creeper_05.fbx");
		wwString.push_back(L"SM_Fort_Creeper_06.fbx");
		wwString.push_back(L"SM_Fort_Creeper_07.fbx");
		wwString.push_back(L"SM_Fort_Creeper_08.fbx");
		wwString.push_back(L"SM_Fort_Creeper_10.fbx");
		wwString.push_back(L"SM_Fort_Creeper_11.fbx");
		wwString.push_back(L"SM_Fort_Creeper_12.fbx");
		wwString.push_back(L"SM_Fort_Creeper_13.fbx");
		wwString.push_back(L"SM_Fort_Creeper_15.fbx");
		wwString.push_back(L"SM_Fort_Creeper_16.fbx");
		wwString.push_back(L"SM_Fort_Creeper_17.fbx");
		wwString.push_back(L"SM_Fort_Creeper_18.fbx");
		wwString.push_back(L"SM_Fort_Creeper_19.fbx");
		wwString.push_back(L"SM_Fort_Creeper_20.fbx");
		wwString.push_back(L"SM_Fort_Creeper_21.fbx");
		wwString.push_back(L"SM_Fort_Creeper_22.fbx");
		wwString.push_back(L"SM_Fort_Creeper_23.fbx");
		wwString.push_back(L"SM_Fort_Creeper_24.fbx");
		wwString.push_back(L"SM_Fort_Creeper_25.fbx");
		wwString.push_back(L"SM_Fort_Creeper_26.fbx");
		wwString.push_back(L"SM_Fort_Creeper_27.fbx");
		wwString.push_back(L"SM_Fort_Creeper_28.fbx");
		wwString.push_back(L"SM_Fort_Creeper_29.fbx");
		wwString.push_back(L"SM_Fort_Creeper_30.fbx");
		wwString.push_back(L"SM_Fort_Creeper_32.fbx");
		wwString.push_back(L"SM_Fort_Creeper_33.fbx");
		wwString.push_back(L"SM_Fort_Creeper_34.fbx");
		wwString.push_back(L"SM_Fort_RedCreeper_01.fbx");
		wwString.push_back(L"SM_Fort_RedCreeper_02.fbx");
		wwString.push_back(L"SM_Fort_RedCreeper_03.fbx");
		wwString.push_back(L"SM_Fort_RedCreeper_04.fbx");
		wwString.push_back(L"SM_Fort_WhiteFlower_01.fbx");
		wwString.push_back(L"SM_Fortwall_blocker_01.fbx");
		wwString.push_back(L"SM_Fortwall_blocker_02.fbx");
		wwString.push_back(L"SM_Fortwall_blocker_03.fbx");
		wwString.push_back(L"SM_Fortwall_blocker_04.fbx");
		wwString.push_back(L"SM_Fortwall_blocker_05.fbx");
		wwString.push_back(L"SM_Fortwall_blocker_06.fbx");
		wwString.push_back(L"SM_Fortwall_Cover_01.fbx");
		wwString.push_back(L"SM_FortWall_Market_01.fbx");
		wwString.push_back(L"SM_FortWall_Market_02.fbx");
		wwString.push_back(L"SM_FortWall_Market_03.fbx");
		wwString.push_back(L"SM_FortWall_Market_04.fbx");
		wwString.push_back(L"SM_FortWall_Market_05.fbx");
		wwString.push_back(L"SM_FortWall_Market_06.fbx");
		wwString.push_back(L"SM_FortWall_Market_07.fbx");
		wwString.push_back(L"SM_FortWall_Market_08.fbx");
		wwString.push_back(L"SM_FortWall_Market_09.fbx");
		wwString.push_back(L"SM_FortWall_Market_10.fbx");
		wwString.push_back(L"SM_FortWall_Market_11.fbx");
		wwString.push_back(L"SM_FortWall_Market_12.fbx");
		wwString.push_back(L"SM_Fortwall_Market2_01.fbx");
		wwString.push_back(L"SM_FortWall_side_01.fbx");
		wwString.push_back(L"SM_FortWall_side_02.fbx");
		wwString.push_back(L"SM_Gadasura_Head_03.fbx");
		wwString.push_back(L"SM_GEN_Tree_01.fbx");
		wwString.push_back(L"SM_GEN_Tree_02.fbx");
		wwString.push_back(L"SM_Grass_01A.fbx");
		wwString.push_back(L"SM_Grass_01A1.fbx");
		wwString.push_back(L"SM_Green_Stack_01.fbx");
		wwString.push_back(L"SM_GRND_Bush_01.fbx");
		wwString.push_back(L"SM_GRND_Bush_02.fbx");
		wwString.push_back(L"SM_GRND_Bush_03.fbx");
		wwString.push_back(L"SM_GRND_Bush_05.fbx");
		wwString.push_back(L"SM_GRND_PLANT_01.fbx");
		wwString.push_back(L"SM_GRND_Plant_CVR_01.fbx");
		wwString.push_back(L"SM_GroundClutter_01.fbx");
		wwString.push_back(L"SM_GroundScatter_01A.fbx");
		wwString.push_back(L"SM_GroundScatter_02A.fbx");
		wwString.push_back(L"SM_GroundScatter_03A.fbx");
		wwString.push_back(L"SM_GroundScatter_03B.fbx");
		wwString.push_back(L"SM_Hedgerow_01.fbx");
		wwString.push_back(L"SM_Hedgerow_01_A.fbx");
		wwString.push_back(L"SM_Hedgerow_Corner_01.fbx");
		wwString.push_back(L"SM_lillypad_01.fbx");
		wwString.push_back(L"SM_lillypad_02.fbx");
		wwString.push_back(L"SM_Lillypad_02_A.fbx");
		wwString.push_back(L"SM_Lillypad_02_B.fbx");
		wwString.push_back(L"SM_Lillypad_02_C.fbx");
		wwString.push_back(L"SM_Lillypad_02_D.fbx");
		wwString.push_back(L"SM_lillypad_03.fbx");
		wwString.push_back(L"SM_lillypad_04.fbx");
		wwString.push_back(L"SM_lillypad_05.fbx");
		wwString.push_back(L"SM_lillypad_06.fbx");
		wwString.push_back(L"SM_Lotus_01.fbx");
		wwString.push_back(L"SM_Lotus_01_flower.fbx");
		wwString.push_back(L"SM_Lotus_01_stem.fbx");
		wwString.push_back(L"SM_Lotus_bud_01.fbx");
		wwString.push_back(L"SM_Lotus_bud_main_01.fbx");
		wwString.push_back(L"SM_Lotus_Pink.fbx");
		wwString.push_back(L"SM_MERGED_PRESET_Ivy1_BP_6.fbx");
		wwString.push_back(L"SM_MERGED_PRESET_Liana1_BP_11.fbx");
		wwString.push_back(L"SM_MERGED_PRESET_Liana1_BP_14.fbx");
		wwString.push_back(L"SM_MERGED_PRESET_Liana1_BP2.fbx");
		wwString.push_back(L"SM_MFort_Creeper_07.fbx");
		wwString.push_back(L"SM_Mossplant.fbx");
		wwString.push_back(L"SM_Mystic_Bush_01.fbx");
		wwString.push_back(L"SM_Mystic_Bush_01A.fbx");
		wwString.push_back(L"SM_Mystic_Bush_02A.fbx");
		wwString.push_back(L"SM_Mystic_Roots_Hanging_01.fbx");
		wwString.push_back(L"SM_Mystic_Roots_Hanging_02.fbx");
		wwString.push_back(L"SM_Mystic_Roots_Hanging_03.fbx");
		wwString.push_back(L"SM_Ninja_Run_Pose.fbx");
		wwString.push_back(L"SM_palmbush_01.fbx");
		wwString.push_back(L"SM_palmbush_02.fbx");
		wwString.push_back(L"SM_Pinkflower_vine_01.fbx");
		wwString.push_back(L"SM_Pinkvine_wall_01.fbx");
		wwString.push_back(L"SM_Pinkvine_wall_01a.fbx");
		wwString.push_back(L"SM_PRP_Arrow_01.fbx");
		wwString.push_back(L"SM_PRP_Banner_01_Static.fbx");
		wwString.push_back(L"SM_PRP_Banner_05_Static.fbx");
		wwString.push_back(L"SM_PRP_Bow_01.fbx");
		wwString.push_back(L"SM_PRP_BrokenPillars_01.fbx");
		wwString.push_back(L"SM_PRP_BrokenPillars_06.fbx");
		wwString.push_back(L"SM_PRP_BrokenPillars_07.fbx");
		wwString.push_back(L"SM_PRP_BrokenWall03.fbx");
		wwString.push_back(L"SM_PRP_CC_Anvil_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Barrel_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Barrel_02.fbx");
		wwString.push_back(L"SM_PRP_CC_BarrelGroup_01.fbx");
		wwString.push_back(L"SM_PRP_CC_BarrelGroup_04.fbx");
		wwString.push_back(L"SM_PRP_CC_BarrelGroup_06.fbx");
		wwString.push_back(L"SM_PRP_CC_BarrelGroup_07.fbx");
		wwString.push_back(L"SM_PRP_CC_BarrelGroup_08.fbx");
		wwString.push_back(L"SM_PRP_CC_BoatRound_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Bucket_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Bucket_02.fbx");
		wwString.push_back(L"SM_PRP_CC_Cauldron_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Cauldron_02.fbx");
		wwString.push_back(L"SM_PRP_CC_CauldronLarge_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Chair_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Chisel_02.fbx");
		wwString.push_back(L"SM_PRP_CC_Chisel_05.fbx");
		wwString.push_back(L"SM_PRP_CC_ClaypotGroup_01.fbx");
		wwString.push_back(L"SM_PRP_CC_ClaypotGroup_03.fbx");
		wwString.push_back(L"SM_PRP_CC_ClaypotGroup_04.fbx");
		wwString.push_back(L"SM_PRP_CC_ClaypotGroup_05.fbx");
		wwString.push_back(L"SM_PRP_CC_Cushion_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Cushion_02.fbx");
		wwString.push_back(L"SM_PRP_CC_Cushion_03.fbx");
		wwString.push_back(L"SM_PRP_CC_DockPlanks_01.fbx");
		wwString.push_back(L"SM_PRP_CC_DockPlanks_02.fbx");
		wwString.push_back(L"SM_PRP_CC_DockPlanks_03.fbx");
		wwString.push_back(L"SM_PRP_CC_DockPlanks_04.fbx");
		wwString.push_back(L"SM_PRP_CC_DockPlanks_05.fbx");
		wwString.push_back(L"SM_PRP_CC_DockPlanks_06.fbx");
		wwString.push_back(L"SM_PRP_CC_DockPlanks_07.fbx");
		wwString.push_back(L"SM_PRP_CC_DockStairs_01.fbx");
		wwString.push_back(L"SM_PRP_CC_DockStepWall_01.fbx");
		wwString.push_back(L"SM_PRP_CC_ElephantHead_01.fbx");
		wwString.push_back(L"SM_PRP_CC_FishCrate_01.fbx");
		wwString.push_back(L"SM_PRP_CC_FishCrate_02.fbx");
		wwString.push_back(L"SM_PRP_CC_FishCrate_03.fbx");
		wwString.push_back(L"SM_PRP_CC_FishCrate_04.fbx");
		wwString.push_back(L"SM_PRP_CC_FishCrate_05.fbx");
		wwString.push_back(L"SM_PRP_CC_FishCrate_06.fbx");
		wwString.push_back(L"SM_PRP_CC_GlassPanel_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Grinder_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Hammer_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Jar_02.fbx");
		wwString.push_back(L"SM_PRP_CC_Ladle_01.fbx");
		wwString.push_back(L"SM_PRP_CC_LampPillar_01.fbx");
		wwString.push_back(L"SM_PRP_CC_LanternCotton_01.fbx");
		wwString.push_back(L"SM_PRP_CC_LanternCotton_02.fbx");
		wwString.push_back(L"SM_PRP_CC_LanternMetal_01.fbx");
		wwString.push_back(L"SM_PRP_CC_LanternMetal_02.fbx");
		wwString.push_back(L"SM_PRP_CC_LeverBase_02.fbx");
		wwString.push_back(L"SM_PRP_CC_Pier_01.fbx");
		wwString.push_back(L"SM_PRP_CC_PierCorner_01.fbx");
		wwString.push_back(L"SM_PRP_CC_PierCorner_02.fbx");
		wwString.push_back(L"SM_PRP_CC_PierFenceEnd_01.fbx");
		wwString.push_back(L"SM_PRP_CC_PierFenceEnd_02.fbx");
		wwString.push_back(L"SM_PRP_CC_PierFenceStart_01.fbx");
		wwString.push_back(L"SM_PRP_CC_PierFenceStart_02.fbx");
		wwString.push_back(L"SM_PRP_CC_PierNarrow_01.fbx");
		wwString.push_back(L"SM_PRP_CC_PierNarrow_02.fbx");
		wwString.push_back(L"SM_PRP_CC_PierNarrow_03.fbx");
		wwString.push_back(L"SM_PRP_CC_PierNarrow_04.fbx");
		wwString.push_back(L"SM_PRP_CC_PierWide_01.fbx");
		wwString.push_back(L"SM_PRP_CC_PierWide_02.fbx");
		wwString.push_back(L"SM_PRP_CC_PierWide_03.fbx");
		wwString.push_back(L"SM_PRP_CC_PierWide_04.fbx");
		wwString.push_back(L"SM_PRP_CC_RedCarpet_01.fbx");
		wwString.push_back(L"SM_PRP_CC_RedCarpet_02.fbx");
		wwString.push_back(L"SM_PRP_CC_RedCarpet_03.fbx");
		wwString.push_back(L"SM_PRP_CC_SackGroup_01.fbx");
		wwString.push_back(L"SM_PRP_CC_SackGroup_02.fbx");
		wwString.push_back(L"SM_PRP_CC_SackGroup_03.fbx");
		wwString.push_back(L"SM_PRP_CC_SackGroup_04.fbx");
		wwString.push_back(L"SM_PRP_CC_SackGroup_05.fbx");
		wwString.push_back(L"SM_PRP_CC_SackGroup_06.fbx");
		wwString.push_back(L"SM_PRP_CC_ShieldStandGroup_01.fbx");
		wwString.push_back(L"SM_PRP_CC_ShieldStandGroup_02.fbx");
		wwString.push_back(L"SM_PRP_CC_ShieldStandGroup_03.fbx");
		wwString.push_back(L"SM_PRP_CC_ShieldStandGroup_04.fbx");
		wwString.push_back(L"SM_PRP_CC_Soldier_01.fbx");
		wwString.push_back(L"SM_PRP_CC_Soldier_02.fbx");
		wwString.push_back(L"SM_PRP_CC_SpearStandGroup_02.fbx");
		wwString.push_back(L"SM_PRP_CC_SquareLantern_01.fbx");
		wwString.push_back(L"SM_PRP_CC_SquareLantern_02.fbx");
		wwString.push_back(L"SM_PRP_CC_SquareLantern_03.fbx");
		wwString.push_back(L"SM_PRP_CC_Stool_01.fbx");
		wwString.push_back(L"SM_PRP_CC_SwordStandGroup_01.fbx");
		wwString.push_back(L"SM_PRP_CC_SwordStandGroup_02.fbx");
		wwString.push_back(L"SM_PRP_CC_SwordStandGroup_04.fbx");
		wwString.push_back(L"SM_PRP_CC_SwordStandGroup_05.fbx");
		wwString.push_back(L"SM_PRP_CC_Table_01.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_01.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_02.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_03.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_04.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_05.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_06.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_07.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_08.fbx");
		wwString.push_back(L"SM_PRP_CC_TableGroup_09.fbx");
		wwString.push_back(L"SM_PRP_CC_TableSmall_01.fbx");
		wwString.push_back(L"SM_PRP_CC_ToolsGroup_01.fbx");
		wwString.push_back(L"SM_PRP_CC_ToolsGroup_02.fbx");
		wwString.push_back(L"SM_PRP_CC_ToolsGroup_03.fbx");
		wwString.push_back(L"SM_PRP_CC_ToolsGroup_04.fbx");
		wwString.push_back(L"SM_PRP_CC_ToolsGroup_05.fbx");
		wwString.push_back(L"SM_PRP_CC_ToolsGroup_06.fbx");
		wwString.push_back(L"SM_PRP_CC_ToolsGroup_07.fbx");
		wwString.push_back(L"SM_PRP_CC_TreeBase_01.fbx");
		wwString.push_back(L"SM_PRP_CC_WoodBox_01.fbx");
		wwString.push_back(L"SM_PRP_CC_WoodBoxGroup_01.fbx");
		wwString.push_back(L"SM_PRP_CC_WoodBoxGroup_02.fbx");
		wwString.push_back(L"SM_PRP_CC_WoodBoxGroup_03.fbx");
		wwString.push_back(L"SM_PRP_CC_WoodBoxGroup_04.fbx");
		wwString.push_back(L"SM_PRP_CC_WoodBoxGroup_05.fbx");
		wwString.push_back(L"SM_PRP_CC_WoodBoxGroup_06.fbx");
		wwString.push_back(L"SM_PRP_CC_WoodBoxGroup_07.fbx");
		wwString.push_back(L"SM_PRP_ChainLink_01.fbx");
		wwString.push_back(L"SM_PRP_Curtain_01.fbx");
		wwString.push_back(L"SM_PRP_Curtain_02.fbx");
		wwString.push_back(L"SM_PRP_Curtain_03.fbx");
		wwString.push_back(L"SM_PRP_Curtain_04.fbx");
		wwString.push_back(L"SM_PRP_D_Ribbon_Extension.fbx");
		wwString.push_back(L"SM_PRP_D_RibbonLarge_01.fbx");
		wwString.push_back(L"SM_PRP_D_RibbonLarge_02.fbx");
		wwString.push_back(L"SM_PRP_D_RibbonLarge_03.fbx");
		wwString.push_back(L"SM_PRP_D_RibbonSmall_01.fbx");
		wwString.push_back(L"SM_PRP_D_StoneStep_01.fbx");
		wwString.push_back(L"SM_PRP_D_StoneStep_02.fbx");
		wwString.push_back(L"SM_PRP_D_TreePedestal_01.fbx");
		wwString.push_back(L"SM_PRP_D_WillowTREE_01.fbx");
		wwString.push_back(L"SM_PRP_DBPully.fbx");
		wwString.push_back(L"SM_PRP_DBPully_DM.fbx");
		wwString.push_back(L"SM_PRP_DBPully_Handle_.fbx");
		wwString.push_back(L"SM_PRP_DeadSoldier_02.fbx");
		wwString.push_back(L"SM_PRP_DeadSoldier_03.fbx");
		wwString.push_back(L"SM_PRP_DeadSoldier_04.fbx");
		wwString.push_back(L"SM_PRP_DeadSoldier_05.fbx");
		wwString.push_back(L"SM_PRP_DeadSoldier_06.fbx");
		wwString.push_back(L"SM_PRP_DeadSoldier_07.fbx");
		wwString.push_back(L"SM_PRP_DeadSoldier_08.fbx");
		wwString.push_back(L"SM_PRP_DeadSoldier_09.fbx");
		wwString.push_back(L"SM_PRP_DT_Arm_left_01.fbx");
		wwString.push_back(L"SM_PRP_DT_Arm_left_02.fbx");
		wwString.push_back(L"SM_PRP_DT_Arm_Right_01.fbx");
		wwString.push_back(L"SM_PRP_DT_Arm_Right_02.fbx");
		wwString.push_back(L"SM_PRP_DT_Durga_Statue_01.fbx");
		wwString.push_back(L"SM_PRP_DT_ElephantHead_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Bell_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Bow_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Chakra_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Drum_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Gada_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Lotus_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Shield_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Sword_01.fbx");
		wwString.push_back(L"SM_PRP_DT_WP_Trishul_01.fbx");
		wwString.push_back(L"SM_PRP_F_DoorLedge_01.fbx");
		wwString.push_back(L"SM_PRP_F_FemaleStatue_DemonTree_01.fbx");
		wwString.push_back(L"SM_PRP_F_GlassPanel_01.fbx");
		wwString.push_back(L"SM_PRP_F_GlassPanel_02.fbx");
		wwString.push_back(L"SM_PRP_F_PaintingFrame_01.fbx");
		wwString.push_back(L"SM_PRP_F_PaintingFrame_02.fbx");
		wwString.push_back(L"SM_PRP_Fireplace_01.fbx");
		wwString.push_back(L"SM_PRP_Fireplace_02.fbx");
		wwString.push_back(L"SM_PRP_FireplaceCooking_01.fbx");
wwString.push_back(L"SM_PRP_MandalaFrame_01.fbx");
wwString.push_back(L"SM_PRP_MS_RootBlocker_01.fbx");
wwString.push_back(L"SM_PRP_Rock01.fbx");
wwString.push_back(L"SM_PRP_Scarecrow_01.fbx");
wwString.push_back(L"SM_PRP_Sculpture01.fbx");
wwString.push_back(L"SM_PRP_Sculpture02.fbx");
wwString.push_back(L"SM_PRP_Shield_01.fbx");
wwString.push_back(L"SM_PRP_Spear_01.fbx");
wwString.push_back(L"SM_PRP_TrishulStatue.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_01.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_02.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_03.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_04.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_05.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_06.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_07.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_08.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_10.fbx");
wwString.push_back(L"SM_PRP_UtensilGroup_11.fbx");
wwString.push_back(L"SM_PRP_Wall02.fbx");
wwString.push_back(L"SM_PRP_WallPillar01.fbx");
wwString.push_back(L"SM_PRP_WeaponGroup_01.fbx");
wwString.push_back(L"SM_PRP_WeaponGroup_02.fbx");
wwString.push_back(L"SM_PRP_WeaponGroup_03.fbx");
wwString.push_back(L"SM_PRP_WeaponGroup_04.fbx");
wwString.push_back(L"SM_PRP_WeaponGroup_05.fbx");
wwString.push_back(L"SM_PRP_WeaponGroup_06.fbx");
wwString.push_back(L"SM_PRP_WeaponGroup_07.fbx");
wwString.push_back(L"SM_Rangda_Finger_Left.fbx");
wwString.push_back(L"SM_rock_river_01.fbx");
wwString.push_back(L"SM_rock_river_02.fbx");
wwString.push_back(L"SM_rock_river_03.fbx");
wwString.push_back(L"SM_rock_river_04.fbx");
wwString.push_back(L"SM_rock_river_05.fbx");
wwString.push_back(L"SM_rock_river_06.fbx");
wwString.push_back(L"SM_rock_river_07.fbx");
wwString.push_back(L"SM_rock_river_08.fbx");
wwString.push_back(L"SM_rock_river_10.fbx");
wwString.push_back(L"SM_Root_01.fbx");
wwString.push_back(L"SM_Root_02.fbx");
wwString.push_back(L"SM_Root_03.fbx");
wwString.push_back(L"SM_Roots_01.fbx");
wwString.push_back(L"SM_Roots_Hanging_01.fbx");
wwString.push_back(L"SM_Roots_Hanging_02.fbx");
wwString.push_back(L"SM_Roots_Hanging_03.fbx");
wwString.push_back(L"SM_Roots_Hanging_04.fbx");
wwString.push_back(L"SM_Roots_Hanging_05.fbx");
wwString.push_back(L"SM_Roots_Hanging_06.fbx");
wwString.push_back(L"SM_Roundbush_01.fbx");
wwString.push_back(L"SM_Roundbush_02.fbx");
wwString.push_back(L"SM_Statue_Female_ReBirth_Body.fbx");
wwString.push_back(L"SM_Statue_Female_ReBirth_Feet.fbx");
wwString.push_back(L"SM_Statue_Female_ReBirth_Leg_02.fbx");
wwString.push_back(L"SM_Statue_Female_ReBirth_Leg_03.fbx");
wwString.push_back(L"SM_Statue_Ganga_01.fbx");
wwString.push_back(L"SM_Statue_Shiva_01.fbx");
wwString.push_back(L"SM_T_Creeper_02.fbx");
wwString.push_back(L"SM_T_Creeper_04.fbx");
wwString.push_back(L"SM_T_Creeper_05.fbx");
wwString.push_back(L"SM_T_Creeper_06.fbx");
wwString.push_back(L"SM_T_Creeper_09.fbx");
wwString.push_back(L"SM_T_Creeper_11.fbx");
wwString.push_back(L"SM_T_Creeper_12.fbx");
wwString.push_back(L"SM_T_Creeper_13.fbx");
wwString.push_back(L"SM_T_Creepers_10.fbx");
wwString.push_back(L"SM_T_Creepers_14.fbx");
wwString.push_back(L"SM_T_Creepers_15.fbx");
wwString.push_back(L"SM_T_Roots_01.fbx");
wwString.push_back(L"SM_T_Roots_02.fbx");
wwString.push_back(L"SM_T_Roots_03.fbx");
wwString.push_back(L"SM_Tree_Big_01.fbx");
wwString.push_back(L"SM_Tree_Big_02.fbx");
wwString.push_back(L"SM_Tree_Trunk_1.fbx");
wwString.push_back(L"SM_tree_trunk2.fbx");
wwString.push_back(L"SM_Tropical_Tree_02.fbx");
wwString.push_back(L"SM_Tropical_Tree_03.fbx");
wwString.push_back(L"SM_TropicalTree_01.fbx");
wwString.push_back(L"SM_Trunk_Ground_01.fbx");
wwString.push_back(L"SM_Vine_Pillar_01.fbx");
wwString.push_back(L"SM_Vine_Pillar_02.fbx");
wwString.push_back(L"SM_Vine_Pillar_04.fbx");
wwString.push_back(L"SM_Vine_Pillar_05.fbx");
wwString.push_back(L"SM_Vine_Pillar_06.fbx");
wwString.push_back(L"SM_Vine_Pillar_07.fbx");
wwString.push_back(L"SM_Vine_Pillar_08.fbx");
wwString.push_back(L"SM_Vines_Ruins_Exterior_ENT_02.fbx");
wwString.push_back(L"SM_Vines_Straight_004.fbx");
wwString.push_back(L"SM_Vines_Straight_005.fbx");
wwString.push_back(L"SM_Vines_Straight_006.fbx");
wwString.push_back(L"SM_Wall_Scatter_01A.fbx");
wwString.push_back(L"SM_Wall_Scatter_01B.fbx");
wwString.push_back(L"SM_Whiteflower_vine_01.fbx");
wwString.push_back(L"SM_Whitevine_wall_01.fbx");
wwString.push_back(L"SM_Whitevine_wall_01a.fbx");
wwString.push_back(L"SM_Whitevine_wall_02.fbx");
wwString.push_back(L"SM_Whitevine_wall_03.fbx");
wwString.push_back(L"SM_Whitevine_wall_04a.fbx");
	}

	{
		{

			wwPro.push_back(L"Prototype_Mesh_ENV_BLD_Palace_02");
			wwPro.push_back(L"Prototype_Mesh_ENV_BLD_Palace_05");
			wwPro.push_back(L"Prototype_Mesh_ENV_BLD_Palace_08");
			wwPro.push_back(L"Prototype_Mesh_ENV_BLD_Palace_11");
			wwPro.push_back(L"Prototype_Mesh_ENV_BLD_Palace_16");
			wwPro.push_back(L"Prototype_Mesh_ENV_BLD_Window_Pane_02");
			wwPro.push_back(L"Prototype_Mesh_ENV_F_DecorativePillar_02");
			wwPro.push_back(L"Prototype_Mesh_F_Vinestest");
			wwPro.push_back(L"Prototype_Mesh_PillarPlatforming");
			wwPro.push_back(L"Prototype_Mesh_PillarPlatforming_DM");
			wwPro.push_back(L"Prototype_Mesh_SM_Arrow");
			wwPro.push_back(L"Prototype_Mesh_SM_Arrow1");
			wwPro.push_back(L"Prototype_Mesh_SM_BG_Mountain1");
			wwPro.push_back(L"Prototype_Mesh_SM_BG_Mountain2");
			wwPro.push_back(L"Prototype_Mesh_SM_BG_Mountain3");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_Arch_Base");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_Arch_Stairs");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_ArchStairs_03");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_BridgeMiddle01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_BridgeMiddle03");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_BrokenArch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_BrokenArch_03");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_BrokenArch_04");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_CC_TemplePainting_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_CC_TemplePainting_03");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_CC_TemplePainting_04");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_CombatArena01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DB_WoodenPlank");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DBPillar01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DBPillar02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DBPillar04");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DBSculpture");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DiyaBridge");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_Drawbridge");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DS_BrokenArch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DS_BrokenArch_02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DS_BrokenArch_03");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_DS_SpiralPlatform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Column_B_02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Arch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Block_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Block_02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Column_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Column_02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Column_03");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Column_04");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Door_02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Door_Arch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_STAIRS_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_TILE_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_WALL_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Window_LG_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Window_LG_02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_F_Palace_Window_SM_01");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_Lion_Statue");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_StraightWall02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_StraightWall06");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_SV_PalaceBig_02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_Temple_Arch");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_TempleEnt");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_Tower02");
			wwPro.push_back(L"Prototype_Mesh_SM_BLD_TrishulGrant03");
			wwPro.push_back(L"Prototype_Mesh_SM_BouVines_01");
			wwPro.push_back(L"Prototype_Mesh_SM_boxbush_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_Face_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_Face_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_LG_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_LG_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_LG_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_LG_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_LG_05");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_Piece_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Branch_SM_01");
			wwPro.push_back(L"Prototype_Mesh_SM_bri_decal_08");
			wwPro.push_back(L"Prototype_Mesh_SM_Bridge_Chainhook");
			wwPro.push_back(L"Prototype_Mesh_SM_Broken_Branch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Broken_Branch_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Broken_Branch_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Broken_Branch_04");
			wwPro.push_back(L"Prototype_Mesh_SM_CC_archi_merg_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CC_archi_merg_02");
			wwPro.push_back(L"Prototype_Mesh_SM_CC_straightboxwall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CC_straightboxwall_02");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Gadasura_Black_Mask_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Gadasura_Rage_Mask_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Mahinasura_Leader_Mask_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Mahinasura_Leader_Stinger_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Mahinasura_Leader_Tail_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Mahinasura_Minion_Mask_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Tezabsura_Kamikaze_Mask_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Tezabsura_Landmine_Mask_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Vayusura_Mask_01");
			wwPro.push_back(L"Prototype_Mesh_SM_CH_Vayusura_Mask_02");
			wwPro.push_back(L"Prototype_Mesh_SM_circular_plane");
			wwPro.push_back(L"Prototype_Mesh_SM_Crack_Decal_Ground_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Crack_Decal_Ground_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Crack_Decal_Ground_05");
			wwPro.push_back(L"Prototype_Mesh_SM_Crack_Decal_Ground_06");
			wwPro.push_back(L"Prototype_Mesh_SM_D_BLD_PalaceArch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_D_BLD_PalaceCOL_01");
			wwPro.push_back(L"Prototype_Mesh_SM_D_BLD_PalaceCOL_02");
			wwPro.push_back(L"Prototype_Mesh_SM_D_BLD_PalaceENT_01");
			wwPro.push_back(L"Prototype_Mesh_SM_D_BLD_PalaceRFCORNER_01");
			wwPro.push_back(L"Prototype_Mesh_SM_D_BLD_PalaceRFLEDGE_01");
			wwPro.push_back(L"Prototype_Mesh_SM_D_BLD_PalaceRFSEG_01");
			wwPro.push_back(L"Prototype_Mesh_SM_D_BLD_PalaceWIN_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Damage_Decal_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Damage_Decal_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Damage_Decal_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Debris_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Debris_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Debris_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Debris_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Demon_Tree_Seg_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Demon_Tree_Seg_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Demon_Tree_Seg_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Demon_Tree_Seg_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Demon_Tree_Seg_05");
			wwPro.push_back(L"Prototype_Mesh_SM_DemonT_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_DemonT_Creeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_DemonT_Creeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_DiyaSmall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Dry_Green_Bush_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Dry_Green_Bush_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Dry_Green_Bush_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Dry_Stack_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Dry_Stack_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Dry_Stack_03");
			wwPro.push_back(L"Prototype_Mesh_SM_DryLeaves_Clutter_01");
			wwPro.push_back(L"Prototype_Mesh_SM_DryLeaves_Clutter_02");
			wwPro.push_back(L"Prototype_Mesh_SM_DryLeaves_Clutter_03");
			wwPro.push_back(L"Prototype_Mesh_SM_DryLeaves_Clutter_04");
			wwPro.push_back(L"Prototype_Mesh_SM_DryLeaves_Clutter_05");
			wwPro.push_back(L"Prototype_Mesh_SM_Dune_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Dune_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Dune_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_BLD_TopFloor_Wall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_BLD_TopFloor_Wall_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Arch_B");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Arch_Base_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Arch_C");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Arch_D");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Boat_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Border_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Border_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Border_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Border_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Bridge_Arch");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Bridge_Pillar_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogLarge_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogLarge_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogLarge_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogLarge_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogMedium_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogMedium_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogMedium_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogMedium_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogMedium_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogSmall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_CogTiny_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Diya_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_DockPlatform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Dome_A");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Dome_B");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Dome_C");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Dome_D");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Dome_E");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_DoorWay_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Elevator_Horizontal_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Elevator_Platform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Elevator_Platform_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Elevator_Vertical_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_ElevatorHorizontal_CogTR_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_ElevatorHousing_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_ElevatorLever_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_ElevatorLeverBase_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_ElevatorRailing_Horizontal_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_ElevatorRailing_Vertical_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_ElevatorVertical_Cog_Bottom_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_ElevatorVertical_Cog_Top_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Garuda_Gate");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Garuda_Tower");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Garuda_Tower_Balcony");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_MandalaCircle_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_MandalaCircle_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_MandalaCircle_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_MandalaCircle_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PaddyField_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PaddyField_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PaddyField_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PaddyField_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PaddyField_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PaddyField_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PaddyField_07");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Panel_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Panel_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Panel_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Panel_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Pillar_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Pillar_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Pillar_043");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_054_SM_ENV_CC_Platform_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_Circular_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_Circular_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_Circular_low");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Platform_Circular_low01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Puzzle_Water");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_08");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_10");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_11");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_12");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_15");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Railing_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Railing_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Railing_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_RangdaArena_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Stairs_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Stairs_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Stairs_Railing");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Statue");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_StoryPanel_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_StraightWall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_StraightWall_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_StraightWall_02_SM_ENV_CC_StraightWall_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_StraightWall_03_SM_ENV_CC_StraightWall_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_StraightWall_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_StraightWall_Withwindow_SM_ENV_CC_StraightWall_WithWindow");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_StraighWall_Withwindow1");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_TempleBase_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_TempleTop_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_TempleWalls_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Vishnu_Statue");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_VishnuTemple_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_VishnuTemple_Base_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_VishnuTemple_DoorAssembly_Left");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_VishnuTemple_DoorAssembly_Right");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Water_Platform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Water_Platform_02a");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Water_Platform_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Water_Platform_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Water_Platform_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Water_Platform_07");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Water_Platform_10");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_WaterFountain_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_WaterFountain_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Base");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Boundary_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Boundary_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_WaterWheel_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_WaterWheel_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Window_A");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Window_A1");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Window_Ba");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Window_D");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Window1");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Wooden_Planks_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_CC_Wooden_Planks_08");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_BasePlatform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_BossArenaArch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_BossArenaArch_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_CenterPlatform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_ExitDoor_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_ExitPlatformTop_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_FemaleStatue_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_FemaleStatue_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_FrontPlatformBottom_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_FrontPlatformTop_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_GodHand_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_GroundArch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_GroundColumn_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_GroundFloorPlatform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_GroundFloorPlatform_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_MandalaCircle_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_MandalaCircle_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_MandalaCircle_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_MandalaCircle_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_MandalaCircle_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_OuterWall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_OuterWall_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_OuterWall_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_OuterWall_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_OuterWall_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_OuterWall_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_OuterWall_07");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_OuterWall_08");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_Roof_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_SidePlatformTop_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_SoldierStatue_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_SoldierStatue_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_StepwellStairs_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_StepwellStairs_Half_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_StepwellWater_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_TombBase_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_D_Window_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DS_Platform_28");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_BackdropTemple_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_BellSpline_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Bull");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_BullPlatform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_BullPlatform_Broken_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Diya_02_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_DurgaBase_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_DurgaBase_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_DurgaBaseStep_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_DurgaBaseStep_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_LionEntrance_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_LionEntranceSplit_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Mountain_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Mountain_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Mountain_08");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_PillarLarge_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_PillarLarge_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Platform01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Platform02_2");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Platform03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Platform04_2");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Platform05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Platform06_Base");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Platform06_StatueBase");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_Stairs_01_2");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_StatueBase_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_StatueBase_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_StatuePillar_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_StatuePillar_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_TempleBase_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_TempleBase_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_DT_TempleBell_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_3_Decorative_Piece_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_3_Window_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_3_Window_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_3_Window_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_3_WindowArch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_3_WindowArch_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Arch_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Arch_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Balcony_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Balcony_1B");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Balcony_1C");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Balcony_1D");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Balcony_1E");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Balcony_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Balcony_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Bow_Platform_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Bow_Platform_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Building_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Chatri_Base");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Corner_Wall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Corner_Wall_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Corner_Wall_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Defences_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Defences_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Defences_02B");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Dome_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Dome_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Dome_08");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Dome_09");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Dome_014");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Door_Damaged_S_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Door_Damaged_S_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Door_NS_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Door_NS_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Door_S_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Door_S_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_DoorWay_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_DoorWayAbandoned_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_DurgaAvatar_Temple_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_DurgaAvatarTemple_Floor_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FirstFloorEntrance_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FirstFloorTile_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FirstFloorTile_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FirstFloorTile_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FirstFloorTile_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FirstFloorTile_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FirstFloorTile_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FloatingWall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FloatingWall_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FloatingWall_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FloatingWall_Trim_07");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Floor_01_2");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Fort_Pathway_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Fort_Pathway_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Fort_Pathway_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Fort_Pathway_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FortBridge_Broken_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FortBridge_Broken_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FortBridge_Broken_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FortBridge_Broken_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FortCobbleArena_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FortWALL_01b");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FortWALL_01d");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FortWALL_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Frame_01_a_int");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Frame_01_int");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_FrameClosed_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_HighWall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Mystic_Creeper_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Mystic_Creeper_07");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Mystic_Creeper_09");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_07");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_08");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_09A");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_09B");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_09C");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_09D");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_PlatFORM_09E");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Platform_Wall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Rani_Platform_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Roof_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_RoofWall_Assembled_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_RoofWall_Assembled_02a");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_RoofWall_Single");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_RoofWall_Single_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Stairs_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Stairs_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Stairs_Railing_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_StatueBase_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_StoryPanel_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Tez_Balcony");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Tower_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Tower_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Tower_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Tower_07");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Tower_08");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_ViewPlatform_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Corner_Holes_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Holes_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Holes_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Holes_Half_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Holes_Turret_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Holes_Turret_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Solid_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Solid_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Wall_Step_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_WallTrim_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Window_Pane_White");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_FemaleStatue_Pose_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_FemaleStatue_Pose_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_06");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_T_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_T_DemonT_Roots_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_T_DemonT_Roots_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_T_HangingPlant_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_T_TetrisBase_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_T_TetrisBase_Trim_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_WallBasic_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_WallBasic_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_WallBasic_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_WallBasic_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_WallBasic_08");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_WallBasic_09");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_WallBasic_11");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_Window_A_RED_BLOCK_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_ArchIvy_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_ArchIvy_03");
			wwPro.push_back(L"Prototype_Mesh_SM_F_BalconyPlant01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_BalconyPlant03");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Creepers_V01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Flower02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_HangingIvy_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Cliff_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Cliff_Creeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_05");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_06");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_07");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_08");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_09");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_10");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_11");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_12");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_13");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_14");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_15");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_16");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_17");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_18");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_19");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_20");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_21");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_22");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_24");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_25");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_MergedRoots_04");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Roots_02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_S_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_S_Creeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Shiv_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Snake_Creeper_0");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Snake_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Snake_Creeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Snake_Creeper_05");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Snake_Creeper_06");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Snake_Creeper_07");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystic_Creeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystics_Creeper_04");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystics_Creeper_05");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystics_MergedRoots_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystics_MergedRoots_02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystics_MergedRoots_03");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystics_MergedRoots_05");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystics_MergedRoots_06");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Mystics_Roots_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_PurpleFlower_Fort_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_PurpleFlower_Fort_02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_PurpleFlower_Opt02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_PurpleFlowerOpt_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_PurpleFlowerOpt_04");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Roots_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Roots_03");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Roots_05");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Roots01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Statue_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Statue_Creeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Statue_Creeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Statue_Creeper_04");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine06");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine07");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine08");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine09");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine10");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine12");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine14");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine15");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine16");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine18");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine19");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine22");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine23");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine24");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine25");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine26");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine27");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine28");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine31");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine32");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine33");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vine36");
			wwPro.push_back(L"Prototype_Mesh_SM_F_Vines40");
			wwPro.push_back(L"Prototype_Mesh_SM_Fern_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Fern_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Fern_03");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerBush_01");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerBush_02");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerBush_03");
			wwPro.push_back(L"Prototype_Mesh_SM_FLowerTest_06");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVine_01");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVine01");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVines_02");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVines_03");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVines_04");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVines_05");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVines_07");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVines_08");
			wwPro.push_back(L"Prototype_Mesh_SM_FlowerVines_09");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Bow_Creeper_0");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Bow_Creeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Bow_Creeper_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Courtyard_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Courtyard_Creeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Courtyard_Creeper_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_05");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_06");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_07");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_08");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_10");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_11");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_12");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_13");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_15");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_16");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_17");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_18");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_19");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_20");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_21");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_22");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_23");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_24");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_25");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_26");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_27");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_28");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_29");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_30");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_32");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_33");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_Creeper_34");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_RedCreeper_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_RedCreeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_RedCreeper_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_RedCreeper_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Fort_WhiteFlower_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Fortwall_blocker_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Fortwall_blocker_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Fortwall_blocker_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Fortwall_blocker_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Fortwall_blocker_05");
			wwPro.push_back(L"Prototype_Mesh_SM_Fortwall_blocker_06");
			wwPro.push_back(L"Prototype_Mesh_SM_Fortwall_Cover_01");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_01");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_02");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_03");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_04");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_05");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_06");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_07");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_08");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_09");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_10");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_11");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_Market_12");
			wwPro.push_back(L"Prototype_Mesh_SM_Fortwall_Market2_01");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_side_01");
			wwPro.push_back(L"Prototype_Mesh_SM_FortWall_side_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Gadasura_Head_03");
			wwPro.push_back(L"Prototype_Mesh_SM_GEN_Tree_01");
			wwPro.push_back(L"Prototype_Mesh_SM_GEN_Tree_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Grass_01A");
			wwPro.push_back(L"Prototype_Mesh_SM_Grass_01A1");
			wwPro.push_back(L"Prototype_Mesh_SM_Green_Stack_01");
			wwPro.push_back(L"Prototype_Mesh_SM_GRND_Bush_01");
			wwPro.push_back(L"Prototype_Mesh_SM_GRND_Bush_02");
			wwPro.push_back(L"Prototype_Mesh_SM_GRND_Bush_03");
			wwPro.push_back(L"Prototype_Mesh_SM_GRND_Bush_05");
			wwPro.push_back(L"Prototype_Mesh_SM_GRND_PLANT_01");
			wwPro.push_back(L"Prototype_Mesh_SM_GRND_Plant_CVR_01");
			wwPro.push_back(L"Prototype_Mesh_SM_GroundClutter_01");
			wwPro.push_back(L"Prototype_Mesh_SM_GroundScatter_01A");
			wwPro.push_back(L"Prototype_Mesh_SM_GroundScatter_02A");
			wwPro.push_back(L"Prototype_Mesh_SM_GroundScatter_03A");
			wwPro.push_back(L"Prototype_Mesh_SM_GroundScatter_03B");
			wwPro.push_back(L"Prototype_Mesh_SM_Hedgerow_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Hedgerow_01_A");
			wwPro.push_back(L"Prototype_Mesh_SM_Hedgerow_Corner_01");
			wwPro.push_back(L"Prototype_Mesh_SM_lillypad_01");
			wwPro.push_back(L"Prototype_Mesh_SM_lillypad_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Lillypad_02_A");
			wwPro.push_back(L"Prototype_Mesh_SM_Lillypad_02_B");
			wwPro.push_back(L"Prototype_Mesh_SM_Lillypad_02_C");
			wwPro.push_back(L"Prototype_Mesh_SM_Lillypad_02_D");
			wwPro.push_back(L"Prototype_Mesh_SM_lillypad_03");
			wwPro.push_back(L"Prototype_Mesh_SM_lillypad_04");
			wwPro.push_back(L"Prototype_Mesh_SM_lillypad_05");
			wwPro.push_back(L"Prototype_Mesh_SM_lillypad_06");
			wwPro.push_back(L"Prototype_Mesh_SM_Lotus_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Lotus_01_flower");
			wwPro.push_back(L"Prototype_Mesh_SM_Lotus_01_stem");
			wwPro.push_back(L"Prototype_Mesh_SM_Lotus_bud_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Lotus_bud_main_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Lotus_Pink");
			wwPro.push_back(L"Prototype_Mesh_SM_MERGED_PRESET_Ivy1_BP_6");
			wwPro.push_back(L"Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP_11");
			wwPro.push_back(L"Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP_14");
			wwPro.push_back(L"Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP2");
			wwPro.push_back(L"Prototype_Mesh_SM_MFort_Creeper_07");
			wwPro.push_back(L"Prototype_Mesh_SM_Mossplant");
			wwPro.push_back(L"Prototype_Mesh_SM_Mystic_Bush_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Mystic_Bush_01A");
			wwPro.push_back(L"Prototype_Mesh_SM_Mystic_Bush_02A");
			wwPro.push_back(L"Prototype_Mesh_SM_Mystic_Roots_Hanging_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Mystic_Roots_Hanging_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Mystic_Roots_Hanging_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Ninja_Run_Pose");
			wwPro.push_back(L"Prototype_Mesh_SM_palmbush_01");
			wwPro.push_back(L"Prototype_Mesh_SM_palmbush_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Pinkflower_vine_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Pinkvine_wall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Pinkvine_wall_01a");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Arrow_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Banner_01_Static");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Banner_05_Static");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Bow_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_BrokenPillars_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_BrokenPillars_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_BrokenPillars_07");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_BrokenWall03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Anvil_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Barrel_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Barrel_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_BarrelGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_BarrelGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_BarrelGroup_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_BarrelGroup_07");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_BarrelGroup_08");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_BoatRound_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Bucket_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Bucket_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Cauldron_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Cauldron_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_CauldronLarge_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Chair_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Chisel_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Chisel_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ClaypotGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ClaypotGroup_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ClaypotGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ClaypotGroup_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Cushion_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Cushion_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Cushion_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockPlanks_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockPlanks_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockPlanks_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockPlanks_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockPlanks_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockPlanks_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockPlanks_07");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockStairs_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_DockStepWall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ElephantHead_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_FishCrate_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_FishCrate_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_FishCrate_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_FishCrate_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_FishCrate_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_FishCrate_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_GlassPanel_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Grinder_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Hammer_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Jar_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Ladle_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_LampPillar_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_LanternCotton_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_LanternCotton_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_LanternMetal_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_LanternMetal_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_LeverBase_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Pier_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierCorner_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierCorner_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierFenceEnd_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierFenceEnd_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierFenceStart_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierFenceStart_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierNarrow_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierNarrow_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierNarrow_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierNarrow_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierWide_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierWide_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierWide_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_PierWide_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_RedCarpet_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_RedCarpet_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_RedCarpet_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SackGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SackGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SackGroup_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SackGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SackGroup_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SackGroup_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Soldier_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Soldier_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SpearStandGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SquareLantern_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SquareLantern_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SquareLantern_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Stool_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SwordStandGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SwordStandGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SwordStandGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_SwordStandGroup_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_Table_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_07");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_08");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableGroup_09");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TableSmall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ToolsGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ToolsGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ToolsGroup_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ToolsGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ToolsGroup_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ToolsGroup_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_ToolsGroup_07");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_TreeBase_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_WoodBox_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_07");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_ChainLink_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Curtain_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Curtain_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Curtain_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Curtain_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_Ribbon_Extension");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_RibbonLarge_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_RibbonLarge_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_RibbonLarge_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_RibbonSmall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_StoneStep_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_StoneStep_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_TreePedestal_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_D_WillowTREE_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DBPully");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DBPully_DM");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DBPully_Handle_");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DeadSoldier_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DeadSoldier_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DeadSoldier_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DeadSoldier_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DeadSoldier_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DeadSoldier_07");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DeadSoldier_08");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DeadSoldier_09");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_Arm_left_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_Arm_left_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_Arm_Right_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_Arm_Right_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_Durga_Statue_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_ElephantHead_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Bell_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Bow_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Chakra_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Drum_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Gada_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Lotus_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Shield_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Sword_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_DT_WP_Trishul_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_F_DoorLedge_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_F_FemaleStatue_DemonTree_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_F_GlassPanel_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_F_GlassPanel_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_F_PaintingFrame_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_F_PaintingFrame_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Fireplace_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Fireplace_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_FireplaceCooking_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_MandalaFrame_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_MS_RootBlocker_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Rock01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Scarecrow_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Sculpture01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Sculpture02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Shield_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Spear_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_TrishulStatue");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_07");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_08");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_10");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_UtensilGroup_11");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_Wall02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_WallPillar01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_WeaponGroup_01");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_WeaponGroup_02");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_WeaponGroup_03");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_WeaponGroup_04");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_WeaponGroup_05");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_WeaponGroup_06");
			wwPro.push_back(L"Prototype_Mesh_SM_PRP_WeaponGroup_07");
			wwPro.push_back(L"Prototype_Mesh_SM_Rangda_Finger_Left");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_01");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_02");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_03");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_04");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_05");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_06");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_07");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_08");
			wwPro.push_back(L"Prototype_Mesh_SM_rock_river_10");
			wwPro.push_back(L"Prototype_Mesh_SM_Root_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Root_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Root_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Roots_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Roots_Hanging_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Roots_Hanging_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Roots_Hanging_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Roots_Hanging_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Roots_Hanging_05");
			wwPro.push_back(L"Prototype_Mesh_SM_Roots_Hanging_06");
			wwPro.push_back(L"Prototype_Mesh_SM_Roundbush_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Roundbush_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Statue_Female_ReBirth_Body");
			wwPro.push_back(L"Prototype_Mesh_SM_Statue_Female_ReBirth_Feet");
			wwPro.push_back(L"Prototype_Mesh_SM_Statue_Female_ReBirth_Leg_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Statue_Female_ReBirth_Leg_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Statue_Ganga_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Statue_Shiva_01");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creeper_02");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creeper_04");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creeper_05");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creeper_06");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creeper_09");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creeper_11");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creeper_12");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creeper_13");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creepers_10");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creepers_14");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Creepers_15");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Roots_01");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Roots_02");
			wwPro.push_back(L"Prototype_Mesh_SM_T_Roots_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Tree_Big_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Tree_Big_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Tree_Trunk_1");
			wwPro.push_back(L"Prototype_Mesh_SM_tree_trunk2");
			wwPro.push_back(L"Prototype_Mesh_SM_Tropical_Tree_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Tropical_Tree_03");
			wwPro.push_back(L"Prototype_Mesh_SM_TropicalTree_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Trunk_Ground_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Vine_Pillar_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Vine_Pillar_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Vine_Pillar_04");
			wwPro.push_back(L"Prototype_Mesh_SM_Vine_Pillar_05");
			wwPro.push_back(L"Prototype_Mesh_SM_Vine_Pillar_06");
			wwPro.push_back(L"Prototype_Mesh_SM_Vine_Pillar_07");
			wwPro.push_back(L"Prototype_Mesh_SM_Vine_Pillar_08");
			wwPro.push_back(L"Prototype_Mesh_SM_Vines_Ruins_Exterior_ENT_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Vines_Straight_004");
			wwPro.push_back(L"Prototype_Mesh_SM_Vines_Straight_005");
			wwPro.push_back(L"Prototype_Mesh_SM_Vines_Straight_006");
			wwPro.push_back(L"Prototype_Mesh_SM_Wall_Scatter_01A");
			wwPro.push_back(L"Prototype_Mesh_SM_Wall_Scatter_01B");
			wwPro.push_back(L"Prototype_Mesh_SM_Whiteflower_vine_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Whitevine_wall_01");
			wwPro.push_back(L"Prototype_Mesh_SM_Whitevine_wall_01a");
			wwPro.push_back(L"Prototype_Mesh_SM_Whitevine_wall_02");
			wwPro.push_back(L"Prototype_Mesh_SM_Whitevine_wall_03");
			wwPro.push_back(L"Prototype_Mesh_SM_Whitevine_wall_04a");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Mandala02_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Mandala02_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Mandala02_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Mandala02_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_Mandala02_05");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_MandalaCircle_01");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_MandalaCircle_02");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_MandalaCircle_03");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_MandalaCircle_04");
			wwPro.push_back(L"Prototype_Mesh_SM_ENV_F_MandalaCircle_05");

		}
	}


	for (_uint i = 0; i<wwString.size(); i++)
	{
		wstring Ouput;
		Ouput = L"case " + wwPro[i] + L":\n"
			+ L"return " + L"TEXT(\"" + wwString[i] + L"\");\nbreak;\n";

		OutputDebugStringW(Ouput.c_str());
	}

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
