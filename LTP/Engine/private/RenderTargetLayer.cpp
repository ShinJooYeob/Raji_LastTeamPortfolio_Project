#include "..\Public\RenderTargetLayer.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"

CRenderTargetLayer::CRenderTargetLayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CRenderTargetLayer::Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor, _bool bAutoClearing)
{
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	m_vClearColor = vClearColor; 
	m_bAutoClearing = bAutoClearing;

	/* 1. */

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iSizeX;
	TextureDesc.Height = iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;	

	FAILED_CHECK(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D));

	/* 2. ·»´õÅ¸°Ùºä */
	FAILED_CHECK(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV));

	/* 3. ¼ÎÀÌ´õ¸®¼Ò½ººä*/
	FAILED_CHECK(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV));

	return S_OK;
}

HRESULT CRenderTargetLayer::Clear()
{
	NULL_CHECK_RETURN(m_pDeviceContext, E_FAIL);

	m_pDeviceContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderTargetLayer::Ready_DebugDesc(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	_uint		iNumViewports = 1;

	D3D11_VIEWPORT		Viewport;

	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;
	m_WorldMatrix._33 = 1.f;
	memcpy(&m_WorldMatrix.m[3][0], &_float4(fX - (Viewport.Width * 0.5f), -fY + (Viewport.Height * 0.5f), 0.f, 1.f), sizeof(_float4));	

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));

	return S_OK;
}

HRESULT CRenderTargetLayer::Render_DebugBuffer(CShader* pShader, CVIBuffer_Rect * pVIBuffer)
{
	pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));	
	pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	pShader->Set_Texture("g_TargetTexture", m_pSRV);

	pVIBuffer->Render(pShader, 0);

	return S_OK;
}

#endif // _DEBUG

CRenderTargetLayer * CRenderTargetLayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSizeX, _uint iSizeY,
	DXGI_FORMAT eFormat, _float4 vClearColor , _bool bAutoClearing)
{
	CRenderTargetLayer*	pInstance = new CRenderTargetLayer(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize(iSizeX, iSizeY, eFormat, vClearColor, bAutoClearing)))
	{
		MSGBOX("Failed to Created CRenderTarget");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRenderTargetLayer::Free()
{	
	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);
	Safe_Release(m_pTexture2D);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
