#include "stdafx.h"
#include "CflameTerrain.h"

CflameTerrain::CflameTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CflameTerrain::CflameTerrain(const CflameTerrain & rhs)
	: CMonsterWeapon(rhs),
	m_FireCrackParticle(rhs.m_FireCrackParticle)
{
}

HRESULT CflameTerrain::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	m_FireCrackParticle = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_14");

	return S_OK;
}

HRESULT CflameTerrain::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
	}

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	m_pTransformCom->Rotation_CW(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(90));
	m_pTransformCom->Turn_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(pUtil->RandomFloat(0, 360)));

	//Set_LimLight_N_Emissive(_float4(255.f, 0.f, 10.f, 255.f), _float4(0));

	_float Sacle = pUtil->RandomFloat(10.f, 15.f);
	m_pTransformCom->Scaled_All(_float3(Sacle));

	m_fAliveTime = 7.f;

	Set_LimLight_N_Emissive(_float4(0), _float4(0,0.5f,0.01f,1));
	m_fPassedTime = 0;
	m_iTextureIndex = rand() % 8 + 2;


	m_FireCrackParticle.vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	m_FireCrackParticle.eInstanceCount = Prototype_VIBuffer_Point_Instance_128;
	Sacle *= 0.5f;
	m_FireCrackParticle.ParticleStartRandomPosMin = _float3(-Sacle, 0, -Sacle);
	m_FireCrackParticle.ParticleStartRandomPosMax = _float3(Sacle, 2.f, Sacle);
	m_bCreatedParticle = false;
	return S_OK;
}

_int CflameTerrain::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	m_fAliveTime -= (_float)fDeltaTime;
	m_fPassedTime += (_float)fDeltaTime;

	if (m_fAliveTime <= 0)

	{
		Set_IsDead();
	}

	if (!m_bCreatedParticle && m_fPassedTime > m_fApearTime)
	{
		m_bCreatedParticle = true;
		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_FireCrackParticle);
	}

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_Matrix Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pCollider->Update_Transform(0, Matrix);

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	return _int();
}

_int CflameTerrain::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SUBBLEND, this));

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pCollider));
#endif // _DEBUG

	return _int();
}

_int CflameTerrain::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	_float g_Alpha = 0.35f;

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_Alpha", &g_Alpha, sizeof(_float)));

	

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;




	//m_fPassedTime = 0;
	//m_fTotalTargetTime = 7.f;
	//m_fApearTime = 1.5f;
	FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DissolveNoiseTexture", 0));
	FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_BurnRampTexture", 1));

	//float DissolveValue = g_fTimer - (g_fMaxTime - g_fAppearTimer) / g_fAppearTimer;

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &(m_fPassedTime), sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &(m_fTotalTargetTime), sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fPaperCurlIntensive", &(m_fApearTime), sizeof(_float)));
	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &(m_fDisApearTime), sizeof(_float)));




	m_pVIBufferCom->Render(m_pShaderCom, 9);

	return _int();
}

_int CflameTerrain::LateRender()
{
	return _int();
}

void CflameTerrain::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CflameTerrain::Compute_RenderSortValue()
{
	
	return m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y - 99999999.f;
}

HRESULT CflameTerrain::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Texture_ChiedTianFlame), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	m_pTextureCom->Change_TextureLayer(L"ChiedTianFlame");

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = 1;
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	return S_OK;
}

CflameTerrain * CflameTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CflameTerrain*	pInstance = NEW CflameTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CVolcano");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CflameTerrain::Clone(void * pArg)
{
	CflameTerrain*	pInstance = NEW CflameTerrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CChiedtuan_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CflameTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
