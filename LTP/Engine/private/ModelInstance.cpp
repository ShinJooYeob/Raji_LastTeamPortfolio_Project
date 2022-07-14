#include "ModelInstance.h"
#include "VIBuffer_Model_Instance.h"
#include "ComponentMgr.h"




CModelInstance::CModelInstance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CModelInstance::CModelInstance(const CModelInstance & rhs)
	: CComponent(rhs),
	m_szInstancePrototypeTag(rhs.m_szInstancePrototypeTag),
	m_ePrototypeLayerIndex(m_ePrototypeLayerIndex)
{


}

HRESULT CModelInstance::Initialize_Prototype(_uint LayerSceneNum, _uint iNumInstance)
{
	m_ePrototypeLayerIndex = LayerSceneNum;
	m_szInstancePrototypeTag = L"Prototype_VIBuffer_Model_Instance_" + to_wstring(iNumInstance);

	FAILED_CHECK(GetSingle(CComponentMgr)->Add_Component_Prototype(m_ePrototypeLayerIndex, m_szInstancePrototypeTag.c_str(),
		CVIBuffer_Model_Instance::Create(m_pDevice, m_pDeviceContext, iNumInstance)));


	return S_OK;
}

HRESULT CModelInstance::Initialize_Clone(void * pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);
	memcpy(&m_tDesc, pArg, sizeof(MODELINSTDESC));
	
	m_pInstanceBuffer = (CVIBuffer_Model_Instance*)GetSingle(CComponentMgr)->Clone_Component(m_ePrototypeLayerIndex, m_szInstancePrototypeTag.c_str(), m_tDesc.m_pTargetModel);

	NULL_CHECK_RETURN(m_pInstanceBuffer, E_FAIL);

	return S_OK;
}


HRESULT CModelInstance::Render(class CShader* pShader, _uint iPassIndex, vector<CTransform*>* pvecWorldMatrixs, _float fFrustumsize, vector<_float4>*  pvecLimLight , vector<_float4>*  pvecEmissive, vector<_float4>*  pvecTimmer)
{

	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, ));
	_float4x4		mat = XMMatrixIdentity();
	FAILED_CHECK(pShader->Set_RawValue("g_WorldMatrix", &mat, sizeof(_float4x4)));


	_uint NumMaterial = m_tDesc.m_pTargetModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_tDesc.m_pTargetModel->Bind_OnShader(pShader, i, j, MODLETEXTYPEFORENGINE(j)));

		FAILED_CHECK(m_tDesc.m_pTargetModel->Render_ForInstancing(pShader, iPassIndex, i,  m_pInstanceBuffer,
			pvecWorldMatrixs, "g_BoneMatrices", fFrustumsize , pvecLimLight, pvecEmissive, pvecTimmer));
	}

	return 0;
}

HRESULT CModelInstance::Render_By_float4x4(CShader * pShader, _uint iPassIndex, vector<_float4x4>* pvecWorldMatrixs, _float fFrustumsize, vector<_float4>*  pvecLimLight , vector<_float4>*  pvecEmissive , vector<_float4>*  pvecTiemer)
{

	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, ));
	_float4x4		mat = XMMatrixIdentity();
	FAILED_CHECK(pShader->Set_RawValue("g_WorldMatrix", &mat, sizeof(_float4x4)));


	_uint NumMaterial = m_tDesc.m_pTargetModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_tDesc.m_pTargetModel->Bind_OnShader(pShader, i, j, MODLETEXTYPEFORENGINE(j)));


		FAILED_CHECK(m_tDesc.m_pTargetModel->Render_ForInstancing_float4x4(pShader, iPassIndex, i, m_pInstanceBuffer,
			pvecWorldMatrixs, "g_BoneMatrices", fFrustumsize, pvecLimLight, pvecEmissive,pvecTiemer));
	}





	return S_OK;
}


CModelInstance * CModelInstance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint LayerSceneNum, _uint iNumInstance)
{
	CModelInstance*	pInstance = new CModelInstance(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(LayerSceneNum,iNumInstance)))
	{
		MSGBOX("Failed to Creating CModelInstance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModelInstance::Clone(void * pArg)
{
	CModelInstance*	pInstance = new CModelInstance(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CModelInstance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModelInstance::Free()
{
	__super::Free();

	Safe_Release(m_pInstanceBuffer);


}
