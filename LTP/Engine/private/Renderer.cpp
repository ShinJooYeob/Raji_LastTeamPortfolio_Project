#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Camera.h"

#include "RenderTargetMgr.h"
#include "LightMgr.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "Model.h"
#include "VIBuffer.h"

#include "Input_Device.h"
#include "PipeLineMgr.h"
#include "Graphic_Device.h"
#include "LightMgr.h"


CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pRenderTargetMgr(CRenderTargetMgr::GetInstance())
	, m_pLightMgr(CLightMgr::GetInstance())
	, m_pGraphicDevice(CGraphic_Device::GetInstance())
{
	Safe_AddRef(m_pRenderTargetMgr);
	Safe_AddRef(m_pLightMgr);
	Safe_AddRef(m_pGraphicDevice);

}
#define ShadowMapQuality 10

HRESULT CRenderer::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));



	_uint		iNumViewports = 1;

	D3D11_VIEWPORT		Viewport;

	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	/* For.Target_Diffuse */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget( TEXT("Target_MtrlDiffuse"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	/* For.Target_Normal */
	/* 노멀벡터의 경우, 정규화하기가 쉽다. */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_MtrlNormal"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));
	/* For.Target_Emissive*/
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_MtrlEmissive"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	/* For.Target_SpecularMap */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_MtrlSpecular"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 1.f, 0.f)));
	/* For.Target_Depth */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_Depth"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 0.f, 0.f, 1.f)));
	/* For.Target_WorldPosition */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_WorldPosition"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 0.f, 0.f, 1.f)));


	/* For.Target_Shade */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_LightShade"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));
	/* For.Target_Specular */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_LightSpecular"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));


	/* For.Target_Shadow*/
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_Shadow"), (_uint)Viewport.Width * ShadowMapQuality, (_uint)Viewport.Width * ShadowMapQuality, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_DownScaledShadow"), (_uint)(Viewport.Width * 0.5f), (_uint)(Viewport.Height * 0.5f), DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_ReferenceDownScaledShadow"), (_uint)(Viewport.Width * 0.5f), (_uint)(Viewport.Height * 0.5f), DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_UpScaledBluredShadow"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));


	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_Defferred"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_ReferenceDefferred"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_BluredDefferred"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));


	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_AvgLuminece"), (_uint)1, (_uint)1, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_LumineceMask"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));

	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_VolumatricFog"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));

	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_GodRay"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f)));


	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_DownScaled_By2"), (_uint)(Viewport.Width / 2), (_uint)(Viewport.Height / 2), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_DownScaled_By3"), (_uint)(Viewport.Width / 3), (_uint)(Viewport.Height / 3), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_DownScaled_By4"), (_uint)(Viewport.Width / 4), (_uint)(Viewport.Height / 4), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_DownScaled_By5"), (_uint)(Viewport.Width / 5), (_uint)(Viewport.Height / 5), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_DownScaled_By6"), (_uint)(Viewport.Width / 6), (_uint)(Viewport.Height / 6), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_UpScaled_By2"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_UpScaled_By3"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_UpScaled_By4"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_UpScaled_By5"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_UpScaled_By6"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));



	/* For.MRT_Deferred : 객체들을 그릴때 바인드. */
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_MtrlDiffuse")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_MtrlNormal")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_MtrlSpecular")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_MtrlEmissive")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_Depth")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_WorldPosition")));
	

	/* For.MRT_LightAcc : 빛을 그릴때 바인드 */
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_LightShade")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_LightSpecular")));
	

	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DefferredNReference"), TEXT("Target_Defferred")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DefferredNReference"), TEXT("Target_ReferenceDefferred")));
	
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Defferred"), TEXT("Target_Defferred")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_ReferenceDefferred"), TEXT("Target_ReferenceDefferred")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_BluredDefferred"), TEXT("Target_BluredDefferred")));

	
	/* For.MRT_LightAcc : 그림자 그릴때 바인드 */
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_Shadow")));

	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_ShadowDownScaling"), TEXT("Target_ReferenceDownScaledShadow")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_ShadowVBlur"), TEXT("Target_DownScaledShadow")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_ShadowHBlur_N_UpScaling"), TEXT("Target_UpScaledBluredShadow")));


	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_AvgLuminece"), TEXT("Target_AvgLuminece")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_MakeLumiMask"), TEXT("Target_LumineceMask")));


	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_VolumatricFog"), TEXT("Target_VolumatricFog")));

	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_GodRay"), TEXT("Target_GodRay")));

	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DownScaled_By2"), TEXT("Target_DownScaled_By2")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DownScaled_By3"), TEXT("Target_DownScaled_By3")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DownScaled_By4"), TEXT("Target_DownScaled_By4")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DownScaled_By5"), TEXT("Target_DownScaled_By5")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DownScaled_By6"), TEXT("Target_DownScaled_By6")));

	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_UpScaled_By2"), TEXT("Target_UpScaled_By2")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_UpScaled_By3"), TEXT("Target_UpScaled_By3")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_UpScaled_By4"), TEXT("Target_UpScaled_By4")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_UpScaled_By5"), TEXT("Target_UpScaled_By5")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_UpScaled_By6"), TEXT("Target_UpScaled_By6")));



	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);


