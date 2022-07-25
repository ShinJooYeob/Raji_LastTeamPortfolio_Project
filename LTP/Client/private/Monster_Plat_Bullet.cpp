#include "stdafx.h"
#include "..\public\Monster_Plat_Bullet.h"


CMonster_Plat_Bullet::CMonster_Plat_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CMonster_Bullet(pDevice, pDeviceContext)
{
}

CMonster_Plat_Bullet::CMonster_Plat_Bullet(const CMonster_Plat_Bullet & rhs)
	: CMonster_Bullet(rhs)
{
}

HRESULT CMonster_Plat_Bullet::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMonster_Plat_Bullet::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	NULL_CHECK_RETURN(pArg, E_FAIL);
	memcpy(&m_tMeshDesc, pArg, sizeof(NONINSTNESHEFTDESC));


	if (m_tMeshDesc.m_iPassIndex < 16 || m_tMeshDesc.m_iPassIndex > 19)
		return E_FAIL;


	FAILED_CHECK(SetUp_Components());
	


	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_tMeshDesc.vPosition);
	m_pTransformCom->LookDir(m_tMeshDesc.vLookDir.XMVector());

	m_pTransformCom->Scaled_All(m_tMeshDesc.vSize);

	switch (m_tMeshDesc.RotAxis)
	{
	case FollowingDir_Right:
		m_vRotAxis = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_RIGHT);
		break;
	case FollowingDir_Up:
		m_vRotAxis = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_UP);
		break;
	case FollowingDir_Look:
		m_vRotAxis = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_LOOK);
		break;
	default:
		__debugbreak();
		break;
	}

	if (m_tMeshDesc.OnceStartRot)
	{
		_float Old = m_pTransformCom->Get_TurnSpeed();
		m_pTransformCom->Set_TurnSpeed(1);
		m_pTransformCom->Turn_CW(m_vRotAxis.XMVector(), XMConvertToRadians(m_tMeshDesc.OnceStartRot));
		m_pTransformCom->Set_TurnSpeed(Old);

	}

	m_pTransformCom->Turn_CW(m_vRotAxis.XMVector(), m_tMeshDesc.StartRot);
	

	switch (m_tMeshDesc.MoveDir)
	{
	case FollowingDir_Right:
		m_vMoveDir = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_RIGHT);
		break;
	case FollowingDir_Up:
		m_vMoveDir = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_UP);
		break;
	case FollowingDir_Look:
		m_vMoveDir = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_LOOK);
		break;
	default:
		__debugbreak();
		break;
	}

	Set_LimLight_N_Emissive( m_tMeshDesc.vLimLight ,  m_tMeshDesc.vEmissive );


	if (m_tMeshDesc.m_CheckerBuffer == Prototype_Object_Monster_Tezabsura_Landmine)
	{

		INSTPARTICLEDESC tParticleDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"Monster_LandMine_6");
		tParticleDesc.FollowingTarget = m_pTransformCom;
		tParticleDesc.iFollowingDir = FollowingDir_Up;
		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, tParticleDesc);
	}
	return S_OK;
}

_int CMonster_Plat_Bullet::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	m_fCurTime_Duration += (_float)fDeltaTime;

	if (m_fCurTime_Duration > m_tMeshDesc.fAppearTime && m_fCurTime_Duration < m_tMeshDesc.fMaxTime_Duration - m_tMeshDesc.fAppearTime)
	{
		m_pTransformCom->Turn_CW(m_vRotAxis.XMVector(), fDeltaTime);

		if (m_tMeshDesc.MoveSpeed != 0)
		{
			m_pTransformCom->MovetoDir_bySpeed(m_vMoveDir.XMVector(), m_tMeshDesc.MoveSpeed, fDeltaTime);
		}
	}



	//m_tMeshDesc.NoiseTextureIndex = 381;
	//m_tMeshDesc.MaskTextureIndex = 10;
	//m_tMeshDesc.iDiffuseTextureIndex = 300;
	//m_tMeshDesc.m_iPassIndex = 19;
	//m_tMeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
	//m_tMeshDesc.vLimLight = _float4(1, 0, 0, 1);


	//Set_LimLight_N_Emissive(m_tMeshDesc.vLimLight, m_tMeshDesc.vEmissive);


	Update_Collider(fDeltaTime);


	if (m_fCurTime_Duration >= m_tMeshDesc.fMaxTime_Duration)
	{
		Set_IsDead();
	}

	return _int();
}

_int CMonster_Plat_Bullet::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	if (m_tMeshDesc.m_iPassIndex > 17)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	else
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DISTORTION, this));
	}


#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
#endif

	return _int();
}

_int CMonster_Plat_Bullet::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);


	//_float4 color = _float4(1.f, 0.734375f, 0.75234375f, 1);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_tMeshDesc.vColor, sizeof(_float4)));
	
	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_fCurTime_Duration, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &m_tMeshDesc.fAppearTime, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &m_tMeshDesc.fMaxTime_Duration, sizeof(_float)));
	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &m_tMeshDesc.noisingdir, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &m_tMeshDesc.fDistortionNoisingPushPower, sizeof(_float)));



	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_tMeshDesc.NoiseTextureIndex));
	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_tMeshDesc.MaskTextureIndex));
	FAILED_CHECK(m_pShaderCom->Set_Texture("g_BackBufferTexture", g_pGameInstance->Get_SRV(L"Target_ReferenceDefferred")));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			if (j == 1)
			{
				FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", m_tMeshDesc.iDiffuseTextureIndex));
			}
			else
			{
				FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
			}
		}

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_tMeshDesc.m_iPassIndex, i));
	}

	return _int();
}

_int CMonster_Plat_Bullet::LateRender()
{
	return _int();
}

void CMonster_Plat_Bullet::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMonster_Plat_Bullet::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

HRESULT CMonster_Plat_Bullet::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

	/////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(70.f, 70.f, 70.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(30.f, 80.f, 10.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	return S_OK;
}

HRESULT CMonster_Plat_Bullet::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider

	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());
	

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pColliderCom));

	return S_OK;
}


HRESULT CMonster_Plat_Bullet::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(m_tMeshDesc.eMeshType), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30;
	tDesc.fRotationPerSec = XMConvertToRadians(m_tMeshDesc.RotationSpeedPerSec);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	SetUp_Collider();

	return S_OK;
}


CMonster_Plat_Bullet * CMonster_Plat_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Plat_Bullet*	pInstance = NEW CMonster_Plat_Bullet(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Plat_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Plat_Bullet::Clone(void * pArg)
{
	CMonster_Plat_Bullet*	pInstance = NEW CMonster_Plat_Bullet(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Plat_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Plat_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pParentTranscom);
	Safe_Release(m_pColliderCom);


}
