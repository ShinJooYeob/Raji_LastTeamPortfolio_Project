#include "..\public\Light.h"
#include "EasingMgr.h"



CLight::CLight(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CLight::Initilize_Protoype(const LIGHTDESC & LightDesc)
{
	memcpy(&m_LightDesc, &LightDesc, sizeof(LIGHTDESC));

	
	return S_OK;
}

HRESULT CLight::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer, _float3* vDirectionalAtPoint, _double fDeltaTime)
{
	_uint		iPassIndex = 0;

	if (LIGHTDESC::TYPE_DIRECTIONAL == m_LightDesc.eLightType)
	{

		_float4 ShaderLightDir = XMVector3Normalize(XMVectorSetW(XMVectorSet(vDirectionalAtPoint->x, vDirectionalAtPoint->y, vDirectionalAtPoint->z, 1) 
			- XMVectorSetW(m_LightDesc.vVector.XMVector(),1) ,0));
		pShader->Set_RawValue("g_vLightDir", &ShaderLightDir, sizeof(_float4));
		iPassIndex = 1;
	}
	else
	{
		m_PassedTime += (_float)fDeltaTime;
		m_TargetTime += (_float)fDeltaTime;
		
		_float Range = GetSingle(CEasingMgr)->Easing_Return(TYPE_QuadInOut, TYPE_QuadInOut,  m_LightDesc.fRange * 1.1f, m_LightDesc.fRange * 0.9f, (_float)m_PassedTime, 1.f);
		Range *= (m_LightDesc.fTargetDeadTime - m_TargetTime) / m_LightDesc.fTargetDeadTime;
		if (m_PassedTime > 1.f) m_PassedTime = 0;
		if (m_TargetTime > m_LightDesc.fTargetDeadTime)m_LightDesc.bIsLightDead = true;


		pShader->Set_RawValue("g_fTimer", &m_PassedTime, sizeof(_float));

		pShader->Set_RawValue("g_vLightPos", &m_LightDesc.vVector, sizeof(_float4));
		pShader->Set_RawValue("g_fLightRange", &Range, sizeof(_float));
		iPassIndex = 2;
	}

	pShader->Set_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4));
	pShader->Set_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4));
	pShader->Set_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4));

	return pVIBuffer->Render(pShader, iPassIndex);
}

CLight * CLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc)
{
	CLight*	pInstance = new CLight(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initilize_Protoype(LightDesc)))
	{
		MSGBOX("Failed to Created CLight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
