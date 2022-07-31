#include "stdafx.h"
#include "..\Public\Scene_Stage6.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Player.h"

#include "StaticInstanceMapObject.h"


#include "physX/PhyxSampleTest.h"
#include "TestObject_PhysX.h"
#include "physX/Collider_PhysX_Dynamic.h"
#include "MapObject.h"
#include "AssimpCreateMgr.h"


CScene_Stage6::CScene_Stage6(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice,pDeviceContext)
{
}

HRESULT CScene_Stage6::Initialize()
{

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_SkyBox(TAG_LAY(Layer_SkyBox)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_TestMapObject(TAG_LAY(Layer_StaticMapObj)));
	FAILED_CHECK(Ready_MapData(L"Stage_3.dat", SCENE_STAGE6, TAG_LAY(Layer_StaticMapObj)));


//	FAILED_CHECK(Ready_Layer_Monster_Boss(TAG_LAY(Layer_Monster)));


	
	// Assimp Test
//	FAILED_CHECK(Ready_Layer_AssimpModelTest(TAG_LAY(Layer_TeethObj)));
//	GetSingle(CPhysXMgr)->CreateDemoMap();
//	GetSingle(CAssimpCreateMgr)->Free_VertexData_STATIC();


	// Effect
	FAILED_CHECK(Ready_LoadEffectMesh());


	return S_OK;
}

_int CScene_Stage6::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	if (m_iSceneStartChecker <= 2)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}


#ifdef _DEBUG
	//if (KEYDOWN(DIK_B))
	//{
	//	// 해당 영역에 보냄

	//	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE6, TAG_LAY(Layer_Player)));
	//	NULL_CHECK_BREAK(pPlayer);
	//	CTransform* PlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
	//	CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));


	//	// pick pos
	//	POINT ptMouse;
	//	GetCursorPos(&ptMouse);
	//	ScreenToClient(g_hWnd, &ptMouse);

	//	_Vector vCursorPos = XMVectorSet(
	//		(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
	//		(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
	//		0, 1.f);

	//	_Matrix InvProjMat = XMMatrixInverse(nullptr, GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_PROJ));
	//	_Matrix InvViewMat = XMMatrixInverse(nullptr, GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_VIEW));

	//	_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

	//	vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


	//	_Vector vCamPos = m_pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);
	//	_Vector vOldPos = vCamPos;
	//	_Vector vNewPos;
	//	vNewPos = vOldPos + vRayDir;

	//	static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, vNewPos);
	//	PlayerNavi->FindCellIndex(PlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));

	//}

#endif // _DEBUG


	return 0;
}
static float Z1 = 0;
static float yy = 0;

_int CScene_Stage6::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

	return 0;
}

_int CScene_Stage6::Render()
{
	if (__super::Render() < 0)
		return -1;

	if (m_bIsNeedToSceneChange) return S_FALSE;

	if (m_fSceneStartTimer < 0.5f)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->SCD_Rendering_Rolling(((_float)m_fSceneStartTimer), 0.5f, L"Target_ToonDeferredSceneChaging2"));
	}
	else if (m_fSceneStartTimer < 2.5f)
	{

		FAILED_CHECK(GetSingle(CUtilityMgr)->SCD_Rendering_FadeOut(((_float)m_fSceneStartTimer - 0.5f), 2.f, L"Target_ToonDeferredSceneChaging2"));
	}


	return 0;
}

_int CScene_Stage6::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;



#ifndef _DEBUG

	if (KEYDOWN(DIK_F))
	{
		const wchar_t* layerDynamic = TAG_LAY(Layer_ColDynamic);
		CCollider_PhysX_Base::PHYSXDESC_DYNAMIC createDynamic;

			FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
			(g_pGameInstance->Get_NowSceneNum(), layerDynamic, TAG_OP(Prototype_Object_Dynamic_PhysX)));
			CTestObject_PhysX* obj =
				static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerDynamic));

			obj->Set_ColSetID(E_PHYTYPE_DYNAMIC);
			obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);
			CCollider_PhysX_Dynamic* coldynamic = (CCollider_PhysX_Dynamic*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));


			CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
			objTrans->Set_MatrixState(CTransform::STATE_POS, _float3(0,2,0));
			objTrans->Scaled_All(_float3(1, 1, 1));

			createDynamic.eShapeType = E_GEOMAT_SPEHE;
			createDynamic.mTrnasform = objTrans;
			createDynamic.mGameObect = obj;
			NULL_CHECK_BREAK(createDynamic.mTrnasform);
			createDynamic.mVelocity = _float3(10000, 0, 0);
			coldynamic->Set_ColiiderDesc(createDynamic);

	}

#endif // _DEBUG
	return 0;
}

_int CScene_Stage6::Change_to_NextScene()
{

	FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, (SCENEID)m_eNextScene), SCENEID::SCENE_LOADING));

	return _int();
}



HRESULT CScene_Stage6::Ready_Light()
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

HRESULT CScene_Stage6::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 0, -15.f);
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
		m_pMainCam->Set_NowSceneNum(SCENE_STAGE3);
	}
	
	return S_OK;
}

HRESULT CScene_Stage6::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_STATIC, pLayerTag, TAG_OP(Prototype_SkyBox)));

	return S_OK;
}
HRESULT CScene_Stage6::Ready_Layer_AssimpModelTest(const _tchar * pLayerTag)
{
//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_TestObject)));
	return S_OK;
}

HRESULT CScene_Stage6::Ready_Layer_Player(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Player)));
	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE6, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	CTransform* PlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
	CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));

	// 43 43.7 30
//	static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(43.f, 43.7f, 30.0f));
	static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, _float3(18, 20, 10));
	

	PlayerNavi->FindCellIndex(PlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));

	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));


	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);

	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
	m_pMainCam->Set_FocusTarget(pPlayer);
	m_pMainCam->Set_TargetArmLength(0.f);

	return S_OK;
}


HRESULT CScene_Stage6::Ready_Layer_TestMapObject(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_StaticMapObject)));
	CTransform* pTransform = (CTransform*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE6, pLayerTag)->Get_Component(TAG_COM(Com_Transform)));
	NULL_CHECK_RETURN(pTransform, E_FAIL);
	_Matrix tt = XMMatrixScaling(20, 1, 20) * XMMatrixTranslation(0, -3, 0);
	pTransform->Set_Matrix(tt);
	((CMapObject*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE6, pLayerTag))->Set_FrustumSize(99999999.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, L"TestNonAnimInstance"));

	return S_OK;
}

HRESULT CScene_Stage6::Ready_Layer_Monster_Boss(const _tchar * pLayerTag)
{
	// 몬스터들
	// FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Minion)));

	// FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Leader))); 

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Minion)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Leader)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Ninjasura_Minion)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Ninjasura)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Black)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Rage)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Rage_Hollogram)));


	// 보스들
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Boss_Chiedtian)));

	return S_OK;
}

HRESULT CScene_Stage6::Ready_MapData(const _tchar * szMapDataFileName, SCENEID eSceneID, const _tchar * pLayerTag)
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


	}




	CloseHandle(hFile);

	return S_OK;

}




HRESULT CScene_Stage6::Ready_Layer_Phycis()
{

	return S_OK;
}

HRESULT CScene_Stage6::Ready_LoadEffectMesh()
{

	_Matrix TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	for (_uint i = (_uint)Prototype_Mesh_KurtzpelStart + 1; i < (_uint)Prototype_Mesh_KurtzpelEnd; i++)
		STATIC_EFFECTLOAD((COMPONENTPROTOTYPEID)i);


	return S_OK;
}


HRESULT CScene_Stage6::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	return S_OK;
}



CScene_Stage6 * CScene_Stage6::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Stage6* pTempLoby = NEW CScene_Stage6(pDevice,pDeviceContext);

	if (FAILED(pTempLoby->Initialize())) 
	{
		Safe_Release(pTempLoby); 
		MSGBOX("Failed to Creating CScene_Stage6");
		return nullptr;
	}

	return pTempLoby;

}

void CScene_Stage6::Free()
{
	__super::Free();
//	Safe_Release(m_pPhySample);
}