#ifdef _DEBUG

	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlDiffuse"), 50, 50, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlNormal"), 50, 150, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlSpecular"), 50, 250, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlEmissive"), 50, 350, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_WorldPosition"), 50, 450, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_Depth"), 50, 550, 100, 100));
	
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_LightShade"), 150, 50, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_LightSpecular"), 150, 150, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_Shadow"), 150, 250, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_UpScaledBluredShadow"), 150, 350, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_AvgLuminece"), 150, 450, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_LumineceMask"), 150, 550, 100, 100));

	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_GodRay"), 640, 360, 720, 720));



	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_MtrlDiffuse")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_MtrlNormal")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_MtrlSpecular")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_MtrlEmissive")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_WorldPosition")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_Depth")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_LightShade")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_LightSpecular")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_Shadow")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_UpScaledBluredShadow")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_AvgLuminece")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_LumineceMask")));


	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DebugRender"), TEXT("Target_GodRay")));


	
#endif


	XMStoreFloat4x4(&m_WVPmat.WorldMatrix, XMMatrixIdentity());

	m_WVPmat.WorldMatrix._11 = Viewport.Width;
	m_WVPmat.WorldMatrix._22 = Viewport.Height;
	m_WVPmat.WorldMatrix._33 = 1.f;
	memcpy(&m_WVPmat.WorldMatrix.m[3][0], &_float4(Viewport.Width * 0.5f - (Viewport.Width * 0.5f), -Viewport.Height * 0.5f + (Viewport.Height * 0.5f), 0.f, 1.f), sizeof(_float4));

	XMStoreFloat4x4(&m_WVPmat.WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WVPmat.WorldMatrix)));
	XMStoreFloat4x4(&m_WVPmat.ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WVPmat.ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));


	m_LightWVPmat.ViewMatrix = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSet(0, 10, -10, 1), XMVectorSet(10, -10, 10, 1), XMVectorSet(0, 1, 0, 0)));
	m_LightWVPmat.ProjMatrix = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(60.f), 1, 0.2f, 3000));



	ID3D11Texture2D*		pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = (_uint)Viewport.Width * ShadowMapQuality;
	TextureDesc.Height = (_uint)Viewport.Width * ShadowMapQuality;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	FAILED_CHECK(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture));
	FAILED_CHECK(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_LightDepthStencil));
	Safe_Release(pDepthStencilTexture);



	TextureDesc.Width = 1;
	TextureDesc.Height = 1;
	FAILED_CHECK(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture));
	FAILED_CHECK(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_AvgLumiDepthStencil));
	Safe_Release(pDepthStencilTexture);

	

	for (_uint i = 0; i < 5; i++)
	{
		TextureDesc.Width = (_uint)(Viewport.Width / (i + 2.f));
		TextureDesc.Height = (_uint)(Viewport.Height / (i + 2.f));
		FAILED_CHECK(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture));
		FAILED_CHECK(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_DownScaledDepthStencil[i]));
		Safe_Release(pDepthStencilTexture);
	}


	return S_OK;
}

