#include "stdafx.h"
#include "Scene_Laboratory_Jino.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Player.h"
#include "Golu.h"
#include "DynamicPlatform.h"
#include "RepelWall.h"

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

	m_fGameOverDelay = 11.f;
	return S_OK;
}

_int CScene_Laboratory_Jino::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	Update_Game();
	Check_GameOver();

	if (m_iSceneStartChecker <= 2)
	{
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}

	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	_Vector vDir = XMVector3Normalize(XMVectorSetY(m_pTransform_Raji->Get_MatrixState(CTransform::STATE_POS), 10) - XMVectorSet(128.f, -64.f, 256.f, 0));
	g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, XMVectorSet(128.f, -64.f, 256.f, 0) + vDir * 330.f);

	return 0;
}

_int CScene_Laboratory_Jino::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (true == m_bGameOver)
	{
		m_fGameOverDelay -= (_float)fDeltaTime;
		if (0 >= m_fGameOverDelay)
		{
			FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_LABORATORY_JINO), SCENEID::SCENE_LOADING));
		}
	}
	else if (true == m_bGameClear)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_LOBY), SCENEID::SCENE_LOADING));
	}

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

void CScene_Laboratory_Jino::Set_PlatformIndex_Raji(_uint iIndex)
{
	m_iPlatformIndex_Raji = iIndex;
}

void CScene_Laboratory_Jino::Set_PlatformIndex_Golu(_uint iIndex)
{
	m_iPlatformIndex_Golu = iIndex;
}

void CScene_Laboratory_Jino::Update_Game()
{
	if (true == m_bEndUpdate)
	{
		return;
	}

	if (m_iPlatformIndex_Raji == m_iPlatformIndex_Golu)
	{
		if (m_vecDynamicPlatforms_Raji.size() - 1 == m_iPlatformIndex_Raji)
		{
			m_pEndPlatform->Set_Appear(true);
		}
		else if (99 == m_iPlatformIndex_Raji && 99 == m_iPlatformIndex_Golu)
		{
			m_vecDynamicPlatforms_Raji[m_vecDynamicPlatforms_Raji.size() - 1]->Set_Appear(false);
			m_vecDynamicPlatforms_Golu[m_vecDynamicPlatforms_Golu.size() - 1]->Set_Appear(false);
			m_bEndUpdate = true;
			m_pMainCam->Set_MaxTargetArmLength(5.f);
			m_pMainCam->Set_MinTargetArmLength(5.f);
		}
		else
		{
			if (0 < m_iPlatformIndex_Raji)
			{
				m_vecDynamicPlatforms_Raji[m_iPlatformIndex_Raji - 1]->Set_Appear(false);
				m_vecDynamicPlatforms_Golu[m_iPlatformIndex_Golu - 1]->Set_Appear(false);
			}

			if (m_vecDynamicPlatforms_Raji.size() > m_iPlatformIndex_Raji)
			{
				m_vecDynamicPlatforms_Raji[m_iPlatformIndex_Raji + 1]->Set_Appear(true);
				m_vecDynamicPlatforms_Golu[m_iPlatformIndex_Golu + 1]->Set_Appear(true);
			}
		}
	}
	
	// 


}

void CScene_Laboratory_Jino::Check_GameOver()
{
	if (99 == m_iPlatformIndex_Raji)
	{
		_float fRajiPos_Y = XMVectorGetY(m_pTransform_Raji->Get_MatrixState(CTransform::TransformState::STATE_POS));
		_float fLasePlatform_Y = XMVectorGetY(m_pEndPlatform->Get_TransformCom()->Get_MatrixState(CTransform::TransformState::STATE_POS));
		if (fLasePlatform_Y - 5.f > fRajiPos_Y)
		{
			m_pPlayer->Set_FallingDead(true);
			m_pPlayer->Set_State_OnDynamicPlatformDead();
			m_pGolu->Set_GameOver();
			m_bGameOver = true;
		}
	}
	else if (m_vecDynamicPlatforms_Raji.size() - 1 > m_iPlatformIndex_Raji)
	{
		_float fRajiPos_Y = XMVectorGetY(m_pTransform_Raji->Get_MatrixState(CTransform::TransformState::STATE_POS));
		_float fNextPlatform_Y = XMVectorGetY(m_vecDynamicPlatforms_Raji[m_iPlatformIndex_Raji + 1]->Get_TransformCom()->Get_MatrixState(CTransform::TransformState::STATE_POS));
		if (fNextPlatform_Y - 2.f > fRajiPos_Y)
		{
			m_pPlayer->Set_FallingDead(true);
			m_pPlayer->Set_State_OnDynamicPlatformDead();
			m_pGolu->Set_GameOver();
			m_bGameOver = true;
		}

	}


	if (99 == m_iPlatformIndex_Golu)
	{
		_float fGoluPos_Y = XMVectorGetY(m_pTransform_Golu->Get_MatrixState(CTransform::TransformState::STATE_POS));
		_float fLasePlatform_Y = XMVectorGetY(m_pEndPlatform->Get_TransformCom()->Get_MatrixState(CTransform::TransformState::STATE_POS));
		if (fLasePlatform_Y - 5.f > fGoluPos_Y)
		{
			m_pPlayer->Set_State_OnDynamicPlatformDead();
			m_bGameOver = true;
		}
	}
	else if (m_vecDynamicPlatforms_Golu.size() - 1 > m_iPlatformIndex_Golu)
	{
		_float fGoluPos_Y = XMVectorGetY(m_pTransform_Golu->Get_MatrixState(CTransform::TransformState::STATE_POS));
		_float fNextPlatform_Y = XMVectorGetY(m_vecDynamicPlatforms_Golu[m_iPlatformIndex_Golu + 1]->Get_TransformCom()->Get_MatrixState(CTransform::TransformState::STATE_POS));
		if (fNextPlatform_Y > fGoluPos_Y)
		{
			m_pPlayer->Set_State_OnDynamicPlatformDead();
			m_bGameOver = true;
		}
	}
}

