#include "stdafx.h"
#include "..\public\PlayerWeapon_Chakra.h"
#include "Camera_Main.h"


CPlayerWeapon_Chakra::CPlayerWeapon_Chakra(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CPlayerWeapon(pDevice, pDeviceContext)
{
}

CPlayerWeapon_Chakra::CPlayerWeapon_Chakra(const CPlayerWeapon_Chakra & rhs)
	: CPlayerWeapon(rhs)
{
}

HRESULT CPlayerWeapon_Chakra::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon_Chakra::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

_int CPlayerWeapon_Chakra::Update(_double fDeltaTime)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_eCurState)
	{
	case EChakraState::CHAKRA_IDLE:
		Update_IdleState(fDeltaTime);
		break;
	case EChakraState::CHAKRA_MOV:
		Update_Trail(fDeltaTime);
		Update_MovState(fDeltaTime);
		break;
	case EChakraState::CHAKRA_GOBACK:
		Update_Trail(fDeltaTime);
		Update_GoBackState(fDeltaTime);
		break;
	}

	Check_AttackStart();

	m_pSwordTrail->Set_TextureIndex(6);
	m_pSwordTrail->Set_Color(_float4(1.f, 1.f, 0.6f, 1.f));
	m_pModel->Change_AnimIndex(0, 0.f);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, true));

	Update_Colliders();
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider));
	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));

	return _int();
}

_int CPlayerWeapon_Chakra::LateUpdate(_double fDeltaTimer)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this,m_pTransformCom,m_pShaderCom,m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRENONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_SWORD, m_pSwordTrail));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	return _int();
}

_int CPlayerWeapon_Chakra::Render()
{
	if (__super::Render() < 0)		return -1;

	FAILED_CHECK(m_pRendererCom->End_RenderTarget(TEXT("MRT_Material")));
	FAILED_CHECK(m_pRendererCom->Begin_RenderTarget(TEXT("MRT_OccludedMaterial")));

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(17));

	FAILED_CHECK(m_pRendererCom->End_RenderTarget(TEXT("MRT_OccludedMaterial")));
	FAILED_CHECK(m_pRendererCom->Begin_RenderTarget(TEXT("MRT_Material")));
	return _int();
}

_int CPlayerWeapon_Chakra::LateRender()
{
	return _int();
}

void CPlayerWeapon_Chakra::Active_Trail(_bool bActivate)
{
	__super::Active_Trail(bActivate);

	if (true == m_bActiveTrail)
	{
		m_pSwordTrail->Set_TrailTurnOn(
			true,
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.5f),
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.5f)
		);

	}
	else
	{
		m_pSwordTrail->Set_TrailTurnOn(false, _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
	}
}

void CPlayerWeapon_Chakra::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Monster == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		if (0.f > pConflictedObj->Take_Damage(this, 1.f, vDamageDir, true, 2.f))
		{
			GetSingle(CUtilityMgr)->SlowMotionStart(2.f, 0.02f);
		}
		pConflictedCollider->Set_Conflicted(0.5f);

		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Chakra_Impact.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_EFFECT, 0.1f);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(55.f, 3.f, 0.2f, true);
	}
}

_bool CPlayerWeapon_Chakra::AbleToChangeWeapon()
{
	return ((false == m_pDissolveCom->Get_IsDissolving()) && (EChakraState::CHAKRA_IDLE == m_eCurState));
}

void CPlayerWeapon_Chakra::Dissolve_In(_double fTargetTime)
{
	m_pDissolveCom->Set_DissolveOn(true, fTargetTime);
}

void CPlayerWeapon_Chakra::Dissolve_Out(_double fTargetTime)
{
	m_pDissolveCom->Set_DissolveOn(false, fTargetTime);
}

void CPlayerWeapon_Chakra::Set_ChakraState(EChakraState eChakraState)
{
	m_eCurState = eChakraState;
}

CPlayerWeapon_Chakra::EChakraState CPlayerWeapon_Chakra::Get_ChakraState()
{
	return m_eCurState;
}

_int CPlayerWeapon_Chakra::Update_IdleState(_double fDeltaTime)
{
	_Vector vCenterPos = m_tPlayerWeaponDesc.eAttachedDesc.Get_AttachObjectTransform()->Get_MatrixState(CTransform::STATE_POS);
	vCenterPos = XMVectorSetY(vCenterPos, XMVectorGetY(vCenterPos) + 0.7f);
	m_pTransformCom->Turn_Revolution_CW(vCenterPos, 0.9f, fDeltaTime * 0.8f);
	 
	m_pTransformCom->LookDir(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) - vCenterPos));

	Active_Trail(false);

	Set_LimLight_N_Emissive();
	return _int();
}

_int CPlayerWeapon_Chakra::Update_MovState(_double fDeltaTime)
{
	m_fCur_AttackSoundDelay += (_float)g_fDeltaTime;
	if (m_fCur_AttackSoundDelay >= m_fMax_AttackSoundDelay)
	{
		m_fCur_AttackSoundDelay = 0.f;
		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Chakra_Attack.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.5f);
	}

	_Vector vTurnDir = m_fAttackTargetPoint.XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	vTurnDir = XMVectorSetY(vTurnDir, 0);
	m_pTransformCom->Move_Forward(fDeltaTime);
	m_pTransformCom->Turn_Dir(vTurnDir, m_fTurnDirWeight);
	m_fTurnDirWeight -= (_float)fDeltaTime;
	if (0.1f > m_fTurnDirWeight)
	{
		m_fTurnDirWeight = 0.1f;
	}

	if (0.95f <= XMVectorGetX(XMVector3Dot(XMVector3Normalize(vTurnDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)))))
	{
		_float fSpeed = m_pTransformCom->Get_MoveSpeed();
		_float Length = XMVectorGetX(XMVector3Length(vTurnDir));

		if (fSpeed < Length)
		{
			m_pTransformCom->Set_MoveSpeed(fSpeed + 0.6f);
		}
		else if (fSpeed > 20.f)
		{
			m_pTransformCom->Set_MoveSpeed(fSpeed - 0.4f);
		}
		else
		{
			m_pTransformCom->Set_MoveSpeed(20.f);
		}
	}
	else
	{
		m_pTransformCom->Set_MoveSpeed(20.f);
	}
	
	if (XMVectorGetX(XMVector3Length(vTurnDir)) <= 0.5f)
	{
		m_eCurState = CHAKRA_GOBACK;
		m_fTurnDirWeight = 0.9f;
	}

	m_fAnimSpeed = 2.f;

	Set_LimLight_N_Emissive(_float4(1.f, 1.f, 0.2f, 1.f), _float4(0));
	return _int();
}