HRESULT CRenderer::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));



	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderID, CGameObject * pGameObject)
{
	if (eRenderID >= RENDER_END ||
		pGameObject == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	if (eRenderID == CRenderer::RENDER_BLEND)
		pGameObject->Compute_RenderSortValue();

	m_RenderObjectList[eRenderID].emplace_back(pGameObject);
	
	Safe_AddRef(pGameObject);


	return S_OK;
}

HRESULT CRenderer::Add_ShadowGroup(SHADOWGROUP eShadowID, CGameObject * pGameObject, CTransform * pTransform, CShader * pShader, CModel* pModel, _float4x4 * AttacehdMatrix)
{
	if (pGameObject == nullptr || pTransform == nullptr || pShader == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	if (eShadowID >= SHADOW_ANIMMODEL && eShadowID <= SHADOW_NONANIMMODEL_ATTACHED && pModel == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	if ((eShadowID == SHADOW_ANIMMODEL_ATTACHED || eShadowID == SHADOW_NONANIMMODEL_ATTACHED) && AttacehdMatrix == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	SHADOWDESC tDesc;

	tDesc.pGameObject = pGameObject;
	tDesc.pTransform = pTransform;
	tDesc.pModel = pModel;
	tDesc.pShader = pShader;

	if (AttacehdMatrix)
		tDesc.AttacehdMatrix = AttacehdMatrix->TransposeXMatrix();


	Safe_AddRef(tDesc.pGameObject);
	m_ShadowObjectList[eShadowID].push_back(tDesc);

	return S_OK;
}


HRESULT CRenderer::Add_DebugGroup(CComponent * pComponent)
{
	m_DebugObjectList.push_back(pComponent);
	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CRenderer::Render_RenderGroup(_double fDeltaTime)
{
	FAILED_CHECK(m_pRenderTargetMgr->Clear_All_RenderTargetColor());


	FAILED_CHECK(Render_ShadowMap());
	 
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Material")));
	FAILED_CHECK(Render_Priority());
	FAILED_CHECK(Render_NonBlend());
	FAILED_CHECK(Render_Blend());
	FAILED_CHECK(Render_AfterObj());
	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Material")));
	
	FAILED_CHECK(Render_BlurShadow());
	FAILED_CHECK(Render_Lights(fDeltaTime));

	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_DefferredNReference")));
	FAILED_CHECK(Render_DeferredTexture());
	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_DefferredNReference")));

	FAILED_CHECK(Render_NonBlend_NoLight());

	//FAILED_CHECK(Render_GodRay());
	//FAILED_CHECK(Render_Volumatric());
	FAILED_CHECK(Render_Bloom());
	FAILED_CHECK(Render_DepthOfField());

	FAILED_CHECK(Copy_DeferredToBackBuffer());

	FAILED_CHECK(Render_UI());


#ifdef _DEBUG
	static _uint DrawDebug = 0;
	if (GetSingle(CInput_Device)->Get_DIKeyState(DIK_F1) &DIS_Down)
	{
		DrawDebug++;
		if (DrawDebug > 3) DrawDebug = 0;
	}

	if (DrawDebug)
	{
		if (DrawDebug == 1 || DrawDebug == 3)
			FAILED_CHECK(Render_Debug());

		if (DrawDebug >= 2)
		{
			FAILED_CHECK(m_pRenderTargetMgr->Render_DebugBuffer(TEXT("MRT_DebugRender")));
		}
		ID3D11ShaderResourceView* pSRV[8] = { nullptr };
		m_pDeviceContext->PSSetShaderResources(0, 8, pSRV);
	}

#endif

	return S_OK;
}


HRESULT CRenderer::Clear_RenderGroup_forSceneChaging()
{
	NULL_CHECK_RETURN(m_pLightMgr, E_FAIL);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& RenderObject : m_RenderObjectList[i])
		{
			Safe_Release(RenderObject);
		}
		m_RenderObjectList[i].clear();
	}

	for (_uint i =0; i < SHADOW_END; i++)
	{
		for (auto& ShadowDesc : m_ShadowObjectList[i])
		{
			Safe_Release(ShadowDesc.pGameObject);
		}
		m_ShadowObjectList[i].clear();
	}



	for (auto& DebugObject : m_DebugObjectList)
	{
		Safe_Release(DebugObject);
	}
	m_DebugObjectList.clear();


	m_pLightMgr->Clear_PointLightList();

	return S_OK;
}





HRESULT CRenderer::Render_Priority()
{
	for (auto& RenderObject : m_RenderObjectList[RENDER_PRIORITY])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_PRIORITY].clear();
	return S_OK;
}


HRESULT CRenderer::Render_NonBlend()
{
	for (auto& RenderObject : m_RenderObjectList[RENDER_NONBLEND])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_NONBLEND].clear();


	for (auto& RenderObject : m_RenderObjectList[RENDER_SUBNONBLEND])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_SUBNONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Lights(_double fDeltaTime)
{
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_LightAcc")));

	FAILED_CHECK(m_pLightMgr->Render(m_pShader, m_pVIBuffer, &m_WVPmat, fDeltaTime));

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_LightAcc")));

	return S_OK;
}

HRESULT CRenderer::Render_DeferredTexture()
{
	static _float Rimright = 1.0f;

	if (GetSingle(CInput_Device)->Get_DIKeyState(DIK_N) & DIS_Press)
	{
		Rimright += 0.001f;
		if (Rimright > 1)Rimright = 1;
		OutputDebugStringW((to_wstring(Rimright) + L"\n").c_str());
	}
	if (GetSingle(CInput_Device)->Get_DIKeyState(DIK_M) & DIS_Press)
	{
		Rimright -= 0.001f;
		if (Rimright < 0)Rimright = 0;
		OutputDebugStringW((to_wstring(Rimright) + L"\n").c_str());
	}



	FAILED_CHECK(m_pShader->Set_RawValue("g_fRimWidth", &Rimright, sizeof(_float)));
	
	
	
	CPipeLineMgr*		pPipeLineMgr = GetSingle(CPipeLineMgr);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_PROJ))));

	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4)));


	FAILED_CHECK(m_pShader->Set_RawValue("g_vCamPosition", &pPipeLineMgr->Get_TargetPostion_float4(PLV_CAMERA), sizeof(_float4)));

	


	FAILED_CHECK(m_pShader->Set_Texture("g_DiffuseTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_MtrlDiffuse"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_ShadeTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_LightShade"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_SpecularTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_LightSpecular"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_WorldPosTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_WorldPosition"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_ShadowMapTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_UpScaledBluredShadow"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_NormalTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_MtrlNormal"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_EmissiveTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_MtrlEmissive"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_MaskTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_AvgLuminece"))));
	
	
	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 3));


	return S_OK;
}