// Test Str ADDSTATIC Test
//static void TestStr()
//{
//	vector<wstring> wwString;
//	vector<wstring> wwPro;
//
//	// wwString.push_back(L"SM_Vine_Pillar_07.fbx");
//	{
//
//
//		wwString.push_back(L"01_R_Punch_SM_01_Mesh003.fbx");
//		wwString.push_back(L"01_R_Punch_SM_01_Mesh004.fbx");
//		wwString.push_back(L"02_L_Upper_SM.fbx");
//		wwString.push_back(L"Aura_Arrow.fbx");
//		wwString.push_back(L"Aura_Arrow_Wing.fbx");
//		wwString.push_back(L"auraHousya.fbx");
//		wwString.push_back(L"chantDeco2.fbx");
//		wwString.push_back(L"circle.fbx");
//		wwString.push_back(L"circle_inv.fbx");
//		wwString.push_back(L"circleDist2.fbx");
//		wwString.push_back(L"circleDist4.fbx");
//		wwString.push_back(L"circleDist5.fbx");
//		wwString.push_back(L"circleDist5_02.fbx");
//		wwString.push_back(L"doomBvert.fbx");
//		wwString.push_back(L"doomBvert2Inv.fbx");
//		wwString.push_back(L"FX_Aura_Trail_02.fbx");
//		wwString.push_back(L"FX_Aura_Trail_05.fbx");
//		wwString.push_back(L"Grudge_Wraith_weapon_mesh.fbx");
//		wwString.push_back(L"ice2.fbx");
//		wwString.push_back(L"Line_SM_Low_R_Re.fbx");
//		wwString.push_back(L"Lire_Leaf01.fbx");
//		wwString.push_back(L"Mesh_Ama.fbx");
//		wwString.push_back(L"Mesh_Ama_Sword.fbx");
//		wwString.push_back(L"Mesh_BasicAttack_02_04.fbx");
//		wwString.push_back(L"Mesh_cylinder.fbx");
//		wwString.push_back(L"Mesh_Cylinder_03.fbx");
//		wwString.push_back(L"Mesh_Dust08_SUB_5X4_L.fbx");
//		wwString.push_back(L"Mesh_Dust08_SUB_5X4_R.fbx");
//		wwString.push_back(L"Mesh_HFB_Arrow_Square_M01.fbx");
//		wwString.push_back(L"Mesh_HFB_Arrow_Square_M02.fbx");
//		wwString.push_back(L"Mesh_HFB_Arrow_Square_M03.fbx");
//		wwString.push_back(L"Mesh_HFB_Arrow_Square_M04.fbx");
//		wwString.push_back(L"Mesh_HFB_Arrow_Wing_L.fbx");
//		wwString.push_back(L"Mesh_HFB_Arrow_Wing_R.fbx");
//		wwString.push_back(L"Mesh_HFB_CIRCLE.fbx");
//		wwString.push_back(L"Mesh_HFB_Grand_Git.fbx");
//		wwString.push_back(L"Mesh_HFB_Hit_M01.fbx");
//		wwString.push_back(L"Mesh_HFB_Uplight_M01.fbx");
//		wwString.push_back(L"Mesh_HFH_Dash_Wind01.fbx");
//		wwString.push_back(L"Mesh_HFH_Dash_Wind02.fbx");
//		wwString.push_back(L"Mesh_HFH_Dash_Wind03.fbx");
//		wwString.push_back(L"Mesh_HFS_Charge_Wind_01_Inv.fbx");
//		wwString.push_back(L"Mesh_HFS_Charge_Wind_02.fbx");
//		wwString.push_back(L"Mesh_HFS_ChargeAttack_M01.fbx");
//		wwString.push_back(L"Mesh_HFS_ChargeAttack_M02.fbx");
//		wwString.push_back(L"Mesh_Leap_Ground_Light.fbx");
//		wwString.push_back(L"Mesh_Leap_Ground_Light2.fbx");
//		wwString.push_back(L"Mesh_Leap_Ground_M04.fbx");
//		wwString.push_back(L"Mesh_Leap_Wind_M01.fbx");
//		wwString.push_back(L"Mesh_Leap_Wind_M02.fbx");
//		wwString.push_back(L"Mesh_Plane_03.fbx");
//		wwString.push_back(L"Mesh_Trace_M01.fbx");
//		wwString.push_back(L"Mesh_Trace_M02.fbx");
//		wwString.push_back(L"Mesh_Trace_M03.fbx");
//		wwString.push_back(L"Mesh_Trace_M04.fbx");
//		wwString.push_back(L"Mesh_Trace_M05.fbx");
//		wwString.push_back(L"Mesh_Trace_M06.fbx");
//		wwString.push_back(L"Mesh_Trace_M07.fbx");
//		wwString.push_back(L"Mesh_TS_Double_Slash_01.fbx");
//		wwString.push_back(L"Mesh_TS_Double_Slash_05.fbx");
//		wwString.push_back(L"MS_Arc_00.fbx");
//		wwString.push_back(L"MS_Cloud.fbx");
//		wwString.push_back(L"MS_Cylinder_00.fbx");
//		wwString.push_back(L"MS_Cylinder_02.fbx");
//		wwString.push_back(L"MS_Cylinder_05.fbx");
//		wwString.push_back(L"MS_GeoSphere.fbx");
//		wwString.push_back(L"MS_HalfSphere.fbx");
//		wwString.push_back(L"MS_Land_Half_Cylinder.fbx");
//		wwString.push_back(L"MS_LandCrack_00.fbx");
//		wwString.push_back(L"MS_Line_00.fbx");
//		wwString.push_back(L"MS_Line_01.fbx");
//		wwString.push_back(L"MS_Line_02.fbx");
//		wwString.push_back(L"MS_Line_03.fbx");
//		wwString.push_back(L"MS_Line_04.fbx");
//		wwString.push_back(L"MS_Line_05.fbx");
//		wwString.push_back(L"MS_Shield_Sphere_00.fbx");
//		wwString.push_back(L"MS_Shot_Cylinder_00.fbx");
//		wwString.push_back(L"MS_Shpere_00.fbx");
//		wwString.push_back(L"MS_SmokePlane.fbx");
//		wwString.push_back(L"MS_Sphere_00.fbx");
//		wwString.push_back(L"MS_Sphere_Bill_00.fbx");
//		wwString.push_back(L"MS_Sphere_Bill_01.fbx");
//		wwString.push_back(L"MS_ST3_Portal_00.fbx");
//		wwString.push_back(L"MS_ST3_Portal_01.fbx");
//		wwString.push_back(L"MS_St3_Tornado_Bottom_Cyl_01.fbx");
//		wwString.push_back(L"MS_St3_Tornado_Bottom_Cyl_10.fbx");
//		wwString.push_back(L"MS_Tornado_Cyl.fbx");
//		wwString.push_back(L"MS_Tornado_Cyl_2side.fbx");
//		wwString.push_back(L"MS_Tornado_Cyl_2side_01.fbx");
//		wwString.push_back(L"MS_Tornado_Cyl_2side_03.fbx");
//		wwString.push_back(L"MS_Tornado_Helix_00.fbx");
//		wwString.push_back(L"MS_Trail_00.fbx");
//		wwString.push_back(L"MS_Trail_Twist_00.fbx");
//		wwString.push_back(L"MS_Trail_Twist_01.fbx");
//		wwString.push_back(L"MS_Twis_Attack.fbx");
//		wwString.push_back(L"MS_Twis_Attack_01.fbx");
//		wwString.push_back(L"myCylinderKaiRotB.fbx");
//		wwString.push_back(L"mySphere.fbx");
//		wwString.push_back(L"S_BasicAttack00_M01.fbx");
//		wwString.push_back(L"S_BasicAttack00_M03.fbx");
//		wwString.push_back(L"S_JumpAttack_02_M01.fbx");
//		wwString.push_back(L"S_JumpAttack_02_M02.fbx");
//		wwString.push_back(L"S_JumpAttack_02_M04.fbx");
//		wwString.push_back(L"Sample_Mesh_Fake_Mesh_Lighting.fbx");
//		wwString.push_back(L"Sample_Mesh_Fresnal.fbx");
//		wwString.push_back(L"Sample_Mesh_Hit.fbx");
//		wwString.push_back(L"Sample_Mesh_WP_Cylinder_Z.fbx");
//		wwString.push_back(L"shield.fbx");
//		wwString.push_back(L"SK_03_L_Foot.fbx");
//		wwString.push_back(L"SK_BF_Skill07_AuraMesh.fbx");
//		wwString.push_back(L"SK_BF_Skill07_RectangleMesh.fbx");
//		wwString.push_back(L"SK_EM_CatPot_Stick.fbx");
//		wwString.push_back(L"SK_Guadian_Skill07_Loop.fbx");
//		wwString.push_back(L"SK_Guadian_Skill07_Loop_02.fbx");
//		wwString.push_back(L"SM_02_L_Upper_Mesh02.fbx");
//		wwString.push_back(L"SM_03_L_Foot_SM_03_L_Foot_M1.fbx");
//		wwString.push_back(L"SM_03_L_Foot_SM_03_L_Foot_M2.fbx");
//		wwString.push_back(L"SM_03_L_Foot_SM_03_L_Foot_M3.fbx");
//		wwString.push_back(L"SM_4E_DarkTails_01.fbx");
//		wwString.push_back(L"SM_4E_IceShards_01.fbx");
//		wwString.push_back(L"SM_4E_IceShards_02.fbx");
//		wwString.push_back(L"SM_4E_IceShards_03.fbx");
//		wwString.push_back(L"SM_4E_IceSpike_01.fbx");
//		wwString.push_back(L"SM_4E_ImpactFX_01.fbx");
//		wwString.push_back(L"SM_4E_ImpactFX_02.fbx");
//		wwString.push_back(L"SM_4E_LightCastRing_01.fbx");
//		wwString.push_back(L"SM_4E_LightCircle_01.fbx");
//		wwString.push_back(L"SM_4E_LightCircle_02.fbx");
//		wwString.push_back(L"SM_4E_LightningArcs_01.fbx");
//		wwString.push_back(L"SM_4E_LightningArcs_02_re.fbx");
//		wwString.push_back(L"SM_4E_LightningArcs_03.fbx");
//		wwString.push_back(L"SM_4E_LightningArcs_04.fbx");
//		wwString.push_back(L"SM_4E_LightningArcs_05.fbx");
//		wwString.push_back(L"SM_4E_LightningBolt_01.fbx");
//		wwString.push_back(L"SM_4E_LightningOrb_01.fbx");
//		wwString.push_back(L"SM_4E_LightRay_01.fbx");
//		wwString.push_back(L"SM_4E_LightSpike_02.fbx");
//		wwString.push_back(L"SM_4E_RegularPlane_01.fbx");
//		wwString.push_back(L"SM_4E_RegularPlane_02.fbx");
//		wwString.push_back(L"SM_4E_SpiralRay_01.fbx");
//		wwString.push_back(L"SM_06_06_SM001.fbx");
//		wwString.push_back(L"SM_06_06_SM002.fbx");
//		wwString.push_back(L"SM_06_06_SM003.fbx");
//		wwString.push_back(L"SM_07_Mesh001.fbx");
//		wwString.push_back(L"SM_07_Mesh002.fbx");
//		wwString.push_back(L"SM_07_Mesh003.fbx");
//		wwString.push_back(L"SM_21_Christmas_01.fbx");
//		wwString.push_back(L"SM_AB_Lightning_01.fbx");
//		wwString.push_back(L"SM_AB_Lightning_02.fbx");
//		wwString.push_back(L"SM_AB_Lightning_03.fbx");
//		wwString.push_back(L"SM_AB_Lightning_04.fbx");
//		wwString.push_back(L"SM_AB_Trail_01.fbx");
//		wwString.push_back(L"SM_AB_Trail_02.fbx");
//		wwString.push_back(L"SM_AB_Trail_03.fbx");
//		wwString.push_back(L"SM_AB_Tree.fbx");
//		wwString.push_back(L"SM_AB_Wind_01.fbx");
//		wwString.push_back(L"SM_AB_Wind_02.fbx");
//		wwString.push_back(L"SM_AB_Wind_03.fbx");
//		wwString.push_back(L"SM_AOG_Helix01.fbx");
//		wwString.push_back(L"SM_AOG_Helix02.fbx");
//		wwString.push_back(L"SM_AOG_Plan01.fbx");
//		wwString.push_back(L"SM_AOG_Ring.fbx");
//		wwString.push_back(L"SM_AOG_Weapon.fbx");
//		wwString.push_back(L"SM_AOG_Weapon_Jewllery.fbx");
//		wwString.push_back(L"SM_Arc_00.fbx");
//		wwString.push_back(L"SM_Arc_01.fbx");
//		wwString.push_back(L"SM_Arrow_Circle_01.fbx");
//		wwString.push_back(L"SM_Arrow_Circle_02.fbx");
//		wwString.push_back(L"SM_Arrow_Wing_01.fbx");
//		wwString.push_back(L"SM_Arrow_Wing_02.fbx");
//		wwString.push_back(L"SM_ArrowBurstTrail_01_New.fbx");
//		wwString.push_back(L"SM_ArrowBurstTrail_02_New.fbx");
//		wwString.push_back(L"SM_ArrowCase.fbx");
//		wwString.push_back(L"SM_Asura_Cillynder01.fbx");
//		wwString.push_back(L"SM_Asura_Circle01.fbx");
//		wwString.push_back(L"SM_Asura_Sphere02.fbx");
//		wwString.push_back(L"SM_Aura_Foot_L_03.fbx");
//		wwString.push_back(L"SM_Aura_Linear_01.fbx");
//		wwString.push_back(L"SM_Aura_Linear_03.fbx");
//		wwString.push_back(L"SM_Aura_Trail01.fbx");
//		wwString.push_back(L"SM_auraHousya.fbx");
//		wwString.push_back(L"SM_auraWallSq.fbx");
//		wwString.push_back(L"SM_auraWallSq2.fbx");
//		wwString.push_back(L"SM_AXTrail_01.fbx");
//		wwString.push_back(L"SM_Baren_Weapon.fbx");
//		wwString.push_back(L"SM_Baren_Weapon_1.fbx");
//		wwString.push_back(L"SM_Baren_Weapon_2.fbx");
//		wwString.push_back(L"SM_Basic_Cy01_Cylinder001.fbx");
//		wwString.push_back(L"SM_Basic_EX02_Basic_EX02_Doddgy.fbx");
//		wwString.push_back(L"SM_Basic_EX02_Basic_EX02_Main.fbx");
//		wwString.push_back(L"SM_Basic_EX02_Basic_EX02_Sub.fbx");
//		wwString.push_back(L"SM_Basic_EX02_Basic_EX02_Sub_vectorUV.fbx");
//		wwString.push_back(L"SM_Basic_EX03_2_Doddgy_Y.fbx");
//		wwString.push_back(L"SM_Basic_EX03_2_Main_Y.fbx");
//		wwString.push_back(L"SM_Basic_EX03_2_Sub_Y.fbx");
//		wwString.push_back(L"SM_Basic_Trail_SM_Dodggy_X.fbx");
//		wwString.push_back(L"SM_Basic_Trail_SM_Dodggy_Z.fbx");
//		wwString.push_back(L"SM_Basic_Trail_SM_Main_X.fbx");
//		wwString.push_back(L"SM_Basic_Trail_SM_Main_Z.fbx");
//		wwString.push_back(L"SM_Basic_Trail_SM_Sub_X.fbx");
//		wwString.push_back(L"SM_Basic_Trail_SM_Sub_Z.fbx");
//		wwString.push_back(L"SM_Basic_windLine.fbx");
//		wwString.push_back(L"SM_BBT_Paw.fbx");
//		wwString.push_back(L"SM_BBT_Paw_L.fbx");
//		wwString.push_back(L"SM_BBT_Teeth_Down.fbx");
//		wwString.push_back(L"SM_BBT_Teeth_UP.fbx");
//		wwString.push_back(L"SM_BerserkerRush_01_Doddgy_Trail_X.fbx");
//		wwString.push_back(L"SM_BerserkerRush_01_Doddgy_Trail_Y.fbx");
//		wwString.push_back(L"SM_BerserkerRush_01_Main_Trail_X.fbx");
//		wwString.push_back(L"SM_BerserkerRush_01_Main_Trail_Y.fbx");
//		wwString.push_back(L"SM_BerserkerRush_01_Sub_Trail_X.fbx");
//		wwString.push_back(L"SM_BerserkerRush_01_Sub_Trail_Y.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_Base.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_Base_Z.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_Doddgy.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_Doddgy_Z.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_Sub.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_Sub_Z.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_X_Small.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_XY_SM_X.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_XY_SM_Y.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_2_Y_Small.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_03_01_D1.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_03_01_D2.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_03_01_Main.fbx");
//		wwString.push_back(L"SM_BF_Skill_01_03_01_Sub.fbx");
//		wwString.push_back(L"SM_BF_XY.fbx");
//		wwString.push_back(L"SM_Bow_Base.fbx");
//		wwString.push_back(L"SM_Bow_Em_01.fbx");
//		wwString.push_back(L"SM_Bow_Em_02.fbx");
//		wwString.push_back(L"SM_Bow_Em_Wing_T.fbx");
//		wwString.push_back(L"SM_bowing_01.fbx");
//		wwString.push_back(L"SM_bowing_02.fbx");
//		wwString.push_back(L"SM_bowing_03.fbx");
//		wwString.push_back(L"SM_bowing_04.fbx");
//		wwString.push_back(L"SM_bowing_05.fbx");
//		wwString.push_back(L"SM_Box_Basic.fbx");
//		wwString.push_back(L"SM_BufRing_01.fbx");
//		wwString.push_back(L"SM_BufSwirl_01.fbx");
//		wwString.push_back(L"SM_BufSwirl_02.fbx");
//		wwString.push_back(L"SM_Bullet.fbx");
//		wwString.push_back(L"SM_Chain.fbx");
//		wwString.push_back(L"SM_Chain_Buff.fbx");
//		wwString.push_back(L"SM_Chaos_Dust_01.fbx");
//		wwString.push_back(L"SM_Chaos_leg.fbx");
//		wwString.push_back(L"SM_Chaos_legMesh.fbx");
//		wwString.push_back(L"SM_Cine_Mask_00.fbx");
//		wwString.push_back(L"SM_Cine_Mask_01.fbx");
//		wwString.push_back(L"SM_Cine_Mask_02.fbx");
//		wwString.push_back(L"SM_Cine_Mask_03.fbx");
//		wwString.push_back(L"SM_Cine_Mask_04.fbx");
//		wwString.push_back(L"SM_Cine_Nae_Bow.fbx");
//		wwString.push_back(L"SM_circle.fbx");
//		wwString.push_back(L"SM_circle_inv.fbx");
//		wwString.push_back(L"SM_Circle_Magic.fbx");
//		wwString.push_back(L"SM_Cone_01.fbx");
//		wwString.push_back(L"SM_ControlPointMatch_Square_01.fbx");
//		wwString.push_back(L"SM_ControlPointMatch_Square_02.fbx");
//		wwString.push_back(L"SM_CoreRing_3.fbx");
//		wwString.push_back(L"SM_CoreRing_4.fbx");
//		wwString.push_back(L"SM_Crack_In_H.fbx");
//		wwString.push_back(L"SM_Crack_In_V.fbx");
//		wwString.push_back(L"SM_Crack_Out_H.fbx");
//		wwString.push_back(L"SM_Crack_Out_V.fbx");
//		wwString.push_back(L"SM_Crack_OutLine.fbx");
//		wwString.push_back(L"SM_Critical_R_D_Plane.fbx");
//		wwString.push_back(L"SM_Critical_R_X_Plane.fbx");
//		wwString.push_back(L"SM_Critical_R_Z_Plane.fbx");
//		wwString.push_back(L"SM_Cross.fbx");
//		wwString.push_back(L"SM_Crossbowman_Arrow_chork.fbx");
//		wwString.push_back(L"SM_CS_SWTrail_02.fbx");
//		wwString.push_back(L"SM_CS_SWTrail_02UFlip.fbx");
//		wwString.push_back(L"SM_CS_SWTrail_03UFlip.fbx");
//		wwString.push_back(L"SM_CS_Trail_01.fbx");
//		wwString.push_back(L"SM_CW_Trail_04.fbx");
//		wwString.push_back(L"SM_Cylinder.fbx");
//		wwString.push_back(L"SM_cylinderKaiRotB_vc.fbx");
//		wwString.push_back(L"SM_Darknight_Skill01_01.fbx");
//		wwString.push_back(L"SM_Darknight_Skill01_02.fbx");
//		wwString.push_back(L"SM_Darknight_Skill01_03.fbx");
//		wwString.push_back(L"SM_Darknight_Skill01_04.fbx");
//		wwString.push_back(L"SM_DivineLaserBeam_02.fbx");
//		wwString.push_back(L"SM_doomBvert2Inv.fbx");
//		wwString.push_back(L"SM_Dough.fbx");
//		wwString.push_back(L"SM_DoughSide.fbx");
//		wwString.push_back(L"SM_DS_250010_L_01.fbx");
//		wwString.push_back(L"SM_DS_AirCombo01_D_L.fbx");
//		wwString.push_back(L"SM_DS_AirCombo01_D_R.fbx");
//		wwString.push_back(L"SM_DS_AirCombo01_X_L.fbx");
//		wwString.push_back(L"SM_DS_AirCombo01_X_R.fbx");
//		wwString.push_back(L"SM_DS_AirCombo01_Z_L.fbx");
//		wwString.push_back(L"SM_DS_AirCombo01_Z_R.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_02_L_D.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_02_L_X.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_02_L_Z.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_02_R_D.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_02_R_X.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_02_R_Z.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_03_L_D.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_03_L_X.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_03_L_Z.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_03_R_D.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_03_R_X.fbx");
//		wwString.push_back(L"SM_DS_AirTrail_03_R_Z.fbx");
//		wwString.push_back(L"SM_DS_Basic_01_2_Trail_H.fbx");
//		wwString.push_back(L"SM_DS_Basic_01_2_Trail_P.fbx");
//		wwString.push_back(L"SM_DS_Basic_01_2_Trail_X.fbx");
//		wwString.push_back(L"SM_DS_Basic_01_Trail_H.fbx");
//		wwString.push_back(L"SM_DS_Basic_01_Trail_X.fbx");
//		wwString.push_back(L"SM_DS_Basic_01_Trail_Z.fbx");
//		wwString.push_back(L"SM_DS_Basic_02_1_X_Doddgy.fbx");
//		wwString.push_back(L"SM_DS_Basic_02_1_X_Plane.fbx");
//		wwString.push_back(L"SM_DS_Basic_02_1_Z_Plane.fbx");
//		wwString.push_back(L"SM_DS_Basic_02_2_Cross.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_1_X_Plane.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_1_Z_Plane.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_2_X_Orign.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_2_Z_Orign.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_3_D_L.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_3_D_R.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_3_X_L.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_3_X_R.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_3_Z_L.fbx");
//		wwString.push_back(L"SM_DS_Basic_03_3_Z_R.fbx");
//		wwString.push_back(L"SM_DS_Basic04_1_X_Doddgy_R.fbx");
//		wwString.push_back(L"SM_DS_Basic04_1_X_R.fbx");
//		wwString.push_back(L"SM_DS_Basic04_1_Z_R.fbx");
//		wwString.push_back(L"SM_DS_Basic04_2_X_Doddgy_L.fbx");
//		wwString.push_back(L"SM_DS_Basic04_2_X_L.fbx");
//		wwString.push_back(L"SM_DS_Basic04_2_Z_L.fbx");
//		wwString.push_back(L"SM_DS_Basic04_3_X_Doddgy_R.fbx");
//		wwString.push_back(L"SM_DS_Basic04_3_X_R.fbx");
//		wwString.push_back(L"SM_DS_Basic04_3_Z_R.fbx");
//		wwString.push_back(L"SM_DS_Basic05_1_R_Doddgy.fbx");
//		wwString.push_back(L"SM_DS_Basic05_1_R_X.fbx");
//		wwString.push_back(L"SM_DS_Basic05_1_R_Z.fbx");
//		wwString.push_back(L"SM_DS_Basic05_2_L_Doddgy.fbx");
//		wwString.push_back(L"SM_DS_Basic05_2_L_X.fbx");
//		wwString.push_back(L"SM_DS_Basic05_2_L_Z.fbx");
//		wwString.push_back(L"SM_DS_Basic05_3_R_Doddgy.fbx");
//		wwString.push_back(L"SM_DS_Basic05_3_R_X.fbx");
//		wwString.push_back(L"SM_DS_Basic05_3_R_Z.fbx");
//		wwString.push_back(L"SM_DS_BasicTrace_01.fbx");
//		wwString.push_back(L"SM_DS_Blade_D_L_1_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_D_L_2_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_D_R_1_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_D_R_2_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_X_L_1_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_X_L_2_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_X_R_1_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_X_R_2_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_Z_L_1_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_Z_L_2_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_Z_R_1_Plane.fbx");
//		wwString.push_back(L"SM_DS_Blade_Z_R_2_Plane.fbx");
//		wwString.push_back(L"SM_DS_Dash_Dust_01.fbx");
//		wwString.push_back(L"SM_DS_Dragon_Weapon.fbx");
//		wwString.push_back(L"SM_DS_Dust_Dash_01.fbx");
//		wwString.push_back(L"SM_DS_Dust_Dash_02.fbx");
//		wwString.push_back(L"SM_DS_F_BasicTrace_G_02_SM_DS_F_BasicTrace_0.bx");
//		wwString.push_back(L"SM_DS_Fated_Proj_X.fbx");
//		wwString.push_back(L"SM_DS_Fated_Proj_Z.fbx");
//		wwString.push_back(L"SM_DS_Fated_Proj_Z_02.fbx");
//		wwString.push_back(L"SM_DS_Fated_Proj_Z_03.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_F_L003.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_F_L004.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_F_L005.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_F_R003.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_F_R004.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_F_R005.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Front_L001.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Front_R001.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Front_R002_1.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Front_R002_2.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_L001.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_L003.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_L004.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_L005.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_R001.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_R002_1.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_R002_2.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_R003.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_R004.fbx");
//		wwString.push_back(L"SM_DS_FatedCircle_Top_R005.fbx");
//		wwString.push_back(L"SM_Ds_Genocide_01.fbx");
//		wwString.push_back(L"SM_DS_Genocide_Trace_D_L.fbx");
//		wwString.push_back(L"SM_DS_Genocide_Trace_D_R.fbx");
//		wwString.push_back(L"SM_DS_Genocide_Trace_X_L.fbx");
//		wwString.push_back(L"SM_DS_Genocide_Trace_X_R.fbx");
//		wwString.push_back(L"SM_DS_Genocide_Trace_Z_L.fbx");
//		wwString.push_back(L"SM_DS_Genocide_Trace_Z_R.fbx");
//		wwString.push_back(L"SM_DS_Helix_Aura_02.fbx");
//		wwString.push_back(L"SM_DS_Plane_D_L.fbx");
//		wwString.push_back(L"SM_DS_Plane_D_R.fbx");
//		wwString.push_back(L"SM_DS_Plane_X_L.fbx");
//		wwString.push_back(L"SM_DS_Plane_X_R.fbx");
//		wwString.push_back(L"SM_DS_Plane_Z_L.fbx");
//		wwString.push_back(L"SM_DS_Plane_Z_R.fbx");
//		wwString.push_back(L"SM_DS_Projectile_Main_X_High.fbx");
//		wwString.push_back(L"SM_DS_Projectile_Main_Y_High.fbx");
//		wwString.push_back(L"SM_DS_Projectile_Sub_Y_High.fbx");
//		wwString.push_back(L"SM_DS_Projectile_Wind_Y.fbx");
//		wwString.push_back(L"SM_DS_Projectile_Wind_Y_High.fbx");
//		wwString.push_back(L"SM_DS_Projectile_Wind_Y_High02.fbx");
//		wwString.push_back(L"SM_DS_Rage_Buff_L_D.fbx");
//		wwString.push_back(L"SM_DS_Rage_Buff_L_X.fbx");
//		wwString.push_back(L"SM_DS_Rage_Buff_L_Z.fbx");
//		wwString.push_back(L"SM_DS_Rage_Buff_R_D.fbx");
//		wwString.push_back(L"SM_DS_Rage_Buff_R_X.fbx");
//		wwString.push_back(L"SM_DS_Rage_Buff_R_Z.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_01.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_02.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_03.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_D_L.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_D_R.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_X_L.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_X_R.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_Z_L.fbx");
//		wwString.push_back(L"SM_DS_Rage_Genocide_Z_R.fbx");
//		wwString.push_back(L"SM_DS_Scart_X.fbx");
//		wwString.push_back(L"SM_DS_Scart_X_D.fbx");
//		wwString.push_back(L"SM_DS_Scart_Z.fbx");
//		wwString.push_back(L"SM_DS_Skill_Fate_Trail_Front01.fbx");
//		wwString.push_back(L"SM_DS_Skill_Fate_Trail_Stop_Front02.fbx");
//		wwString.push_back(L"SM_DS_Skill_Fate_Trail_Stop_Top01.fbx");
//		wwString.push_back(L"SM_DS_Skill_Fate_Trail_Stop_Top02.fbx");
//		wwString.push_back(L"SM_DS_Skill_Trace_Front01.fbx");
//		wwString.push_back(L"SM_DS_Skill_Trace_Top01.fbx");
//		wwString.push_back(L"SM_DS_SP_Charging_Ground_01.fbx");
//		wwString.push_back(L"SM_DS_SP_Charging_Ground_02.fbx");
//		wwString.push_back(L"SM_DS_SP_EX1_D_L.fbx");
//		wwString.push_back(L"SM_DS_SP_EX1_D_R.fbx");
//		wwString.push_back(L"SM_DS_SP_EX1_X_L.fbx");
//		wwString.push_back(L"SM_DS_SP_EX1_X_R.fbx");
//		wwString.push_back(L"SM_DS_SP_EX1_Z_L.fbx");
//		wwString.push_back(L"SM_DS_SP_EX1_Z_R.fbx");
//		wwString.push_back(L"SM_DS_Switching_L_D_Plane.fbx");
//		wwString.push_back(L"SM_DS_Switching_L_X_Plane.fbx");
//		wwString.push_back(L"SM_DS_Switching_L_Z_Plane.fbx");
//		wwString.push_back(L"SM_DS_Switching_R_D_Plane.fbx");
//		wwString.push_back(L"SM_DS_Switching_R_X_Plane.fbx");
//		wwString.push_back(L"SM_DS_Switching_R_Z_Plane.fbx");
//		wwString.push_back(L"SM_DS_SwitchingA_L_D.fbx");
//		wwString.push_back(L"SM_DS_SwitchingA_L_X.fbx");
//		wwString.push_back(L"SM_DS_SwitchingA_L_Z.fbx");
//		wwString.push_back(L"SM_DS_SwitchingA_R_D.fbx");
//		wwString.push_back(L"SM_DS_SwitchingA_R_X.fbx");
//		wwString.push_back(L"SM_DS_SwitchingA_R_Z.fbx");
//		wwString.push_back(L"SM_DS_T_BasicTrace_L.fbx");
//		wwString.push_back(L"SM_DS_Tornado_001.fbx");
//		wwString.push_back(L"SM_DS_Trail_01_L_D.fbx");
//		wwString.push_back(L"SM_DS_Trail_01_L_X.fbx");
//		wwString.push_back(L"SM_DS_Trail_01_L_Z.fbx");
//		wwString.push_back(L"SM_DS_Trail_02_L_D.fbx");
//		wwString.push_back(L"SM_DS_Trail_02_L_X.fbx");
//		wwString.push_back(L"SM_DS_Trail_02_L_Z.fbx");
//		wwString.push_back(L"SM_DS_Trail_02_R_D.fbx");
//		wwString.push_back(L"SM_DS_Trail_02_R_X.fbx");
//		wwString.push_back(L"SM_DS_Trail_02_R_X2.fbx");
//		wwString.push_back(L"SM_DS_Trail_02_R_Z.fbx");
//		wwString.push_back(L"SM_DS_Trail_Cross.fbx");
//		wwString.push_back(L"SM_DS_Trail_Cross_Invs.fbx");
//		wwString.push_back(L"SM_DS_Trail_Front.fbx");
//		wwString.push_back(L"SM_DS_Trail_Top.fbx");
//		wwString.push_back(L"SM_DS_Trail_Top_Spindle.fbx");
//		wwString.push_back(L"SM_DS_Trail_X.fbx");
//		wwString.push_back(L"SM_DS_Trail_Z.fbx");
//		wwString.push_back(L"SM_DS_Trail03_L.fbx");
//		wwString.push_back(L"SM_DualSword_Basic.fbx");
//		wwString.push_back(L"SM_DualSword_Basic_01.fbx");
//		wwString.push_back(L"SM_Dust01.fbx");
//		wwString.push_back(L"SM_DW_ring.fbx");
//		wwString.push_back(L"SM_DW_Weapon.fbx");
//		wwString.push_back(L"SM_Dynamite_03.fbx");
//		wwString.push_back(L"SM_EB_Bullet.fbx");
//		wwString.push_back(L"SM_EB_Bullet_01.fbx");
//		wwString.push_back(L"SM_EB_Bullet_Extend.fbx");
//		wwString.push_back(L"SM_EB_Rage_Bullet_.fbx");
//		wwString.push_back(L"SM_EB_Trap_01.fbx");
//		wwString.push_back(L"SM_EB_Trap_02.fbx");
//		wwString.push_back(L"SM_EB_Trap_03.fbx");
//		wwString.push_back(L"SM_EB_Trap_04.fbx");
//		wwString.push_back(L"SM_Edan_Sword.fbx");
//		wwString.push_back(L"SM_Edan_Sword_Tuto.fbx");
//		wwString.push_back(L"SM_eggshell.fbx");
//		wwString.push_back(L"SM_EM_AngelSword.fbx");
//		wwString.push_back(L"SM_EM_AngelSword_01.fbx");
//		wwString.push_back(L"SM_EM_barbell_01.fbx");
//		wwString.push_back(L"SM_EM_Batting.fbx");
//		wwString.push_back(L"SM_EM_Bench.fbx");
//		wwString.push_back(L"SM_EM_Boombox.fbx");
//		wwString.push_back(L"SM_EM_Bouquet.fbx");
//		wwString.push_back(L"SM_EM_Box.fbx");
//		wwString.push_back(L"SM_EM_BrokenKeyboard_01.fbx");
//		wwString.push_back(L"SM_EM_BrokenKeyboard_02.fbx");
//		wwString.push_back(L"SM_EM_Broom.fbx");
//		wwString.push_back(L"SM_EM_CampingChair.fbx");
//		wwString.push_back(L"SM_EM_CampingWood.fbx");
//		wwString.push_back(L"SM_EM_Chair.fbx");
//		wwString.push_back(L"SM_EM_Coin.fbx");
//		wwString.push_back(L"SM_EM_Cook_Dish.fbx");
//		wwString.push_back(L"SM_EM_Cook_fryingFan.fbx");
//		wwString.push_back(L"SM_EM_Copycat_03.fbx");
//		wwString.push_back(L"SM_EM_CopyCow_grass_01.fbx");
//		wwString.push_back(L"SM_EM_DanceGame.fbx");
//		wwString.push_back(L"SM_EM_DanceGame_01.fbx");
//		wwString.push_back(L"SM_EM_Dumbbell_01.fbx");
//		wwString.push_back(L"SM_EM_EyeCover.fbx");
//		wwString.push_back(L"SM_EM_FireworkRocket.fbx");
//		wwString.push_back(L"SM_EM_Flashlight.fbx");
//		wwString.push_back(L"SM_EM_Grank_01.fbx");
//		wwString.push_back(L"SM_EM_GRankS04.fbx");
//		wwString.push_back(L"SM_EM_GRankS04_Sword.fbx");
//		wwString.push_back(L"SM_EM_GRankS06.fbx");
//		wwString.push_back(L"SM_EM_Gun.fbx");
//		wwString.push_back(L"SM_EM_Hula.fbx");
//		wwString.push_back(L"SM_EM_Kettlebelll_01.fbx");
//		wwString.push_back(L"SM_EM_Keyboard_01.fbx");
//		wwString.push_back(L"SM_EM_Keyboard_part_01.fbx");
//		wwString.push_back(L"SM_EM_Lightning_01.fbx");
//		wwString.push_back(L"SM_EM_Lightning_02.fbx");
//		wwString.push_back(L"SM_EM_Lightning_06.fbx");
//		wwString.push_back(L"SM_EM_Lightning_07.fbx");
//		wwString.push_back(L"SM_EM_Lightning_08.fbx");
//		wwString.push_back(L"SM_EM_LoveLetter.fbx");
//		wwString.push_back(L"SM_EM_LoveLetter_02.fbx");
//		wwString.push_back(L"SM_EM_MagnifierBig.fbx");
//		wwString.push_back(L"SM_EM_MagnifierSmall.fbx");
//		wwString.push_back(L"SM_EM_Marionette.fbx");
//		wwString.push_back(L"SM_EM_Mic.fbx");
//		wwString.push_back(L"SM_EM_Mic_01.fbx");
//		wwString.push_back(L"SM_EM_MoneyGun.fbx");
//		wwString.push_back(L"SM_EM_MoneyPaper.fbx");
//		wwString.push_back(L"SM_EM_Mrank_05.fbx");
//		wwString.push_back(L"SM_EM_Notebook.fbx");
//		wwString.push_back(L"SM_EM_Phone.fbx");
//		wwString.push_back(L"SM_EM_PowerBow.fbx");
//		wwString.push_back(L"SM_EM_Powered01.fbx");
//		wwString.push_back(L"SM_EM_PowerSwordShield.fbx");
//		wwString.push_back(L"SM_EM_PowerTSword.fbx");
//		wwString.push_back(L"SM_EM_Pumpkin_06.fbx");
//		wwString.push_back(L"SM_EM_RoyalSaluteSword.fbx");
//		wwString.push_back(L"SM_EM_ShoulderPress_Bench_01.fbx");
//		wwString.push_back(L"SM_EM_Shuriken.fbx");
//		wwString.push_back(L"SM_EM_Shuriken_02.fbx");
//		wwString.push_back(L"SM_EM_Shuriken_03.fbx");
//		wwString.push_back(L"SM_EM_SkyLantern.fbx");
//		wwString.push_back(L"SM_EM_SpotLight.fbx");
//		wwString.push_back(L"SM_EM_SpotLight_01.fbx");
//		wwString.push_back(L"SM_EM_Sunbed.fbx");
//		wwString.push_back(L"SM_EM_SurfingBoard.fbx");
//		wwString.push_back(L"SM_EM_Tennis_Racket.fbx");
//		wwString.push_back(L"SM_EM_Tournament.fbx");
//		wwString.push_back(L"SM_EM_Two.fbx");
//		wwString.push_back(L"SM_EM_Ukulele.fbx");
//		wwString.push_back(L"SM_EM_VRGearL.fbx");
//		wwString.push_back(L"SM_EM_VRGearR.fbx");
//		wwString.push_back(L"SM_EM_VRHead.fbx");
//		wwString.push_back(L"SM_EM_WineGlass.fbx");
//		wwString.push_back(L"SM_EM_Zero.fbx");
//		wwString.push_back(L"SM_Embossing.fbx");
//		wwString.push_back(L"SM_FallingSun_Aura01.fbx");
//		wwString.push_back(L"SM_FallingSun_Aura02.fbx");
//		wwString.push_back(L"SM_FallingSun_Light02.fbx");
//		wwString.push_back(L"SM_FallingSun_Light03.fbx");
//		wwString.push_back(L"SM_FallingSun_Light04.fbx");
//		wwString.push_back(L"SM_FallingSun_Wind01.fbx");
//		wwString.push_back(L"SM_Fire_Cylinder_02.fbx");
//		wwString.push_back(L"SM_Fire_Cylinder_03.fbx");
//		wwString.push_back(L"SM_Fire_Piece.fbx");
//		wwString.push_back(L"SM_FireBall_Helix_Wind_01.fbx");
//		wwString.push_back(L"SM_FireBall_Helix_Wind_02.fbx");
//		wwString.push_back(L"SM_FireBall_Round_Wind_01.fbx");
//		wwString.push_back(L"SM_FireBall_Round_Wind_03.fbx");
//		wwString.push_back(L"SM_FireBall_Round_Wind_05.fbx");
//		wwString.push_back(L"SM_FireBall_Sphere_01.fbx");
//		wwString.push_back(L"SM_FireBall_Sphere_02.fbx");
//		wwString.push_back(L"SM_FireBall_Sphere_Front_01.fbx");
//		wwString.push_back(L"SM_FireBall_Sphere_Wind_01.fbx");
//		wwString.push_back(L"SM_FireBall_Sphere_Wind_03.fbx");
//		wwString.push_back(L"SM_Fireball_Sphere01.fbx");
//		wwString.push_back(L"SM_fish.fbx");
//		wwString.push_back(L"SM_FrontWind.fbx");
//		wwString.push_back(L"SM_FrontWind_02.fbx");
//		wwString.push_back(L"SM_FrozenSpear_Meteo03.fbx");
//		wwString.push_back(L"SM_FrozenSpear_Meteo04.fbx");
//		wwString.push_back(L"SM_Garvan_Trail_01.fbx");
//		wwString.push_back(L"SM_Garvan_Trail_02.fbx");
//		wwString.push_back(L"SM_Garvan_Trail_03.fbx");
//		wwString.push_back(L"SM_Garvan_Trail_04.fbx");
//		wwString.push_back(L"SM_Garvan_Trail_05.fbx");
//		wwString.push_back(L"SM_Garvan_Trail_06.fbx");
//		wwString.push_back(L"SM_Garvan_Weapon.fbx");
//		wwString.push_back(L"SM_Gem.fbx");
//		wwString.push_back(L"SM_GH_GodHands_Wing.fbx");
//		wwString.push_back(L"SM_GH_GodHands_Wing_01.fbx");
//		wwString.push_back(L"SM_GH_GodHands_Wing_02.fbx");
//		wwString.push_back(L"SM_GH_Guard_01.fbx");
//		wwString.push_back(L"SM_GH_Guard_cross.fbx");
//		wwString.push_back(L"SM_GH_Guard_cross_01.fbx");
//		wwString.push_back(L"SM_Ghost_Chase_Sphere01.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Hit01.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill01_L01.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill01_L02.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill01_R01.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill01_R02.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill02_L01.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill02_L02.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill04_L01.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill04_L02.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill05_Base.fbx");
//		wwString.push_back(L"SM_Giantic_Creature_Skill05_Edge.fbx");
//		wwString.push_back(L"SM_GN_Projectile_01.fbx");
//		wwString.push_back(L"SM_GN_Spring01.fbx");
//		wwString.push_back(L"SM_GN_Spring02.fbx");
//		wwString.push_back(L"SM_GN_Wind.fbx");
//		wwString.push_back(L"SM_GN_Wind01.fbx");
//		wwString.push_back(L"SM_Grudge_Wraith_skill05.fbx");
//		wwString.push_back(L"SM_Grudge_Wraith_skill05_mesh.fbx");
//		wwString.push_back(L"SM_Grudge_Wraith_skill05_mesh_02.fbx");
//		wwString.push_back(L"SM_GT_AirCombo01_01.fbx");
//		wwString.push_back(L"SM_GT_AirCombo01_02_Trace_Y.fbx");
//		wwString.push_back(L"SM_GT_AirCombo01_02_Trace_Z.fbx");
//		wwString.push_back(L"SM_GT_AirCombo02_SM_Air02_Fire_Y.fbx");
//		wwString.push_back(L"SM_GT_AirCombo02_SM_Air02_Main_X.fbx");
//		wwString.push_back(L"SM_GT_AirCombo02_SM_Air02_Main_Y.fbx");
//		wwString.push_back(L"SM_GT_AirCombo02_SM_Air02_Sub_X.fbx");
//		wwString.push_back(L"SM_GT_AirCombo02_SM_Air02_Sub_Y.fbx");
//		wwString.push_back(L"SM_GT_AirCombo03_Air03_Fire_X_Mesh.fbx");
//		wwString.push_back(L"SM_GT_AirCombo03_Air03_Main_X_Mesh.fbx");
//		wwString.push_back(L"SM_GT_AirCombo03_Air03_Main_Y_Mesh.fbx");
//		wwString.push_back(L"SM_GT_AirCombo03_Air03_Sub_X_Mesh.fbx");
//		wwString.push_back(L"SM_GT_AirCombo03_Air03_Sub_X_Mesh_1.fbx");
//		wwString.push_back(L"SM_GT_AirCombo03_Air03_Sub_Y_Mesh.fbx");
//		wwString.push_back(L"SM_GT_AirCombo04_Doddgy_Trace.fbx");
//		wwString.push_back(L"SM_GT_AirCombo04_Main_Trace.fbx");
//		wwString.push_back(L"SM_GT_AirCombo04_Sub_Trace.fbx");
//		wwString.push_back(L"SM_GT_AirComboEX_Trail_SM_Base.fbx");
//		wwString.push_back(L"SM_GT_AirComboEX_Trail_SM_Doddgy.fbx");
//		wwString.push_back(L"SM_GT_AirComboEX_Trail_SM_Sub.fbx");
//		wwString.push_back(L"SM_GT_Cyinder_01.fbx");
//		wwString.push_back(L"SM_GT_DashCombo_DashCombo_Doddgy_Mesh.fbx");
//		wwString.push_back(L"SM_GT_DashCombo_DashCombo_Main_Mesh.fbx");
//		wwString.push_back(L"SM_GT_DashCombo_DashCombo_Sub_Mesh.fbx");
//		wwString.push_back(L"SM_GT_Hellpunch_Line01_V.fbx");
//		wwString.push_back(L"SM_GT_SphereHarf.fbx");
//		wwString.push_back(L"SM_GT_Trail_Main_X01.fbx");
//		wwString.push_back(L"SM_Guadian_Skill07_Shild_Center.fbx");
//		wwString.push_back(L"SM_Guardian_Shield.fbx");
//		wwString.push_back(L"SM_Guardian_Shield_01.fbx");
//		wwString.push_back(L"SM_Guardian_Spear.fbx");
//		wwString.push_back(L"SM_Guardian_Trail_01.fbx");
//		wwString.push_back(L"SM_Guardian_Trail_01_2.fbx");
//		wwString.push_back(L"SM_Guardian_Trail_02.fbx");
//		wwString.push_back(L"SM_half_ball.fbx");
//		wwString.push_back(L"SM_half_ball_closed.fbx");
//		wwString.push_back(L"SM_half_ball_closed_1.fbx");
//		wwString.push_back(L"SM_half_cylinder.fbx");
//		wwString.push_back(L"SM_Harf_Donut00.fbx");
//		wwString.push_back(L"SM_Helix_01.fbx");
//		wwString.push_back(L"SM_Helix_01_Small.fbx");
//		wwString.push_back(L"SM_Helix_02.fbx");
//		wwString.push_back(L"SM_Helix_02_Small.fbx");
//		wwString.push_back(L"SM_helix_2.fbx");
//		wwString.push_back(L"SM_helix_04.fbx");
//		wwString.push_back(L"SM_Helix_05.fbx");
//		wwString.push_back(L"SM_Helix_07.fbx");
//		wwString.push_back(L"SM_Helix_08.fbx");
//		wwString.push_back(L"SM_Helix_09.fbx");
//		wwString.push_back(L"SM_Helix_Crystal.fbx");
//		wwString.push_back(L"SM_Helix_Crystal2.fbx");
//		wwString.push_back(L"SM_Helix_Crystal2_01.fbx");
//		wwString.push_back(L"SM_Helix_Crystal2_02.fbx");
//		wwString.push_back(L"SM_helix003.fbx");
//		wwString.push_back(L"SM_Hit_01.fbx");
//		wwString.push_back(L"SM_Hit_02.fbx");
//		wwString.push_back(L"SM_HolyCross02.fbx");
//		wwString.push_back(L"SM_HP_Sphere_01.fbx");
//		wwString.push_back(L"SM_ice2.fbx");
//		wwString.push_back(L"SM_IceTrap_Ice.fbx");
//		wwString.push_back(L"SM_IceWall_2.fbx");
//		wwString.push_back(L"SM_Incomplete_Palaspel_Weapon.fbx");
//		wwString.push_back(L"SM_Karin_Weapon_01.fbx");
//		wwString.push_back(L"SM_Karin_Weapon_02.fbx");
//		wwString.push_back(L"SM_Karin_Weapon_03.fbx");
//		wwString.push_back(L"SM_ky_auraHousya.fbx");
//		wwString.push_back(L"SM_ky_auraWall_distA.fbx");
//		wwString.push_back(L"SM_ky_auraWall_distB.fbx");
//		wwString.push_back(L"SM_ky_auraWallSq3.fbx");
//		wwString.push_back(L"SM_ky_auraWallSq4.fbx");
//		wwString.push_back(L"SM_ky_auraWallSq4_H.fbx");
//		wwString.push_back(L"SM_ky_circle.fbx");
//		wwString.push_back(L"SM_ky_circle_inv.fbx");
//		wwString.push_back(L"SM_ky_circle_inv_midpoly.fbx");
//		wwString.push_back(L"SM_ky_circle_midpoly.fbx");
//		wwString.push_back(L"SM_ky_cylinder_harf01.fbx");
//		wwString.push_back(L"SM_ky_cylinder_lowPoly.fbx");
//		wwString.push_back(L"SM_ky_cylinder_superLow.fbx");
//		wwString.push_back(L"SM_ky_cylinder_superLow_inv.fbx");
//		wwString.push_back(L"SM_ky_cylinderBend03_RE.fbx");
//		wwString.push_back(L"SM_ky_cylinderBend03_RE02.fbx");
//		wwString.push_back(L"SM_ky_doomBvert2Inv.fbx");
//		wwString.push_back(L"SM_ky_dust_triangle.fbx");
//		wwString.push_back(L"SM_ky_lans_rot.fbx");
//		wwString.push_back(L"SM_ky_needle01.fbx");
//		wwString.push_back(L"SM_ky_planeBend2.fbx");
//		wwString.push_back(L"SM_ky_quad_centerRot.fbx");
//		wwString.push_back(L"SM_ky_quadA.fbx");
//		wwString.push_back(L"SM_ky_quadA_02.fbx");
//		wwString.push_back(L"SM_ky_quadA_03.fbx");
//		wwString.push_back(L"SM_ky_quadCross.fbx");
//		wwString.push_back(L"SM_ky_quadCross02.fbx");
//		wwString.push_back(L"SM_ky_quadCross03.fbx");
//		wwString.push_back(L"SM_ky_quads.fbx");
//		wwString.push_back(L"SM_ky_rayOffset.fbx");
//		wwString.push_back(L"SM_ky_rayOffset2.fbx");
//		wwString.push_back(L"SM_ky_rock_single.fbx");
//		wwString.push_back(L"SM_ky_rock2B.fbx");
//		wwString.push_back(L"SM_ky_rock06.fbx");
//		wwString.push_back(L"SM_ky_rock07.fbx");
//		wwString.push_back(L"SM_ky_sphere.fbx");
//		wwString.push_back(L"SM_ky_sphere_dist.fbx");
//		wwString.push_back(L"SM_ky_sphere_dist_wind.fbx");
//		wwString.push_back(L"SM_ky_sphereHarf.fbx");
//		wwString.push_back(L"SM_ky_sphereHarf01Inv.fbx");
//		wwString.push_back(L"SM_ky_sphereHarf02_dist.fbx");
//		wwString.push_back(L"SM_ky_sphereHarf03.fbx");
//		wwString.push_back(L"SM_ky_sphereHarf05.fbx");
//		wwString.push_back(L"SM_ky_sphereIco.fbx");
//		wwString.push_back(L"SM_ky_sphereThree_01.fbx");
//		wwString.push_back(L"SM_ky_tornado3.fbx");
//		wwString.push_back(L"SM_ky_tornado6.fbx");
//		wwString.push_back(L"SM_ky_tornado7.fbx");
//		wwString.push_back(L"SM_ky_wallCylinder_center_smooth.fbx");
//		wwString.push_back(L"SM_ky_windLine10B.fbx");
//		wwString.push_back(L"SM_ky_windLine13b.fbx");
//		wwString.push_back(L"SM_ky_windLine19_1.fbx");
//		wwString.push_back(L"SM_ky_windLine20.fbx");
//		wwString.push_back(L"SM_ky_windLine22.fbx");
//		wwString.push_back(L"SM_ky_windLine23.fbx");
//		wwString.push_back(L"SM_ky_windLine24.fbx");
//		wwString.push_back(L"SM_ky_windLine26.fbx");
//		wwString.push_back(L"SM_ky_windLine27midPoly.fbx");
//		wwString.push_back(L"SM_ky_windLine29Inv.fbx");
//		wwString.push_back(L"SM_ky_windLine32.fbx");
//		wwString.push_back(L"SM_ky_windSmoke.fbx");
//		wwString.push_back(L"SM_ky_windSmoke_spin.fbx");
//		wwString.push_back(L"SM_ky_windSmokeDist.fbx");
//		wwString.push_back(L"SM_ky_windSmokeVertRot.fbx");
//		wwString.push_back(L"SM_ky_windSmokeVertRot_01.fbx");
//		wwString.push_back(L"SM_ky_windSmokeVertRot_02.fbx");
//		wwString.push_back(L"SM_ky_wing.fbx");
//		wwString.push_back(L"SM_L_DS_DashCombo_H_R.fbx");
//		wwString.push_back(L"SM_L_DS_Dashcombo_X_L.fbx");
//		wwString.push_back(L"SM_L_DS_DashCombo_X_R.fbx");
//		wwString.push_back(L"SM_L_DS_Dashcombo_Y_L.fbx");
//		wwString.push_back(L"SM_L_DS_DashCombo_Y_R.fbx");
//		wwString.push_back(L"SM_lans.fbx");
//		wwString.push_back(L"SM_lans_rot.fbx");
//		wwString.push_back(L"SM_lans_UV_HorizenFlip.fbx");
//		wwString.push_back(L"SM_lans_UV_VerticalFlip.fbx");
//		wwString.push_back(L"SM_LB_Arrow_001.fbx");
//		wwString.push_back(L"SM_LB_Arrow_002.fbx");
//		wwString.push_back(L"SM_LB_Arrow_003.fbx");
//		wwString.push_back(L"SM_LB_Arrow_004.fbx");
//		wwString.push_back(L"SM_LB_Arrow_005_1.fbx");
//		wwString.push_back(L"SM_LB_Arrow_005_2.fbx");
//		wwString.push_back(L"SM_LB_Arrow_005_3.fbx");
//		wwString.push_back(L"SM_LB_Arrow_005_4.fbx");
//		wwString.push_back(L"SM_LB_Arrow_Aura_01.fbx");
//		wwString.push_back(L"SM_LB_Arrow_Aura_01_Inv.fbx");
//		wwString.push_back(L"SM_LB_Arrow_Charging_001.fbx");
//		wwString.push_back(L"SM_LB_Arrow_Charging_002.fbx");
//		wwString.push_back(L"SM_LB_Arrow_Charging_003.fbx");
//		wwString.push_back(L"SM_LB_Arrow_Charging_004.fbx");
//		wwString.push_back(L"SM_LB_Arrow_Charging_005.fbx");
//		wwString.push_back(L"SM_LB_Arrow_Main_01.fbx");
//		wwString.push_back(L"SM_LB_Basic_Projectile_Base.fbx");
//		wwString.push_back(L"SM_LB_H_Arrow_aura.fbx");
//		wwString.push_back(L"SM_LB_H_Arrow_aura2.fbx");
//		wwString.push_back(L"SM_LB_Invisibility_01.fbx");
//		wwString.push_back(L"SM_LB_Invisibility_02.fbx");
//		wwString.push_back(L"SM_LB_Invisibility_03.fbx");
//		wwString.push_back(L"SM_LB_Projectile_01.fbx");
//		wwString.push_back(L"SM_LB_Rain_Arrow.fbx");
//		wwString.push_back(L"SM_Lighting.fbx");
//		wwString.push_back(L"SM_Lighting_01.fbx");
//		wwString.push_back(L"SM_Lightning_Base.fbx");
//		wwString.push_back(L"SM_Lightning_Cyl_00.fbx");
//		wwString.push_back(L"SM_Lire_Weapon_Tuto.fbx");
//		wwString.push_back(L"SM_LongBow_1020050.fbx");
//		wwString.push_back(L"SM_Map_Aura_1.fbx");
//		wwString.push_back(L"SM_Map_Aura_2.fbx");
//		wwString.push_back(L"SM_Map_Aura_3.fbx");
//		wwString.push_back(L"SM_Map_Aura_4.fbx");
//		wwString.push_back(L"SM_meteo.fbx");
//		wwString.push_back(L"SM_Meteo_Sphere.fbx");
//		wwString.push_back(L"SM_Meteo_Sphere3.fbx");
//		wwString.push_back(L"SM_Muzzle.fbx");
//		wwString.push_back(L"SM_Muzzle_01.fbx");
//		wwString.push_back(L"SM_Muzzle_02.fbx");
//		wwString.push_back(L"SM_Nae_Arrow.fbx");
//		wwString.push_back(L"SM_Nae_Arrow_01.fbx");
//		wwString.push_back(L"SM_Nae_Arrow_02.fbx");
//		wwString.push_back(L"SM_Nae_Arrow_03.fbx");
//		wwString.push_back(L"SM_Nae_Bow.fbx");
//		wwString.push_back(L"SM_Nae_kick.fbx");
//		wwString.push_back(L"SM_Nae_Pro_Base.fbx");
//		wwString.push_back(L"SM_OB_01.fbx");
//		wwString.push_back(L"SM_OB_Helix_01.fbx");
//		wwString.push_back(L"SM_OB_Helix_02.fbx");
//		wwString.push_back(L"SM_OB_Helix_03.fbx");
//		wwString.push_back(L"SM_Palaspel.fbx");
//		wwString.push_back(L"SM_Palaspel_01.fbx");
//		wwString.push_back(L"SM_Pheonix_Aura_Wind01.fbx");
//		wwString.push_back(L"SM_Pheonix_MagicSquare01.fbx");
//		wwString.push_back(L"SM_Pheonix_MagicSquare02.fbx");
//		wwString.push_back(L"SM_Pheonix_UP_Z_Wind01.fbx");
//		wwString.push_back(L"SM_Pheonix_UP_Z_Wind02.fbx");
//		wwString.push_back(L"SM_Pheonix_Wind01.fbx");
//		wwString.push_back(L"SM_Pheonix_Wind02.fbx");
//		wwString.push_back(L"SM_Pheonix_Wind03.fbx");
//		wwString.push_back(L"SM_Pheonix_Wind04.fbx");
//		wwString.push_back(L"SM_Pheonix_Wind05.fbx");
//		wwString.push_back(L"SM_Pheonix_Wing_01.fbx");
//		wwString.push_back(L"SM_Pheonix_Wing_02.fbx");
//		wwString.push_back(L"SM_PiercingArrow_Arrow.fbx");
//		wwString.push_back(L"SM_PiercingArrow_Aura_01.fbx");
//		wwString.push_back(L"SM_PiercingArrow_Aura_02.fbx");
//		wwString.push_back(L"SM_PiercingArrow_Aura_06.fbx");
//		wwString.push_back(L"SM_plane_pillar.fbx");
//		wwString.push_back(L"SM_PlaneSlash_01.fbx");
//		wwString.push_back(L"SM_quad_centerRot.fbx");
//		wwString.push_back(L"SM_RainDrop.fbx");
//		wwString.push_back(L"SM_Reorgea_Hand_L_New_888.fbx");
//		wwString.push_back(L"SM_Reorgea_Hand_R_New_3.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_cylinder.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_Dust_mesh.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_1.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_2.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_3.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_4.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_5.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_6.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_7.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_8.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone1_9.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone2_3.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone2_4.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone2_5.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone2_8.fbx");
//		wwString.push_back(L"SM_Reorgeaskil04_stone2_10.fbx");
//		wwString.push_back(L"SM_Ring_01.fbx");
//		wwString.push_back(L"SM_Ring_03.fbx");
//		wwString.push_back(L"SM_rockRing.fbx");
//		wwString.push_back(L"SM_Sae_Trail.fbx");
//		wwString.push_back(L"SM_Sae_Weapon.fbx");
//		wwString.push_back(L"SM_Sae_wind.fbx");
//		wwString.push_back(L"SM_sakura_patal.fbx");
//		wwString.push_back(L"SM_SC_01.fbx");
//		wwString.push_back(L"SM_SC_Helix_01.fbx");
//		wwString.push_back(L"SM_SC_Helix_02.fbx");
//		wwString.push_back(L"SM_SC_Helix_03.fbx");
//		wwString.push_back(L"SM_Scart_Doddgy_X.fbx");
//		wwString.push_back(L"SM_Scart_Main_X.fbx");
//		wwString.push_back(L"SM_Scart_Main_Z.fbx");
//		wwString.push_back(L"SM_Scart_Main_Z02.fbx");
//		wwString.push_back(L"SM_Scart_Main_Z03.fbx");
//		wwString.push_back(L"SM_Scart_Sub_Z.fbx");
//		wwString.push_back(L"SM_Scart_Wind_Z.fbx");
//		wwString.push_back(L"SM_SG_corn.fbx");
//		wwString.push_back(L"SM_SG_corn_1.fbx");
//		wwString.push_back(L"SM_SG_Projectile_Main_Y_High.fbx");
//		wwString.push_back(L"SM_SG_Projectile_Main_Y_High_01.fbx");
//		wwString.push_back(L"SM_SG_spin_sub.fbx");
//		wwString.push_back(L"SM_SG_tornado.fbx");
//		wwString.push_back(L"SM_SG_tornado_sub.fbx");
//		wwString.push_back(L"SM_SG_tornado_sub_2.fbx");
//		wwString.push_back(L"SM_SG_tornado_sub_2_02.fbx");
//		wwString.push_back(L"SM_Shard.fbx");
//		wwString.push_back(L"SM_shock_wind.fbx");
//		wwString.push_back(L"SM_Shot_Cylinder_01.fbx");
//		wwString.push_back(L"SM_Shot_Cylinder_03.fbx");
//		wwString.push_back(L"SM_side_wind_down.fbx");
//		wwString.push_back(L"SM_side_wind_golem.fbx");
//		wwString.push_back(L"SM_side_wind_golem_1.fbx");
//		wwString.push_back(L"SM_SP_End_X_L.fbx");
//		wwString.push_back(L"SM_SP_End_X_R.fbx");
//		wwString.push_back(L"SM_SP_End_Z_L.fbx");
//		wwString.push_back(L"SM_SP_End_Z_R.fbx");
//		wwString.push_back(L"SM_sphere.fbx");
//		wwString.push_back(L"SM_Sphere_01_Dom.fbx");
//		wwString.push_back(L"SM_Sphere_02_Dom.fbx");
//		wwString.push_back(L"SM_sphere_melon.fbx");
//		wwString.push_back(L"SM_Sphere_Plane_01.fbx");
//		wwString.push_back(L"SM_Sphere_RPG.fbx");
//		wwString.push_back(L"SM_SpherePlate_01.fbx");
//		wwString.push_back(L"SM_sphereUV_lowpoly.fbx");
//		wwString.push_back(L"SM_SpinningSlash_O_L_X.fbx");
//		wwString.push_back(L"SM_SpinningSlash_O_L_XD.fbx");
//		wwString.push_back(L"SM_SpinningSlash_O_L_Z.fbx");
//		wwString.push_back(L"SM_SpinningSlash_O_R_DX.fbx");
//		wwString.push_back(L"SM_SpinningSlash_O_R_X.fbx");
//		wwString.push_back(L"SM_SpinningSlash_O_R_Z.fbx");
//		wwString.push_back(L"SM_SS_Aura_01.fbx");
//		wwString.push_back(L"SM_SS_Aura_02.fbx");
//		wwString.push_back(L"SM_SS_Shield.fbx");
//		wwString.push_back(L"SM_SS_Shield_pivot.fbx");
//		wwString.push_back(L"SM_SS_Shield_Throw.fbx");
//		wwString.push_back(L"SM_SS_ShieldEdge.fbx");
//		wwString.push_back(L"SM_SS_Stone_01.fbx");
//		wwString.push_back(L"SM_SS_Stone_02.fbx");
//		wwString.push_back(L"SM_SS_Stone_03.fbx");
//		wwString.push_back(L"SM_SS_Trail_01.fbx");
//		wwString.push_back(L"SM_SS_Trail_02.fbx");
//		wwString.push_back(L"SM_SS_Trail_03.fbx");
//		wwString.push_back(L"SM_ST3_Portal_00.fbx");
//		wwString.push_back(L"SM_ST3_Portal_01.fbx");
//		wwString.push_back(L"SM_Stone_01.fbx");
//		wwString.push_back(L"SM_Stone_02.fbx");
//		wwString.push_back(L"SM_Stone_L.fbx");
//		wwString.push_back(L"SM_Stone_R.fbx");
//		wwString.push_back(L"SM_Survivor_Buff.fbx");
//		wwString.push_back(L"SM_Survivor_Buff_plane.fbx");
//		wwString.push_back(L"SM_Text_Floor.fbx");
//		wwString.push_back(L"SM_Theo_Trail_01.fbx");
//		wwString.push_back(L"SM_Theo_Trail_02.fbx");
//		wwString.push_back(L"SM_Theo_Trail01.fbx");
//		wwString.push_back(L"SM_ToonKaboom01.fbx");
//		wwString.push_back(L"SM_Tornado_01.fbx");
//		wwString.push_back(L"SM_Tornado_02.fbx");
//		wwString.push_back(L"SM_Tornado_360_01.fbx");
//		wwString.push_back(L"SM_Trail_Line.fbx");
//		wwString.push_back(L"SM_Trail_Twist_02.fbx");
//		wwString.push_back(L"SM_Trail_Twist_03.fbx");
//		wwString.push_back(L"SM_tray.fbx");
//		wwString.push_back(L"SM_TS_Basic_combo_01_Front.fbx");
//		wwString.push_back(L"SM_TS_Basic_combo_01_Top.fbx");
//		wwString.push_back(L"SM_TS_Basic_combo_Cyl01.fbx");
//		wwString.push_back(L"SM_TS_Basic_combo_EX2_Aura.fbx");
//		wwString.push_back(L"SM_TS_Basic_combo_EX2_Sword.fbx");
//		wwString.push_back(L"SM_TS_Basic_Trail_Main_X.fbx");
//		wwString.push_back(L"SM_TS_Basic_Trail_Main_Z.fbx");
//		wwString.push_back(L"SM_TS_Basic_Trail_Main_Z_UVHorizenFlip.fbx");
//		wwString.push_back(L"SM_TS_Basic_Trail_Slash_X.fbx");
//		wwString.push_back(L"SM_TS_Basic_Trail_Sub_X.fbx");
//		wwString.push_back(L"SM_TS_Basic_Trail_Sub_Z.fbx");
//		wwString.push_back(L"SM_TS_Double_Slash_01.fbx");
//		wwString.push_back(L"SM_TS_Double_Slash_02.fbx");
//		wwString.push_back(L"SM_TS_Dragon_Weapon.fbx");
//		wwString.push_back(L"SM_TS_Trace_01_Re.fbx");
//		wwString.push_back(L"SM_Upright_noFloor.fbx");
//		wwString.push_back(L"SM_wallCylinder_center_smooth.fbx");
//		wwString.push_back(L"SM_Wave_01.fbx");
//		wwString.push_back(L"SM_Wave_02.fbx");
//		wwString.push_back(L"SM_Wind_Line13b_F.fbx");
//		wwString.push_back(L"SM_Wind_Line13b_R.fbx");
//		wwString.push_back(L"SM_Wind_M02.fbx");
//		wwString.push_back(L"SM_Wind_RTR_Base.fbx");
//		wwString.push_back(L"SM_Wind_RTR_Base2.fbx");
//		wwString.push_back(L"SM_windLine11C.fbx");
//		wwString.push_back(L"SM_windLine13_multi.fbx");
//		wwString.push_back(L"SM_windLine15.fbx");
//		wwString.push_back(L"SM_windLine16.fbx");
//		wwString.push_back(L"SM_windLine16_midPoly.fbx");
//		wwString.push_back(L"SM_windLine19.fbx");
//		wwString.push_back(L"SM_windMesh.fbx");
//		wwString.push_back(L"SM_windMesh_a.fbx");
//		wwString.push_back(L"SM_windMesh_b.fbx");
//		wwString.push_back(L"SM_windSmoke.fbx");
//		wwString.push_back(L"SM_Wing_T_01.fbx");
//	}
//
//	//  wwPro.push_back(L"Prototype_Mesh_ENV_BLD_Palace_02");
//
//	{
//
//		wwPro.push_back(L"Prototype_Mesh_01_R_Punch_SM_01_Mesh003");
//		wwPro.push_back(L"Prototype_Mesh_01_R_Punch_SM_01_Mesh004");
//		wwPro.push_back(L"Prototype_Mesh_02_L_Upper_SM");
//		wwPro.push_back(L"Prototype_Mesh_Aura_Arrow");
//		wwPro.push_back(L"Prototype_Mesh_Aura_Arrow_Wing");
//		wwPro.push_back(L"Prototype_Mesh_auraHousya");
//		wwPro.push_back(L"Prototype_Mesh_chantDeco2");
//		wwPro.push_back(L"Prototype_Mesh_circle");
//		wwPro.push_back(L"Prototype_Mesh_circle_inv");
//		wwPro.push_back(L"Prototype_Mesh_circleDist2");
//		wwPro.push_back(L"Prototype_Mesh_circleDist4");
//		wwPro.push_back(L"Prototype_Mesh_circleDist5");
//		wwPro.push_back(L"Prototype_Mesh_circleDist5_02");
//		wwPro.push_back(L"Prototype_Mesh_doomBvert");
//		wwPro.push_back(L"Prototype_Mesh_doomBvert2Inv");
//		wwPro.push_back(L"Prototype_Mesh_FX_Aura_Trail_02");
//		wwPro.push_back(L"Prototype_Mesh_FX_Aura_Trail_05");
//		wwPro.push_back(L"Prototype_Mesh_Grudge_Wraith_weapon_mesh");
//		wwPro.push_back(L"Prototype_Mesh_ice2");
//		wwPro.push_back(L"Prototype_Mesh_Line_SM_Low_R_Re");
//		wwPro.push_back(L"Prototype_Mesh_Lire_Leaf01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Ama");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Ama_Sword");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_BasicAttack_02_04");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_cylinder");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Cylinder_03");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Dust08_SUB_5X4_L");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Dust08_SUB_5X4_R");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Arrow_Square_M01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Arrow_Square_M02");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Arrow_Square_M03");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Arrow_Square_M04");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Arrow_Wing_L");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Arrow_Wing_R");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_CIRCLE");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Grand_Git");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Hit_M01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFB_Uplight_M01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFH_Dash_Wind01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFH_Dash_Wind02");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFH_Dash_Wind03");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFS_Charge_Wind_01_Inv");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFS_Charge_Wind_02");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFS_ChargeAttack_M01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_HFS_ChargeAttack_M02");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Leap_Ground_Light");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Leap_Ground_Light2");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Leap_Ground_M04");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Leap_Wind_M01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Leap_Wind_M02");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Plane_03");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Trace_M01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Trace_M02");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Trace_M03");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Trace_M04");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Trace_M05");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Trace_M06");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_Trace_M07");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_TS_Double_Slash_01");
//		wwPro.push_back(L"Prototype_Mesh_Mesh_TS_Double_Slash_05");
//		wwPro.push_back(L"Prototype_Mesh_MS_Arc_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Cloud");
//		wwPro.push_back(L"Prototype_Mesh_MS_Cylinder_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Cylinder_02");
//		wwPro.push_back(L"Prototype_Mesh_MS_Cylinder_05");
//		wwPro.push_back(L"Prototype_Mesh_MS_GeoSphere");
//		wwPro.push_back(L"Prototype_Mesh_MS_HalfSphere");
//		wwPro.push_back(L"Prototype_Mesh_MS_Land_Half_Cylinder");
//		wwPro.push_back(L"Prototype_Mesh_MS_LandCrack_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Line_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Line_01");
//		wwPro.push_back(L"Prototype_Mesh_MS_Line_02");
//		wwPro.push_back(L"Prototype_Mesh_MS_Line_03");
//		wwPro.push_back(L"Prototype_Mesh_MS_Line_04");
//		wwPro.push_back(L"Prototype_Mesh_MS_Line_05");
//		wwPro.push_back(L"Prototype_Mesh_MS_Shield_Sphere_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Shot_Cylinder_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Shpere_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_SmokePlane");
//		wwPro.push_back(L"Prototype_Mesh_MS_Sphere_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Sphere_Bill_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Sphere_Bill_01");
//		wwPro.push_back(L"Prototype_Mesh_MS_ST3_Portal_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_ST3_Portal_01");
//		wwPro.push_back(L"Prototype_Mesh_MS_St3_Tornado_Bottom_Cyl_01");
//		wwPro.push_back(L"Prototype_Mesh_MS_St3_Tornado_Bottom_Cyl_10");
//		wwPro.push_back(L"Prototype_Mesh_MS_Tornado_Cyl");
//		wwPro.push_back(L"Prototype_Mesh_MS_Tornado_Cyl_2side");
//		wwPro.push_back(L"Prototype_Mesh_MS_Tornado_Cyl_2side_01");
//		wwPro.push_back(L"Prototype_Mesh_MS_Tornado_Cyl_2side_03");
//		wwPro.push_back(L"Prototype_Mesh_MS_Tornado_Helix_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Trail_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Trail_Twist_00");
//		wwPro.push_back(L"Prototype_Mesh_MS_Trail_Twist_01");
//		wwPro.push_back(L"Prototype_Mesh_MS_Twis_Attack");
//		wwPro.push_back(L"Prototype_Mesh_MS_Twis_Attack_01");
//		wwPro.push_back(L"Prototype_Mesh_myCylinderKaiRotB");
//		wwPro.push_back(L"Prototype_Mesh_mySphere");
//		wwPro.push_back(L"Prototype_Mesh_S_BasicAttack00_M01");
//		wwPro.push_back(L"Prototype_Mesh_S_BasicAttack00_M03");
//		wwPro.push_back(L"Prototype_Mesh_S_JumpAttack_02_M01");
//		wwPro.push_back(L"Prototype_Mesh_S_JumpAttack_02_M02");
//		wwPro.push_back(L"Prototype_Mesh_S_JumpAttack_02_M04");
//		wwPro.push_back(L"Prototype_Mesh_Sample_Mesh_Fake_Mesh_Lighting");
//		wwPro.push_back(L"Prototype_Mesh_Sample_Mesh_Fresnal");
//		wwPro.push_back(L"Prototype_Mesh_Sample_Mesh_Hit");
//		wwPro.push_back(L"Prototype_Mesh_Sample_Mesh_WP_Cylinder_Z");
//		wwPro.push_back(L"Prototype_Mesh_shield");
//		wwPro.push_back(L"Prototype_Mesh_SK_03_L_Foot");
//		wwPro.push_back(L"Prototype_Mesh_SK_BF_Skill07_AuraMesh");
//		wwPro.push_back(L"Prototype_Mesh_SK_BF_Skill07_RectangleMesh");
//		wwPro.push_back(L"Prototype_Mesh_SK_EM_CatPot_Stick");
//		wwPro.push_back(L"Prototype_Mesh_SK_Guadian_Skill07_Loop");
//		wwPro.push_back(L"Prototype_Mesh_SK_Guadian_Skill07_Loop_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_02_L_Upper_Mesh02");
//		wwPro.push_back(L"Prototype_Mesh_SM_03_L_Foot_SM_03_L_Foot_M1");
//		wwPro.push_back(L"Prototype_Mesh_SM_03_L_Foot_SM_03_L_Foot_M2");
//		wwPro.push_back(L"Prototype_Mesh_SM_03_L_Foot_SM_03_L_Foot_M3");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_DarkTails_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_IceShards_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_IceShards_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_IceShards_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_IceSpike_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_ImpactFX_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_ImpactFX_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightCastRing_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightCircle_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightCircle_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightningArcs_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightningArcs_02_re");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightningArcs_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightningArcs_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightningArcs_05");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightningBolt_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightningOrb_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightRay_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_LightSpike_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_RegularPlane_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_RegularPlane_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_4E_SpiralRay_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_06_06_SM001");
//		wwPro.push_back(L"Prototype_Mesh_SM_06_06_SM002");
//		wwPro.push_back(L"Prototype_Mesh_SM_06_06_SM003");
//		wwPro.push_back(L"Prototype_Mesh_SM_07_Mesh001");
//		wwPro.push_back(L"Prototype_Mesh_SM_07_Mesh002");
//		wwPro.push_back(L"Prototype_Mesh_SM_07_Mesh003");
//		wwPro.push_back(L"Prototype_Mesh_SM_21_Christmas_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Lightning_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Lightning_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Lightning_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Lightning_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Trail_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Trail_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Trail_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Tree");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Wind_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Wind_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_AB_Wind_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_AOG_Helix01");
//		wwPro.push_back(L"Prototype_Mesh_SM_AOG_Helix02");
//		wwPro.push_back(L"Prototype_Mesh_SM_AOG_Plan01");
//		wwPro.push_back(L"Prototype_Mesh_SM_AOG_Ring");
//		wwPro.push_back(L"Prototype_Mesh_SM_AOG_Weapon");
//		wwPro.push_back(L"Prototype_Mesh_SM_AOG_Weapon_Jewllery");
//		wwPro.push_back(L"Prototype_Mesh_SM_Arc_00");
//		wwPro.push_back(L"Prototype_Mesh_SM_Arc_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Arrow_Circle_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Arrow_Circle_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Arrow_Wing_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Arrow_Wing_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_ArrowBurstTrail_01_New");
//		wwPro.push_back(L"Prototype_Mesh_SM_ArrowBurstTrail_02_New");
//		wwPro.push_back(L"Prototype_Mesh_SM_ArrowCase");
//		wwPro.push_back(L"Prototype_Mesh_SM_Asura_Cillynder01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Asura_Circle01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Asura_Sphere02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Aura_Foot_L_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Aura_Linear_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Aura_Linear_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Aura_Trail01");
//		wwPro.push_back(L"Prototype_Mesh_SM_auraHousya");
//		wwPro.push_back(L"Prototype_Mesh_SM_auraWallSq");
//		wwPro.push_back(L"Prototype_Mesh_SM_auraWallSq2");
//		wwPro.push_back(L"Prototype_Mesh_SM_AXTrail_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Baren_Weapon");
//		wwPro.push_back(L"Prototype_Mesh_SM_Baren_Weapon_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_Baren_Weapon_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_Cy01_Cylinder001");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_EX02_Basic_EX02_Doddgy");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_EX02_Basic_EX02_Main");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_EX02_Basic_EX02_Sub");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_EX02_Basic_EX02_Sub_vectorUV");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_EX03_2_Doddgy_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_EX03_2_Main_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_EX03_2_Sub_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_Trail_SM_Dodggy_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_Trail_SM_Dodggy_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_Trail_SM_Main_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_Trail_SM_Main_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_Trail_SM_Sub_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_Trail_SM_Sub_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_Basic_windLine");
//		wwPro.push_back(L"Prototype_Mesh_SM_BBT_Paw");
//		wwPro.push_back(L"Prototype_Mesh_SM_BBT_Paw_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_BBT_Teeth_Down");
//		wwPro.push_back(L"Prototype_Mesh_SM_BBT_Teeth_UP");
//		wwPro.push_back(L"Prototype_Mesh_SM_BerserkerRush_01_Doddgy_Trail_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_BerserkerRush_01_Doddgy_Trail_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_BerserkerRush_01_Main_Trail_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_BerserkerRush_01_Main_Trail_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_BerserkerRush_01_Sub_Trail_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_BerserkerRush_01_Sub_Trail_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_Base");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_Base_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_Doddgy");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_Doddgy_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_Sub");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_Sub_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_X_Small");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_XY_SM_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_XY_SM_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_2_Y_Small");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_03_01_D1");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_03_01_D2");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_03_01_Main");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_Skill_01_03_01_Sub");
//		wwPro.push_back(L"Prototype_Mesh_SM_BF_XY");
//		wwPro.push_back(L"Prototype_Mesh_SM_Bow_Base");
//		wwPro.push_back(L"Prototype_Mesh_SM_Bow_Em_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Bow_Em_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Bow_Em_Wing_T");
//		wwPro.push_back(L"Prototype_Mesh_SM_bowing_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_bowing_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_bowing_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_bowing_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_bowing_05");
//		wwPro.push_back(L"Prototype_Mesh_SM_Box_Basic");
//		wwPro.push_back(L"Prototype_Mesh_SM_BufRing_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_BufSwirl_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_BufSwirl_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Bullet");
//		wwPro.push_back(L"Prototype_Mesh_SM_Chain");
//		wwPro.push_back(L"Prototype_Mesh_SM_Chain_Buff");
//		wwPro.push_back(L"Prototype_Mesh_SM_Chaos_Dust_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Chaos_leg");
//		wwPro.push_back(L"Prototype_Mesh_SM_Chaos_legMesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cine_Mask_00");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cine_Mask_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cine_Mask_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cine_Mask_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cine_Mask_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cine_Nae_Bow");
//		wwPro.push_back(L"Prototype_Mesh_SM_circle");
//		wwPro.push_back(L"Prototype_Mesh_SM_circle_inv");
//		wwPro.push_back(L"Prototype_Mesh_SM_Circle_Magic");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cone_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_ControlPointMatch_Square_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_ControlPointMatch_Square_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_CoreRing_3");
//		wwPro.push_back(L"Prototype_Mesh_SM_CoreRing_4");
//		wwPro.push_back(L"Prototype_Mesh_SM_Crack_In_H");
//		wwPro.push_back(L"Prototype_Mesh_SM_Crack_In_V");
//		wwPro.push_back(L"Prototype_Mesh_SM_Crack_Out_H");
//		wwPro.push_back(L"Prototype_Mesh_SM_Crack_Out_V");
//		wwPro.push_back(L"Prototype_Mesh_SM_Crack_OutLine");
//		wwPro.push_back(L"Prototype_Mesh_SM_Critical_R_D_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_Critical_R_X_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_Critical_R_Z_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cross");
//		wwPro.push_back(L"Prototype_Mesh_SM_Crossbowman_Arrow_chork");
//		wwPro.push_back(L"Prototype_Mesh_SM_CS_SWTrail_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_CS_SWTrail_02UFlip");
//		wwPro.push_back(L"Prototype_Mesh_SM_CS_SWTrail_03UFlip");
//		wwPro.push_back(L"Prototype_Mesh_SM_CS_Trail_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_CW_Trail_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_Cylinder");
//		wwPro.push_back(L"Prototype_Mesh_SM_cylinderKaiRotB_vc");
//		wwPro.push_back(L"Prototype_Mesh_SM_Darknight_Skill01_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Darknight_Skill01_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Darknight_Skill01_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Darknight_Skill01_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_DivineLaserBeam_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_doomBvert2Inv");
//		wwPro.push_back(L"Prototype_Mesh_SM_Dough");
//		wwPro.push_back(L"Prototype_Mesh_SM_DoughSide");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_250010_L_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirCombo01_D_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirCombo01_D_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirCombo01_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirCombo01_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirCombo01_Z_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirCombo01_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_02_L_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_02_L_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_02_L_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_02_R_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_02_R_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_02_R_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_03_L_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_03_L_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_03_L_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_03_R_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_03_R_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_AirTrail_03_R_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_01_2_Trail_H");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_01_2_Trail_P");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_01_2_Trail_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_01_Trail_H");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_01_Trail_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_01_Trail_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_02_1_X_Doddgy");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_02_1_X_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_02_1_Z_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_02_2_Cross");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_1_X_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_1_Z_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_2_X_Orign");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_2_Z_Orign");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_3_D_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_3_D_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_3_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_3_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_3_Z_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic_03_3_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_1_X_Doddgy_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_1_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_1_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_2_X_Doddgy_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_2_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_2_Z_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_3_X_Doddgy_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_3_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic04_3_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_1_R_Doddgy");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_1_R_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_1_R_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_2_L_Doddgy");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_2_L_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_2_L_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_3_R_Doddgy");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_3_R_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Basic05_3_R_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_BasicTrace_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_D_L_1_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_D_L_2_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_D_R_1_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_D_R_2_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_X_L_1_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_X_L_2_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_X_R_1_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_X_R_2_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_Z_L_1_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_Z_L_2_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_Z_R_1_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Blade_Z_R_2_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Dash_Dust_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Dragon_Weapon");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Dust_Dash_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Dust_Dash_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_F_BasicTrace_G_02_SM_DS_F_BasicTrace_G_LOD0");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Fated_Proj_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Fated_Proj_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Fated_Proj_Z_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Fated_Proj_Z_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_F_L003");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_F_L004");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_F_L005");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_F_R003");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_F_R004");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_F_R005");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Front_L001");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Front_R001");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Front_R002_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Front_R002_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_L001");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_L003");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_L004");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_L005");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_R001");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_R002_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_R002_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_R003");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_R004");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_FatedCircle_Top_R005");
//		wwPro.push_back(L"Prototype_Mesh_SM_Ds_Genocide_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Genocide_Trace_D_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Genocide_Trace_D_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Genocide_Trace_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Genocide_Trace_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Genocide_Trace_Z_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Genocide_Trace_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Helix_Aura_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Plane_D_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Plane_D_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Plane_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Plane_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Plane_Z_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Plane_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Projectile_Main_X_High");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Projectile_Main_Y_High");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Projectile_Sub_Y_High");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Projectile_Wind_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Projectile_Wind_Y_High");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Projectile_Wind_Y_High02");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Buff_L_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Buff_L_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Buff_L_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Buff_R_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Buff_R_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Buff_R_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_D_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_D_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_Z_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Rage_Genocide_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Scart_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Scart_X_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Scart_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Skill_Fate_Trail_Front01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Skill_Fate_Trail_Stop_Front02");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Skill_Fate_Trail_Stop_Top01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Skill_Fate_Trail_Stop_Top02");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Skill_Trace_Front01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Skill_Trace_Top01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SP_Charging_Ground_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SP_Charging_Ground_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SP_EX1_D_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SP_EX1_D_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SP_EX1_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SP_EX1_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SP_EX1_Z_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SP_EX1_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Switching_L_D_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Switching_L_X_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Switching_L_Z_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Switching_R_D_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Switching_R_X_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Switching_R_Z_Plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SwitchingA_L_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SwitchingA_L_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SwitchingA_L_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SwitchingA_R_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SwitchingA_R_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_SwitchingA_R_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_T_BasicTrace_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Tornado_001");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_01_L_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_01_L_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_01_L_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_02_L_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_02_L_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_02_L_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_02_R_D");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_02_R_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_02_R_X2");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_02_R_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_Cross");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_Cross_Invs");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_Front");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_Top");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_Top_Spindle");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_DS_Trail03_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_DualSword_Basic");
//		wwPro.push_back(L"Prototype_Mesh_SM_DualSword_Basic_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Dust01");
//		wwPro.push_back(L"Prototype_Mesh_SM_DW_ring");
//		wwPro.push_back(L"Prototype_Mesh_SM_DW_Weapon");
//		wwPro.push_back(L"Prototype_Mesh_SM_Dynamite_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_EB_Bullet");
//		wwPro.push_back(L"Prototype_Mesh_SM_EB_Bullet_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EB_Bullet_Extend");
//		wwPro.push_back(L"Prototype_Mesh_SM_EB_Rage_Bullet_");
//		wwPro.push_back(L"Prototype_Mesh_SM_EB_Trap_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EB_Trap_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_EB_Trap_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_EB_Trap_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_Edan_Sword");
//		wwPro.push_back(L"Prototype_Mesh_SM_Edan_Sword_Tuto");
//		wwPro.push_back(L"Prototype_Mesh_SM_eggshell");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_AngelSword");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_AngelSword_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_barbell_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Batting");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Bench");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Boombox");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Bouquet");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Box");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_BrokenKeyboard_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_BrokenKeyboard_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Broom");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_CampingChair");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_CampingWood");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Chair");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Coin");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Cook_Dish");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Cook_fryingFan");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Copycat_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_CopyCow_grass_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_DanceGame");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_DanceGame_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Dumbbell_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_EyeCover");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_FireworkRocket");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Flashlight");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Grank_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_GRankS04");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_GRankS04_Sword");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_GRankS06");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Gun");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Hula");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Kettlebelll_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Keyboard_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Keyboard_part_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Lightning_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Lightning_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Lightning_06");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Lightning_07");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Lightning_08");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_LoveLetter");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_LoveLetter_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_MagnifierBig");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_MagnifierSmall");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Marionette");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Mic");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Mic_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_MoneyGun");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_MoneyPaper");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Mrank_05");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Notebook");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Phone");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_PowerBow");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Powered01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_PowerSwordShield");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_PowerTSword");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Pumpkin_06");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_RoyalSaluteSword");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_ShoulderPress_Bench_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Shuriken");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Shuriken_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Shuriken_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_SkyLantern");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_SpotLight");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_SpotLight_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Sunbed");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_SurfingBoard");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Tennis_Racket");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Tournament");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Two");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Ukulele");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_VRGearL");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_VRGearR");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_VRHead");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_WineGlass");
//		wwPro.push_back(L"Prototype_Mesh_SM_EM_Zero");
//		wwPro.push_back(L"Prototype_Mesh_SM_Embossing");
//		wwPro.push_back(L"Prototype_Mesh_SM_FallingSun_Aura01");
//		wwPro.push_back(L"Prototype_Mesh_SM_FallingSun_Aura02");
//		wwPro.push_back(L"Prototype_Mesh_SM_FallingSun_Light02");
//		wwPro.push_back(L"Prototype_Mesh_SM_FallingSun_Light03");
//		wwPro.push_back(L"Prototype_Mesh_SM_FallingSun_Light04");
//		wwPro.push_back(L"Prototype_Mesh_SM_FallingSun_Wind01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Fire_Cylinder_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Fire_Cylinder_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Fire_Piece");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Helix_Wind_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Helix_Wind_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Round_Wind_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Round_Wind_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Round_Wind_05");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Sphere_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Sphere_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Sphere_Front_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Sphere_Wind_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_FireBall_Sphere_Wind_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Fireball_Sphere01");
//		wwPro.push_back(L"Prototype_Mesh_SM_fish");
//		wwPro.push_back(L"Prototype_Mesh_SM_FrontWind");
//		wwPro.push_back(L"Prototype_Mesh_SM_FrontWind_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_FrozenSpear_Meteo03");
//		wwPro.push_back(L"Prototype_Mesh_SM_FrozenSpear_Meteo04");
//		wwPro.push_back(L"Prototype_Mesh_SM_Garvan_Trail_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Garvan_Trail_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Garvan_Trail_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Garvan_Trail_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_Garvan_Trail_05");
//		wwPro.push_back(L"Prototype_Mesh_SM_Garvan_Trail_06");
//		wwPro.push_back(L"Prototype_Mesh_SM_Garvan_Weapon");
//		wwPro.push_back(L"Prototype_Mesh_SM_Gem");
//		wwPro.push_back(L"Prototype_Mesh_SM_GH_GodHands_Wing");
//		wwPro.push_back(L"Prototype_Mesh_SM_GH_GodHands_Wing_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_GH_GodHands_Wing_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_GH_Guard_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_GH_Guard_cross");
//		wwPro.push_back(L"Prototype_Mesh_SM_GH_Guard_cross_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Ghost_Chase_Sphere01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Hit01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill01_L01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill01_L02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill01_R01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill01_R02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill02_L01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill02_L02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill04_L01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill04_L02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill05_Base");
//		wwPro.push_back(L"Prototype_Mesh_SM_Giantic_Creature_Skill05_Edge");
//		wwPro.push_back(L"Prototype_Mesh_SM_GN_Projectile_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_GN_Spring01");
//		wwPro.push_back(L"Prototype_Mesh_SM_GN_Spring02");
//		wwPro.push_back(L"Prototype_Mesh_SM_GN_Wind");
//		wwPro.push_back(L"Prototype_Mesh_SM_GN_Wind01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Grudge_Wraith_skill05");
//		wwPro.push_back(L"Prototype_Mesh_SM_Grudge_Wraith_skill05_mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_Grudge_Wraith_skill05_mesh_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo01_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo01_02_Trace_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo01_02_Trace_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo02_SM_Air02_Fire_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo02_SM_Air02_Main_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo02_SM_Air02_Main_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo02_SM_Air02_Sub_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo02_SM_Air02_Sub_Y");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo03_Air03_Fire_X_Mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo03_Air03_Main_X_Mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo03_Air03_Main_Y_Mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo03_Air03_Sub_X_Mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo03_Air03_Sub_X_Mesh_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo03_Air03_Sub_Y_Mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo04_Doddgy_Trace");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo04_Main_Trace");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirCombo04_Sub_Trace");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirComboEX_Trail_SM_Base");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirComboEX_Trail_SM_Doddgy");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_AirComboEX_Trail_SM_Sub");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_Cyinder_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_DashCombo_DashCombo_Doddgy_Mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_DashCombo_DashCombo_Main_Mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_DashCombo_DashCombo_Sub_Mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_Hellpunch_Line01_V");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_SphereHarf");
//		wwPro.push_back(L"Prototype_Mesh_SM_GT_Trail_Main_X01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Guadian_Skill07_Shild_Center");
//		wwPro.push_back(L"Prototype_Mesh_SM_Guardian_Shield");
//		wwPro.push_back(L"Prototype_Mesh_SM_Guardian_Shield_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Guardian_Spear");
//		wwPro.push_back(L"Prototype_Mesh_SM_Guardian_Trail_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Guardian_Trail_01_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_Guardian_Trail_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_half_ball");
//		wwPro.push_back(L"Prototype_Mesh_SM_half_ball_closed");
//		wwPro.push_back(L"Prototype_Mesh_SM_half_ball_closed_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_half_cylinder");
//		wwPro.push_back(L"Prototype_Mesh_SM_Harf_Donut00");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_01_Small");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_02_Small");
//		wwPro.push_back(L"Prototype_Mesh_SM_helix_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_helix_04");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_05");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_07");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_08");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_09");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_Crystal");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_Crystal2");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_Crystal2_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Helix_Crystal2_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_helix003");
//		wwPro.push_back(L"Prototype_Mesh_SM_Hit_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Hit_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_HolyCross02");
//		wwPro.push_back(L"Prototype_Mesh_SM_HP_Sphere_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_ice2");
//		wwPro.push_back(L"Prototype_Mesh_SM_IceTrap_Ice");
//		wwPro.push_back(L"Prototype_Mesh_SM_IceWall_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_Incomplete_Palaspel_Weapon");
//		wwPro.push_back(L"Prototype_Mesh_SM_Karin_Weapon_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Karin_Weapon_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Karin_Weapon_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_auraHousya");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_auraWall_distA");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_auraWall_distB");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_auraWallSq3");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_auraWallSq4");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_auraWallSq4_H");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_circle");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_circle_inv");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_circle_inv_midpoly");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_circle_midpoly");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_cylinder_harf01");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_cylinder_lowPoly");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_cylinder_superLow");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_cylinder_superLow_inv");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_cylinderBend03_RE");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_cylinderBend03_RE02");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_doomBvert2Inv");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_dust_triangle");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_lans_rot");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_needle01");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_planeBend2");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_quad_centerRot");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_quadA");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_quadA_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_quadA_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_quadCross");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_quadCross02");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_quadCross03");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_quads");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_rayOffset");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_rayOffset2");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_rock_single");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_rock2B");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_rock06");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_rock07");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphere");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphere_dist");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphere_dist_wind");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphereHarf");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphereHarf01Inv");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphereHarf02_dist");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphereHarf03");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphereHarf05");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphereIco");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_sphereThree_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_tornado3");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_tornado6");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_tornado7");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_wallCylinder_center_smooth");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine10B");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine13b");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine19_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine20");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine22");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine23");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine24");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine26");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine27midPoly");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine29Inv");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windLine32");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windSmoke");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windSmoke_spin");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windSmokeDist");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windSmokeVertRot");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windSmokeVertRot_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_windSmokeVertRot_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_ky_wing");
//		wwPro.push_back(L"Prototype_Mesh_SM_L_DS_DashCombo_H_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_L_DS_Dashcombo_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_L_DS_DashCombo_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_L_DS_Dashcombo_Y_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_L_DS_DashCombo_Y_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_lans");
//		wwPro.push_back(L"Prototype_Mesh_SM_lans_rot");
//		wwPro.push_back(L"Prototype_Mesh_SM_lans_UV_HorizenFlip");
//		wwPro.push_back(L"Prototype_Mesh_SM_lans_UV_VerticalFlip");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_001");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_002");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_003");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_004");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_005_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_005_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_005_3");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_005_4");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_Aura_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_Aura_01_Inv");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_Charging_001");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_Charging_002");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_Charging_003");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_Charging_004");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_Charging_005");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Arrow_Main_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Basic_Projectile_Base");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_H_Arrow_aura");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_H_Arrow_aura2");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Invisibility_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Invisibility_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Invisibility_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Projectile_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_LB_Rain_Arrow");
//		wwPro.push_back(L"Prototype_Mesh_SM_Lighting");
//		wwPro.push_back(L"Prototype_Mesh_SM_Lighting_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Lightning_Base");
//		wwPro.push_back(L"Prototype_Mesh_SM_Lightning_Cyl_00");
//		wwPro.push_back(L"Prototype_Mesh_SM_Lire_Weapon_Tuto");
//		wwPro.push_back(L"Prototype_Mesh_SM_LongBow_1020050");
//		wwPro.push_back(L"Prototype_Mesh_SM_Map_Aura_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_Map_Aura_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_Map_Aura_3");
//		wwPro.push_back(L"Prototype_Mesh_SM_Map_Aura_4");
//		wwPro.push_back(L"Prototype_Mesh_SM_meteo");
//		wwPro.push_back(L"Prototype_Mesh_SM_Meteo_Sphere");
//		wwPro.push_back(L"Prototype_Mesh_SM_Meteo_Sphere3");
//		wwPro.push_back(L"Prototype_Mesh_SM_Muzzle");
//		wwPro.push_back(L"Prototype_Mesh_SM_Muzzle_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Muzzle_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Nae_Arrow");
//		wwPro.push_back(L"Prototype_Mesh_SM_Nae_Arrow_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Nae_Arrow_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Nae_Arrow_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Nae_Bow");
//		wwPro.push_back(L"Prototype_Mesh_SM_Nae_kick");
//		wwPro.push_back(L"Prototype_Mesh_SM_Nae_Pro_Base");
//		wwPro.push_back(L"Prototype_Mesh_SM_OB_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_OB_Helix_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_OB_Helix_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_OB_Helix_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Palaspel");
//		wwPro.push_back(L"Prototype_Mesh_SM_Palaspel_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_Aura_Wind01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_MagicSquare01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_MagicSquare02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_UP_Z_Wind01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_UP_Z_Wind02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_Wind01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_Wind02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_Wind03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_Wind04");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_Wind05");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_Wing_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Pheonix_Wing_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_PiercingArrow_Arrow");
//		wwPro.push_back(L"Prototype_Mesh_SM_PiercingArrow_Aura_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_PiercingArrow_Aura_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_PiercingArrow_Aura_06");
//		wwPro.push_back(L"Prototype_Mesh_SM_plane_pillar");
//		wwPro.push_back(L"Prototype_Mesh_SM_PlaneSlash_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_quad_centerRot");
//		wwPro.push_back(L"Prototype_Mesh_SM_RainDrop");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgea_Hand_L_New_888");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgea_Hand_R_New_3");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_cylinder");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_Dust_mesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_3");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_4");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_5");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_6");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_7");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_8");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone1_9");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone2_3");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone2_4");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone2_5");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone2_8");
//		wwPro.push_back(L"Prototype_Mesh_SM_Reorgeaskil04_stone2_10");
//		wwPro.push_back(L"Prototype_Mesh_SM_Ring_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Ring_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_rockRing");
//		wwPro.push_back(L"Prototype_Mesh_SM_Sae_Trail");
//		wwPro.push_back(L"Prototype_Mesh_SM_Sae_Weapon");
//		wwPro.push_back(L"Prototype_Mesh_SM_Sae_wind");
//		wwPro.push_back(L"Prototype_Mesh_SM_sakura_patal");
//		wwPro.push_back(L"Prototype_Mesh_SM_SC_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_SC_Helix_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_SC_Helix_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_SC_Helix_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Scart_Doddgy_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_Scart_Main_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_Scart_Main_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_Scart_Main_Z02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Scart_Main_Z03");
//		wwPro.push_back(L"Prototype_Mesh_SM_Scart_Sub_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_Scart_Wind_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_corn");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_corn_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_Projectile_Main_Y_High");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_Projectile_Main_Y_High_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_spin_sub");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_tornado");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_tornado_sub");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_tornado_sub_2");
//		wwPro.push_back(L"Prototype_Mesh_SM_SG_tornado_sub_2_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Shard");
//		wwPro.push_back(L"Prototype_Mesh_SM_shock_wind");
//		wwPro.push_back(L"Prototype_Mesh_SM_Shot_Cylinder_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Shot_Cylinder_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_side_wind_down");
//		wwPro.push_back(L"Prototype_Mesh_SM_side_wind_golem");
//		wwPro.push_back(L"Prototype_Mesh_SM_side_wind_golem_1");
//		wwPro.push_back(L"Prototype_Mesh_SM_SP_End_X_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_SP_End_X_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_SP_End_Z_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_SP_End_Z_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_sphere");
//		wwPro.push_back(L"Prototype_Mesh_SM_Sphere_01_Dom");
//		wwPro.push_back(L"Prototype_Mesh_SM_Sphere_02_Dom");
//		wwPro.push_back(L"Prototype_Mesh_SM_sphere_melon");
//		wwPro.push_back(L"Prototype_Mesh_SM_Sphere_Plane_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Sphere_RPG");
//		wwPro.push_back(L"Prototype_Mesh_SM_SpherePlate_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_sphereUV_lowpoly");
//		wwPro.push_back(L"Prototype_Mesh_SM_SpinningSlash_O_L_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_SpinningSlash_O_L_XD");
//		wwPro.push_back(L"Prototype_Mesh_SM_SpinningSlash_O_L_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_SpinningSlash_O_R_DX");
//		wwPro.push_back(L"Prototype_Mesh_SM_SpinningSlash_O_R_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_SpinningSlash_O_R_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Aura_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Aura_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Shield");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Shield_pivot");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Shield_Throw");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_ShieldEdge");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Stone_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Stone_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Stone_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Trail_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Trail_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_SS_Trail_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_ST3_Portal_00");
//		wwPro.push_back(L"Prototype_Mesh_SM_ST3_Portal_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Stone_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Stone_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Stone_L");
//		wwPro.push_back(L"Prototype_Mesh_SM_Stone_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_Survivor_Buff");
//		wwPro.push_back(L"Prototype_Mesh_SM_Survivor_Buff_plane");
//		wwPro.push_back(L"Prototype_Mesh_SM_Text_Floor");
//		wwPro.push_back(L"Prototype_Mesh_SM_Theo_Trail_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Theo_Trail_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Theo_Trail01");
//		wwPro.push_back(L"Prototype_Mesh_SM_ToonKaboom01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Tornado_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Tornado_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Tornado_360_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Trail_Line");
//		wwPro.push_back(L"Prototype_Mesh_SM_Trail_Twist_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Trail_Twist_03");
//		wwPro.push_back(L"Prototype_Mesh_SM_tray");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_combo_01_Front");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_combo_01_Top");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_combo_Cyl01");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_combo_EX2_Aura");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_combo_EX2_Sword");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_Trail_Main_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_Trail_Main_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_Trail_Main_Z_UVHorizenFlip");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_Trail_Slash_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_Trail_Sub_X");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Basic_Trail_Sub_Z");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Double_Slash_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Double_Slash_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Dragon_Weapon");
//		wwPro.push_back(L"Prototype_Mesh_SM_TS_Trace_01_Re");
//		wwPro.push_back(L"Prototype_Mesh_SM_Upright_noFloor");
//		wwPro.push_back(L"Prototype_Mesh_SM_wallCylinder_center_smooth");
//		wwPro.push_back(L"Prototype_Mesh_SM_Wave_01");
//		wwPro.push_back(L"Prototype_Mesh_SM_Wave_02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Wind_Line13b_F");
//		wwPro.push_back(L"Prototype_Mesh_SM_Wind_Line13b_R");
//		wwPro.push_back(L"Prototype_Mesh_SM_Wind_M02");
//		wwPro.push_back(L"Prototype_Mesh_SM_Wind_RTR_Base");
//		wwPro.push_back(L"Prototype_Mesh_SM_Wind_RTR_Base2");
//		wwPro.push_back(L"Prototype_Mesh_SM_windLine11C");
//		wwPro.push_back(L"Prototype_Mesh_SM_windLine13_multi");
//		wwPro.push_back(L"Prototype_Mesh_SM_windLine15");
//		wwPro.push_back(L"Prototype_Mesh_SM_windLine16");
//		wwPro.push_back(L"Prototype_Mesh_SM_windLine16_midPoly");
//		wwPro.push_back(L"Prototype_Mesh_SM_windLine19");
//		wwPro.push_back(L"Prototype_Mesh_SM_windMesh");
//		wwPro.push_back(L"Prototype_Mesh_SM_windMesh_a");
//		wwPro.push_back(L"Prototype_Mesh_SM_windMesh_b");
//		wwPro.push_back(L"Prototype_Mesh_SM_windSmoke");
//		wwPro.push_back(L"Prototype_Mesh_SM_Wing_T_01");
//
//	}
//
//
//	for (_uint i = 0; i < wwString.size(); i++)
//	{
//		wstring Ouput;
//		Ouput = L"case " + wwPro[i] + L":\n"
//			+ L"return " + L"TEXT(\"" + wwString[i] + L"\");\nbreak;\n";
//
//		OutputDebugStringW(Ouput.c_str());
//	}
//}

