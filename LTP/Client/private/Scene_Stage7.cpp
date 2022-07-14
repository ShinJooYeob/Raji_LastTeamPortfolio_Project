#include "stdafx.h"
#include "..\Public\Scene_Stage7.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Player.h"
#include "MapObject.h"
#include "AssimpCreateMgr.h"

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
	
	
	
	return S_OK;
}

_int CScene_Stage7::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

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
	CameraDesc.fFar = 300.f;

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
	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);

	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
	m_pMainCam->Set_FocusTarget(pPlayer);
	m_pMainCam->Set_TargetArmLength(0.f);




	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_StaticMapObject)));

	CTransform* pTransform = (CTransform*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE7, pLayerTag)->Get_Component(TAG_COM(Com_Transform)));

	NULL_CHECK_RETURN(pTransform, E_FAIL);


	_Matrix tt = XMMatrixScaling(20, 1, 20) * XMMatrixTranslation(0, -2, 0);

	pTransform->Set_Matrix(tt);

	((CMapObject*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_STAGE7, pLayerTag))->Set_FrustumSize(99999999.f);

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

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Mahinasura_Leader))); //속도 올라가는거 고치고 다시

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Minion)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Vayusura_Leader)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Obejct_Monster_Tezabsura_Minion)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Obejct_Monster_Tezabsura_Purple)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Obejct_Monster_Tezabsura_Bomber)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Obejct_Monster_Tezabsura_Landmine)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Jalsura)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Ninjasura_Minion)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Ninjasura)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Black)));

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Rage)));
	
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Gadasura_Rage_Hollogram)));


	//Mesh Instance
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Wasp)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Wormgrub)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Spider)));
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE7, pLayerTag, TAG_OP(Prototype_Object_Monster_Wolf)));



	return S_OK;
}



CScene_Stage7 * CScene_Stage7::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Stage7* pTempLoby = new CScene_Stage7(pDevice,pDeviceContext);

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