HRESULT CRenderer::Copy_DeferredToReference()
{
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_ReferenceDefferred")));
	

	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Defferred"))));
	

	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));


	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 0));

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_ReferenceDefferred")));
	return S_OK;
}

HRESULT CRenderer::Copy_DeferredToBackBuffer()
{
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Defferred"))));


	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));
	
	
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 14));

	return S_OK;
}

HRESULT CRenderer::Ready_DepthStencilBuffer(_uint iDSVIndex, D3D11_VIEWPORT* pOutOldViewPort)
{
	if (iDSVIndex >= 5 || pOutOldViewPort == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}


	m_pDeviceContext->ClearDepthStencilView(m_DownScaledDepthStencil[iDSVIndex], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);


	_uint		iNumViewports = 1;
	D3D11_VIEWPORT		OldViewPortDesc;
	m_pDeviceContext->RSGetViewports(&iNumViewports, &OldViewPortDesc);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)(OldViewPortDesc.Width / (iDSVIndex + 2.f));
	ViewPortDesc.Height = (_float)(OldViewPortDesc.Height / (iDSVIndex + 2.f));
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(iNumViewports, &ViewPortDesc);


	*pOutOldViewPort = OldViewPortDesc;

	return S_OK;
}

HRESULT CRenderer::Render_BlurShadow()
{



	D3D11_VIEWPORT		OldViewPortDesc;


	FAILED_CHECK(Ready_DepthStencilBuffer(0, &OldViewPortDesc));
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_ShadowDownScaling"), m_DownScaledDepthStencil[0]));


	CPipeLineMgr*		pPipeLineMgr = GetSingle(CPipeLineMgr);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_PROJ))));

	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4)));

	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pShader->Set_RawValue("g_LightViewMatrix", &m_LightWVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_LightProjMatrix", &m_LightWVPmat.ProjMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pShader->Set_Texture("g_WorldPosTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_WorldPosition"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Shadow"))));
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 8));

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_ShadowDownScaling")));







	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_ShadowVBlur"), m_DownScaledDepthStencil[0]));


	m_fTexleSize = 3.f;
	FAILED_CHECK(m_pShader->Set_RawValue("g_fTexelSize", &m_fTexleSize, sizeof(_float)));

	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_ReferenceDownScaledShadow"))));
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 6));

	m_pDeviceContext->RSSetViewports(1, &OldViewPortDesc);
	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_ShadowVBlur")));






	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_ShadowHBlur_N_UpScaling")));

	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_DownScaledShadow"))));
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 7));

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_ShadowHBlur_N_UpScaling")));




	return S_OK;
}

HRESULT CRenderer::Caculate_AvgLuminence()
{


	m_pDeviceContext->ClearDepthStencilView(m_AvgLumiDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);


	_uint		iNumViewports = 1;
	D3D11_VIEWPORT		OldViewPortDesc;
	m_pDeviceContext->RSGetViewports(&iNumViewports, &OldViewPortDesc);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = 1;
	ViewPortDesc.Height =1;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(iNumViewports, &ViewPortDesc);
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_AvgLuminece"), m_AvgLumiDepthStencil));


	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));


	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Defferred"))));
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 10));

	m_pDeviceContext->RSSetViewports(1, &OldViewPortDesc);
	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_AvgLuminece")));






	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_MakeLumiMask")));

	FAILED_CHECK(m_pShader->Set_Texture("g_MaskTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_AvgLuminece"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Defferred"))));
	
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 11));

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_MakeLumiMask")));

	return S_OK;
}

