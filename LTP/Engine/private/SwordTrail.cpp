#include "..\public\SwordTrail.h"
#include "Shader.h"
#include "VIBuffer_Point_Instance.h"
#include "Texture.h"
#include "PipeLineMgr.h"
#include "GameInstance.h"
#include "RenderTargetMgr.h"




CSwordTrail::CSwordTrail(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CSwordTrail::CSwordTrail(const CSwordTrail & rhs)
	: CComponent(rhs),
	m_pVIBuffer(rhs.m_pVIBuffer),
	m_pShader(rhs.m_pShader),
	m_pTexture(rhs.m_pTexture),
	m_iNumTrailPoints(rhs.m_iNumTrailPoints)
{
	Safe_AddRef(m_pVIBuffer);
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pTexture);
}


HRESULT CSwordTrail::Initialize_Prototype(const _tchar*	szTextureFilePath , _uint iNumTrailBufferCount)
{
	m_iNumTrailPoints = iNumTrailBufferCount;
	m_pVIBuffer = CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, m_iNumTrailPoints - 1);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_VtxTrailInstance.hlsl"), VTXPOINT_INSTMAT_DECLARATION::Elements, VTXPOINT_INSTMAT_DECLARATION::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);

	FAILED_CHECK(GetSingle(CGameInstance)->Add_Component_Prototype(0, L"Prototype_TrailTexture", CTexture::Create(m_pDevice, m_pDeviceContext, szTextureFilePath)));

	m_pTexture = (CTexture*)GetSingle(CGameInstance)->Clone_Component(0, L"Prototype_TrailTexture");
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);


	return S_OK;
}

HRESULT CSwordTrail::Initialize_Clone(void * pArg)
{
	m_TrailPointList.clear();

	if (pArg != nullptr)
		memcpy(&m_tDesc, pArg, sizeof(TRAILDESC));
	

	return S_OK;
}

void CSwordTrail::Set_TrailTurnOn(_bool bBool, _float3 tSourPoint, _float3 tDestPoint)
{
	if (m_bDrawTrail == bBool) return;
	
	m_bDrawTrail = bBool;

	if (m_bDrawTrail)
	{
		m_TrailPointList.clear();

		TRAILPOINT tPoints;
		tPoints.vSourPoint = tSourPoint;
		tPoints.vDestPoint = tDestPoint;


		for (_uint i = 0 ; i < m_iNumTrailPoints; i++)
			m_TrailPointList.push_back(tPoints);
	}
	else
	{
		m_TrailPointList.clear();
	}

}

_uint CSwordTrail::Update_SwordTrail(_float3 tSourPoint, _float3 tDestPoint,_double fDeltaTime, _float Rate)
{
	if (!m_bDrawTrail)return 0;

	m_PassedTime += (_float)fDeltaTime * m_tDesc.NoiseSpeed;
	if (m_PassedTime > 1) m_PassedTime = 0;

	TRAILPOINT tPoints;
	tPoints.vSourPoint = tSourPoint;
	tPoints.vDestPoint = tDestPoint;


	m_TrailPointList.pop_back();
	m_TrailPointList.push_front(tPoints);

	auto Points0 = m_TrailPointList.begin();
	auto Points1 = m_TrailPointList.begin();
	Points1++;
	auto Points2 = m_TrailPointList.begin();
	Points1++;
	Points2++;
	auto Points3 = m_TrailPointList.begin();
	Points1++;
	Points2++;
	Points3++;

#define TargetRate 0.5f
	for (_uint i = 1; i < m_TrailPointList.size() - 2; i++)
	{
		Points1->vDestPoint = XMVectorCatmullRom(Points0->vDestPoint.XMVector(), Points1->vDestPoint.XMVector(), Points2->vDestPoint.XMVector(), Points3->vDestPoint.XMVector(), Rate);
		Points1->vSourPoint = XMVectorCatmullRom(Points0->vSourPoint.XMVector(), Points1->vSourPoint.XMVector(), Points2->vSourPoint.XMVector(), Points3->vSourPoint.XMVector(), Rate);

		Points0++;
		Points1++;
		Points2++;
		Points3++;
	}

	 


	return _uint();
}

HRESULT CSwordTrail::Render()
{
	if (!m_bDrawTrail)return S_FALSE;

	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	//FAILED_CHECK(Update_ParticleAttribute(g_fDeltaTime));

	auto iter = m_TrailPointList.begin();
	auto iterNext = m_TrailPointList.begin();

	_float UVWidth = 1 / _float(m_iNumTrailPoints - 1);

	D3D11_MAPPED_SUBRESOURCE SubResource;

	FAILED_CHECK(m_pVIBuffer->Lock(&SubResource));


	for (_uint i = 0; i < m_iNumTrailPoints - 1; ++i)
	{
		iterNext++;
		
		XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vRight), iter->vSourPoint.XMVector());
		XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vUp), iter->vDestPoint.XMVector());
		XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vLook), (iterNext)->vSourPoint.XMVector());
		XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vTranslation), (iterNext)->vDestPoint.XMVector()); 

		((VTXINSTMAT*)SubResource.pData)[i].vUV_WHSize = _float4(i * UVWidth, 0 , (i + 1) * UVWidth, 1);
		((VTXINSTMAT*)SubResource.pData)[i].vColor = m_tDesc.vColor;

		iter++;
	}

	m_pVIBuffer->UnLock();


	CPipeLineMgr* pInstance = GetSingle(CPipeLineMgr);
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTexture->Bind_OnShader(m_pShader, "g_DiffuseTexture", m_tDesc.iTextureIndex));



	if (m_tDesc.iPassIndex == 2)
	{
		FAILED_CHECK(m_pTexture->Bind_OnShader(m_pShader, "g_NoiseTexture", 0));
		FAILED_CHECK(m_pShader->Set_Texture("g_BackBufferTexture", GetSingle(CRenderTargetMgr)->Get_SRV(L"Target_AfterDefferred")));
		FAILED_CHECK(m_pShader->Set_RawValue("g_fTime", &m_PassedTime, sizeof(_float)));

		FAILED_CHECK(m_pVIBuffer->Render(m_pShader, m_tDesc.iPassIndex));

	}
	else
	{
		FAILED_CHECK(m_pVIBuffer->Render(m_pShader, m_tDesc.iPassIndex));
	}



	return S_OK;
}

CSwordTrail * CSwordTrail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar*	szTextureFilePath , _uint iNumTrailBufferCount)
{
	CSwordTrail*	pInstance = new CSwordTrail(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(szTextureFilePath,iNumTrailBufferCount)))
	{
		MSGBOX("Failed to Creating CSwordTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CSwordTrail::Clone(void * pArg)
{
	CSwordTrail*	pInstance = new CSwordTrail(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CSwordTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSwordTrail::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);


	m_TrailPointList.clear();

}