_int CPlayerWeapon_Chakra::Update_GoBackState(_double fDeltaTime)
{
	_Vector vTurnDir = m_tPlayerWeaponDesc.eAttachedDesc.Get_AttachObjectTransform()->Get_MatrixState(CTransform::TransformState::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);//m_fAttackTargetPoint.XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	vTurnDir = XMVectorSetY(vTurnDir, 0);
	m_pTransformCom->Move_Forward(fDeltaTime);
	m_pTransformCom->Turn_Dir(vTurnDir, m_fTurnDirWeight);
	m_fTurnDirWeight -= (_float)fDeltaTime;
	if (0.1f > m_fTurnDirWeight)
	{
		m_fTurnDirWeight = 0.1f;
	}

	if (0.95f <= XMVectorGetX(XMVector3Dot(XMVector3Normalize(vTurnDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)))))
	{
		_float fSpeed = m_pTransformCom->Get_MoveSpeed();
		_float Length = XMVectorGetX(XMVector3Length(vTurnDir));

		if (fSpeed < Length)
		{
			m_pTransformCom->Set_MoveSpeed(fSpeed + 0.6f);
		}
		else if (fSpeed > 20.f)
		{
			m_pTransformCom->Set_MoveSpeed(fSpeed - 0.4f);
		}
		else
		{
			m_pTransformCom->Set_MoveSpeed(20.f);
		}
	}
	else
	{
		m_pTransformCom->Set_MoveSpeed(20.f);
	}

	if (XMVectorGetX(XMVector3Length(vTurnDir)) <= 1.f)
	{
		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Chakra_Start.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		m_pTransformCom->LookDir(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * -1);
		m_eCurState = CHAKRA_IDLE;
		m_bAttackStart = false;
		m_fAnimSpeed = 1.f;
	}

	return _int();
}

void CPlayerWeapon_Chakra::Update_Trail(_double fDeltaTime)
{
	m_pSwordTrail->Update_SwordTrail(
		m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.5f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.25f),
		m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.5f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.25f),
		fDeltaTime
	);
}

void CPlayerWeapon_Chakra::Update_Colliders()
{
	m_pCollider->Update_Transform(0, m_pTransformCom->Get_WorldMatrix());
	m_pCollider->Update_Transform(1, m_pTransformCom->Get_WorldMatrix());
}

void CPlayerWeapon_Chakra::Check_AttackStart()
{
	if (g_pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);


		_Vector vCursorPos = XMVectorSet(
			(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
			(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
			0, 1.f);

		_Matrix InvProjMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_PROJ));

		_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

		_Matrix InvViewMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
		vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


		CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
		_Vector vCamPos = pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

		if (XMVectorGetY(vCamPos) * XMVectorGetY(vRayDir) < 0)
		{
			_float fPos_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			_float Scale = (XMVectorGetY(vCamPos) - fPos_Y) / -(XMVectorGetY(vRayDir));

			_float3 vTargetPos = vCamPos + (Scale)* vRayDir;

			m_fAttackTargetPoint.x = vTargetPos.x;
			m_fAttackTargetPoint.y = fPos_Y;
			m_fAttackTargetPoint.z = vTargetPos.z;
		}

		if (false == m_bAttackStart)
		{
			m_bAttackStart = true;
			m_pTransformCom->LookAt(XMLoadFloat3(&m_fAttackTargetPoint));
		}

		if (CHAKRA_IDLE == m_eCurState)
		{
			Active_Trail(true);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Chakra_Start.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

		m_fTurnDirWeight = 0.9f;
		m_eCurState = CHAKRA_MOV;
	}
}

HRESULT CPlayerWeapon_Chakra::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Chakra), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 10;
	tDesc.fRotationPerSec = XMConvertToRadians(120);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CSwordTrail::TRAILDESC tSwordDesc;
	tSwordDesc.iPassIndex = 0;
	tSwordDesc.vColor = _float4(1.f, 0.5745f, 0.9745f, 1.f);
	tSwordDesc.iTextureIndex = 1;
	tSwordDesc.NoiseSpeed = 0;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_SwordTrail), TAG_COM(Com_SwordTrail), (CComponent**)&m_pSwordTrail, &tSwordDesc));

	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM_ATTACHED_OLCD;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CPlayerWeapon_Chakra::SetUp_EtcInfo()
{
	return S_OK;
}

HRESULT CPlayerWeapon_Chakra::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

CPlayerWeapon_Chakra * CPlayerWeapon_Chakra::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Chakra*	pInstance = NEW CPlayerWeapon_Chakra(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Chakra");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Chakra::Clone(void * pArg)
{
	CPlayerWeapon_Chakra*	pInstance = NEW CPlayerWeapon_Chakra(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Chakra");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerWeapon_Chakra::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pSwordTrail);
	Safe_Release(m_pCollider);
	Safe_Release(m_pDissolveCom);
}