HRESULT CRenderer::Render_Bloom()
{
	FAILED_CHECK(Make_BluredDeffered());
	FAILED_CHECK(Caculate_AvgLuminence());

	m_fTexleSize = 2.f;
	FAILED_CHECK(m_pShader->Set_RawValue("g_fTexelSize", &m_fTexleSize, sizeof(_float)));


	D3D11_VIEWPORT OldVp;
	FAILED_CHECK(Ready_DepthStencilBuffer(0, &OldVp));

	wstring TargetMrt = L"MRT_DownScaled_By" + to_wstring((_uint)(2));

	FAILED_CHECK(m_pRenderTargetMgr->Begin(TargetMrt.c_str(), m_DownScaledDepthStencil[0]));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_LumineceMask"))));

	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 6));


	m_pDeviceContext->RSSetViewports(1, &OldVp);
	FAILED_CHECK(m_pRenderTargetMgr->End(TargetMrt.c_str()));


	wstring TargetUpMrt = L"MRT_UpScaled_By" + to_wstring((_uint)(2));
	wstring TargetTex = L"Target_DownScaled_By" + to_wstring((_uint)(2));
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TargetUpMrt.c_str()));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TargetTex.c_str())));
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 7));
	FAILED_CHECK(m_pRenderTargetMgr->End(TargetUpMrt.c_str()));




	FAILED_CHECK(m_pRenderTargetMgr->Begin(L"MRT_Defferred"));

	
	FAILED_CHECK(m_pShader->Set_Texture("g_MaskTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_UpScaled_By2"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_BluredTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_BluredDefferred"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_ReferenceDefferred"))));
	
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 13));


	FAILED_CHECK(m_pRenderTargetMgr->End(L"MRT_Defferred"));
	FAILED_CHECK(Copy_DeferredToReference());


	return S_OK;
}

HRESULT CRenderer::Make_BluredDeffered()
{
	//Target_BluredDefferred
	FAILED_CHECK(m_pRenderTargetMgr->Clear_SpecificMRT(TEXT("MRT_BluredDefferred")));

	D3D11_VIEWPORT OldVp;
	FAILED_CHECK(Ready_DepthStencilBuffer(0, &OldVp));

	wstring TargetMrt = L"MRT_DownScaled_By" + to_wstring((_uint)(2));

	FAILED_CHECK(m_pRenderTargetMgr->Clear_SpecificMRT(TargetMrt.c_str()));

	FAILED_CHECK(m_pRenderTargetMgr->Begin(TargetMrt.c_str(), m_DownScaledDepthStencil[0]));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Defferred"))));

	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 6));


	m_pDeviceContext->RSSetViewports(1, &OldVp);
	FAILED_CHECK(m_pRenderTargetMgr->End(TargetMrt.c_str()));


	wstring TargetTex = L"Target_DownScaled_By" + to_wstring((_uint)(2));

	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_BluredDefferred")));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TargetTex.c_str())));
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 7));
	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_BluredDefferred")));


	return S_OK;
}

HRESULT CRenderer::Render_Volumatric()
{



	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_VolumatricFog")));
	
	CPipeLineMgr*		pPipeLineMgr = GetSingle(CPipeLineMgr);

	_float4x4		ViewMatrixInv, ProjMatrixInv;



	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_PROJ))));

	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4)));

	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pShader->Set_Texture("g_DepthTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Depth"))));
	
	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 15));

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_VolumatricFog")));




	return S_OK;
}

