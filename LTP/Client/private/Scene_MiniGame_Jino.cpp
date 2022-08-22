#include "stdafx.h"
#include "Scene_MiniGame_Jino.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "MiniGame_Jino_Player.h"
#include "Rope.h"
#include "StaticMapObject.h"
#include "StaticInstanceMapObject.h"
#include "GoalTrigger.h"
#include "NormalMonkey.h"
#include "BeachBall.h"
#include "MiniGame_Jino_Monster.h"
#include "JumpingMonkey.h"
#include "FireRing.h"
#include "CircusBackground.h"
#include "MiniGameBuilding.h"

CScene_MiniGame_Jino::CScene_MiniGame_Jino(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CScene_MiniGame_Jino::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pUtilMgr = GetSingle(CUtilityMgr);

	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_Objects(TAG_LAY(Layer_MapObject)));
	FAILED_CHECK(Ready_MapData(L"MiniGame_Jino.dat", SCENE_MINIGAME_Jino, TAG_LAY(Layer_StaticMapObj)));
	FAILED_CHECK(Ready_PostPorcessing());

	Ready_ObjectPool_NormalMonkey();
	Ready_ObjectPool_JumpingMonkey();
	Ready_ObjectPool_FireRing();
	Ready_ObjectPool_BeachBall();

	m_fDelayTime_Spawn = 3.f;
	m_fDelayTime_Spawn_BeachBall = 4.f;
	
	g_pGameInstance->PlayBGM(TEXT("Jino_MiniGame_BGM.mp3"));
	return S_OK;
}

_int CScene_MiniGame_Jino::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	m_pMainCam->Ortho_OnOff(true, 10.f);

	auto iMonsterAmount = g_pGameInstance->Get_ObjectList_from_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))->size();
	if (iMonsterAmount == (m_ObjectPool_NormalMonkey.size() + m_ObjectPool_JumpingMonkey.size()))
	{
		Spawn_Monster();
	}

	if (30.f < XMVectorGetX(m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS)))
	{
		m_fDelayTime_Spawn_BeachBall -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime_Spawn_BeachBall)
		{
			m_fDelayTime_Spawn_BeachBall = 5.f;
			Spawn_BeachBall();
		}
	}

	if (m_iSceneStartChecker <= SceneChangeCopyFrame && CScene_Loading::m_iLoadingKinds == CScene_Loading::LOADINGKINDS_NORMAL)
	{
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_RETURN)&DIS_Down)
	{

		CMiniGameBuilding::Copy_NowScreenToBuliding(CMiniGameBuilding::MINIGAME_CIRCUS);

		FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE1), SCENEID::SCENE_LOADING));
		return 0;
	}

	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	_Vector vDir = XMVector3Normalize(XMVectorSetY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS), 10) - XMVectorSet(128.f, -64.f, 256.f, 0));
	g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, XMVectorSet(128.f, -64.f, 256.f, 0) + vDir * 330.f);
	
	return 0;
}

_int CScene_MiniGame_Jino::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (true == m_bGameClear)
	{
		CMiniGameBuilding::Copy_NowScreenToBuliding(CMiniGameBuilding::MINIGAME_CIRCUS);

		GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange();
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE1), SCENEID::SCENE_LOADING));
	}

	return 0;
}

_int CScene_MiniGame_Jino::Render()
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

_int CScene_MiniGame_Jino::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

