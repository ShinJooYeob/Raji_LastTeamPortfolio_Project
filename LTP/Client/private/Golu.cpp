#include "stdafx.h"
#include "..\public\Golu.h"
#include "Camera_Main.h"
#include "Scene_Loading.h"
#include "Scene_Laboratory_Jino.h"

CGolu::CGolu(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CGolu::CGolu(const CGolu & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGolu::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CGolu::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Colliders());

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
	}

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

_int CGolu::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	// Reset AnimSpeed
	m_fAnimSpeed = 1.f;


	//**************** Checks *****************//
	{
		// Check Key Input
		Check_Mov_KeyInput(fDeltaTime);
	}
	//


	//**************** Process Player State Logic *****************//
	switch (m_eCurState)
	{
	case EPLAYER_STATE::STATE_IDLE:
		FAILED_CHECK(Update_State_Idle(fDeltaTime));
		break;
	case EPLAYER_STATE::STATE_MOV:
		FAILED_CHECK(Update_State_Move(fDeltaTime));
		break;
	case EPLAYER_STATE::STATE_LANDING:
		FAILED_CHECK(Update_State_Landing(fDeltaTime));
		break;
	case EPLAYER_STATE::STATE_FALL:
		FAILED_CHECK(Update_State_Fall(fDeltaTime));
		break;
	default:
		MSGBOX("CGolu::Update : Unknown Golu Cur_State Value");
		break;
	}
	//

	Update_Collider(fDeltaTime);

	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));

	return _int();
}

_int CGolu::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolveCom));

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider_GameClear));
#endif // _DEBUG
	return _int();
}

_int CGolu::Render()
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

_int CGolu::LateRender()
{
	return _int();
}

void CGolu::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (pMyCollider == m_pCollider_GameClear && eConflictedObjCollisionType == CollisionType_Player && (STATE_IDLE == m_eCurState || STATE_MOV == m_eCurState))
	{
		static_cast<CScene_Laboratory_Jino*>(g_pGameInstance->Get_NowScene())->Set_GameClear();
	}
}

_float CGolu::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{

	return _float();
}

_float CGolu::Apply_Damage(CGameObject * pTargetObject, _float fDamageAmount, _bool bKnockback)
{
	return _float();
}

void CGolu::Set_State_IdleStart(_double fDeltaTime)
{
	m_eCurState = STATE_IDLE;
	m_pModel->Change_AnimIndex(BASE_ANIM_IDLE, 0.15f, true);	
}

void CGolu::Set_State_MoveStart(_double fDeltaTime)
{
	if (true == m_bGameOver)
	{
		return;
	}

	Move(m_eInputDir, fDeltaTime);
	m_eCurState = STATE_MOV;
	m_pModel->Change_AnimIndex(BASE_ANIM_WALK);
}

void CGolu::Set_State_FallingStart(_double fDeltaTime)
{
	m_eCurState = STATE_FALL;
	m_pModel->Change_AnimIndex(BASE_ANIM_FALL);
	m_fFallingAcc = 0.f;
	m_fJumpStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
}

void CGolu::Set_State_LandingStart(_double fDeltaTime)
{
	m_eCurState = STATE_LANDING;
	m_pModel->Change_AnimIndex(BASE_ANIM_LANDING);
}

void CGolu::Set_GameOver()
{
	m_bGameOver = true;
}

HRESULT CGolu::Update_State_Idle(_double fDeltaTime)
{
	if (MOVDIR_END != m_eInputDir)
	{
		Set_State_MoveStart(fDeltaTime);
	}
	else
	{
		Set_State_IdleStart(fDeltaTime);
	}

	return _int();
}

HRESULT CGolu::Update_State_Move(_double fDeltaTime)
{
	m_fAnimSpeed = 3.f;

	if (MOVDIR_END != m_eInputDir)
	{
		Set_State_MoveStart(fDeltaTime);
	}
	else
	{
		Set_State_IdleStart(fDeltaTime);
	}

	return _int();
}

HRESULT CGolu::Update_State_Fall(_double fDeltaTime)
{
	m_fAnimSpeed = 1.f;
	m_fFallingAcc += 0.03f;
	_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_float fPos_y = m_fJumpStart_Y + (0 * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);

	m_pTransformCom->Move_Forward(fDeltaTime * (m_fJumpPower * 0.8f), nullptr);
	vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	vMyPos = XMVectorSetY(vMyPos, fPos_y);
	
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);

	return S_OK;
}

HRESULT CGolu::Update_State_Landing(_double fDeltaTime)
{
	m_fAnimSpeed = 1.f;
	_float fAnimRate = (_float)m_pModel->Get_PlayRate();
	
	if(0.98f <= fAnimRate)
	{
		Set_State_IdleStart(g_fDeltaTime);
	}

	return S_OK;
}

