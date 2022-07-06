#include "stdafx.h"
#include "..\public\Mahabalasura_Weapon.h"

CMahabalasura_Weapon::CMahabalasura_Weapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CMahabalasura_Weapon::CMahabalasura_Weapon(const CMahabalasura_Weapon & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CMahabalasura_Weapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMahabalasura_Weapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_eAttachedDesc, sizeof(ATTACHEDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_eAttachedDesc, pArg, sizeof(ATTACHEDESC));
	}

	FAILED_CHECK(SetUp_Components());

	//m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(170.f));
	m_pTransformCom->Scaled_All(_float3(0.8f));
	return S_OK;
}

_int CMahabalasura_Weapon::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;


	return _int();
}

_int CMahabalasura_Weapon::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	Update_AttachMatrix();
	//m_pTransformCom->Scaled_All(_float3(1.f, 1.f, 1.f));
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &m_fAttachedMatrix));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);



	//_Matrix mat =
	//	XMMatrixRotationX(XMConvertToRadians(0))*
	//	XMMatrixRotationY(XMConvertToRadians(0))*
	//	XMMatrixRotationZ(XMConvertToRadians(0));

	//m_pTransformCom->Set_Matrix(mat);




	return _int();
}

_int CMahabalasura_Weapon::Render()
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

_int CMahabalasura_Weapon::LateRender()
{
	return _int();
}

void CMahabalasura_Weapon::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_eAttachedDesc.Caculate_AttachedBoneMatrix();
}

HRESULT CMahabalasura_Weapon::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasurWeapon), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

CMahabalasura_Weapon * CMahabalasura_Weapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura_Weapon*	pInstance = new CMahabalasura_Weapon(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChiedtuan_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura_Weapon::Clone(void * pArg)
{
	CMahabalasura_Weapon*	pInstance = new CMahabalasura_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMahabalasura_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
