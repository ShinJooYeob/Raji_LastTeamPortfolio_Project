#include "stdafx.h"
#include "..\public\Rangda_MagicCircle.h"




CRangda_MagicCircle::CRangda_MagicCircle(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRangda_MagicCircle::CRangda_MagicCircle(const CRangda_MagicCircle & rhs)
	: CGameObject(rhs),
	m_vecJYTextureParticleDesc(rhs.m_vecJYTextureParticleDesc),
	m_vecJYNonInstMeshDesc(rhs.m_vecJYNonInstMeshDesc)
{
}

HRESULT CRangda_MagicCircle::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	m_vecJYTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Boss_Rangda_mandara"));
	m_vecJYTextureParticleDesc[0].vEmissive_SBB = _float3(1.f, 1.f, 0.5f);
	m_vecJYTextureParticleDesc[0].ParticleSize = _float3(5.0f);
	m_vecJYTextureParticleDesc[0].EachParticleLifeTime = 2.f;


	//////////////////////////////////////////////////////////////////////////
	//0
	{
		NONINSTNESHEFTDESC tNIMEDesc;


		tNIMEDesc.eMeshType = Prototype_Mesh_Rangda_Effect_1;

		tNIMEDesc.fMaxTime_Duration = 0.7f;
		tNIMEDesc.fAppearTime = 0.35f;
		tNIMEDesc.noisingdir = _float2(1, 0);

		tNIMEDesc.NoiseTextureIndex = 350;
		tNIMEDesc.MaskTextureIndex = 108;
		tNIMEDesc.iDiffuseTextureIndex = 331;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 1.f, 1.f, 1);

		tNIMEDesc.vLimLight = _float4(0.8f, 0.f, 1.f, 1.f);
		tNIMEDesc.vColor = _float3(0.3f, 0.f, 0.f);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.vSize = _float3(2.f, 0.7f, 2.f);

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0;

		m_vecJYNonInstMeshDesc.push_back(tNIMEDesc);
	}
	//1
	{


		NONINSTNESHEFTDESC tNIMEDesc;


		tNIMEDesc.eMeshType = Prototype_Mesh_Cylinder;

		tNIMEDesc.fMaxTime_Duration = 1.5f;
		tNIMEDesc.fAppearTime = 0.15f;
		tNIMEDesc.noisingdir = _float2(0, -1).Get_Nomalize();

		tNIMEDesc.NoiseTextureIndex = 199;
		tNIMEDesc.MaskTextureIndex = 109;
		tNIMEDesc.iDiffuseTextureIndex = 331;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 1);

		tNIMEDesc.vLimLight = _float4(0.8f, 0.f, 1.f, 1.f);
		tNIMEDesc.vColor = _float3(0.3f, 0.f, 0.f);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.vSize = _float3(1.f, 3.f, 1.f);

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0;


		m_vecJYNonInstMeshDesc.push_back(tNIMEDesc);

		//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);


	}

	return S_OK;
}

HRESULT CRangda_MagicCircle::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(90));

	//Set_LimLight_N_Emissive(_float3(0.2f, 0.5f, 1.f ));


	m_pTransformCom->Scaled_All(_float3(10.f, 10.f, 10.f));

	m_fAlpha = 0.00001f;


	g_pGameInstance->Play3D_Sound(TEXT("JJB_Rangda_Spell_02.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

	m_vecJYTextureParticleDesc[0].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(0, 0.25f,0,0);
	m_vecJYNonInstMeshDesc[0].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	m_vecJYNonInstMeshDesc[0].vLookDir = _float3(1, 0, 0);

	m_vecJYNonInstMeshDesc[1].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	//m_vecJYTextureParticleDesc[0].EachParticleLifeTime = 2.f;
	//m_vecJYNonInstMeshDesc[0].fMaxTime_Duration = 1.4f;
	//m_vecJYNonInstMeshDesc[0].fAppearTime = 0.35f;


	switch (rand() % 3)
	{
	case 0:
		m_vecJYNonInstMeshDesc[1].vLookDir = _float3(1, 0, 0);
		break;
	case 1:
		m_vecJYNonInstMeshDesc[1].vLookDir = _float3(-1, 0, 0);
		break;
	case 2:
		m_vecJYNonInstMeshDesc[1].vLookDir = _float3(0, 0, 1);
		break;
	default:
		m_vecJYNonInstMeshDesc[1].vLookDir = _float3(0, 0, 1);
		break;
	}


	GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[0]);
	g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYNonInstMeshDesc[0]);

	return S_OK;
}

_int CRangda_MagicCircle::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	m_fAlpha += (_float)fDeltaTime * 1.5f;

	if (m_bIsVanish)
	{
		m_fVanishTime -= (_float)fDeltaTime;
	}

	if (m_fAlpha >= 1.f)
	{
		if (!m_bIsVanish)
		{
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYNonInstMeshDesc[1]);
		}
		m_bIsVanish = true;
	}

	if (m_fVanishTime <= 0)
	{
		Set_IsDead();
	}

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_Matrix Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pCollider->Update_Transform(0, Matrix);

	if (m_bIsVanish)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));
	}

	return _int();
}

_int CRangda_MagicCircle::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	//if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	//{
	//	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this));
	//}

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pCollider));
#endif // _DEBUG

	return _int();
}

_int CRangda_MagicCircle::Render()
{
	if (__super::Render() < 0)
		return -1;

	//NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);


	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//CGameInstance* pInstance = GetSingle(CGameInstance);
	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	//if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture",0)))
	//	return E_FAIL;

	//m_pShaderCom->Set_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));

	//m_pVIBufferCom->Render(m_pShaderCom, 2);

	return 0;
}

_int CRangda_MagicCircle::LateRender()
{
	return _int();
}

void CRangda_MagicCircle::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

void CRangda_MagicCircle::ChangeScaledAndTexture()
{

	m_pTransformCom->Scaled_All(_float3(15.f, 15.f, 15.f));
}

HRESULT CRangda_MagicCircle::SetUp_Components()
{

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));


	
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	return S_OK;
}

CRangda_MagicCircle * CRangda_MagicCircle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRangda_MagicCircle*	pInstance = NEW CRangda_MagicCircle(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRangda_MagicCircle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRangda_MagicCircle::Clone(void * pArg)
{
	CRangda_MagicCircle*	pInstance = NEW CRangda_MagicCircle(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CRangda_MagicCircle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRangda_MagicCircle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
}
