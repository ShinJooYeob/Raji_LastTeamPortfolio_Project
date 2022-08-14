#include "stdafx.h"
#include "..\public\Rangda_Statue.h"

CRangda_Statue::CRangda_Statue(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRangda_Statue::CRangda_Statue(const CRangda_Statue & rhs)
	: CGameObject(rhs),
	m_tMeshDesc(rhs.m_tMeshDesc),
	m_tTexDesc(rhs.m_tTexDesc)
{
}

HRESULT CRangda_Statue::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));





	m_tMeshDesc.fMaxTime_Duration = 4.f;
	m_tMeshDesc.fAppearTime = 1.f;
	m_tMeshDesc.noisingdir = _float2(0, 1);
	m_tMeshDesc.fDistortionNoisingPushPower = 1.f;

	m_tMeshDesc.NoiseTextureIndex = 350;
	m_tMeshDesc.MaskTextureIndex = 109;
	m_tMeshDesc.iDiffuseTextureIndex = 300;


	m_tMeshDesc.m_iPassIndex = 19;
	m_tMeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1);

	m_tMeshDesc.vLimLight = _float4(0.5f, 0.5f, 1.f, 1.f);
	m_tMeshDesc.vColor = _float3(1.f, 0.f, 0.f);


	m_tTexDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"Boss_Rangda_Particle");


	m_tTexDesc.EachParticleLifeTime = 1.4f;
	m_tTexDesc.ParticleSize = _float3(0.5f, 1.f, 0.5f);



	return S_OK;
}
HRESULT CRangda_Statue::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	FAILED_CHECK(SetUp_Components());


	if (pArg != nullptr)
	{
		_float3 Pos = *(_float3*)pArg;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);
		m_tTexDesc.vFixedPosition = Pos;
	}
	m_iPassIndex = 3;

	m_pTransformCom->Scaled_All(_float3(0.05f, 0.025f, 0.05f));

	m_pTransformCom->Rotation_CCW(XMVectorSet(0,1.f,0,0),XMConvertToRadians(GetSingle(CUtilityMgr)->RandomFloat(0,360.f)));


	GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_tTexDesc);

	//Set_LimLight_N_Emissive(m_tMeshDesc.vLimLight, m_tMeshDesc.vEmissive);


	return S_OK;
}

_int CRangda_Statue::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;



	m_pDissolveCom->Update_Dissolving(fDeltaTime);

	m_fAliveTime -= (_float)fDeltaTime;


	if (!m_bIsDissolveStart && m_fAliveTime <= 3 && m_fAliveTime >= 1)
	{

		m_pTransformCom->Set_TurnSpeed(XMConvertToRadians((1.f - fabs(2.f - m_fAliveTime)) * 180.f));

		m_pTransformCom->Turn_CCW(XMVectorSet(0, 1.f, 0, 0), fDeltaTime);
	}

	if (m_fAliveTime <= 0 && m_bIsDissolveStart == false)
	{
		m_bIsDissolveStart = true;
		//Set_IsDead();
		m_fAliveTime = 4.f;
		m_bIsCollison = false;
	}

	if (m_pDissolveCom->Get_IsFadeIn() == false && m_pDissolveCom->Get_DissolvingRate() >= 1.0)
	{
		Set_IsDead();
	}

	if (m_bIsDissolveStart)
		m_pDissolveCom->Set_DissolveOn(false, 5.f);




	//m_pSubTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	_Matrix WorldMat = m_pTransformCom->Get_WorldMatrix();
	_float RotSpeed = m_pTransformCom->Get_TurnSpeed();
	m_pTransformCom->Set_TurnSpeed(1);
	m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), XMConvertToRadians(89.95f));
	m_pSubTransformCom->Set_Matrix(m_pTransformCom->Get_WorldMatrix());
	m_pSubTransformCom->Scaled_All(_float3(0.8f, 20.f, 0.5f));
	m_pTransformCom->Set_Matrix(WorldMat);
	m_pTransformCom->Set_TurnSpeed(RotSpeed);

	_Matrix Matrix = XMMatrixIdentity();
	Matrix = m_pSubTransformCom->Get_WorldMatrix(); /* * XMMatrixScaling(1.f, 3.f, 1.f);*/

	m_pRaserCollider->Update_Transform(0, Matrix);
	m_pRaserCollider->Update_ConflictPassedTime(1);
	if (m_bIsCollison)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pRaserCollider));
	}


	return _int();
}

_int CRangda_Statue::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel,nullptr,m_pDissolveCom));

	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pRaserCollider));

	return _int();
}

_int CRangda_Statue::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pDissolveCom->Render(3));



	if (m_fAliveTime < 4.f&&m_bIsDissolveStart == false)
	{



		//_float4 color = _float4(1.f, 0.734375f, 0.75234375f, 1);

		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_tMeshDesc.vColor, sizeof(_float4)));

		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_fAliveTime, sizeof(_float)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &m_tMeshDesc.fAppearTime, sizeof(_float)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &m_tMeshDesc.fMaxTime_Duration, sizeof(_float)));

		FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &m_tMeshDesc.noisingdir, sizeof(_float2)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &m_tMeshDesc.fDistortionNoisingPushPower, sizeof(_float)));



		FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_tMeshDesc.NoiseTextureIndex));
		FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_tMeshDesc.MaskTextureIndex));


		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vLimLight", &m_tMeshDesc.vLimLight, sizeof(_float4)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fEmissive", &m_tMeshDesc.vEmissive, sizeof(_float4)));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			if (j == 1)
			{
				FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", m_tMeshDesc.iDiffuseTextureIndex));
			}
			else
			{
				FAILED_CHECK(m_pSubModel->Bind_OnShader(m_pShaderCom, 0, j, MODLETEXTYPE(j)));
			}
		}

		FAILED_CHECK(m_pSubTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
		FAILED_CHECK(m_pSubModel->Render(m_pShaderCom, m_tMeshDesc.m_iPassIndex, 0));

		m_pSubTransformCom->Scaled(CTransform::STATE_UP, -20.f);
		FAILED_CHECK(m_pSubTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
		FAILED_CHECK(m_pSubModel->Render(m_pShaderCom, m_tMeshDesc.m_iPassIndex, 0));
	}
	

	return 0;
}

_int CRangda_Statue::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CRangda_Statue::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

HRESULT CRangda_Statue::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Moai_Gate), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Cylinder), TAG_COM(Com_SubModel), (CComponent**)&m_pSubModel));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_SubTransform), (CComponent**)&m_pSubTransformCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pRaserCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(8.f, 250.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pRaserCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
	

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	DissolveDesc.pShader = m_pShaderCom;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &DissolveDesc));
	return S_OK;
}

CRangda_Statue * CRangda_Statue::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRangda_Statue*	pInstance = NEW CRangda_Statue(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRangda_Statue");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRangda_Statue::Clone(void * pArg)
{
	CRangda_Statue*	pInstance = NEW CRangda_Statue(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CRangda_Statue");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRangda_Statue::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolveCom);

	Safe_Release(m_pSubModel);
	Safe_Release(m_pSubTransformCom);

	
	
}
