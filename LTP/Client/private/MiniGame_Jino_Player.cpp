#include "stdafx.h"
#include "MiniGame_Jino_Player.h"
#include "physx\Collider_PhysX_Joint.h"

CMiniGame_Jino_Player::CMiniGame_Jino_Player(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMiniGame_Jino_Player::CMiniGame_Jino_Player(const CMiniGame_Jino_Player & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMiniGame_Jino_Player::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMiniGame_Jino_Player::Initialize_Clone(void * pArg)
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

_int CMiniGame_Jino_Player::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	m_fAnimSpeed = 0.5f;

	if (g_pGameInstance->Get_DIKeyState(DIK_J) & DIS_Down)
	{
		Set_State_DeadStart();
	}

	Check_PlayerInput();
	Check_Move_OutOfStage();

	switch (m_eCurState)
	{
	case CMiniGame_Jino_Player::STATE_IDLE:
		Update_State_Idle(fDeltaTime);
		break;
	case CMiniGame_Jino_Player::STATE_MOV:
		Update_State_Move(fDeltaTime);
		break;
	case CMiniGame_Jino_Player::STATE_JUMP:
		Update_State_Jump(fDeltaTime);
		break;
	case CMiniGame_Jino_Player::STATE_DEAD:
		Update_State_Dead(fDeltaTime);
		break;
	default:
		MSGBOX("CMiniGame_Jino_Player::Update : Unknown CMiniGame_Jino_Player Cur_State Value");
		break;
	}

	Update_BeachBallPos();

	Update_Collider(fDeltaTime);


	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));

	return _int();
}

_int CMiniGame_Jino_Player::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolveCom));

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
#endif // _DEBUG

	return _int();
}

_int CMiniGame_Jino_Player::Render()
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

_int CMiniGame_Jino_Player::LateRender()
{
	return _int();
}

void CMiniGame_Jino_Player::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	// DEAD
}

_float CMiniGame_Jino_Player::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

_float CMiniGame_Jino_Player::Apply_Damage(CGameObject * pTargetObject, _float fDamageAmount, _bool bKnockback)
{
	return _float();
}

void CMiniGame_Jino_Player::Set_State_IdleStart()
{
	m_pModel->Change_AnimIndex(EPLAYER_ANIM::ANIM_IDLE);
	m_eCurState = STATE_IDLE;
}

void CMiniGame_Jino_Player::Set_State_MoveStart()
{
	m_eCurState = STATE_MOV;
}

void CMiniGame_Jino_Player::Set_State_JumpStart()
{
	m_eCurState = STATE_JUMP;

	m_fJumpStart_Y = 0.83f;
	m_fFallingStart_Y = 0.f;
	m_fFallingAcc = 0.f;
	m_fJumpPower = 7.f;

	m_pTransformCom->Set_MoveSpeed(4.f);
}

void CMiniGame_Jino_Player::Set_State_DeadStart()
{
	if (STATE_DEAD != m_eCurState)
	{
		m_eCurState = STATE_DEAD;
		m_pModel->Change_AnimIndex(ANIM_JUMP);
		m_fFallingAcc = 0.f;
		m_fJumpStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	}
}

void CMiniGame_Jino_Player::Set_GameStart()
{
}

void CMiniGame_Jino_Player::Set_GameOver()
{
}

void CMiniGame_Jino_Player::Set_BeachBallTransform(CTransform* pTransformCom)
{
	m_pBeachBallTransform = pTransformCom;
}

HRESULT CMiniGame_Jino_Player::Update_State_Idle(_double fDeltaTime)
{
	return S_OK;
}

HRESULT CMiniGame_Jino_Player::Update_State_Move(_double fDeltaTime)
{
	return S_OK;
}

HRESULT CMiniGame_Jino_Player::Update_State_Jump(_double fDeltaTime)
{
	_float fPos_y = m_fJumpStart_Y + (m_fJumpPower * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
	m_fFallingAcc += 0.035f;
	 
	_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	if (fPos_y < 0.83f)
	{
		Set_State_IdleStart();
		vMyPos = XMVectorSetY(vMyPos, 0.83f);
		m_pTransformCom->Set_MoveSpeed(5.f);
	}
	else
	{
		vMyPos = XMVectorSetY(vMyPos, fPos_y);
	}

	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);

	return S_OK;
}

HRESULT CMiniGame_Jino_Player::Update_State_Dead(_double fDeltaTime)
{
	_float fPos_y = m_fJumpStart_Y + (4.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
	m_fFallingAcc += 0.04f;

	_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	if (fPos_y < -5.f)
	{
		// GameOver
		m_fFallingAcc = 0.f;
		Set_State_IdleStart();
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, XMVectorSet(0.f, 0.83f, 0.f, 0.f));
		m_pTransformCom->Set_MoveSpeed(5.f);
		return S_OK;
	}
	else
	{
		vMyPos = XMVectorSetY(vMyPos, fPos_y);
	}

	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
	return S_OK;
}

HRESULT CMiniGame_Jino_Player::Update_Collider(_double fDeltaTime)
{
	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Player, this, m_pCollider));
	return S_OK;
}

HRESULT CMiniGame_Jino_Player::Update_BeachBallPos()
{
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	vPos = XMVectorSetY(vPos, 0.4f);
	m_pBeachBallTransform->Set_MatrixState(CTransform::STATE_POS, vPos);

	return S_OK;
}

