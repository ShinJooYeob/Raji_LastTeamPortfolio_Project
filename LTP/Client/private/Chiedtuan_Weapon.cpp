#include "stdafx.h"
#include "..\public\Chiedtuan_Weapon.h"



CChiedtuan_Weapon::CChiedtuan_Weapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CChiedtuan_Weapon::CChiedtuan_Weapon(const CChiedtuan_Weapon & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CChiedtuan_Weapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CChiedtuan_Weapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_WeaponDesc, sizeof(WEAPOPNDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_WeaponDesc, pArg, sizeof(WEAPOPNDESC));
	}

	FAILED_CHECK(SetUp_Components());

	//m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(170.f));
	m_pTransformCom->Scaled_All(_float3(0.8f));
	return S_OK;
}

_int CChiedtuan_Weapon::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;


	return _int();
}

_int CChiedtuan_Weapon::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	Update_AttachMatrix();
	//m_pTransformCom->Scaled_All(_float3(1.f, 1.f, 1.f));


	_Matrix mat = m_fAttachedMatrix.XMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &_float4x4(mat)));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);



	//_Matrix mat =
	//	XMMatrixRotationX(XMConvertToRadians(0))*
	//	XMMatrixRotationY(XMConvertToRadians(0))*
	//	XMMatrixRotationZ(XMConvertToRadians(0));

	//m_pTransformCom->Set_Matrix(mat);




	return _int();
}

_int CChiedtuan_Weapon::Render()
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

_int CChiedtuan_Weapon::LateRender()
{
	return _int();
}

void CChiedtuan_Weapon::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_WeaponDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix();
}

HRESULT CChiedtuan_Weapon::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_TR)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_TL)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon2), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_BR)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon3), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_BL)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon4), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

CChiedtuan_Weapon * CChiedtuan_Weapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CChiedtuan_Weapon*	pInstance = new CChiedtuan_Weapon(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChiedtuan_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CChiedtuan_Weapon::Clone(void * pArg)
{
	CChiedtuan_Weapon*	pInstance = new CChiedtuan_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CChiedtuan_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChiedtuan_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
