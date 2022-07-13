#include "stdafx.h"
#include "..\public\TreeMesh.h"

CTreeMesh::CTreeMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CTreeMesh::CTreeMesh(const CTreeMesh & rhs)
	: CMapObject(rhs)
{
}

HRESULT CTreeMesh::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}

HRESULT CTreeMesh::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
	{
		m_DemonTreeDesc = *(DEMONTREEDESC*)pArg;
	}

	FAILED_CHECK(SetUp_Components());
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_DemonTreeDesc.m_Pos);
	m_iPassIndex = 3;

	m_pTransformCom->Scaled_All(_float3(1.3f));

	return S_OK;
}

_int CTreeMesh::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;



	return _int();
}

_int CTreeMesh::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;




	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));

	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	return 0;
}

_int CTreeMesh::Render()
{
	if (__super::Render() < 0)
		return -1;


	NULL_CHECK_RETURN(m_pModel, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	FAILED_CHECK(__super::SetUp_ConstTable(m_pShaderCom));


	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_iPassIndex, i));
	}

	return 0;
}

_int CTreeMesh::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

void CTreeMesh::Turn_CCW()
{
	--m_fRotationAngle;
	m_pTransformCom->Rotation_CCW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(m_fRotationAngle));
}

void CTreeMesh::Turn_CW()
{
	++m_fRotationAngle;
	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(m_fRotationAngle));
}

HRESULT CTreeMesh::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	switch (m_DemonTreeDesc.m_eDemonTreeNum)
	{
	case CTreeMesh::DEMONTREE_01:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_DemonTree_Seg01), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case CTreeMesh::DEMONTREE_02:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_DemonTree_Seg02), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case CTreeMesh::DEMONTREE_03:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_DemonTree_Seg03), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case CTreeMesh::DEMONTREE_04:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_DemonTree_Seg04), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case CTreeMesh::DEMONTREE_05:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_DemonTree_Seg05), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	return S_OK;
}

CTreeMesh * CTreeMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTreeMesh*	pInstance = new CTreeMesh(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTreeMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTreeMesh::Clone(void * pArg)
{
	CTreeMesh*	pInstance = new CTreeMesh(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CTreeMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTreeMesh::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
