#include "stdafx.h"
#include "..\Public\Scene_Stage2.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Player.h"
#include "StaticInstanceMapObject.h"
#include "MonsterBatchTrigger.h"
#include "MapObject.h"
#include "MiniGameBuilding.h"

// #SCENE STAGE2

CScene_Stage2::CScene_Stage2(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice,pDeviceContext)
{
}

HRESULT CScene_Stage2::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pUtilMgr = GetSingle(CUtilityMgr);

	FAILED_CHECK(Ready_Light());

	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_SkyBox(TAG_LAY(Layer_SkyBox)));
	FAILED_CHECK(Ready_Layer_Terrain(TAG_LAY(Layer_Terrain)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	//FAILED_CHECK(Ready_Layer_Monster(TAG_LAY(Layer_Monster)));

	FAILED_CHECK(Ready_MiniGameBuilding(TAG_LAY(Layer_MiniGameBuilding)));
	FAILED_CHECK(Ready_Layer_Monster(TAG_LAY(Layer_Monster)));
	
	FAILED_CHECK(Ready_MapData(L"Stage_2.dat", SCENE_STAGE2, TAG_LAY(Layer_StaticMapObj)));
	FAILED_CHECK(Ready_TriggerObject(L"Stage2Trigger.dat", SCENE_STAGE2, TAG_LAY(Layer_ColTrigger)));
	FAILED_CHECK(Ready_Layer_MapObject(TAG_LAY(Layer_MapObject)));
	
	//EH
//	FAILED_CHECK(Ready_TriggerObject(L"Stage2_InstanceMonsterTrigger.dat", SCENE_STAGE2, TAG_LAY(Layer_ColTrigger)));
//
//	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage2_MonsterTrigger_1.dat", SCENE_STAGE2, TAG_LAY(Layer_BatchMonsterTrigger)));
//	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage2_MonsterTrigger_2.dat", SCENE_STAGE2, TAG_LAY(Layer_BatchMonsterTrigger)));
//	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage2_MonsterTrigger_3.dat", SCENE_STAGE2, TAG_LAY(Layer_BatchMonsterTrigger)));
//	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage2_MonsterTrigger_4.dat", SCENE_STAGE2, TAG_LAY(Layer_BatchMonsterTrigger)));
//	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage2_MonsterTrigger_5.dat", SCENE_STAGE2, TAG_LAY(Layer_BatchMonsterTrigger)));
//	FAILED_CHECK(Ready_MonsterBatchTrigger(L"Stage2_MonsterTrigger_6.dat", SCENE_STAGE2, TAG_LAY(Layer_BatchMonsterTrigger)));
	//								

	FAILED_CHECK(Ready_PostPorcessing());

	FAILED_CHECK(GETPARTICLE->Ready_MapParticle_Stage(SCENE_STAGE2));

//	FAILED_CHECK(Ready_MapParticle());
	FAILED_CHECK(Ready_Layer_UI(TAG_LAY(Layer_UI)));
	
	Play_Scene_BGM();

	return S_OK;
}

_int CScene_Stage2::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	if (g_pGameInstance->Get_DIKeyState(DIK_RETURN)&DIS_Down)
	{
		FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE7), SCENEID::SCENE_LOADING));
		return 0;
	}
	if (m_iSceneStartChecker <= SceneChangeCopyFrame && CScene_Loading::m_iLoadingKinds == CScene_Loading::LOADINGKINDS_NORMAL)
	{
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}

	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	_Vector AtPoint = m_pUtilMgr->Get_Renderer()->Get_SunAtPoint().XMVector();
	_Vector vDir = XMVector3Normalize(XMVector3Normalize(XMVectorSetY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS), 10) - AtPoint) + XMVectorSet(0,0.5f,0,0));
	g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) + vDir * 60.f);


