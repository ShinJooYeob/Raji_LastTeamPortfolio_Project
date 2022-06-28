#include "stdafx.h"
#include "EditorTerrain.h"



CEditorTerrain::CEditorTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext) 
	: CGameObject(pDevice, pDeviceContext)
{
}

CEditorTerrain::CEditorTerrain(const CEditorTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEditorTerrain::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CEditorTerrain::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());


	m_vPickedPos = NOT_EXIST_VECTOR;
	return S_OK;
}

_int CEditorTerrain::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	//m_InverseWorldMat = m_pTransformCom->Get_InverseWorldMatrix();



	return _int();
}

_int CEditorTerrain::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;



	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this));
	//FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this));

	return _int();
}

_int CEditorTerrain::Render()
{
	if (__super::Render() < 0)
		return -1;
	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	CGameInstance* pInstance = GetSingle(CGameInstance);


	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(TRANSFORMSTATETYPE::PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(TRANSFORMSTATETYPE::PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fRadius", &(m_fRadius), sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vBrushPos", &(_float4(m_vPickedPos, 1)), sizeof(_float4)));



	if (m_iPassIndex == 3)
	{

		FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"Diffuse"));
		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_SourDiffuseTexture", 0));
		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DestDiffuseTexture1", 1));
		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DestDiffuseTexture2", 2));
		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DestDiffuseTexture3", 3));
		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DestDiffuseTexture4", 4));

		FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"Brush"));
		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_BrushTexture", 0));

		
		_float fMinMap = m_pVIBufferCom->Get_MinMapSize();
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMimMapSize", &fMinMap, sizeof(_float)));



	}
	FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, m_iPassIndex));

	return _int();
}

_int CEditorTerrain::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

_float3 CEditorTerrain::Pick_OnTerrain(_bool * pbIsObTerrain, _fVector ObjectWorldPos, _fVector ObjectOldWorldPos, _float3 * vOutPlaneNormalVec)
{
	if (XMVectorGetY(ObjectOldWorldPos) < XMVectorGetY(ObjectWorldPos))
		return ObjectWorldPos;

	_Matrix InverMat = m_InverseWorldMat.XMatrix();

	_Vector CaculatedFloat3 = m_pVIBufferCom->Caculate_Terrain_Pick_byRay(pbIsObTerrain,
		(XMVector3TransformCoord(ObjectWorldPos, InverMat)), (XMVector3TransformCoord(ObjectOldWorldPos, InverMat)), vOutPlaneNormalVec);


	if (*pbIsObTerrain)
	{
		if (vOutPlaneNormalVec != nullptr)
			*vOutPlaneNormalVec = XMVector3TransformNormal(vOutPlaneNormalVec->XMVector(), m_pTransformCom->Get_WorldMatrix());

		return XMVector3TransformCoord(CaculatedFloat3, m_pTransformCom->Get_WorldMatrix());
	}

	return ObjectWorldPos;
}



HRESULT CEditorTerrain::SetUp_Components()
{


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_VIBuffer_Terrain), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Texture_Edit_Terrain), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	m_pTextureCom->Change_TextureLayer(L"Diffuse");
	
	CTransform::TRANSFORMDESC tDesc = {};

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));
	m_InverseWorldMat = m_pTransformCom->Get_InverseWorldMatrix();
	

	return S_OK;
}


CEditorTerrain * CEditorTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CEditorTerrain*	pInstance = new CEditorTerrain(pDevice,pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CEditorTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEditorTerrain::Clone(void * pArg)
{
	CEditorTerrain*	pInstance = new CEditorTerrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CEditorTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEditorTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);

}
