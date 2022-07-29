#include "stdafx.h"
#include "..\public\Mahabalasura_SpearWave.h"


CMahabalasura_SpearWave::CMahabalasura_SpearWave(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMahabalasura_SpearWave::CMahabalasura_SpearWave(const CMahabalasura_SpearWave & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMahabalasura_SpearWave::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMahabalasura_SpearWave::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Collider());

	
	memcpy(&m_tDesc, pArg, sizeof(SPEARWAVEDESC));

	m_pTransformCom->LookDir_ver2(m_tDesc.fLookDir.XMVector());

	//if (tSpearWaveDesc.IsHorizon)
	//	m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK), 1);

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_tDesc.fStartPos);





	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	//	0
	m_tTextureParticleDesc = pUtil->Get_TextureParticleDesc(L"SpearWave_SmallParticle");
	m_tTextureParticleDesc.FollowingTarget = m_pTransformCom;
	m_tTextureParticleDesc.iFollowingDir = FollowingDir_Look;
	m_tTextureParticleDesc.TotalParticleTime = m_fMaxTime_Duration - 0.5f;

	pUtil->Create_TextureInstance(m_eNowSceneNum, m_tTextureParticleDesc);


	m_tTextureParticleDesc.ParticleStartRandomPosMax = _float3(2.f, 0.1f, 2.0f);
	m_tTextureParticleDesc.ParticleStartRandomPosMin = _float3(-2.f, -0.1f, -2.0f);
	

	pUtil->Create_TextureInstance(m_eNowSceneNum, m_tTextureParticleDesc);


	m_vRight = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), 0));
	if(m_tDesc.iDir)
		m_pTransformCom->Set_MoveSpeed(m_pTransformCom->Get_MoveSpeed() * 1.1f);
	return S_OK;
}

_int CMahabalasura_SpearWave::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	Update_Colliders();
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	m_fCurTime_Duration += (_float)fDeltaTime;
	m_fDirTurningTime_Duration += (_float)fDeltaTime;

	if (m_tDesc.iDir )
	{
		if (m_fDirTurningTime_Duration < 0.5f)
		{

			switch (m_tDesc.iDir)
			{
			case 1:
				m_pTransformCom->LookDir_ver2(m_vRight.XMVector() * 0.05f +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.95f);
					
				break;
			case 2:
				m_pTransformCom->LookDir_ver2(m_vRight.XMVector() * -0.05f +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.95f);
				break;

			}
		}
		else
		{

			m_fDirTurningTime_Duration = 0.0f;
			m_tDesc.iDir = (m_tDesc.iDir == 1) ? 2: 1;
		}

	}


	if (m_fCurTime_Duration < 0.5f)
	{

		m_pTransformCom->Move_Forward(fDeltaTime * ( 0.5f - m_fCurTime_Duration));
		_float3 vScale = g_pGameInstance->Easing_Vector(TYPE_Linear, _float3(11.0f, 5.0f, 5.0f), _float3(7.f, 3.0f, 3.0f),
			m_fCurTime_Duration, 0.5f);


		m_pTransformCom->Scaled_All(vScale);
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK), fDeltaTime * 4);
	}
	else
	{
		m_pTransformCom->Move_Forward(fDeltaTime );
	}


	if (m_fCurTime_Duration >= m_fMaxTime_Duration)
	{
		Set_IsDead();
	}
	return _int();
}

_int CMahabalasura_SpearWave::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DISTORTION, this));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CMahabalasura_SpearWave::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);





	//tNIMEDesc.NoiseTextureIndex = 381;
	//tNIMEDesc.MaskTextureIndex = 10;
	//tNIMEDesc.iDiffuseTextureIndex = 365;
	//tNIMEDesc.m_iPassIndex = 19;
	//tNIMEDesc.vColor = _float3(1.0f, 1, 1);




	_float4 color = _float4(1.0f, 1, 1, 1);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &color, sizeof(_float4)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	_float2 noisingdir = _float2(1, 0);
	_float fDistortionNoisingPushPower = 0.5f;

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_fCurTime_Duration, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &noisingdir, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &fDistortionNoisingPushPower, sizeof(_float)));



	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", 137));
	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", 0));
	FAILED_CHECK(m_pShaderCom->Set_Texture("g_BackBufferTexture", g_pGameInstance->Get_SRV(L"Target_ReferenceDefferred")));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			if (j == 1)
			{
				FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", 365));
			}
			else
			{
				FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
			}
		}

		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK), 1);
		FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 16, i));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK), -1);
		FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 16, i));
	}

	return _int();
}

_int CMahabalasura_SpearWave::LateRender()
{
	return _int();
}

void CMahabalasura_SpearWave::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Monster == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(0.5f);

	}
}

void CMahabalasura_SpearWave::Update_Colliders()
{
	_Matrix Matrix = m_pTransformCom->Get_WorldMatrix();

	m_pCollider->Update_Transform(0, Matrix);
	m_pCollider->Update_Transform(1, Matrix);
}

HRESULT CMahabalasura_SpearWave::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerSkill_SpearWave), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(90);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CMahabalasura_SpearWave::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

CMahabalasura_SpearWave * CMahabalasura_SpearWave::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura_SpearWave*	pInstance = NEW CMahabalasura_SpearWave(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMahabalasura_SpearWave");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura_SpearWave::Clone(void * pArg)
{
	CMahabalasura_SpearWave*	pInstance = NEW CMahabalasura_SpearWave(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMahabalasura_SpearWave");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura_SpearWave::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
}
