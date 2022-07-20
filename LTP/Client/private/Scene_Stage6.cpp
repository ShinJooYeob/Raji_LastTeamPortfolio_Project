#include "stdafx.h"
#include "..\Public\Scene_Stage6.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Player.h"

#include "physX/PhyxSampleTest.h"
#include "TestObject_PhysX.h"
#include "physX/Collider_PhysX_Dynamic.h"
#include "MapObject.h"


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
	FAILED_CHECK(Ready_Layer_Monster_Boss(TAG_LAY(Layer_Monster)));

	
	// Assimp Test
//	FAILED_CHECK(Ready_Layer_AssimpModelTest(TAG_LAY(Layer_TeethObj)));
//	GetSingle(CPhysXMgr)->CreateDemoMap();

	return S_OK;
}

_int CScene_Stage6::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();
	

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
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Minion)));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE6, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Leader))); //속도 올라가는거 고치고 다시

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




HRESULT CScene_Stage6::Ready_Layer_Phycis()
{

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