HRESULT CRenderer::Render_GodRay()
{


	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_GodRay")));

	CPipeLineMgr*		pPipeLineMgr = GetSingle(CPipeLineMgr);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	_float4 ProjSpacePos = XMVector3TransformCoord(_float3(0, 30, 10).Multiply_Matrix_AsPosVector(pPipeLineMgr->Get_Transform_Matrix(PLM_VIEW)), pPipeLineMgr->Get_Transform_Matrix(PLM_PROJ));
	_float2 ScreenLightPos = _float2(ProjSpacePos.x * 0.5f + 0.5f, ProjSpacePos.y * -0.5f + 0.5f);


	FAILED_CHECK(m_pShader->Set_RawValue("g_vLightShaftValue", &_float4(0.002f, 1.05f, 0.8f, 1.f), sizeof(_float4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_vScreenLightUVPos", &ScreenLightPos, sizeof(_float2)));

	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));


	FAILED_CHECK(m_pShader->Set_Texture("g_MaskTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Depth"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Depth"))));

	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 16));

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_GodRay")));





	return S_OK;
}

HRESULT CRenderer::Render_DepthOfField()
{
	m_fTexleSize = 2.f;
	FAILED_CHECK(m_pShader->Set_RawValue("g_fTexelSize", &m_fTexleSize, sizeof(_float)));

	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));
	
	/* Multi GausianBlur
	//for (_uint i = 0;  i < 5;  i++)
	//{
	//	D3D11_VIEWPORT OldVp;
	//	FAILED_CHECK(Ready_DepthStencilBuffer(i, &OldVp));

	//	wstring TargetMrt = L"MRT_DownScaled_By" + to_wstring((_uint)(2 + i));

	//	FAILED_CHECK(m_pRenderTargetMgr->Begin(TargetMrt.c_str(), m_DownScaledDepthStencil[i]));

	//	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Defferred"))));

	//	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 0));


	//	m_pDeviceContext->RSSetViewports(1, &OldVp);
	//	FAILED_CHECK(m_pRenderTargetMgr->End(TargetMrt.c_str()));


	//	wstring TargetUpMrt = L"MRT_UpScaled_By" + to_wstring((_uint)(2 + i));
	//	wstring TargetTex = L"Target_DownScaled_By" + to_wstring((_uint)(2 + i));
	//	FAILED_CHECK(m_pRenderTargetMgr->Begin(TargetUpMrt.c_str()));

	//	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TargetTex.c_str())));

	//	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 0));

	//	FAILED_CHECK(m_pRenderTargetMgr->End(TargetUpMrt.c_str()));
	//}

	//FAILED_CHECK(m_pRenderTargetMgr->Begin(L"MRT_Defferred"));

	//FAILED_CHECK(m_pShader->Set_Texture("g_UpScaledTexture1", m_pRenderTargetMgr->Get_SRV(TEXT("Target_UpScaled_By2"))));
	//FAILED_CHECK(m_pShader->Set_Texture("g_UpScaledTexture2", m_pRenderTargetMgr->Get_SRV(TEXT("Target_UpScaled_By3"))));
	//FAILED_CHECK(m_pShader->Set_Texture("g_UpScaledTexture3", m_pRenderTargetMgr->Get_SRV(TEXT("Target_UpScaled_By4"))));
	//FAILED_CHECK(m_pShader->Set_Texture("g_UpScaledTexture4", m_pRenderTargetMgr->Get_SRV(TEXT("Target_UpScaled_By5"))));
	//FAILED_CHECK(m_pShader->Set_Texture("g_UpScaledTexture5", m_pRenderTargetMgr->Get_SRV(TEXT("Target_UpScaled_By6"))));

	//FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 5));


	//FAILED_CHECK(m_pRenderTargetMgr->End(L"MRT_Defferred"));

	*/

	FAILED_CHECK(Make_BluredDeffered());


	FAILED_CHECK(m_pRenderTargetMgr->Begin(L"MRT_Defferred"));
	CPipeLineMgr*		pPipeLineMgr = GetSingle(CPipeLineMgr);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_PROJ))));

	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_vDofTargetPosition", &pPipeLineMgr->Get_TargetPostion_float4(PLV_PLAYER), sizeof(_float3)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_fDofTargetLength", &m_fDofLength, sizeof(_float)));

	
	
	FAILED_CHECK(m_pShader->Set_Texture("g_UpScaledTexture1", m_pRenderTargetMgr->Get_SRV(TEXT("Target_BluredDefferred"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_ReferenceDefferred"))));

	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 9));


	FAILED_CHECK(m_pRenderTargetMgr->End(L"MRT_Defferred"));
	FAILED_CHECK(Copy_DeferredToReference());

	return S_OK;
}

HRESULT CRenderer::Render_ShadowMap()
{
	m_pDeviceContext->ClearDepthStencilView(m_LightDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);


	_uint		iNumViewports = 1;
	D3D11_VIEWPORT		OldViewPortDesc;
	m_pDeviceContext->RSGetViewports(&iNumViewports, &OldViewPortDesc);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)OldViewPortDesc.Width * ShadowMapQuality;
	ViewPortDesc.Height = (_float)OldViewPortDesc.Width * ShadowMapQuality;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;







	m_pDeviceContext->RSSetViewports(iNumViewports, &ViewPortDesc);
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Shadow"), m_LightDepthStencil));

	LIGHTDESC* pLightDesc = GetSingle(CLightMgr)->Get_LightDesc(LIGHTDESC::TYPE_DIRECTIONAL, 0);


	if (pLightDesc)
		m_LightWVPmat.ViewMatrix = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSetW(pLightDesc->vVector.XMVector(), 1), XMVectorSet(10, -10, 10, 1), XMVectorSet(0, 1, 0, 0)));


	FAILED_CHECK(Render_ShadowGroup());


	m_pDeviceContext->RSSetViewports(iNumViewports, &OldViewPortDesc);
	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Shadow")));


	return S_OK;
}

