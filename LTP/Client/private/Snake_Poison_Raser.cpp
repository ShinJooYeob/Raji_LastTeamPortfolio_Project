#include "stdafx.h"
#include "..\public\Snake_Poison_Raser.h"

CSnake_Poison_Raser::CSnake_Poison_Raser(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CSnake_Poison_Raser::CSnake_Poison_Raser(const CSnake_Poison_Raser & rhs)
	:CGameObject(rhs)
{
}

HRESULT CSnake_Poison_Raser::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CSnake_Poison_Raser::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_RaserDesc, sizeof(SNAKERASERDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_RaserDesc, pArg, sizeof(SNAKERASERDESC));
	}

	FAILED_CHECK(SetUp_Components());

	m_pTransformCom->Scaled_All(_float3(100.f));

	return S_OK;
}

_int CSnake_Poison_Raser::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;



	return _int();
}

_int CSnake_Poison_Raser::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	Update_AttachMatrix();

	//FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &_float4x4(m_fAttachedMatrix), m_pDissolveCom));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	//FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	_Matrix	RotationMatrixZ = XMMatrixRotationAxis(XMVectorSet(0, 0.f, 1.f, 0), XMConvertToRadians(90.f));
	_Matrix	RotationMatrixX = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0), XMConvertToRadians(15.f));
	m_fAttachedMatrix = RotationMatrixX * RotationMatrixZ * m_fAttachedMatrix.XMatrix();
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

	//m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(90.f));

	//m_fAttachedMatrix = m_fAttachedMatrix.XMatrix() * RotationMatrix;
	//m_fAttachedMatrix.XMatrix().r[0] = XMVector4Transform(m_fAttachedMatrix.XMatrix().r[0] * XMVector3Length(m_fAttachedMatrix.XMatrix().r[0]), RotationMatrix);
	//m_fAttachedMatrix.XMatrix().r[1] = XMVector4Transform(m_fAttachedMatrix.XMatrix().r[1] * XMVector3Length(m_fAttachedMatrix.XMatrix().r[1]), RotationMatrix);
	//m_fAttachedMatrix.XMatrix().r[2] = XMVector4Transform(m_fAttachedMatrix.XMatrix().r[2] * XMVector3Length(m_fAttachedMatrix.XMatrix().r[2]), RotationMatrix);

	//m_fAttachedMatrix.Multiply(m_pTransformCom->Get_WorldMatrix());

	return _int();
}

_int CSnake_Poison_Raser::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);


	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();
	
	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		}
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 8, i));
	}
	return _int();
}

_int CSnake_Poison_Raser::LateRender()
{
	return _int();
}

void CSnake_Poison_Raser::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

void CSnake_Poison_Raser::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_RaserDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed();
}

HRESULT CSnake_Poison_Raser::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(90);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));
	
	return S_OK;
}

CSnake_Poison_Raser * CSnake_Poison_Raser::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSnake_Poison_Raser*	pInstance = NEW CSnake_Poison_Raser(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSnake_Poison_Raser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSnake_Poison_Raser::Clone(void * pArg)
{
	CSnake_Poison_Raser*	pInstance = NEW CSnake_Poison_Raser(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CSnake_Poison_Raser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnake_Poison_Raser::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