void CMiniGame_Jino_Player::Check_PlayerInput()
{
	if (STATE_DEAD == m_eCurState/* || STATE_JUMP == m_eCurState*/)
	{
		return;
	}

	if (STATE_JUMP != m_eCurState)
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
		{
			m_fAnimSpeed = 1.f;
			Set_State_JumpStart();
			m_pModel->Change_AnimIndex(ANIM_JUMP, 0.05f);
		}
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
	{
		m_fAnimSpeed = 1.f;
		m_pTransformCom->Move_Backward(g_fDeltaTime);
		m_pBeachBallTransform->Turn_CCW(XMVectorSet(0.f, 0.f, 1.f, 0.f), g_fDeltaTime);
		if (STATE_JUMP != m_eCurState)
		{
			Set_State_MoveStart();
			m_pModel->Change_AnimIndex(ANIM_MOV_B, 0.05f);
		}
	}
	else if (g_pGameInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
	{
		m_fAnimSpeed = 1.f;
		m_pTransformCom->Move_Forward(g_fDeltaTime);
		m_pBeachBallTransform->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), g_fDeltaTime);
		if (STATE_JUMP != m_eCurState)
		{
			Set_State_MoveStart();
			m_pModel->Change_AnimIndex(ANIM_MOV_F, 0.05f);
		}
	}
	else
	{
		m_fAnimSpeed = 1.f;

		if (STATE_JUMP != m_eCurState)
		{
			Set_State_IdleStart();
		}
	}
}

void CMiniGame_Jino_Player::Check_Move_OutOfStage()
{
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	if (0 > XMVectorGetX(vPos))
	{
		vPos = XMVectorSetX(vPos, 0.f);
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPos);
	}
	else if(164.f > XMVectorGetX(vPos))
	{
		m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		m_fAttachCamPos.y = 1.f;
		m_fAttachCamPos.z -= 8.f;
		m_fAttachCamPos.x += 3.5;
		m_pTransformCom->LookDir(XMVectorSet(1.f, 0.f, 0.f, 0.f));
	}
}

void CMiniGame_Jino_Player::Set_PhysX_Head()
{
	CCollider_PhysX_Base::PHYSXDESC_JOINT  createJoint;

	const _uint length = 9;
	string mBoneNames[length] =
	{
		"skd_head",
		"skd_hair01",
		"skd_hair02", "skd_hair03",
		"skd_hair04","skd_hair05",
		"skd_hair06",
		"skd_hair07", "skd_hairEnd"
	};

	createJoint.mBoneNames = mBoneNames;
	createJoint.mLength = length;
	createJoint.mGameObject = this;
	createJoint.eShapeType = E_GEOMAT_SPEHE;
	createJoint.mMainScale = _float3(0.5f, 0.5f, 0.5f);
	createJoint.mActorScale = _float3(0.5f, 0.5f, 0.5f);
	createJoint.mSeparation = 0.0f;
	createJoint.mAttachModel = m_pModel;

	m_pHeadJoint->Set_ColiderDesc_Hair(createJoint);
}

HRESULT CMiniGame_Jino_Player::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MiniGame_Jino_Player), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	// For PhysX
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Joint), TAG_COM(Com_Collider_PhysX), (CComponent**)&m_pHeadJoint));
	Set_PhysX_Head();


	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CMiniGame_Jino_Player::SetUp_Colliders()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_hip", _float3(1), _float3(0), _float3(-0.074084f, -0.861011f, -75.1948f));
	m_vecAttachedDesc.push_back(tAttachedDesc);


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_ball", _float3(1), _float3(0), _float3(-6.28489f, -9.6995f, -0.966615f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_ball", _float3(1), _float3(0), _float3(6.13672f, -9.69943f, -0.96666f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_knee", _float3(1), _float3(0), _float3(-6.28494f, -5.51913f, -38.0339f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_knee", _float3(1), _float3(0), _float3(6.1366f, -5.7681f, -37.9986f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_leg", _float3(1), _float3(0), _float3(6.13641f, -0.74237f, -75.1206f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_leg", _float3(1), _float3(0), _float3(-6.28502f, -0.74237f, -75.1888f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine01", _float3(1), _float3(0), _float3(0.024159f, -1.80535f, -88.5722f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_arm", _float3(1), _float3(0), _float3(11.6691f, -0.395438f, -114.675f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_arm", _float3(1), _float3(0), _float3(-11.7516f, -0.395438f, -114.675f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_chest", _float3(1), _float3(0), _float3(-0.041261f, -1.80535f, -109.114f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, -0.1f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_head", _float3(1), _float3(0), _float3(0.024161f, -0.499942f, -127.252f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

HRESULT CMiniGame_Jino_Player::SetUp_EtcInfo()
{
	return S_OK;
}

CMiniGame_Jino_Player * CMiniGame_Jino_Player::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMiniGame_Jino_Player*	pInstance = NEW CMiniGame_Jino_Player(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_Jino_Player");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMiniGame_Jino_Player::Clone(void * pArg)
{
	CMiniGame_Jino_Player*	pInstance = NEW CMiniGame_Jino_Player(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_Jino_Player");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMiniGame_Jino_Player::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pHeadJoint);

}