void CScene_MiniGame_Jino::Spawn_Monster()
{
	_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);

	if (XMVectorGetX(vPlayerPos) >= 90.f)
	{
		m_eSpawnPhase = PHASE_4;
	}
	else if(XMVectorGetX(vPlayerPos) >= 60.f)
	{
		m_eSpawnPhase = PHASE_3;
	}
	else if(XMVectorGetX(vPlayerPos) >= 30.f)
	{ 
		m_eSpawnPhase = PHASE_2;
	}
	else
	{
		m_eSpawnPhase = PHASE_1;
	}


	switch (m_eSpawnPhase)
	{
	case PHASE_4:
	{
		_int iSelected = rand() % 3;

		if (0 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 25.f, 0.f, 0.f));
		}
		else if (1 == iSelected)
		{
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));
		}
		else if (2 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 16.f, 0.f, 0.f));

			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));

			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 24.f, 0.f, 0.f));
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 25.f, 0.f, 0.f));
		}
		break;
	}
	case PHASE_3:
	{
		_int iSelected = rand() % 3;

		if (0 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 25.f, 0.f, 0.f));
		}
		else if (1 == iSelected)
		{
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));
		}
		else if (2 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 17.f, 0.f, 0.f));

			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 19.f, 0.f, 0.f));

			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 22.f, 0.f, 0.f));
			Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 24.f, 0.f, 0.f));
		}
		break;
	}
	case PHASE_2:
	{
		_int iSelected = rand() % 4;

		if (0 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 25.f, 0.f, 0.f));
		}
		else if (1 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));
		}
		else if (2 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));

			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_RUNNER, _float3(XMVectorGetX(vPlayerPos) + 22.5f, 0.f, 0.f));

			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 25.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 30.f, 0.f, 0.f));
		}
		else if (3 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 18.f, 0.f, 0.f));

			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 21.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 24.f, 0.f, 0.f));
		}
		break;
	}
	case PHASE_1:
	{
		_int iSelected = rand() % 2;

		if (0 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 25.f, 0.f, 0.f));
		}
		else if (1 == iSelected)
		{
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 15.f, 0.f, 0.f));
			Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE::TYPE_WALKER, _float3(XMVectorGetX(vPlayerPos) + 20.f, 0.f, 0.f));
		}
	}
		break;

	}

}

void CScene_MiniGame_Jino::Spawn_BeachBall()
{
	_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);

	_int iSelected = rand() % 3;

	if (0 == iSelected)
	{
		Pop_BeachBall(_float3(XMVectorGetX(vPlayerPos) + 15.f, 0.4f, 0.f));
		Pop_BeachBall(_float3(XMVectorGetX(vPlayerPos) + 20.f, 0.4f, 0.f));
	}
	else if (1 == iSelected)
	{
		Pop_BeachBall(_float3(XMVectorGetX(vPlayerPos) + 15.f, 0.4f, 0.f));
		Pop_BeachBall(_float3(XMVectorGetX(vPlayerPos) + 20.f, 0.4f, 0.f));
		Pop_BeachBall(_float3(XMVectorGetX(vPlayerPos) + 25.f, 0.4f, 0.f));
	}
}

void CScene_MiniGame_Jino::GameClear()
{
	m_bGameClear = true;
}

void CScene_MiniGame_Jino::GameOver()
{
	auto* MonsterList = g_pGameInstance->Get_ObjectList_from_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster));

	for (auto& iter : *MonsterList)
	{
		if (true == static_cast<CMiniGame_Jino_Monster*>(iter)->Get_Active())
		{
			static_cast<CMiniGame_Jino_Monster*>(iter)->Dead();
		}
	}
}

void CScene_MiniGame_Jino::Push_NormalMonkey(CNormalMonkey * pNormalMonkey)
{
	m_ObjectPool_NormalMonkey.push_back(pNormalMonkey);
}

void CScene_MiniGame_Jino::Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE eMonkeyType, _float3 vSpawnPos)
{
	if (0 >= m_ObjectPool_NormalMonkey.size())
	{
		g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_NormalMonkey), &vSpawnPos);
		CNormalMonkey* pNormalMonkey = static_cast<CNormalMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster)));
		pNormalMonkey->SetUp_MonsterType(eMonkeyType);
		pNormalMonkey->Set_Active(true);
		pNormalMonkey->Set_Position(vSpawnPos);
	}
	else
	{
		CNormalMonkey* pNormalMonkey = m_ObjectPool_NormalMonkey.front();
		pNormalMonkey->SetUp_MonsterType(eMonkeyType);
		pNormalMonkey->Set_Active(true);
		pNormalMonkey->Set_Position(vSpawnPos);
		m_ObjectPool_NormalMonkey.pop_front();
	}
}

