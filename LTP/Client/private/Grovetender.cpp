#include "stdafx.h"
#include "..\public\Grovetender.h"
#include "Player.h"
#include "Camera_Main.h"
#include "Snake.h"

CGrovetender::CGrovetender(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CGrovetender::CGrovetender(const CGrovetender & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGrovetender::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CGrovetender::Initialize_Clone(void * pArg)
{

	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

_int CGrovetender::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;
	m_fAnimSpeed = 1.f;

	switch (m_eCurState)
	{
	case EGOLEM_STATE::STATE_SPAWNIDLE:
		FAILED_CHECK(Update_State_SpawnIdle(fDeltaTime));
		break;
	case EGOLEM_STATE::STATE_SPAWN:
		FAILED_CHECK(Update_State_Spawn(fDeltaTime));
		break;
	case EGOLEM_STATE::STATE_IDLE:
		FAILED_CHECK(Update_State_Idle(fDeltaTime));
		break;
	case EGOLEM_STATE::STATE_MOVE:
		FAILED_CHECK(Update_State_Move(fDeltaTime));
		break;
	}

	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));

	return _int();
}

_int CGrovetender::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolveCom));

	return _int();
}

_int CGrovetender::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(13));

	return _int();
}

_int CGrovetender::LateRender()
{
	return _int();
}

void CGrovetender::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

_float CGrovetender::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

_float CGrovetender::Apply_Damage(CGameObject * pTargetObject, _float fDamageAmount, _bool bKnockback)
{
	return _float();
}

void CGrovetender::Set_CutSceneState(_uint iCutSceneState)
{
	m_iCutSceneState = iCutSceneState;
}

HRESULT CGrovetender::Update_State_SpawnIdle(_double fDeltaTime)
{
	m_fDelayTime -= (float)fDeltaTime;

	if (0.f >= m_fDelayTime)
	{
		m_eCurState = STATE_SPAWN;
		m_pModel->Change_AnimIndex(ANIM_SPAWN);
	}

	return S_OK;
}

HRESULT CGrovetender::Update_State_Spawn(_double fDeltaTime)
{
	//0.257		0.36
	_float fAnimRate = (_float)m_pModel->Get_PlayRate();
	m_pMainCam->Set_CameraMoveWeight(0.9f);
	m_pMainCam->Set_CameraLookWeight(0.9f);


	if (0.719f <= fAnimRate && true == m_bOncePlaySound)
	{
		_Vector vSoundPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_CAMERA) + XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
		g_pGameInstance->Play3D_Sound(L"Jino_Golem_GetUp_2.wav", vSoundPos, CHANNELID::CHANNEL_MONSTER, 1.f);
		m_bOncePlaySound = false;
	}

	if (0.36f <= fAnimRate)
	{
		Set_LimLight_N_Emissive(_float4(1.f, 0.f, 0.f, m_fEmissiveValue * 0.6f), _float4(m_fEmissiveValue, 1.f, m_fEmissiveValue, 0.f));
		m_fAnimSpeed = 0.5f;
		m_fEmissiveValue += (_float)fDeltaTime * 0.4f;
		if (m_fEmissiveValue > 1.f)
		{
			m_fEmissiveValue = 1.f;
		}
	}
	else if (0.33f <= fAnimRate) 
	{
		if (false == m_bOncePlaySound)
		{
			_Vector vSoundPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_CAMERA) + XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			g_pGameInstance->Play3D_Sound(L"Jino_Golem_GetUp.wav", vSoundPos, CHANNELID::CHANNEL_MONSTER, 1.f);
			m_bOncePlaySound = true;
		}
		m_fAnimSpeed = 0.25f;
	}
	
	if (0.98f <= fAnimRate)
	{
		m_eCurState = STATE_IDLE;
		m_pModel->Change_AnimIndex(ANIM_IDLE);
		m_fDelayTime = 4.f;
	}

	return S_OK;
}

