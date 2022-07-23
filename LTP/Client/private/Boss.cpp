#include "stdafx.h"
#include "..\public\Boss.h"

CBoss::CBoss(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CBoss::CBoss(const CBoss & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	FAILED_CHECK(Ready_ParticleDesc());

	return S_OK;
}

HRESULT CBoss::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	//FAILED_CHECK(SetUp_Components());

	//if (pArg != nullptr)
	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	//m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	////Set_LimLight_N_Emissive(_float3(0.2f, 0.5f, 1.f ));

	//m_pModel->Change_AnimIndex(5);

	return S_OK;
}

_int CBoss::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	//FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime, m_bIsOnScreen));
	//FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	Update_Particle(fDeltaTime);


	return _int();
}

_int CBoss::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	//FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	//FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	//m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	////g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	return _int();
}

_int CBoss::Render()
{
	if (__super::Render() < 0)		return -1;

	//NULL_CHECK_RETURN(m_pModel, E_FAIL);

	//CGameInstance* pInstance = GetSingle(CGameInstance);
	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}

	return _int();
}

_int CBoss::LateRender()
{
	if (__super::LateRender() < 0)		return -1;
	return _int();
}






#pragma region Particle

HRESULT CBoss::Ready_ParticleDesc()
{
	// Particle Ready

	return S_OK;
}


HRESULT CBoss::Set_Play_Particle(_uint ParticleIndex, _fVector FixVec, _float3 offset, _float Timer)
{
	if (PARTILCECOUNT <= ParticleIndex)
	{
		DEBUGBREAK;
		return E_FAIL;
	}

	if (m_vecTextureParticleDesc.size() <= ParticleIndex)
	{
		DEBUGBREAK;
		return E_FAIL;
	}
	bool isCreate = false;

	if (m_fPlayParticleTimer[ParticleIndex] <= 0.0f)
	{
		isCreate = true;

		if (Timer == -1)
		{
			m_fPlayParticleTimer[ParticleIndex] = m_vecTextureParticleDesc[ParticleIndex].TotalParticleTime;
		}
		else
			m_fPlayParticleTimer[ParticleIndex] = Timer;
	}

	// 위치
	if (isCreate)
	{
		_Vector CreatePos;

		if (m_vecTextureParticleDesc[ParticleIndex].FollowingTarget)
		{
			if (offset.Get_Lenth() != 0)
			{
				_Matrix  TargetMat = m_vecTextureParticleDesc[ParticleIndex].FollowingTarget->Get_WorldMatrix();

				TargetMat.r[0] = XMVector3Normalize(TargetMat.r[0]);
				TargetMat.r[1] = XMVector3Normalize(TargetMat.r[1]);
				TargetMat.r[2] = XMVector3Normalize(TargetMat.r[2]);

				_Vector WorldPos = TargetMat.r[3];
				CreatePos = WorldPos + TargetMat.r[0] * offset.x + TargetMat.r[1] * offset.y + TargetMat.r[2] * offset.z;
				CTransform* cashtrans = m_vecTextureParticleDesc[ParticleIndex].FollowingTarget;
				m_vecTextureParticleDesc[ParticleIndex].FollowingTarget = nullptr;
				m_vecTextureParticleDesc[ParticleIndex].vFixedPosition = CreatePos;
				GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);
				m_vecTextureParticleDesc[ParticleIndex].FollowingTarget = cashtrans;

			}
			else
				GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);

		}
		else
		{
			// if (defaultTrans == nullptr)
			// 	return S_OK;
			// 
			// _Matrix WeaponMat = defaultTrans->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
			// 
			// WeaponMat.r[0] = XMVector3Normalize(WeaponMat.r[0]);
			// WeaponMat.r[1] = XMVector3Normalize(WeaponMat.r[1]);
			// WeaponMat.r[2] = XMVector3Normalize(WeaponMat.r[2]);
			// 
			// _Vector WorldPos = WeaponMat.r[3];
			// CreatePos = WorldPos + WeaponMat.r[0] * offset.z + WeaponMat.r[1] * offset.z + WeaponMat.r[2] * offset.z;


			m_vecTextureParticleDesc[ParticleIndex].vFixedPosition = FixVec;
			GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);

		}
	}

	return S_OK;
}

HRESULT CBoss::Set_Play_Particle_Must(_uint ParticleIndex, _float3 FixPos, _float3 offset, _float Timer)
{
	if (PARTILCECOUNT <= ParticleIndex)
		return E_FAIL;

	if (m_vecTextureParticleDesc.size() <= ParticleIndex)
		return E_FAIL;

	bool isCreate = true;
	m_fPlayParticleTimer[ParticleIndex] = m_vecTextureParticleDesc[ParticleIndex].TotalParticleTime;


	// 위치
	if (isCreate)
	{
		_Vector CreatePos;

		if (m_vecTextureParticleDesc[ParticleIndex].FollowingTarget)
		{

			if (m_vecTextureParticleDesc[ParticleIndex].FollowingTarget->Get_IsOwnerDead() == true)
				(m_vecTextureParticleDesc[ParticleIndex].FollowingTarget->Set_IsOwnerDead(false));

			GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);



		}
		else
		{

			CreatePos = FixPos.XMVector() + offset.XMVector();

			m_vecTextureParticleDesc[ParticleIndex].vFixedPosition = CreatePos;
			GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);

		}
	}


	return S_OK;
}
HRESULT CBoss::Set_Dead_Transform(_uint ParticleIndex)
{
	if (m_vecTextureParticleDesc[ParticleIndex].FollowingTarget)
	{
		m_vecTextureParticleDesc[ParticleIndex].FollowingTarget->Set_IsOwnerDead(true);

	}

	return S_OK;
}

HRESULT CBoss::Update_Particle(_double timer)
{

	for (auto& t : m_fPlayParticleTimer)
	{
		t -= (_float)timer;
		if (t <= -500)
			t = -1;
	}

	return S_OK;
}

HRESULT CBoss::Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ type, CTransform* trans, bool * pb)
{
	NULL_CHECK_BREAK(trans);

	if (trans)
	{
		if (trans->Get_IsOwnerDead() == true)
		{
			trans->Set_IsOwnerDead(false);
		}

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc_Hard(type, trans);
		if (pb)
			*pb = true;
	}

	return S_OK;
}

INSTPARTICLEDESC & CBoss::Get_VecParticle(_uint index)
{
	if (m_vecTextureParticleDesc.size() < index)
	{
		return m_vecTextureParticleDesc[0];
	}

	return m_vecTextureParticleDesc[index];
}
void CBoss::Set_VecParticle(_uint index, INSTPARTICLEDESC & d)
{
	if (m_vecTextureParticleDesc.size() < index)
		return;

	m_vecTextureParticleDesc[index] = d;
}


#pragma endregion Particle

void CBoss::Free()
{
	__super::Free();
}