void CScene_MiniGame_Jino::Push_JumpingMonkey(CJumpingMonkey * pJumpingMonkey)
{
	m_ObjectPool_JumpingMonkey.push_back(pJumpingMonkey);
}

void CScene_MiniGame_Jino::Pop_JumpingMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE eMonkeyType, _float3 vSpawnPos)
{
	if (0 >= m_ObjectPool_JumpingMonkey.size())
	{
		g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_JumpingMonkey), &vSpawnPos);
		CJumpingMonkey* pJumpingMonkey = static_cast<CJumpingMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster)));
		pJumpingMonkey->SetUp_MonsterType(eMonkeyType);
		pJumpingMonkey->Set_Active(true);
		pJumpingMonkey->Set_Position(vSpawnPos);
	}
	else
	{
		CJumpingMonkey* pJumpingMonkey = m_ObjectPool_JumpingMonkey.front();
		pJumpingMonkey->SetUp_MonsterType(eMonkeyType);
		pJumpingMonkey->Set_Active(true);
		pJumpingMonkey->Set_Position(vSpawnPos);
		m_ObjectPool_JumpingMonkey.pop_front();
	}
}

void CScene_MiniGame_Jino::Push_BeachBall(CBeachBall * pBeachBall)
{
	m_ObjectPool_BeachBall.push_back(pBeachBall);
}

CBeachBall* CScene_MiniGame_Jino::Pop_BeachBall(_float3 vSpawnPos)
{
	if (0 >= m_ObjectPool_BeachBall.size())
	{
		g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_PlayerWeapon), TAG_OP(Prototype_Object_MiniGame_BeachBall), &vSpawnPos);
		CBeachBall* pBeachBall = static_cast<CBeachBall*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_PlayerWeapon)));
		pBeachBall->Set_State_Spawn();
		pBeachBall->Set_Active(true);
		pBeachBall->Set_Position(vSpawnPos);
		pBeachBall->SetUp_PlayerTransform(m_pPlayerTransform);
		return pBeachBall;
	}
	else
	{
		CBeachBall* pBeachBall = m_ObjectPool_BeachBall.front();
		pBeachBall->Set_State_Spawn();
		pBeachBall->Set_Active(true);
		pBeachBall->Set_Position(vSpawnPos);
		m_ObjectPool_BeachBall.pop_front();
		return pBeachBall;
	}
}

HRESULT CScene_MiniGame_Jino::Ready_Light()
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

HRESULT CScene_MiniGame_Jino::Ready_Layer_MainCamera(const _tchar * pLayerTag)
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
		m_pMainCam->Set_CameraDesc(CameraDesc);
	}

	m_pMainCam->Set_TargetArmLength(15.f);
	m_pMainCam->Set_MaxTargetArmLength(15.f);
	m_pMainCam->Set_MinTargetArmLength(15.f);
	m_pMainCam->Set_ProjectMatrix(true);

	return S_OK;
}

HRESULT CScene_MiniGame_Jino::Ready_Layer_Player(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_PlayerWeapon), TAG_OP(Prototype_Object_MiniGame_BeachBall), &_float3(0)));
	CBeachBall* pBeachBall = (CBeachBall*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_MINIGAME_Jino, TAG_LAY(Layer_PlayerWeapon)));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Player), &_float3(0.f, 0.83f, 0.f)));
	m_pPlayer = (CMiniGame_Jino_Player*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_MINIGAME_Jino, TAG_LAY(Layer_Player)));
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	m_pPlayer->Set_BeachBall(pBeachBall);

	m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(Tag_Component(Com_Transform)));
	NULL_CHECK_RETURN(m_pPlayerTransform, E_FAIL);
	m_pPlayer->Update(g_fDeltaTime);

	m_pMainCam->Set_FocusTarget(m_pPlayer);
	m_pMainCam->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
	m_pMainCam->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 0.f));

	pBeachBall->SetUp_PlayerTransform(m_pPlayerTransform);
	pBeachBall->Set_State_Ride();


	m_pMainCam->Set_CameraInitState(XMVectorSet(3.5f, 1.f, -8.f, 1.f), XMVectorSet(0.f, 0.f, 1.f, 0.f));

	return S_OK;
}