#ifdef _DEBUG

	if (KEYDOWN(DIK_H))
	{
		// 해당 영역에 보냄

		CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_NowSceneNum(), TAG_LAY(Layer_Player)));
		NULL_CHECK_BREAK(pPlayer);
		CTransform* PlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
		CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));


		// pick pos
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_Vector vCursorPos = XMVectorSet(
			(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
			(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
			0, 1.f);

		_Matrix InvProjMat = XMMatrixInverse(nullptr, GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_PROJ));
		_Matrix InvViewMat = XMMatrixInverse(nullptr, GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_VIEW));

		_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

		vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


		_Vector vCamPos = m_pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);
		_Vector vOldPos = vCamPos;
		_Vector vNewPos;
		vNewPos = vOldPos + vRayDir;

		static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, vNewPos);
		PlayerNavi->FindCellIndex(PlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));

	}



#endif // _DEBUG

	return 0;
}

_int CScene_Stage2::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	return 0;
}

_int CScene_Stage2::Render()
{
	if (__super::Render() < 0)
		return -1;

	if (CScene_Loading::m_iLoadingKinds == CScene_Loading::LOADINGKINDS_NORMAL)
	{
		if (m_fSceneStartTimer < 0.5f)
		{
			FAILED_CHECK(m_pUtilMgr->SCD_Rendering_Rolling(((_float)m_fSceneStartTimer), 0.5f, L"Target_ToonDeferredSceneChaging2"));
		}
		else if (m_fSceneStartTimer < 2.5f)
		{
			FAILED_CHECK(m_pUtilMgr->SCD_Rendering_FadeOut(((_float)m_fSceneStartTimer - 0.5f), 2.f, L"Target_ToonDeferredSceneChaging2"));
		}
	}

	return 0;
}

_int CScene_Stage2::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

_int CScene_Stage2::Change_to_NextScene()
{

	FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, (SCENEID)m_eNextScene), SCENEID::SCENE_LOADING));

	return _int();
}

void CScene_Stage2::Play_Scene_BGM()
{
	g_pGameInstance->PlayBGM(L"Jino_BGM_Stage2.wav");
}



HRESULT CScene_Stage2::Ready_Light()
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

HRESULT CScene_Stage2::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(454.786407f, 26.5532188f, 77.2386169f);//_float3(0, 5.f, -10.f);
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
		m_pMainCam->Set_NowSceneNum(SCENE_STAGE2);
		m_pMainCam->Set_CameraDesc(CameraDesc);
	}
	
	m_pMainCam->Ortho_OnOff(false);

	m_pMainCam->Set_MaxTargetArmLength(12.f);
	m_pMainCam->Set_TargetArmLength(10.f);
	m_pMainCam->Set_MinTargetArmLength(8.f);
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_SkyBox)));

	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Terrain)));



	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_MapObject(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Map_Demon_Tree),&_float3(606.540f, 24.300f, 419.543f)));
	
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Map_MandalaPuzzle), &_float3(507.443f, 0.f, 423.105f)));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Map_FemaleStatue), &_float3(598.371f, 22.400f, 409.942f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Map_FemaleStatue), &_float3(593.436f, 22.400f, 417.660f)));


	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Minion)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Leader)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Tezabsura_Minion)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Tezabsura_Bomber)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Tezabsura_Landmine)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Tezabsura_Purple)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Minion)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Leader)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Jalsura)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Black)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Rage)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Ninjasura_Minion)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Ninjasura)));


	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Leader)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Leader)));
	
	
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_UI(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_PauseUI)));
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Player(const _tchar * pLayerTag)
{
	// 456.433228f, 26.8234997f, 77.9790649f			Maybe DemonTree Pos
	// 490.f, 7.100010f, 108.571f						Pre Start Pos
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Player), &_float3(454.681f, 18.931f, 87.145f)));
	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE2, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);	
	
	if (CScene_Loading::m_iLoadingKinds == CScene_Loading::LOADINGKINDS_NORMAL)
	{
		m_pPlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
		NULL_CHECK_RETURN(m_pPlayerTransform, E_FAIL);

		CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));
		PlayerNavi->FindCellIndex(m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));

		m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
		NULL_CHECK_RETURN(m_pMainCam, E_FAIL);

		static_cast<CPlayer*>(pPlayer)->Set_AttachCamPos(_float3(454.786407f, 26.5532188f, 77.2386169f));
		m_pMainCam->Set_CameraInitState(XMVectorSet(454.786407f, 26.5532188f, 77.2386169f, 1.f), XMVectorSet(-0.0105281984f, -0.613456666f, 0.789658368f, 0.f));
		pPlayer->Update(g_fDeltaTime);
		static_cast<CPlayer*>(pPlayer)->Set_AttachCamPos(_float3(454.786407f, 26.5532188f, 77.2386169f));
		m_pMainCam->Set_CameraInitState(XMVectorSet(454.786407f, 26.5532188f, 77.2386169f, 1.f), XMVectorSet(-0.0105281984f, -0.613456666f, 0.789658368f, 0.f));

		m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
		m_pMainCam->Set_FocusTarget(pPlayer);
		m_pMainCam->Set_CameraInitState(XMVectorSet(454.786407f, 26.5532188f, 77.2386169f, 1.f), XMVectorSet(-0.0105281984f, -0.613456666f, 0.789658368f, 0.f));

	}
	else
	{
		PPDDESC tDesc = (GetSingle(CUtilityMgr)->Set_SceneChangingData(SCENE_STAGE2));

		m_pPlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
		NULL_CHECK_RETURN(m_pPlayerTransform, E_FAIL);

		CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));
		PlayerNavi->FindCellIndex(m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));
		m_iBuildingIndex = tDesc.ObjMgrLaterIdx;
	}

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, TAG_LAY(Layer_PlayerIndicator), TAG_OP(Prototype_Object_PlayerIndicator)));

	return S_OK;

}