HRESULT CGrovetender::Update_State_Idle(_double fDeltaTime)
{
	m_fDelayTime -= (_float)fDeltaTime;
	if (2 == m_iCutSceneState)
	{
		_float fAnimRate = (_float)m_pModel->Get_PlayRate();
		if (0.24f >= fAnimRate)
		{
			m_fAnimSpeed = 0.5f;
		}
		else if (0.28f >= fAnimRate)
		{
			m_fAnimSpeed = 2.f;
		}
		else
		{
			if (0.2f <= fAnimRate && 0.4f >= fAnimRate && false == m_bOnceSwitch)
			{
				g_pGameInstance->Play3D_Sound(L"Jino_Golem_Clap.wav", m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
				
				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


				CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
				tCameraShakeDirDesc.fTotalTime = 0.3f;
				tCameraShakeDirDesc.fPower = 20.f;
				tCameraShakeDirDesc.fChangeDirectioninterval = 0.01f;
				_float3 fShakeDir = pUtil->RandomFloat3(-3.f, 3.f).XMVector();
				fShakeDir.y = 0.f;
				tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
				pUtil->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, false);

				CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
				tCameraShakeRotDesc.fTotalTime = 0.3f;
				tCameraShakeRotDesc.fPower = 2.f;
				tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
				tCameraShakeRotDesc.fShakingRotAxis = pUtil->Get_MainCamera()->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
				pUtil->Get_MainCamera()->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

				m_bOnceSwitch = true;

				INSTMESHDESC tDesc = pUtil->Get_MeshParticleDesc(L"JY_Mesh_8");
				tDesc.FollowingTarget = nullptr;
				tDesc.ParticleSize2 = _float3(0.001f, 5, 0.001f);
				tDesc.ParticleSize2 = _float3(5, 5, 5);
				tDesc.vFixedPosition =
					m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + 
				m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * -0.25f + 
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 3.f +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 2.5f ;

				tDesc.vPowerDirection = 
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * 0.85f + 
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK)* 0.15f;
				pUtil->Create_MeshInstance(m_eNowSceneNum, tDesc);


			}
			else if (0.98f <= fAnimRate)
			{
				m_bOnceSwitch = false;
				//m_pModel->Change_AnimIndex_ReturnTo_Must(17, 17, 0);

				m_pModel->Change_AnimIndex(ANIM_MOVE);
				m_eCurState = STATE_MOVE;
				g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE4, TAG_LAY(Layer_Boss), TAG_OP(Prototype_Object_Boss_Snake), &_float3(0.f, -150.f, 93.197f));

				_Vector vSoundPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_CAMERA) + XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
				g_pGameInstance->Play3D_Sound(L"Jino_Golem_Grow.wav", vSoundPos, CHANNELID::CHANNEL_MONSTER, 1.f);
				g_pGameInstance->PlayBGM(L"Jino_FakeBoss_Golem.wav");
			}

			m_fAnimSpeed = 0.5f;
		}
	}
	else
	{
		if (0.f >= m_fDelayTime)
		{
			m_pModel->Change_AnimIndex(17);
			m_iCutSceneState = 2;
		}
		else if (2.f >= m_fDelayTime)
		{
			m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -1.f;
			m_pMainCam->Lock_CamLook(true, vLook);
			m_fAttachCamPos = m_fAttachCamPos.XMVector() - (vLook * 11.f);
			m_fAttachCamPos.y += 8.f;
		}
	}
	return S_OK;
}

