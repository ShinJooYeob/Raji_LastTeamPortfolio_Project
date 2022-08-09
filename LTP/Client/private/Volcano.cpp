#include "stdafx.h"
#include "..\public\Volcano.h"

_uint CVolcano::iPatterenCount = 0;

CVolcano::CVolcano(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CVolcano::CVolcano(const CVolcano & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CVolcano::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CVolcano::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
	{
		_float3 Pos =  *((_float3*)pArg);
		Pos.y -= 0.2f;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);
	}

	//Set_LimLight_N_Emissive(_float4(255.f, 0.f, 10.f, 255.f), _float4(0));

	m_pTransformCom->Scaled_All(_float3(1.5f,2.5f,1.5f));

	_float3 Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	Pos.y += -0.87888888888f + _float(iPatterenCount) * 0.01f;;
	iPatterenCount++;
	//Pos.y += 0.2f;
	g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_FlameTerrain), TAG_OP(Prototype_Object_Boss_FlameTeraain), &Pos);

	m_fCurTime_Duration = 0;
	FAILED_CHECK(Ready_MeshDesc());
	m_tSubFlameDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_tSubFlameDesc);
	return S_OK;
}

_int CVolcano::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	m_fAliveTime -= (_float)fDeltaTime;
	m_fCurTime_Duration += (_float)fDeltaTime;

	if (m_fCurTime_Duration > m_tVolcanoDesc.fMaxTime_Duration)
		Set_IsDead();

	m_pTransformCom->Turn_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(m_tVolcanoDesc.RotationSpeedPerSec) * fDeltaTime);

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_Matrix Matrix = m_pTransformCom->Get_WorldMatrix();
	m_pCollider->Update_Transform(0, Matrix);

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	return _int();
}

_int CVolcano::LateUpdate(_double fDeltaTime)
{
	if (m_bIsDead) return 0;
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pCollider));
#endif // _DEBUG



	return _int();
}

_int CVolcano::Render()
{
	if (m_bIsDead) return 0;

	Ready_MeshDesc();
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));



	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
	FAILED_CHECK(Setup_MeshDesc());

	_uint NumMaterial = m_pModel->Get_NumMaterial();
	
	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			if(j == 1)continue;
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		}
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_tVolcanoDesc.m_iPassIndex, i));
	}

	return _int();
}

_int CVolcano::LateRender()
{
	return _int();
}

void CVolcano::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

HRESULT CVolcano::Ready_MeshDesc()
{


	{
		m_tVolcanoDesc.eMeshType = Prototype_Mesh_Tornado;
		m_tVolcanoDesc.fAppearTime = 0.25f;
		m_tVolcanoDesc.fMaxTime_Duration = m_tVolcanoDesc.fAppearTime*2.f;

		m_tVolcanoDesc.noisingdir = _float2(0, 1);

		m_tVolcanoDesc.fDistortionNoisingPushPower = 20.f;
		m_tVolcanoDesc.NoiseTextureIndex = 6;
		m_tVolcanoDesc.MaskTextureIndex = 81;
		m_tVolcanoDesc.iDiffuseTextureIndex = 299;
		m_tVolcanoDesc.m_iPassIndex = 19;
		m_tVolcanoDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		m_tVolcanoDesc.vLimLight = _float4(0.35f, 0.85f, 0.35f, 1);
		m_tVolcanoDesc.NoiseTextureIndex = 381;
		m_tVolcanoDesc.vColor = _float4(1, 1, 1, 1);

		m_tVolcanoDesc.RotAxis = FollowingDir_Up;
		m_tVolcanoDesc.RotationSpeedPerSec = -1080.f;
		m_tVolcanoDesc.vSize = _float3(1.f, 1.5f, 1.f);
		//m_tVolcanoDesc.vLimLight = _float4(_float3(vOldRimLightColor), 1);

		m_tVolcanoDesc.MoveDir = FollowingDir_Look;
		m_tVolcanoDesc.MoveSpeed = 0.f;

	}
	{
		m_tSubFlameDesc.eMeshType = Prototype_Mesh_Tornado2;
		m_tSubFlameDesc.vLookDir = _float3(1, 0, 0);
		m_tSubFlameDesc.fAppearTime = 0.25f;
		m_tSubFlameDesc.fMaxTime_Duration = m_tSubFlameDesc.fAppearTime*2.f;

		m_tSubFlameDesc.noisingdir = _float2(1, 0);

		m_tSubFlameDesc.fDistortionNoisingPushPower = 20.f;
		//m_tSubFlameDesc.NoiseTextureIndex = 381;
		m_tSubFlameDesc.NoiseTextureIndex = 6;
		m_tSubFlameDesc.MaskTextureIndex = 81;
		m_tSubFlameDesc.iDiffuseTextureIndex = 299;
		m_tSubFlameDesc.m_iPassIndex = 19;
		m_tSubFlameDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		//m_tSubFlameDesc.vLimLight = _float4(0.35f, 0.85f, 0.35f, 0.1f);
		m_tSubFlameDesc.vColor = _float4(1, 1, 1, 1);

		m_tSubFlameDesc.RotAxis = FollowingDir_Up;
		m_tSubFlameDesc.RotationSpeedPerSec = 1080.f;
		m_tSubFlameDesc.vSize = _float3(1.5f, 2.0f, 1.5f);
		//m_tSubFlameDesc.vLimLight = _float4(_float3(vOldRimLightColor), 1);

		m_tSubFlameDesc.MoveDir = FollowingDir_Look;
		m_tSubFlameDesc.MoveSpeed = 0.f;
		m_tSubFlameDesc.fAlphaTestValue = 0.8f;
		
	}
	
	return S_OK;
}

HRESULT CVolcano::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Tornado), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = 1;
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.6f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.1f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	return S_OK;
}

HRESULT CVolcano::Setup_MeshDesc()
{
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_fCurTime_Duration, sizeof(_float)));


	Set_LimLight_N_Emissive(m_tVolcanoDesc.vLimLight, m_tVolcanoDesc.vEmissive);


	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_tVolcanoDesc.vColor, sizeof(_float4)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &m_tVolcanoDesc.fAppearTime, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &m_tVolcanoDesc.fMaxTime_Duration, sizeof(_float)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &m_tVolcanoDesc.noisingdir, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &m_tVolcanoDesc.fDistortionNoisingPushPower, sizeof(_float)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &m_tVolcanoDesc.fAlphaTestValue, sizeof(_float)));

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_tVolcanoDesc.NoiseTextureIndex));
	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_tVolcanoDesc.MaskTextureIndex));
	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", m_tVolcanoDesc.iDiffuseTextureIndex));


	return S_OK;
}

CVolcano * CVolcano::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CVolcano*	pInstance = NEW CVolcano(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CVolcano");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CVolcano::Clone(void * pArg)
{
	CVolcano*	pInstance = NEW CVolcano(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CChiedtuan_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVolcano::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}