HRESULT CScene_MiniGame_Jino::Ready_Layer_Objects(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(-9.3f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(0)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(9.3f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(18.6f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(27.9f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(37.2f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(46.5f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(55.8f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(65.1f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(74.4f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(83.7f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(93.f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(102.3f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(111.6f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(120.9f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(130.2f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(139.5f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(148.8f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(158.1f, 0.f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_Jino_Rope), &_float3(167.4f, 0.f, 0.f)));
	
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_GoalTrigger), &_float3(174.f, 0.657f, 0.f)));

	//==============================================================================================================//
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(4.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(24.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(44.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(64.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(84.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(104.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(124.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(144.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(164.6f, -0.94f, 7.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, pLayerTag, TAG_OP(Prototype_Object_MiniGame_CircusBackground), &_float3(184.6f, -0.94f, 7.f)));
	return S_OK;
}

HRESULT CScene_MiniGame_Jino::Ready_MapData(const _tchar * szMapDataFileName, SCENEID eSceneID, const _tchar * pLayerTag)
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

HRESULT CScene_MiniGame_Jino::Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CScene_MiniGame_Jino::Ready_PostPorcessing()
{

#ifndef _DEBUG



	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(1000.f, -128.f, 1000.f));
	pLightDesc->vDiffuse = _float4(0.94140625f, 0.5703125f, 0.23046875f, 1.f);
	pLightDesc->vAmbient = _float4(1.f, 0.83203125f, 0.68359375f, 1.f);
	pLightDesc->vSpecular = _float4(0.40625f, 0.1875f, 0.05078125f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	pRenderer->Set_ShadowIntensive(0.35f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, true);
	pRenderer->Set_SunSize(0.5f);
	pRenderer->Set_GodrayLength(64.f);
	pRenderer->Set_GodrayIntensity(0.1f);
	pRenderer->Set_StartDecay(0.25f);
	pRenderer->Set_DistDecay(1.6f);
	pRenderer->Set_MaxDeltaLen(0.006f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, true);
	_float Value = (1 - 0.98f) * 344.f + 16.f;
	pRenderer->Set_LensfalreSupportSunSize(Value);
	pRenderer->Set_LensefalreNoiseTexIndex(245);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(2.5f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	pRenderer->Set_DofLength(45.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	pRenderer->Set_FogColor(_float3(0.94921875f, 0.4296875f, 0.328125f));
	pRenderer->Set_FogStartDist(0.1f);
	pRenderer->Set_FogGlobalDensity(0.06f);
	pRenderer->Set_FogHeightFalloff(0.16f);
	//POSTPROCESSING_GODRAY
	//POSTPROCESSING_LENSEFLARE
	//POSTPROCESSING_CAMMOTIONBLUR
#else

#ifdef DEBUGONSHADERSETTING




	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(1000.f, -128.f, 1000.f));
	pLightDesc->vDiffuse = _float4(0.94140625f, 0.5703125f, 0.23046875f, 1.f);
	pLightDesc->vAmbient = _float4(1.f, 0.83203125f, 0.68359375f, 1.f);
	pLightDesc->vSpecular = _float4(0.40625f, 0.1875f, 0.05078125f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	pRenderer->Set_ShadowIntensive(0.35f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, false);
	pRenderer->Set_SunSize(0.5f);
	pRenderer->Set_GodrayLength(64.f);
	pRenderer->Set_GodrayIntensity(0.1f);
	pRenderer->Set_StartDecay(0.25f);
	pRenderer->Set_DistDecay(1.6f);
	pRenderer->Set_MaxDeltaLen(0.006f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, false);
	_float Value = (1 - 0.98f) * 344.f + 16.f;
	pRenderer->Set_LensfalreSupportSunSize(Value);
	pRenderer->Set_LensefalreNoiseTexIndex(245);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, false);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(2.5f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, false);
	pRenderer->Set_DofLength(45.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, false);
	pRenderer->Set_FogColor(_float3(0.94921875f, 0.4296875f, 0.328125f));
	pRenderer->Set_FogStartDist(0.1f);
	pRenderer->Set_FogGlobalDensity(0.06f);
	pRenderer->Set_FogHeightFalloff(0.16f);


#endif
#endif // !_DEBUG

	return S_OK;
}

HRESULT CScene_MiniGame_Jino::Ready_ObjectPool_NormalMonkey()
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_NormalMonkey), &_float3(0)));
	m_ObjectPool_NormalMonkey.push_back(static_cast<CNormalMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_NormalMonkey), &_float3(0)));
	m_ObjectPool_NormalMonkey.push_back(static_cast<CNormalMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_NormalMonkey), &_float3(0)));
	m_ObjectPool_NormalMonkey.push_back(static_cast<CNormalMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_NormalMonkey), &_float3(0)));
	m_ObjectPool_NormalMonkey.push_back(static_cast<CNormalMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_NormalMonkey), &_float3(0)));
	m_ObjectPool_NormalMonkey.push_back(static_cast<CNormalMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))));

	return S_OK;
}

HRESULT CScene_MiniGame_Jino::Ready_ObjectPool_JumpingMonkey()
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_JumpingMonkey), &_float3(0.f)));
	m_ObjectPool_JumpingMonkey.push_back(static_cast<CJumpingMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_JumpingMonkey), &_float3(0.f)));
	m_ObjectPool_JumpingMonkey.push_back(static_cast<CJumpingMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))));

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster), TAG_OP(Prototype_Object_MiniGame_JumpingMonkey), &_float3(0.f)));
	m_ObjectPool_JumpingMonkey.push_back(static_cast<CJumpingMonkey*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_Monster))));

	return S_OK;
}