HRESULT CRenderer::Render_ShadowGroup()
{

	//SHADOW_ANIMMODEL, SHADOW_ANIMMODEL_ATTACHED, SHADOW_NONANIMMODEL, SHADOW_NONANIMMODEL_ATTACHED, SHADOW_TERRAIN, SHADOW_END

#pragma region SHADOW_ANIMMODEL

	m_bShadowLightMatBindedChecker = false;
	for (auto& ShadowDesc : m_ShadowObjectList[SHADOW_ANIMMODEL])
	{
		if (!m_bShadowLightMatBindedChecker)
		{
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightViewMatrix", &m_LightWVPmat.ViewMatrix, sizeof(_float4x4)));
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightProjMatrix", &m_LightWVPmat.ProjMatrix, sizeof(_float4x4)));
			m_bShadowLightMatBindedChecker = true;
		}
		FAILED_CHECK(ShadowDesc.pTransform->Bind_OnShader(ShadowDesc.pShader, "g_WorldMatrix"));


		_uint NumMaterial = ShadowDesc.pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			FAILED_CHECK(ShadowDesc.pModel->Render(ShadowDesc.pShader, 0, i, "g_BoneMatrices"));
		}
		Safe_Release(ShadowDesc.pGameObject);
	}
	m_ShadowObjectList[SHADOW_ANIMMODEL].clear();

#pragma endregion


#pragma region SHADOW_ANIMMODEL_ATTACHED

	m_bShadowLightMatBindedChecker = false;
	for (auto& ShadowDesc : m_ShadowObjectList[SHADOW_ANIMMODEL_ATTACHED])
	{
		if (!m_bShadowLightMatBindedChecker)
		{
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightViewMatrix", &m_LightWVPmat.ViewMatrix, sizeof(_float4x4)));
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightProjMatrix", &m_LightWVPmat.ProjMatrix, sizeof(_float4x4)));
			m_bShadowLightMatBindedChecker = true;
		}
		FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_AttechMatrix", &ShadowDesc.AttacehdMatrix, sizeof(_float4x4)));
		FAILED_CHECK(ShadowDesc.pTransform->Bind_OnShader(ShadowDesc.pShader, "g_WorldMatrix"));

		_uint NumMaterial = ShadowDesc.pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			FAILED_CHECK(ShadowDesc.pModel->Render(ShadowDesc.pShader, 1, i, "g_BoneMatrices"));
		}
		Safe_Release(ShadowDesc.pGameObject);

	}
	m_ShadowObjectList[SHADOW_ANIMMODEL_ATTACHED].clear();


#pragma endregion


#pragma region SHADOW_NONANIMMODEL

	m_bShadowLightMatBindedChecker = false;
	for (auto& ShadowDesc : m_ShadowObjectList[SHADOW_NONANIMMODEL])
	{
		if (!m_bShadowLightMatBindedChecker)
		{
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightViewMatrix", &m_LightWVPmat.ViewMatrix, sizeof(_float4x4)));
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightProjMatrix", &m_LightWVPmat.ProjMatrix, sizeof(_float4x4)));
			m_bShadowLightMatBindedChecker = true;
		}
		FAILED_CHECK(ShadowDesc.pTransform->Bind_OnShader(ShadowDesc.pShader, "g_WorldMatrix"));

		_uint NumMaterial = ShadowDesc.pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			FAILED_CHECK(ShadowDesc.pModel->Render(ShadowDesc.pShader, 0, i));
		}

		Safe_Release(ShadowDesc.pGameObject);
	}
	m_ShadowObjectList[SHADOW_NONANIMMODEL].clear();


#pragma endregion

#pragma region SHADOW_NONANIMMODEL_ATTACHED

	m_bShadowLightMatBindedChecker = false;
	for (auto& ShadowDesc : m_ShadowObjectList[SHADOW_ANIMMODEL_ATTACHED])
	{

		if (!m_bShadowLightMatBindedChecker)
		{
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightViewMatrix", &m_LightWVPmat.ViewMatrix, sizeof(_float4x4)));
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightProjMatrix", &m_LightWVPmat.ProjMatrix, sizeof(_float4x4)));
			m_bShadowLightMatBindedChecker = true;
		}
		FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_AttechMatrix", &ShadowDesc.AttacehdMatrix, sizeof(_float4x4)));
		FAILED_CHECK(ShadowDesc.pTransform->Bind_OnShader(ShadowDesc.pShader, "g_WorldMatrix"));

		_uint NumMaterial = ShadowDesc.pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			FAILED_CHECK(ShadowDesc.pModel->Render(ShadowDesc.pShader, 1, i));
		}
		Safe_Release(ShadowDesc.pGameObject);

	}
	m_ShadowObjectList[SHADOW_ANIMMODEL_ATTACHED].clear();

