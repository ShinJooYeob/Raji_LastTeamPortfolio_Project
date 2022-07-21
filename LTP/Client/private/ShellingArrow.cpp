#include "stdafx.h"
#include "..\public\ShellingArrow.h"
#include "..\public\PartilceCreateMgr.h"

CShellingArrow::CShellingArrow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CShellingArrow::CShellingArrow(const CShellingArrow & rhs)
	: CGameObject(rhs)
{
}

HRESULT CShellingArrow::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CShellingArrow::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	memcpy(&m_tShellingArrowDesc, pArg, sizeof(SHELLINGARROWDESC));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Collider());

	return S_OK;
}

_int CShellingArrow::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0) return -1;

	for (_uint i = 0; i < 32; i++)
	{
		if (0.5f >= XMVectorGetY(m_vecInstancedTransform[i]->Get_MatrixState(CTransform::TransformState::STATE_POS)))
		{
	
			continue;
		}

		m_vecInstancedTransform[i]->Move_Forward(dDeltaTime);
	}
	 
	if (false == m_bOnceDamage)
	{
		if (m_tShellingArrowDesc.fTargetPos.y >= XMVectorGetY(m_vecInstancedTransform[0]->Get_MatrixState(CTransform::TransformState::STATE_POS)) - 0.5f)
		{
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Bow_Shelling.wav"), m_vecInstancedTransform[0]->Get_MatrixState(CTransform::TransformState::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);

			_Vector CenterPos = _Vector();
			for (auto trans: m_vecInstancedTransform)
			{
				CenterPos += trans->Get_MatrixState_Float3(CTransform::STATE_POS).XMVector();
			}

			CenterPos /= _float(m_vecInstancedTransform.size());

			auto texDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Shift_Image);
			XMStoreFloat3(&texDesc.vFixedPosition ,CenterPos);
			texDesc.ParticleSize = _float3(1.3f);
			texDesc.ParticleSize2 = _float3(2.5f);
			texDesc.TotalParticleTime = 0.5f;
			GetSingle(CPartilceCreateMgr)->Create_Texture_Effect_Desc(texDesc, m_eNowSceneNum);

			m_bOnceDamage = true;
			Update_Colliders();
			FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider));
		}
	}

	m_fDestroy_Count -= (_float)dDeltaTime;
	if (0.f >= m_fDestroy_Count)
	{
		Set_IsDead();
	}


	return _int();
}

_int CShellingArrow::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_NONANIMINSTANCE, this, &m_vecInstancedTransform, m_pModelInstance, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CShellingArrow::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecInstancedTransform));

	return 0;
}

_int CShellingArrow::LateRender()
{	
	return _int();
}

void CShellingArrow::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Monster == eConflictedObjCollisionType)
	{
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(0.5f);


		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Arrow_Impact_0.wav"), m_vecInstancedTransform[0]->Get_MatrixState(CTransform::TransformState::STATE_POS), CHANNELID::CHANNEL_EFFECT, 0.1f);

		
		
	}
}

void CShellingArrow::Update_Colliders()
{
	_Matrix Matrix = XMMatrixIdentity();
	_float4 fPos = _float4(m_tShellingArrowDesc.fTargetPos, 1.f);
	Matrix.r[3] = XMLoadFloat4(&fPos);

	m_pCollider->Update_Transform(0, Matrix);
	m_pCollider->Update_Transform(1, Matrix);

}

HRESULT CShellingArrow::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Arrow), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	_Vector vLookDir = XMVector3Normalize(m_tShellingArrowDesc.fTargetPos.XMVector() - m_tShellingArrowDesc.fStartPos.XMVector());
	for (_uint i = 0; i < 32; i++)
	{
		CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(pTransform, E_FAIL);
		
		_float3 fStartPos = GetSingle(CUtilityMgr)->RandomFloat3(
			_float3(m_tShellingArrowDesc.fStartPos.x - 1.2f, m_tShellingArrowDesc.fStartPos.y - 3.f, m_tShellingArrowDesc.fStartPos.z - 1.2f),
			_float3(m_tShellingArrowDesc.fStartPos.x + 1.2f, m_tShellingArrowDesc.fStartPos.y + 3.f, m_tShellingArrowDesc.fStartPos.z + 1.2f)
		);

		pTransform->Scaled_All(_float3(1.6f, 1.6f, 1.6f));

		pTransform->LookDir_ver2(vLookDir);
		pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(fStartPos.x, fStartPos.y, fStartPos.z));
		pTransform->Set_MoveSpeed(30.f);

		m_vecInstancedTransform.push_back(pTransform);
	}

	
	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_32), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));

	return S_OK;
}

HRESULT CShellingArrow::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

CShellingArrow * CShellingArrow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CShellingArrow*	pInstance = NEW CShellingArrow(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CShellingArrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CShellingArrow::Clone(void * pArg)
{
	CShellingArrow*	pInstance = NEW CShellingArrow(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CShellingArrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShellingArrow::Free()
{
	__super::Free();

	for (auto& pTransform : m_vecInstancedTransform)
		Safe_Release(pTransform);
	m_vecInstancedTransform.clear();

	Safe_Release(m_pModelInstance);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}
