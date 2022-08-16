#include "stdafx.h"
#include "..\public\MahaHead.h"
#include "Player.h"
#include "Camera_Main.h"

CMahaHead::CMahaHead(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMahaHead::CMahaHead(const CMahaHead & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMahaHead::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMahaHead::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Etc());

	Set_IsOcllusion(true);

	return S_OK;
}

_int CMahaHead::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	m_pDissolveCom->Update_Dissolving(fDeltaTime);

	switch (m_iCurState)
	{
	case 0:
	{
		m_fDelayTime = 2.f;
	}
		break;
	case 1:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_iCurState = 2;
			m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_CAMMOTIONBLUR, true);
			static_cast<CPlayer*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player)))->Set_AttachCamPosOffset(_float3(2.8f, 33.5f, 90.5f));
			static_cast<CCamera_Main*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Camera_Main)))->Lock_CamLook(true, XMVectorSet(0.f, 0.2f, 1.f, 0.f));
			m_fDelayTime = 1.5f;
		}
	}
		break;
	case 2:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_iCurState = 3;
			m_fDelayTime = 5.f;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_MrM_Intro_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
		}

	}
		break;	
	case 3:
	{
		// Jino_MrM_Intro_0
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			if (0 == m_iStateController_0)
			{
				m_pTransformCom->Rotation_CCW(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT), XMConvertToRadians(30.f));
				m_fDelayTime = 1.f;
				m_iStateController_0 = 1;
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Chieftain_BoneBreak_Start.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
				GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.f, 0.2f, false);
			}
			else if (1 == m_iStateController_0)
			{
				static_cast<CPlayer*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player)))->Set_AttachCamPosOffset(_float3(0.f, 2.5f, 2.5f));
				GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, 0.5f, 2.f, 0.f));
				m_iCurState = 4;
				m_fDelayTime = 0.5f;
			}
		}
	}
		break;
	case 4:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_pDissolveCom->Set_DissolveOn(false, 0.8f);
			m_iCurState = 5;
			m_fDelayTime = 2.5f;
		}
		
		_Vector vPlayerPos = static_cast<CTransform*>(static_cast<CPlayer*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player)))->Get_Component(Tag_Component(COMPONENTID::Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
		m_pTransformCom->MovetoTarget_ErrRange(XMVectorSet(XMVectorGetX(vPlayerPos), 38.f, 295.f, 1.f), fDeltaTime, 0.5f);
	}
		break;
	case 5:
	{
		m_pTransformCom->Set_MoveSpeed(90.f);
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_pDissolveCom->Set_DissolveOn(true, 0.1f);
			m_iCurState = 6;
		}

		_Vector vPlayerPos = static_cast<CTransform*>(static_cast<CPlayer*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player)))->Get_Component(Tag_Component(COMPONENTID::Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
		m_pTransformCom->MovetoTarget_ErrRange(XMVectorSet(XMVectorGetX(vPlayerPos), 38.f, 295.f, 1.f), fDeltaTime, 0.5f);
	}
		break;
	case 6:
	{
		_Vector vPlayerPos = static_cast<CTransform*>(static_cast<CPlayer*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player)))->Get_Component(Tag_Component(COMPONENTID::Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
		m_pTransformCom->MovetoTarget_ErrRange(XMVectorSet(XMVectorGetX(vPlayerPos), 38.f, 295.f, 1.f), fDeltaTime, 0.5f);

		m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_GODRAY, false);
		m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_LENSEFLARE, false);
		g_pGameInstance->Play3D_Sound(TEXT("JJB_MrM_Teleport.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
		g_pGameInstance->Play3D_Sound(TEXT("Jino_MrM_Intro_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(55.f, 2.f, 0.2f, false);
		m_iCurState = 7;
		m_fDelayTime = 4.f;
	}
		break;
	case 7:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0 >= m_fDelayTime)
		{
			m_iCurState = 8;
			m_fTargetLookDir = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * -1.f;
			m_fDelayTime = 5.f;
			m_fRotAngle = 8.f;
			m_fDelayTime_2 = 0.f;
		}
	} 
	break;
	case 8:
	{
		if (88.f > m_fDelayTime_2)
		{
			m_pTransformCom->Turn_Direct(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP)), XMConvertToRadians(m_fRotAngle));
			m_fDelayTime_2 += 8.f;
		}
		else if (88.f == m_fDelayTime_2)
		{
			m_pTransformCom->Turn_Direct(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP)), XMConvertToRadians(3.f));
			m_fDelayTime_2 += 8.f;
		}

		m_fDelayTime -= (_float)fDeltaTime;
		if (false == m_bPlayOnceSound && 3.f <= m_fDelayTime)
		{
			g_pGameInstance->Play3D_Sound(TEXT("JJB_Chieftain_BoneBreak_Start.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("JJB_Chieftain_Naration_8.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(56.f, 2.f, 0.2f, false);
			m_bPlayOnceSound = true;
		}
		else if (0.f >= m_fDelayTime)
		{
			m_bPlayOnceSound = false;
			m_fDelayTime = 7.f;
			m_iCurState = 9;
			m_fDelayTime_2 = 9.f;
			m_fRotAngle = 9.f;
		}
	}
		break;
	case 9:
	{
		if (180.f > m_fDelayTime_2)
		{
			m_pTransformCom->Turn_Direct(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP)), XMConvertToRadians(m_fRotAngle));
			m_fDelayTime_2 += 9.f;
		}
		else if (180.f == m_fDelayTime_2)
		{
			m_pTransformCom->Turn_Direct(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP)), XMConvertToRadians(5.f));
			m_fDelayTime_2 += 9.f;
		}

		m_fDelayTime -= (_float)fDeltaTime;
		if (false == m_bPlayOnceSound && 4.f <= m_fDelayTime)
		{
			g_pGameInstance->Play3D_Sound(TEXT("JJB_Chieftain_BoneBreak_Start.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Rangda_Voice_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(56.f, 2.f, 0.2f, false);
			m_bPlayOnceSound = true;
		}
		else if (0.f >= m_fDelayTime)
		{
			m_bPlayOnceSound = false;
			m_fDelayTime = 6.f;
			m_iCurState = 10;
			g_pGameInstance->Play3D_Sound(TEXT("JJB_Chieftain_BoneBreak_Start.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(56.f, 2.f, 0.2f, false);
		}
	}
		break;
	case 10:
	{
		m_pTransformCom->Turn_Dir(m_fTargetLookDir.XMVector(), 0.7f);
		
		if (false == m_bPlayOnceSound && 6.f <= m_fDelayTime)
		{
			g_pGameInstance->Play3D_Sound(TEXT("JJB_Naga_3.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
			m_bPlayOnceSound = true;
		}

		m_fDelayTime -= (_float)fDeltaTime;

		if (0.f >= m_fDelayTime)
		{
			m_iCurState = 11;
			m_pDissolveCom->Set_DissolveOn(false, 0.1f);
			m_fDelayTime = 3.f;
			g_pGameInstance->Play3D_Sound(TEXT("JJB_MrM_Teleport.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(56.f, 2.f, 0.2f, false);
			m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_GODRAY, true);
			m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_LENSEFLARE, true);
		}
	}
		break;
	case 11:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_pDissolveCom->Set_DissolveOn(true, 0.1f);
			_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			vLook = XMVectorSetY(vLook, 0.f);
			m_pTransformCom->Turn_Dir(vLook, 0.6f);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(100.f, 38.f, 320.55f));
			m_iCurState = 12;
			m_fDelayTime = 3.f;
			g_pGameInstance->Play3D_Sound(TEXT("JJB_MrM_Teleport.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(56.f, 2.f, 0.2f, false);
		}
		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vCamPos = GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vLookDir = XMVector3Normalize(vMyPos - vCamPos);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraLookAt_Turn(vLookDir);
	}
		break;
	case 12:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_iCurState = 13;
		}
	}
		break;
	case 13:
	{
		m_pTransformCom->Set_MoveSpeed(10.f);
		m_pTransformCom->Move_Up(fDeltaTime);
		_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		
		if (150.f <= XMVectorGetY(vPos))
		{
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Smash_Air.wav"), g_pGameInstance->Get_TargetPostion_float4(TARGETPOSITIONTYPE::PLV_PLAYER), CHANNELID::CHANNEL_MONSTER, 1.f);
			m_pTransformCom->Set_MoveSpeed(250.f);
			vPos = XMVectorSetY(vPos, 110.f);
			m_iCurState = 14;
		}

		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vCamPos = GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vLookDir = XMVector3Normalize(vMyPos - vCamPos);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraLookAt_Turn(vLookDir);
	}
		break;
	case 14:
	{
		m_pTransformCom->Move_Down(fDeltaTime);
		_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		if (34.f >= XMVectorGetY(vPos))
		{
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 15.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.01f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, false);

			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.1f;
			tCameraShakeRotDesc.fPower = 3.f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Smash_Down.wav"), g_pGameInstance->Get_TargetPostion_float4(TARGETPOSITIONTYPE::PLV_PLAYER), CHANNELID::CHANNEL_MONSTER, 1.f);
			vPos = XMVectorSetY(vPos, 34.f);

			m_pDissolveCom->Set_DissolveOn(false, 2.f);
			m_iCurState = 15;

			FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE5, TAG_LAY(Layer_Boss), TAG_OP(Prototype_Object_Boss_Mahabalasura), &_float3(100.f, 34.3f, 322.283f)));
		
			CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

			INSTPARTICLEDESC tParticleTexDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_23");
			tParticleTexDesc.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
			tParticleTexDesc.vFixedPosition.y = 32.5f;
			tParticleTexDesc.vPowerDirection = _float3(0, 0, 1);
			tParticleTexDesc.TargetColor = _float4(_float3(1.5), tParticleTexDesc.TargetColor.w);
			//tParticleTexDesc.vEmissive_SBB = _float3(0);
			tParticleTexDesc.TempBuffer_0.x = 0.4f;
			tParticleTexDesc.TempBuffer_1.w = 0.4f;
			tParticleTexDesc.ParticleSize = _float3(15.f);
			pUtil->Create_TextureInstance(m_eNowSceneNum, tParticleTexDesc);
		}

		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vCamPos = GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vLookDir = XMVector3Normalize(vMyPos - vCamPos);
		
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraLookAt_Turn(vLookDir);
	}
		break;
	case 15:
	{

	}
		break;
	}

	return _int();
}

_int CMahaHead::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CMahaHead::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(10));

	return _int();
}

_int CMahaHead::LateRender()
{
	return _int();
}

void CMahaHead::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

void CMahaHead::Set_CurState(_uint iState)
{
	m_iCurState = iState;
}

HRESULT CMahaHead::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MahaHead), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CMahaHead::SetUp_Collider()
{
	return S_OK;
}

HRESULT CMahaHead::SetUp_Etc()
{
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(102.562f, 69.63f, 390.532f));
	m_pTransformCom->Scaled_All(_float3(30.f));
	return S_OK;
}

CMahaHead * CMahaHead::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahaHead*	pInstance = NEW CMahaHead(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMahaHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMahaHead::Clone(void * pArg)
{
	CMahaHead*	pInstance = NEW CMahaHead(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMahaHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMahaHead::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
