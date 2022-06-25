#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Camera.h"

#include "RenderTargetMgr.h"
#include "LightMgr.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"

#include "Input_Device.h"
#include "PipeLineMgr.h"
#include "Graphic_Device.h"


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


	/* For.Target_Shade */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_LightShade"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f)));
	/* For.Target_Specular */
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_LightSpecular"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));


	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_Defferred"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));
	FAILED_CHECK(m_pRenderTargetMgr->Add_RenderTarget(TEXT("Target_ReferenceDefferred"), (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f)));




	/* For.MRT_Deferred : 객체들을 그릴때 바인드. */
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_MtrlDiffuse")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_MtrlNormal")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_MtrlSpecular")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_MtrlEmissive")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Material"), TEXT("Target_Depth")));
	

	/* For.MRT_LightAcc : 빛을 그릴때 바인드 */
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_LightShade")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_LightSpecular")));
	

	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DefferredNReference"), TEXT("Target_Defferred")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_DefferredNReference"), TEXT("Target_ReferenceDefferred")));

	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_Defferred"), TEXT("Target_Defferred")));
	FAILED_CHECK(m_pRenderTargetMgr->Add_MRT(TEXT("MRT_ReferenceDefferred"), TEXT("Target_ReferenceDefferred")));


	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);


#ifdef _DEBUG

	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlDiffuse"), 50, 50, 100, 100));
	//FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlNormal"), 640, 360, 1280, 720));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlNormal"), 50, 150, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_Depth"), 50, 250, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlSpecular"), 50, 350, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_MtrlEmissive"), 50, 450, 100, 100));

	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_LightShade"), 150, 50, 100, 100));
	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_LightSpecular"), 150, 150, 100, 100));

	FAILED_CHECK(m_pRenderTargetMgr->Ready_DebugDesc(TEXT("Target_Defferred"), 150, 250, 100, 100));


	
#endif


	XMStoreFloat4x4(&m_WVPmat.WorldMatrix, XMMatrixIdentity());

	m_WVPmat.WorldMatrix._11 = Viewport.Width;
	m_WVPmat.WorldMatrix._22 = Viewport.Height;
	m_WVPmat.WorldMatrix._33 = 1.f;
	memcpy(&m_WVPmat.WorldMatrix.m[3][0], &_float4(Viewport.Width * 0.5f - (Viewport.Width * 0.5f), -Viewport.Height * 0.5f + (Viewport.Height * 0.5f), 0.f, 1.f), sizeof(_float4));

	XMStoreFloat4x4(&m_WVPmat.WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WVPmat.WorldMatrix)));
	XMStoreFloat4x4(&m_WVPmat.ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WVPmat.ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));




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


HRESULT CRenderer::Add_DebugGroup(CComponent * pComponent)
{
	m_DebugObjectList.push_back(pComponent);
	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CRenderer::Render_RenderGroup(_double fDeltaTime)
{
	FAILED_CHECK(m_pRenderTargetMgr->Clear_All_RenderTargetColor());


	//FAILED_CHECK(Render_ShadowMap());

	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Material")));
	FAILED_CHECK(Render_Priority());
	FAILED_CHECK(Render_NonBlend());
	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Material")));

	FAILED_CHECK(Render_Lights(fDeltaTime));

	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_DefferredNReference")));
	FAILED_CHECK(Render_DeferredTexture());
	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_DefferredNReference")));

	FAILED_CHECK(Render_NonBlend_NoLight());
	FAILED_CHECK(Render_Blend());
	FAILED_CHECK(Render_AfterObj());

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

			FAILED_CHECK(m_pRenderTargetMgr->Render_DebugBuffer(TEXT("MRT_Material")));
			FAILED_CHECK(m_pRenderTargetMgr->Render_DebugBuffer(TEXT("MRT_LightAcc")));
			FAILED_CHECK(m_pRenderTargetMgr->Render_DebugBuffer(TEXT("MRT_Defferred")));

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
	CPipeLineMgr*		pPipeLineMgr = GetSingle(CPipeLineMgr);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_PROJ))));

	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4)));
	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4)));



	FAILED_CHECK(m_pShader->Set_Texture("g_DiffuseTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_MtrlDiffuse"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_ShadeTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_LightShade"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_SpecularTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_LightSpecular"))));
	FAILED_CHECK(m_pShader->Set_Texture("g_DepthTexture", m_pRenderTargetMgr->Get_SRV(TEXT("Target_Depth"))));


	
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


	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 0));

	return S_OK;
}

//
//HRESULT CRenderer::Copy_ReferenceToDeferred()
//{
//	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_CopyDefferred")));
//
//	CPipeLineMgr*		pPipeLineMgr = GetSingle(CPipeLineMgr);
//
//	_float4x4		ViewMatrixInv, ProjMatrixInv;
//
//	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_VIEW))));
//	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_PROJ))));
//
//	m_pShader->Set_Texture("g_TargetTexture", m_pRenderTargetMgr->Get_SRV(L"Target_AfterDefferred"));
//
//	FAILED_CHECK(m_pShader->Set_RawValue("g_WorldMatrix", &m_WVPmat.WorldMatrix, sizeof(_float4x4)));
//	FAILED_CHECK(m_pShader->Set_RawValue("g_ViewMatrix", &m_WVPmat.ViewMatrix, sizeof(_float4x4)));
//	FAILED_CHECK(m_pShader->Set_RawValue("g_ProjMatrix", &m_WVPmat.ProjMatrix, sizeof(_float4x4)));
//
//	FAILED_CHECK(m_pVIBuffer->Render(m_pShader, 0));
//	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_CopyDefferred")));
//
//
//
//	return S_OK;
//}

HRESULT CRenderer::Render_NonBlend_NoLight()
{
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Defferred")));

	for (auto& RenderObject : m_RenderObjectList[RENDER_NONBLEND_NOLIGHT])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}

	m_RenderObjectList[RENDER_NONBLEND_NOLIGHT].clear();

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Defferred")));

	FAILED_CHECK(Copy_DeferredToReference());

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{

	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Defferred")));

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

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Defferred")));
	FAILED_CHECK(Copy_DeferredToReference());


	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Defferred")));

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

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Defferred")));
	FAILED_CHECK(Copy_DeferredToReference());

	return S_OK;
}

HRESULT CRenderer::Render_AfterObj()
{
	FAILED_CHECK(m_pRenderTargetMgr->Begin(TEXT("MRT_Defferred")));

	for (auto& RenderObject : m_RenderObjectList[RENDER_AFTERALLOBJ])
	{
		if (RenderObject != nullptr)
		{
			FAILED_CHECK(RenderObject->Render());
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_AFTERALLOBJ].clear();

	FAILED_CHECK(m_pRenderTargetMgr->End(TEXT("MRT_Defferred")));
	FAILED_CHECK(Copy_DeferredToReference());

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

	for (auto& DebugObject : m_DebugObjectList)
	{
		Safe_Release(DebugObject);
	}
	m_DebugObjectList.clear();

}