HRESULT CScene_MiniGame_Jino::Ready_ObjectPool_FireRing()
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(10.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(25.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(28.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(45.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(50.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(55.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(72.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(80.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(110.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(115.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(120.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(150.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(155.f, 3.8f, 0.f)));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_MazeDoor), TAG_OP(Prototype_Object_MiniGame_FireRing), &_float3(160.f, 3.8f, 0.f)));
	return S_OK;
}

HRESULT CScene_MiniGame_Jino::Ready_ObjectPool_BeachBall()
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_PlayerWeapon), TAG_OP(Prototype_Object_MiniGame_BeachBall), &_float3(0)));
	CBeachBall* pBeachBall = (CBeachBall*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENE_MINIGAME_Jino, TAG_LAY(Layer_PlayerWeapon)));
	pBeachBall->Set_Active(false);
	pBeachBall->SetUp_PlayerTransform(m_pPlayerTransform);
	m_ObjectPool_BeachBall.push_back(pBeachBall);

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_PlayerWeapon), TAG_OP(Prototype_Object_MiniGame_BeachBall), &_float3(0)));
	pBeachBall = (CBeachBall*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENE_MINIGAME_Jino, TAG_LAY(Layer_PlayerWeapon)));
	pBeachBall->Set_Active(false);
	pBeachBall->SetUp_PlayerTransform(m_pPlayerTransform);
	m_ObjectPool_BeachBall.push_back(pBeachBall);

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_Jino, Tag_Layer(Layer_PlayerWeapon), TAG_OP(Prototype_Object_MiniGame_BeachBall), &_float3(0)));
	pBeachBall = (CBeachBall*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENE_MINIGAME_Jino, TAG_LAY(Layer_PlayerWeapon)));
	pBeachBall->Set_Active(false);
	pBeachBall->SetUp_PlayerTransform(m_pPlayerTransform);
	m_ObjectPool_BeachBall.push_back(pBeachBall);

	return S_OK;
}

CScene_MiniGame_Jino * CScene_MiniGame_Jino::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_MiniGame_Jino* pTempLoby = NEW CScene_MiniGame_Jino(pDevice, pDeviceContext);

	if (FAILED(pTempLoby->Initialize()))
	{
		Safe_Release(pTempLoby);
		MSGBOX("Failed to Creating CScene_MiniGame_Jino");
		return nullptr;
	}

	return pTempLoby;
}

void CScene_MiniGame_Jino::Free()
{
	__super::Free();
}