#pragma endregion


#pragma region SHADOW_TERRAIN

	m_bShadowLightMatBindedChecker = false;
	for (auto& ShadowDesc : m_ShadowObjectList[SHADOW_TERRAIN])
	{
		if (!m_bShadowLightMatBindedChecker)
		{
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightViewMatrix", &m_LightWVPmat.ViewMatrix, sizeof(_float4x4)));
			FAILED_CHECK(ShadowDesc.pShader->Set_RawValue("g_LightProjMatrix", &m_LightWVPmat.ProjMatrix, sizeof(_float4x4)));
			m_bShadowLightMatBindedChecker = true;
		}

		CVIBuffer* pVIBuffer = (CVIBuffer*)ShadowDesc.pGameObject->Get_Component(L"Com_VIBuffer");
		NULL_CHECK_RETURN(pVIBuffer, E_FAIL);

		FAILED_CHECK(ShadowDesc.pTransform->Bind_OnShader(ShadowDesc.pShader, "g_WorldMatrix"));
		FAILED_CHECK(pVIBuffer->Render(ShadowDesc.pShader, 0));

		Safe_Release(ShadowDesc.pGameObject);
	}
	m_ShadowObjectList[SHADOW_TERRAIN].clear();

#pragma endregion 



	return S_OK;
}


HRESULT CRenderer::Render_NonBlend_NoLight()
{
	//FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Defferred")));

	for (auto& RenderObject : m_RenderObjectList[RENDER_NONBLEND_NOLIGHT])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}

	m_RenderObjectList[RENDER_NONBLEND_NOLIGHT].clear();

	//FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Defferred")));

	//FAILED_CHECK(Copy_DeferredToReference());

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{

	//FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Defferred")));

	m_RenderObjectList[RENDER_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return pSour->Get_RenderSortValue() > pDest->Get_RenderSortValue();
	});

	for (auto& RenderObject : m_RenderObjectList[RENDER_BLEND])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_BLEND].clear();

	//FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Defferred")));
	//FAILED_CHECK(Copy_DeferredToReference());


	//FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Defferred")));

	m_RenderObjectList[RENDER_SUBBLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return pSour->Get_RenderSortValue() > pDest->Get_RenderSortValue();
	});

	for (auto& RenderObject : m_RenderObjectList[RENDER_SUBBLEND])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_SUBBLEND].clear();

	//FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Defferred")));
	//FAILED_CHECK(Copy_DeferredToReference());

	return S_OK;
}

HRESULT CRenderer::Render_AfterObj()
{
	//FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Defferred")));

	for (auto& RenderObject : m_RenderObjectList[RENDER_AFTERALLOBJ])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_AFTERALLOBJ].clear();

	//FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Defferred")));
	//FAILED_CHECK(Copy_DeferredToReference());

	return S_OK;
}


HRESULT CRenderer::Render_UI()
{

	m_RenderObjectList[RENDER_UI].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return pSour->Get_RenderSortValue() > pDest->Get_RenderSortValue();
	});
	for (auto& RenderObject : m_RenderObjectList[RENDER_UI])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_UI].clear();


	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	FAILED_CHECK(m_pShader->Apply(4));

	for (auto& pComponent : m_DebugObjectList)
	{
		if (nullptr != pComponent)
			pComponent->Render();

		Safe_Release(pComponent);
	}

	m_DebugObjectList.clear();

	return S_OK;
}
#endif // _DEBUG

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRenderer* pInstance = new CRenderer(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) 
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	this->AddRef();
	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < 5; i++)
	{
		Safe_Release(m_DownScaledDepthStencil[i]);
	}
	Safe_Release(m_LightDepthStencil);
	Safe_Release(m_AvgLumiDepthStencil);
	
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
	

	Safe_Release(m_pRenderTargetMgr);
	Safe_Release(m_pLightMgr);
	Safe_Release(m_pGraphicDevice);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& RenderObject : m_RenderObjectList[i])
		{
			Safe_Release(RenderObject);
		}
		m_RenderObjectList[i].clear();
	}

	for (_uint i = 0; i < SHADOW_END; i++)
	{
		for (auto& ShadowDesc : m_ShadowObjectList[i])
		{
			Safe_Release(ShadowDesc.pGameObject);
		}
		m_ShadowObjectList[i].clear();
	}


	for (auto& DebugObject : m_DebugObjectList)
	{
		Safe_Release(DebugObject);
	}
	m_DebugObjectList.clear();

}
