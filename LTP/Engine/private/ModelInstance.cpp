#include "ModelInstance.h"
#include "VIBuffer_Model_Instance.h"
#include "ComponentMgr.h"



static const char* Tag_ModelTextureTypeForEngine(_uint eTag)
{
	switch (eTag)
	{
	case aiTextureType_NONE:		return "None";
		break;
	case aiTextureType_DIFFUSE:	return "g_DiffuseTexture";
		break;
	case aiTextureType_SPECULAR:return "g_SpecularTexture";
		break;
	case aiTextureType_AMBIENT:return "g_AmbientTexture";
		break;
	case aiTextureType_EMISSIVE:return "g_EmissiveTexture";
		break;
	case aiTextureType_HEIGHT:return "g_HeightTexture";
		break;
	case aiTextureType_NORMALS:return "g_NormalTexture";
		break;
	case aiTextureType_SHININESS:return "g_ShininessTexture";
		break;
	case aiTextureType_OPACITY:return "g_OpacityTexture";
		break;
	case aiTextureType_DISPLACEMENT:return "g_DisplaceTexture";
		break;
	case aiTextureType_LIGHTMAP:return "g_LightMapTexture";
		break;
	case aiTextureType_REFLECTION:return "g_ReflectTexture";
		break;
	case aiTextureType_BASE_COLOR:return "g_BaseColorTexture";
		break;
	case aiTextureType_NORMAL_CAMERA:return "g_NormalCamTexture";
		break;
	case aiTextureType_EMISSION_COLOR:return "g_EmissionColorTexture";
		break;
	case aiTextureType_METALNESS:return "g_MetalTexture";
		break;
	case aiTextureType_DIFFUSE_ROUGHNESS:return "g_DiffuseRoughTexture";
		break;
	case aiTextureType_AMBIENT_OCCLUSION:return "g_AmbientOcculusionTexture";
		break;
	case aiTextureType_UNKNOWN:return "";
		break;
	case _aiTextureType_Force32Bit:return "";
		break;
	default:
		OutputDebugStringW(L"Wrong Type Texture");
		__debugbreak();
		return nullptr;
		break;
	}
}
#define  MODLETEXTYPEFORENGINE Tag_ModelTextureTypeForEngine

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


HRESULT CModelInstance::Render(class CShader* pShader, _uint iPassIndex, vector<CTransform*>* pvecWorldMatrixs, _float fFrustumsize)
{

	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, ));
	_float4x4		mat = XMMatrixIdentity();
	FAILED_CHECK(pShader->Set_RawValue("g_WorldMatrix", &mat, sizeof(_float4x4)));


	_uint NumMaterial = m_tDesc.m_pTargetModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_tDesc.m_pTargetModel->Bind_OnShader(pShader, i, j, MODLETEXTYPEFORENGINE(j)));

		FAILED_CHECK(m_tDesc.m_pTargetModel->Render_ForInstancing(pShader, iPassIndex, i,  m_pInstanceBuffer, pvecWorldMatrixs, "g_BoneMatrices", fFrustumsize));
	}

	return 0;
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