HRESULT CGrovetender::Update_State_Move(_double fDeltaTime)
{
	_Vector vPlayerPos = static_cast<CTransform*>(m_pPlayer->Get_Component(Tag_Component(COMPONENTID::Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_float3 fPlayerPos = vPlayerPos;
	_float fAnimRate = (_float)m_pModel->Get_PlayRate();
	if ( (0.1f < fAnimRate && 0.42 >= fAnimRate) || (0.5747f < fAnimRate && 0.9f >= fAnimRate))
	{
		m_pTransformCom->MovetoTarget(vPlayerPos, fDeltaTime);
		m_fAnimSpeed = 0.5f;
	}
	else
	{
		m_fAnimSpeed = 0.3f;
	}

	if (0.38f <= fAnimRate && 0.42f >= fAnimRate && false == m_bOncePlaySound)
	{
		GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 0.8f, 0.2f, false);
		g_pGameInstance->Play3D_Sound(L"Jino_Golem_FootStep_L.wav", m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
		
		INSTPARTICLEDESC tPtDesc;

		tPtDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_9");

		tPtDesc.FollowingTarget = nullptr;
		tPtDesc.vFixedPosition =
			m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
			m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * -0.8f +
			//m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 3.f +
			m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.5f;

		tPtDesc.Particle_Power = 10.f;

		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, tPtDesc);
		
		m_bOncePlaySound = true;
	}
	else if (0.89f <= fAnimRate && 0.92f >= fAnimRate && true == m_bOncePlaySound)
	{
		GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 0.8f, 0.2f, false);
		g_pGameInstance->Play3D_Sound(L"Jino_Golem_FootStep_R.wav", m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
		m_bOncePlaySound = false;

		INSTPARTICLEDESC tPtDesc;

		tPtDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_9");

		tPtDesc.FollowingTarget = nullptr;
		tPtDesc.vFixedPosition =
			m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
			m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.8f +
			//m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 3.f +
			m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.5f;

		tPtDesc.Particle_Power = 10.f;

		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, tPtDesc);
	}

	_Vector vLookDir = XMVector3Normalize(vPlayerPos - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
	m_pTransformCom->Turn_Dir(vLookDir, 0.9f);

	m_pTransformCom->Set_MoveSpeed(5.f);
	//m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(97.5f, 2.18f, 40.66f));
	
	m_pMainCam->Set_FocusTarget(m_pPlayer);

	m_pPlayer->Set_AttachCamPos(fPlayerPos);
	m_pPlayer->Set_AttachCamPosOffset(_float3(0.f, 2.5f, -1.f));
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -1.f;
	m_pMainCam->Lock_CamLook(true, vLook);

	if (false == m_bOnceSwitch && 30.f >= XMVectorGetX(XMVector3Length(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) - vPlayerPos)))
	{
		m_bOnceSwitch = true;
		static_cast<CSnake*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(LAYERID::Layer_Boss)))->Set_CutSceneState(0);
	}


	return S_OK;
}

HRESULT CGrovetender::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Golem), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 0.8f;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CGrovetender::SetUp_EtcInfo()
{
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(97.5f, 2.18f, 40.66f));
	m_pTransformCom->Scaled_All(_float3(2.5f));
	m_pTransformCom->LookDir(XMVectorSet(-0.7f, 0.f, -1.f, 0.f));
	m_pModel->Change_AnimIndex(1);
	m_pModel->Update_AnimationClip(g_fDeltaTime * m_fAnimSpeed, m_bIsOnScreen);
	m_pModel->Change_AnimIndex(0);

	m_eCurState = STATE_SPAWNIDLE;

	m_pPlayer = static_cast<CPlayer*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player)));
	m_pPlayer->Set_State_StopActionStart();

	m_pMainCam = static_cast<CCamera_Main*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Camera_Main)));
	m_pMainCam->Set_FocusTarget(this);

	m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -1.f;
	m_pMainCam->Lock_CamLook(true, vLook);
	m_fAttachCamPos = m_fAttachCamPos.XMVector() - (vLook * 18.f);
	m_fAttachCamPos.y += 5.f;
	m_pMainCam->Set_CameraMoveWeight(0.99f);
	m_pMainCam->Set_CameraLookWeight(0.99f);

	m_fDelayTime = 7.f;

	return S_OK;
}

CGrovetender * CGrovetender::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGrovetender*	pInstance = NEW CGrovetender(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGrovetender");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGrovetender::Clone(void * pArg)
{
	CGrovetender*	pInstance = NEW CGrovetender(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGrovetender");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGrovetender::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDissolveCom);
}
