#include "stdafx.h"
#include "..\public\Scene_MiniGame_JJB.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "MiniGameBuilding.h"
#include "GaolCollider.h"

CScene_MiniGame_JJB::CScene_MiniGame_JJB(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CScene_MiniGame_JJB::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	m_pUtilMgr = GetSingle(CUtilityMgr);


	FAILED_CHECK(Ready_Layer_GoalCollider(TAG_LAY(Layer_MiniGameGoalCollider)));
	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_UI(TAG_LAY(Layer_UI)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_MiniGamePlayer_JJB)));
	FAILED_CHECK(Ready_Layer_Monster(TAG_LAY(Layer_MiniGameMonster_JJB)));

	FAILED_CHECK(Ready_PostPorcessing());

	m_pMainCam->Ortho_OnOff(true, 30.f);

	return S_OK;
}

_int CScene_MiniGame_JJB::Update(_double fDeltaTime)
{
	m_pMainCam->Ortho_OnOff(true, 30.f);

	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_iSceneStartChecker <= SceneChangeCopyFrame && CScene_Loading::m_iLoadingKinds == CScene_Loading::LOADINGKINDS_NORMAL)
	{
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_RETURN)&DIS_Down)
	{

		CMiniGameBuilding::Copy_NowScreenToBuliding(CMiniGameBuilding::MINIGAME_RHYTHM);

		FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_LOBY), SCENEID::SCENE_LOADING));
		return 0;
	}

	return _int();
}

_int CScene_MiniGame_JJB::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return _int();
}

_int CScene_MiniGame_JJB::Render()
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

	return _int();
}

_int CScene_MiniGame_JJB::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

HRESULT CScene_MiniGame_JJB::Ready_Light()
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

HRESULT CScene_MiniGame_JJB::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 0.f, -5.f);
	CameraDesc.vAt = _float3(0, 0.f, 0.1f);
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
		m_pMainCam->Set_NowSceneNum(SCENE_MINIGAME_JJB);
		m_pMainCam->Set_CameraDesc(CameraDesc);
	}

	m_pMainCam->Set_TargetArmLength(15.f);
	m_pMainCam->Set_MaxTargetArmLength(15.f);
	m_pMainCam->Set_MinTargetArmLength(15.f);
	m_pMainCam->Set_ProjectMatrix(true);

	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_Objects(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_Player(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_MiniGame_JJB_Player), &_float3(0,0,5.f)));

	m_pMainCam->Set_CameraMode(CAM_MODE_FIX);
	m_pMainCam->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CTaiko_Monster::NOTEDESC NoteMonsterDesc;

	for (_int i = 0; i < 30; ++i)
	{
		NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_SMALL;
		NoteMonsterDesc.NotePosType = CTaiko_Monster::NOTEPOS_IN;
		NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
		NoteMonsterDesc.vScaled = _float3(1.2f);
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));

		CTaiko_Monster* Monster = (CTaiko_Monster*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);
		m_NoteSmallRedMonsterList.push_back(Monster);
	}

	for (_int i = 0; i < 10; ++i)
	{
		NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_BIG;
		NoteMonsterDesc.NotePosType = CTaiko_Monster::NOTEPOS_IN;
		NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
		NoteMonsterDesc.vScaled = _float3(2.2f);
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));

		CTaiko_Monster* Monster = (CTaiko_Monster*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);
		m_NoteBicRedMonsterList.push_back(Monster);
	}

	for (_int i = 0; i < 30; ++i)
	{
		NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_SMALL;
		NoteMonsterDesc.NotePosType = CTaiko_Monster::NOTEPOS_OUT;
		NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
		NoteMonsterDesc.vScaled = _float3(1.2f);
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));

		CTaiko_Monster* Monster = (CTaiko_Monster*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);
		m_NoteSmallBlueMonsterList.push_back(Monster);
	}

	for (_int i = 0; i < 10; ++i)
	{
		NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_BIG;
		NoteMonsterDesc.NotePosType = CTaiko_Monster::NOTEPOS_OUT;
		NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
		NoteMonsterDesc.vScaled = _float3(2.2f);
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));

		CTaiko_Monster* Monster = (CTaiko_Monster*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);
		m_NoteBicBlueMonsterList.push_back(Monster);
	}

	/*NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_SMALL;
	NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
	NoteMonsterDesc.vScaled = _float3(1.2f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));
*/
	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_GoalCollider(const _tchar * pLayerTag)
{
	CGaolCollider::tagGoalColliderDesc ColliderDesc;

	ColliderDesc.ColliderType = CGaolCollider::COLLIDER_GOOD;
	ColliderDesc.vNotePosition = _float3(-13.2f, 0.5f, -1.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_MiniGame_GoalCollider), &ColliderDesc));



	ColliderDesc.ColliderType = CGaolCollider::COLLIDER_DIE;
	ColliderDesc.vNotePosition = _float3(-17.6f, 0.5f, -1.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_MiniGame_GoalCollider), &ColliderDesc));


	return S_OK;

}

HRESULT CScene_MiniGame_JJB::Ready_Layer_UI(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_TaikoUI)));
	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_PostPorcessing()
{
#ifndef _DEBUG

	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	pLightDesc->vVector = _float4(0, 64.f, -64.f, 1);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(0, -64.f, 64.f));
	pLightDesc->vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	pLightDesc->vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	pLightDesc->vSpecular = _float4(0.234375f, 0.234375f, 0.234375f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);

	//POSTPROCESSING_GODRAY
	//POSTPROCESSING_LENSEFLARE
	//POSTPROCESSING_CAMMOTIONBLUR
#else
#ifdef DEBUGONSHADERSETTING

	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	pLightDesc->vVector = _float4(0, 64.f, -64.f, 1);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(0, -64.f, 64.f));
	pLightDesc->vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	pLightDesc->vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	pLightDesc->vSpecular = _float4(0.234375f, 0.234375f, 0.234375f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	//pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	//pRenderer->Set_ShadowIntensive(0.3f);

	//pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	//pRenderer->Set_BloomOverLuminceValue(1.0f);
	//pRenderer->Set_BloomBrightnessMul(1.5f);

	////pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	////pRenderer->Set_DofLength(30.f);
	////pRenderer->Set_DofBlurIntensive(1.f);

	//pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	//pRenderer->Set_FogColor(_float3{ 0.234375f });
	//pRenderer->Set_FogHighlightColor(_float3{ 1.f });
	//pRenderer->Set_FogStartDist(5.f);
	//pRenderer->Set_FogGlobalDensity(0.1f);
	//pRenderer->Set_FogHeightFalloff(0.1f);
#endif

#endif // !_DEBUGee

	return S_OK;
}

CScene_MiniGame_JJB * CScene_MiniGame_JJB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_MiniGame_JJB* pScene_MinigameTaiko = NEW CScene_MiniGame_JJB(pDevice, pDeviceContext);

	if (FAILED(pScene_MinigameTaiko->Initialize()))
	{
		Safe_Release(pScene_MinigameTaiko);
		MSGBOX("Failed to Creating CScene_MiniGame_JJB");
		return nullptr;
	}

	return pScene_MinigameTaiko;
}

void CScene_MiniGame_JJB::Free()
{
	__super::Free();
}