void CScene_Laboratory_Jino::Set_GameClear()
{
	m_bGameClear = true;
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
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, TAG_LAY(Layer_NPC), TAG_OP(Prototype_NPC_Golu), &_float3(10.f, 90.f, 0.f)));
	CGameObject* pGolu = (CGolu*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_LABORATORY_JINO, TAG_LAY(Layer_NPC)));
	static_cast<CGolu*>(pGolu)->Set_State_FallingStart(g_fDeltaTime);


	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	NULL_CHECK_RETURN(m_pMainCam, E_FAIL);
	m_pMainCam->Set_CameraInitState(XMVectorSet(29.8301334f, 43.5636597f, 56.5242004f, 1.f), XMVectorSet(0.242860109f, -0.570451736f, 0.784604549f, 0.f));

	// Player
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Player), &_float3(-10.f, 90.f, 0.f)));
	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_LABORATORY_JINO, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	static_cast<CPlayer*>(pPlayer)->Set_State_FallingStart(g_fDeltaTime);
	m_pTransform_Raji = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pTransform_Raji, E_FAIL);

	CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));
	PlayerNavi->FindCellIndex(m_pTransform_Raji->Get_MatrixState(CTransform::TransformState::STATE_POS));
	pPlayer->Update_AttachCamPos();

	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_RAJIGOLU_MINIGAME);
	m_pMainCam->Set_FocusTarget(pPlayer);
	m_pMainCam->Set_CameraInitState(XMVectorSet(29.8301334f, 43.5636597f, 56.5242004f, 1.f), XMVectorSet(0.242860109f, -0.570451736f, 0.784604549f, 0.f));
	pPlayer->Update_AttachCamPos();


	m_pTransform_Golu = (CTransform*)pGolu->Get_Component(TAG_COM(Com_Transform));
	m_pPlayer = static_cast<CPlayer*>(pPlayer);
	m_pGolu = static_cast<CGolu*>(pGolu);
	return S_OK;
}

HRESULT CScene_Laboratory_Jino::Ready_Layer_Objects(const _tchar * pLayerTag)
{
	CDynamicPlatform::DYNAMICPLATFORMDESC tDynamicPlatSpawnDesc;
	tDynamicPlatSpawnDesc.fSpawnPos = _float3(0.f, 60.f, 0.f);
	tDynamicPlatSpawnDesc.eOnTarget = CDynamicPlatform::EONTARGET::TARGET_BOTH;
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_END;
	tDynamicPlatSpawnDesc.iIndex = 99;
	tDynamicPlatSpawnDesc.bEndPlatform = true;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_pEndPlatform = static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, 90.f, 0.f);
	tDynamicPlatSpawnDesc.eOnTarget = CDynamicPlatform::EONTARGET::TARGET_RAJI;
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_7;
	tDynamicPlatSpawnDesc.iIndex = 0;
	tDynamicPlatSpawnDesc.bEndPlatform = false;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-6.f, 85.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_5;
	tDynamicPlatSpawnDesc.iIndex = 1;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, 75.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_6;
	tDynamicPlatSpawnDesc.iIndex = 2;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, 70.f, 5.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 3;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-6.f, 60.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 4;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, 50.f, -5.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 5;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, 45.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 6;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-15.f, 40.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 7;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(-10.f, 30.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 8;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Raji.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));



	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, 90.f, 0.f);
	tDynamicPlatSpawnDesc.eOnTarget = CDynamicPlatform::EONTARGET::TARGET_GOLU;
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_6;
	tDynamicPlatSpawnDesc.iIndex = 0;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(6.f, 85.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_4;
	tDynamicPlatSpawnDesc.iIndex = 1;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, 75.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_7;
	tDynamicPlatSpawnDesc.iIndex = 2;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, 70.f, -5.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 3;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(6.f, 60.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 4;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, 50.f, 5.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 5;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, 45.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 6;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(15.f, 40.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 7;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

	tDynamicPlatSpawnDesc.fSpawnPos = _float3(10.f, 30.f, 0.f);
	tDynamicPlatSpawnDesc.ePattern = CDynamicPlatform::PATTERN_1;
	tDynamicPlatSpawnDesc.iIndex = 8;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_LABORATORY_JINO, pLayerTag, TAG_OP(Prototype_Object_DynamicPlatform), &tDynamicPlatSpawnDesc));
	m_vecDynamicPlatforms_Golu.push_back(static_cast<CDynamicPlatform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_LABORATORY_JINO, pLayerTag)));

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