HRESULT CScene_Stage2::Ready_MiniGameBuilding(const _tchar * pLayerTag)
{

	_uint iNum = 0;

	CMiniGameBuilding::MGBDESC tDesc;
	tDesc.vPosition = _float3(490.428f, 4.850f, 434.639f);
	tDesc.vScale = _float3(1.f);
	tDesc.eKindsOfMiniGame = CMiniGameBuilding::MINIGAME_RHYTHM;
	tDesc.vLookDir = _float3(0, 0, -1).Get_Nomalize();
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Object_MiniGameBuilding), &tDesc));

	if (m_iBuildingIndex >= (_int)iNum)
	{
		CMiniGameBuilding* pObj = (CMiniGameBuilding*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE2, pLayerTag);
		pObj->Set_OffRadiation();
		if (m_iBuildingIndex == (_int)iNum)
			pObj->Start_ReverseSceneChanging_CamAct();
	}
	iNum++;

	return S_OK;
}

HRESULT CScene_Stage2::Ready_MapData(const _tchar * szMapDataFileName, SCENEID eSceneID, const _tchar * pLayerTag)
{
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

HRESULT CScene_Stage2::Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag)
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

HRESULT CScene_Stage2::Ready_PostPorcessing()
{
#ifndef _DEBUG

	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(1000.f, -128.f, 1000.f));
	pLightDesc->vDiffuse = _float4(0.859375f, 0.9453125f, 1.f, 1.f);
	pLightDesc->vAmbient = _float4(0.65234375f, 0.796875f, 0.9765625f, 1.f);
	pLightDesc->vSpecular = _float4(0.234375f, 0.234375f, 0.234375f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	pRenderer->Set_ShadowIntensive(0.632f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(2.5f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	pRenderer->Set_DofLength(45.f);
	pRenderer->Set_DofBlurIntensive(1.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	pRenderer->Set_FogColor(_float3(0.265625f, 0.46875f, 0.47265625f));
	pRenderer->Set_FogStartDist(0.1f);
	pRenderer->Set_FogGlobalDensity(0.05f);
	pRenderer->Set_FogHeightFalloff(0.3f);

	//POSTPROCESSING_GODRAY
	//POSTPROCESSING_LENSEFLARE
	//POSTPROCESSING_CAMMOTIONBLUR

#else

#ifdef DEBUGONSHADERSETTING
	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(1000.f, -128.f, 1000.f));
	pLightDesc->vDiffuse = _float4(0.859375f, 0.9453125f, 1.f, 1.f);
	pLightDesc->vAmbient = _float4(0.65234375f, 0.796875f, 0.9765625f, 1.f);
	pLightDesc->vSpecular = _float4(0.234375f, 0.234375f, 0.234375f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	pRenderer->Set_ShadowIntensive(0.632f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(2.5f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	pRenderer->Set_DofLength(45.f);
	pRenderer->Set_DofBlurIntensive(1.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	pRenderer->Set_FogColor(_float3(0.265625f, 0.46875f, 0.47265625f));
	pRenderer->Set_FogStartDist(0.1f);
	pRenderer->Set_FogGlobalDensity(0.05f);
	pRenderer->Set_FogHeightFalloff(0.3f);
#endif

#endif // !_DEBUG

	return S_OK;
}
HRESULT CScene_Stage2::Ready_MapParticle()
{
	/*INSTPARTICLEDESC tDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage2_CampFireParticle_0"));
	tDesc.FollowingTarget = nullptr;
	tDesc.vFixedPosition = _float3(395.5f, 4.500f, 227.7f);
	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE2, tDesc);*/

	INSTPARTICLEDESC tDesc2 = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage2_CampFireParticle_1"));
	tDesc2.FollowingTarget = nullptr;
	tDesc2.vFixedPosition = _float3(395.f, 4.400f, 227.5f);
	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE2, tDesc2);


	INSTMESHDESC tDesc3 = GetSingle(CUtilityMgr)->Get_MeshParticleDesc(TEXT("Jino_Stage2_Mesh_PinkBall"));
	tDesc3.FollowingTarget = nullptr;
	tDesc3.vFixedPosition = _float3(473.970f, 4.900f, 414.755f);;
	tDesc3.EachParticleLifeTime = 999999999.f;
	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE2, tDesc3);
	tDesc3.vFixedPosition = _float3(495.42f, 4.61f, 420.7f);
	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE2, tDesc3);
	tDesc3.vFixedPosition = _float3(485.67f, 4.91f, 420.289f);
	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE2, tDesc3);
	tDesc3.vFixedPosition = _float3(475.605f, 4.900f, 420.541f);
	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE2, tDesc3);


	INSTPARTICLEDESC tDesc4 = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage2_FireParticle_0"));
	tDesc4.FollowingTarget = nullptr;
	tDesc4.vFixedPosition = _float3(568.f, 24.300f, 403.5f);
	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE2, tDesc4);

	INSTPARTICLEDESC tDesc5 = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage2_FireParticle_0"));
	tDesc5.FollowingTarget = nullptr;
	tDesc5.vFixedPosition = _float3(573.5f, 24.000f, 395.3f);
	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE2, tDesc5);

	///**/_float3(473.970f, 4.900f, 414.755f);


	//vParticlesPos.push_back(_float3(101.110130f, 35.100010f, 42.033039f));
	//vParticlesPos.push_back(_float3(117.710f, 36.6f, 39.433f));
	
	return S_OK;
}
HRESULT CScene_Stage2::Ready_MonsterBatchTrigger(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag)
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



CScene_Stage2 * CScene_Stage2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Stage2* pTempLoby = NEW CScene_Stage2(pDevice,pDeviceContext);

	if (FAILED(pTempLoby->Initialize())) 
	{
		Safe_Release(pTempLoby); 
		MSGBOX("Failed to Creating CScene_Stage2");
		return nullptr;
	}

	return pTempLoby;

}

void CScene_Stage2::Free()
{
	__super::Free();
}
