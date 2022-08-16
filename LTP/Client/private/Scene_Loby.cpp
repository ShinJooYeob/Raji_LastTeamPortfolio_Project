#include "stdafx.h"
#include "..\Public\Scene_Loby.h"
#include "Scene_Loading.h"

#include "Camera_Main.h"
#include "MapObject.h"



CScene_Loby::CScene_Loby(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice,pDeviceContext)
{
}

HRESULT CScene_Loby::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(Ready_UI(TAG_LAY(Layer_UI_SLIDE)));
	
	
	FAILED_CHECK(Ready_Camera(TAG_LAY(Layer_Camera_Main)));
	


	// #SOUND Test
	// GetSingle(CGameInstance)->PlayBGM(L"The End.mp3", 0, 1.f);
		


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		GetSingle(CUtilityMgr)->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);

	return S_OK;
}

_int CScene_Loby::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	//if (g_pGameInstance->Get_DIKeyState(DIK_Z)&DIS_Down)
	//{
	//	FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
	//	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_LABORATORY_JINO), SCENEID::SCENE_LOADING));
	//}
	//else if (g_pGameInstance->Get_DIKeyState(DIK_X)&DIS_Down)
	//{
	//	FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
	//	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING));
	//}




	if (g_pGameInstance->Get_DIKeyState(DIK_RETURN)&DIS_Down)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE4), SCENEID::SCENE_LOADING));
	}
	if (GetKeyState(VK_F2) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_EDIT), SCENEID::SCENE_LOADING));
	}
	else if (GetKeyState(VK_F3) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_LABORATORY_JINO), SCENEID::SCENE_LOADING));
	}
	else if (GetKeyState(VK_F6) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE6), SCENEID::SCENE_LOADING));
	}



	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);


	if (m_iSceneStartChecker <= 2)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}

	return 0;
}

_int CScene_Loby::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)	return Change_to_NextScene();

	return 0;
}

_int CScene_Loby::Render()
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

_int CScene_Loby::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

_int CScene_Loby::Change_to_NextScene()
{
	FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, (SCENEID)m_eNextScene), SCENEID::SCENE_LOADING));

	return _int();
}



HRESULT CScene_Loby::Ready_Light()
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

HRESULT CScene_Loby::Ready_Camera(const _tchar* pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0.125f, 1.3f, -0.8f);
	CameraDesc.vAt = _float3(0.225f, 1.3f, 0);
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


	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TAG_OP(Prototype_Camera_Main), &CameraDesc));

	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_LOBY, TAG_LAY(Layer_Camera_Main)));

	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);

	return S_OK;
}

//HRESULT CScene_Loby::Ready_Layer_SkyBox(const _tchar * pLayerTag)
//{
//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_LOBY, pLayerTag, TAG_OP(Prototype_SkyBox)));
//
//	return S_OK;
//}

HRESULT CScene_Loby::Ready_UI(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TAG_OP(Prototype_Object_LobbyUI)));
	return S_OK;
}

//
//HRESULT CScene_Loby::Ready_TestObject(const _tchar * pLayerTag)
//{
//
//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TAG_OP(Prototype_TestObject)));
//
//
//	return S_OK;
//}
//
//HRESULT CScene_Loby::Ready_NonAnimObject(const _tchar * pLayerTag)
//{
//
//
//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TAG_OP(Prototype_StaticMapObject)));
//
//	CTransform* pTransform = (CTransform*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LOBY, pLayerTag)->Get_Component(TAG_COM(Com_Transform)));
//
//	NULL_CHECK_RETURN(pTransform, E_FAIL);
//
//
//	_Matrix tt = XMMatrixScaling(100, 1, 100) * XMMatrixTranslation(0, -2.f, 0);
//
//	pTransform->Set_Matrix(tt);
//	((CMapObject*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LOBY, pLayerTag))->Set_FrustumSize(99999999.f);
//
//
//
//
//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, L"TestNonAnimInstance"));
//
//
//
//
//
//	return S_OK;
//}

CScene_Loby * CScene_Loby::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Loby* pTempLoby = NEW CScene_Loby(pDevice,pDeviceContext);

	if (FAILED(pTempLoby->Initialize())) 
	{
		Safe_Release(pTempLoby); 
		MSGBOX("Failed to Creating CScene_Loby");
		return nullptr;
	}
	return pTempLoby;

}

void CScene_Loby::Free()
{
	__super::Free();
}
