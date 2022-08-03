#include "stdafx.h"
#include "..\public\LilyPad.h"
#include "Player.h"

CLilyPad::CLilyPad(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CLilyPad::CLilyPad(const CLilyPad & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLilyPad::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CLilyPad::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	_float3 vPos = *(static_cast<_float3*>(pArg));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_Etc());

	Set_IsOcllusion(false);

	Set_LimLight_N_Emissive(_float4(0.f, 0.0625f, 0.06640625f, 0.55859375f), _float4(0.2f, 0.5f, 1.f, 0.f));

	return S_OK;
}

_int CLilyPad::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;



	if (true == m_bScaling)
	{
		m_pTransformCom->Scaling_All(fDeltaTime);
		if (XMVectorGetX(m_pTransformCom->Get_Scale()) >= 2.5f)
		{
			m_pTransformCom->Scaled_All(_float3(2.5f));
			m_bScaling = false;
		}
	}


	m_fDuaton -= (_float)fDeltaTime;
	if (m_fDuaton <= 0.f)
	{
		Set_IsDead();
		return _int();
	}
	else if (m_fDuaton <= 3.f)
	{
		m_pTransformCom->Move_Down(fDeltaTime);
	}


	Update_Colliders();
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pCollider));

	return _int();
}

_int CLilyPad::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CLilyPad::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPEFORENGINE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 2, i, "g_BoneMatrices"));
	}


	return _int();
}

_int CLilyPad::LateRender()
{
	return _int();
}

void CLilyPad::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player)
	{
		switch (iMyColliderIndex)
		{
		case 1:
			static_cast<CPlayer*>(pConflictedObj)->Set_OnLilyPad(true);
			break;
		case 2:
			static_cast<CPlayer*>(pConflictedObj)->Set_OnLilyPad(false);
			break;
		}
	}
}

void CLilyPad::Update_Colliders()
{
	m_pCollider->Update_Transform(0, m_pTransformCom->Get_WorldMatrix());
	m_pCollider->Update_Transform(1, m_pTransformCom->Get_WorldMatrix());
	m_pCollider->Update_Transform(2, m_pTransformCom->Get_WorldMatrix());
}

HRESULT CLilyPad::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_InteractObj_LilyPad), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CLilyPad::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.9f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

HRESULT CLilyPad::SetUp_Etc()
{ 
	m_pTransformCom->Scaled_All(_float3(0.5f));
	m_pTransformCom->Set_ScalingSpeed(8.f);
	return S_OK;
}

CLilyPad * CLilyPad::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CLilyPad*	pInstance = NEW CLilyPad(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CLilyPad");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLilyPad::Clone(void * pArg)
{
	CLilyPad*	pInstance = NEW CLilyPad(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CLilyPad");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLilyPad::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pModel);
}