HRESULT CGolu::Update_Collider(_double fDeltaTime)
{
	_Matrix WorldMat = m_pTransformCom->Get_WorldMatrix();
	m_pCollider->Update_Transform(0, WorldMat);
	m_pCollider->Update_Transform(1, WorldMat);
	m_pCollider->Update_Transform(2, WorldMat);
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Player2, this, m_pCollider));

	//***************************************************************//

	m_pCollider_GameClear->Update_Transform(0, WorldMat);
	m_pCollider_GameClear->Update_Transform(1, WorldMat);
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pCollider_GameClear));

	return S_OK;
}

_bool CGolu::Check_Mov_KeyInput(_double fDeltaTime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_bool bChekKey = false;
	_int iMov_F = 0, iMov_R = 0;

	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		iMov_F += 1;
		bChekKey = true;
	}
	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		iMov_F -= 1;
		bChekKey = true;
	}
	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		iMov_R -= 1;
		bChekKey = true;
	}
	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		iMov_R += 1;
		bChekKey = true;
	}

	
	Set_InputDir(iMov_F, iMov_R, fDeltaTime);

	RELEASE_INSTANCE(CGameInstance);
	return bChekKey;

}

void CGolu::Set_InputDir(_int iAxis_F, _int iAxis_R, _double fDeltaTime)
{
	if (0 < iAxis_F)
	{
		if (0 < iAxis_R)
		{
			m_eInputDir = MOVDIR_BL;
		}
		else if (0 > iAxis_R)
		{
			m_eInputDir = MOVDIR_BR;
		}
		else
		{
			m_eInputDir = MOVDIR_B;
		}
	}
	else if (0 > iAxis_F)
	{
		if (0 < iAxis_R)
		{
			m_eInputDir = MOVDIR_FL;
		}
		else if (0 > iAxis_R)
		{
			m_eInputDir = MOVDIR_FR;
		}
		else
		{
			m_eInputDir = MOVDIR_F;
		}
	}
	else if (0 < iAxis_R)
	{
		m_eInputDir = MOVDIR_L;
	}
	else if (0 > iAxis_R)
	{
		m_eInputDir = MOVDIR_R;
	}
	else
	{
		m_eInputDir = MOVDIR_END;
	}

}

void CGolu::Move(EINPUT_MOVDIR eMoveDir, _double fDeltaTime)
{
	if (MOVDIR_END == eMoveDir)
		return;

	CCamera_Main* pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	_Vector vMyNormalizedLook = m_pTransformCom->Get_MatrixState_Normalized(CTransform::TransformState::STATE_LOOK);

	_float fDiagonalSpeed = 1.f;
	_float fTurnRate = 0.85f;
	_float fMoveRate = (_float)fDeltaTime;


	_Vector vMovDir;
	switch (eMoveDir)
	{
	case EINPUT_MOVDIR::MOVDIR_F:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_B:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_L:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_R:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += (pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	}

	m_pTransformCom->MovetoDir(vMovDir, fMoveRate, nullptr);
	m_pTransformCom->Turn_Dir(vMovDir, fTurnRate);
	
	m_fMovDir = XMVector3Normalize(vMovDir);
}

HRESULT CGolu::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Golu), TAG_COM(Com_Model), (CComponent**)&m_pModel));


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

HRESULT CGolu::SetUp_Colliders()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "Skd_Hips", _float3(1), _float3(0), _float3(-0.074084f, -0.861011f, -75.1948f));
	m_vecAttachedDesc.push_back(tAttachedDesc);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "Skd_Ball_R", _float3(1), _float3(0), _float3(-0.068955f, 0.00615f, -0.007523f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "Skd_Ball_L", _float3(1), _float3(0), _float3(0.068955f, 0.00615f, -0.007523f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();


	//******************************************************************************//


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider_1), (CComponent**)&m_pCollider_GameClear));
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider_GameClear->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "Skd_Hips", _float3(1), _float3(0), _float3(-0.074084f, -0.861011f, -75.1948f));
	m_vecAttachedDesc.push_back(tAttachedDesc);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider_GameClear->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "Skd_Hips", _float3(1), _float3(0), _float3(0.f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider_GameClear->Set_ParantBuffer();

	return S_OK;
}

HRESULT CGolu::SetUp_EtcInfo()
{
	m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(170.f));

	Set_IsOcllusion(true);

	Set_LimLight_N_Emissive(_float4(0, 0, 0, 0), _float4(0.5f, 0, 0, 0));

	m_eCurState = STATE_IDLE;
	return S_OK;
}

CGolu * CGolu::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGolu*	pInstance = NEW CGolu(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGolu");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGolu::Clone(void * pArg)
{
	CGolu*	pInstance = NEW CGolu(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGolu");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGolu::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pCollider_GameClear);
}
