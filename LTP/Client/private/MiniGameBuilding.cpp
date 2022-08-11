#include "stdafx.h"
#include "..\public\MiniGameBuilding.h"



vector<MGBTARGET> CMiniGameBuilding::m_vecMGBTargets;
ID3D11Device* CMiniGameBuilding::m_pDevice_ForStatic = nullptr;
ID3D11DeviceContext* CMiniGameBuilding::m_pDeviceContext_ForStatic = nullptr;
MATRIXWVP CMiniGameBuilding::m_pFullScreenWVP;
_bool CMiniGameBuilding::m_bIsFirst = true;
			
				
CMiniGameBuilding::CMiniGameBuilding(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CMiniGameBuilding::CMiniGameBuilding(const CMiniGameBuilding & rhs)
	: CMapObject(rhs)
{
}

HRESULT CMiniGameBuilding::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	FAILED_CHECK(Ready_MiniGameScreenTarget_For_Prototype());

	return S_OK;
}

HRESULT CMiniGameBuilding::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));


	NULL_CHECK_RETURN(pArg, E_FAIL);
	memcpy(&m_tMGBDesc, pArg, sizeof(MGBDESC));
	m_tMGBDesc.vScale = m_tMGBDesc.vScale.XMVector() * XMVectorSet(1.471264367816f, 1.81818181818f, 1.471264367816f, 0);

	FAILED_CHECK(SetUp_Components());

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_tMGBDesc.vPosition);
	m_pTransformCom->Scaled_All(m_tMGBDesc.vScale.XMVector() * 0.01f);
	m_pTransformCom->LookDir(m_tMGBDesc.vLookDir.XMVector());


	m_ArrTextureDir[0] = _float2(1, 0);
	m_ArrTextureDir[1] = _float2(0, 0.9f);
	m_ArrTextureDir[2] = _float2(-1, 0);
	m_ArrTextureDir[3] = _float2(0, -1.05f);


	_float3 AllSacle = m_pTransformCom->Get_Scale() * 100.f;
	m_fFrustumRadius = sqrtf(AllSacle.x*AllSacle.x + AllSacle.y * AllSacle.y + AllSacle.z*AllSacle.z) * 2.15f;


	if (m_bIsFirst)
	{
		for (_uint i = 0 ; i < 4 ; i ++)
		{
			FAILED_CHECK(Copy_NowScreenToBuliding(i, m_pTexture->Get_ShaderResourceView(2 + i)));
		}
		m_bIsFirst = false;
	}

	return S_OK;
}

_int CMiniGameBuilding::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	static _uint TextInt = 0;
	if (g_pGameInstance->Get_DIKeyState(DIK_Z)&DIS_Down)
	{
		(Copy_NowScreenToBuliding(TextInt));
		TextInt++;
		if (TextInt > 3)TextInt = 0;
	}

	m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(30));

	m_pTransformCom->Turn_CW(XMVectorSet(0, 1, 0, 0), fDeltaTime);

	return _int();
}

_int CMiniGameBuilding::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;



	if (g_pGameInstance->IsNeedToLightRender(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), m_fFrustumRadius))
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	
	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), m_fFrustumRadius))
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CMiniGameBuilding::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 2, i));
	}


	FAILED_CHECK(m_pSubShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pSubShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	for (_uint i = 0 ; i < 4 ; i ++)
		FAILED_CHECK(Render_Texture(i));

	return _int();
}

_int CMiniGameBuilding::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

HRESULT CMiniGameBuilding::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_ShaderSub), (CComponent**)&m_pSubShaderCom));
	
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_SubTransform), (CComponent**)&m_pSubTransformCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MiniGameBuilding), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Texture_MingameBuildingTex), TAG_COM(Com_Texture), (CComponent**)&m_pTexture));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));


	//CShader*			m_pSubShaderCom = nullptr;
	//CTransform*			m_pSubTransformCom = nullptr;
	//CTexture*			m_pTexture = nullptr;
	//CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	return S_OK;
}

HRESULT CMiniGameBuilding::Render_Texture(_uint iIndex)
{
	switch (iIndex)
	{
	case 0:
	{
		m_pSubTransformCom->Set_MatrixState(CTransform::STATE_POS,
			m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * m_ArrTextureDir[iIndex].x * 1.91f * m_tMGBDesc.vScale.x +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * (m_ArrTextureDir[iIndex].y - 0.015f) * 1.91f* m_tMGBDesc.vScale.z +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 1.285f *m_tMGBDesc.vScale.y);

		m_pSubTransformCom->LookDir(-XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * m_ArrTextureDir[iIndex].x * m_tMGBDesc.vScale.x +
			m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * m_ArrTextureDir[iIndex].y * m_tMGBDesc.vScale.y));

		m_pSubTransformCom->Scaled_All(_float3(3.f * m_tMGBDesc.vScale.z, 1.5f * m_tMGBDesc.vScale.y, 1.f));

	}
	
		break;
	case 1:
	{
		m_pSubTransformCom->Set_MatrixState(CTransform::STATE_POS,
			m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) *  (m_ArrTextureDir[iIndex].x - 0.01f)* 1.91f * m_tMGBDesc.vScale.x +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * m_ArrTextureDir[iIndex].y * 1.91f* m_tMGBDesc.vScale.z +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 1.32f *m_tMGBDesc.vScale.y);

		m_pSubTransformCom->LookDir(-XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * m_ArrTextureDir[iIndex].x * m_tMGBDesc.vScale.x +
			m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * m_ArrTextureDir[iIndex].y * m_tMGBDesc.vScale.y));

		m_pSubTransformCom->Scaled_All(_float3(3.13f* m_tMGBDesc.vScale.x, 1.375f* m_tMGBDesc.vScale.y, 1.f));
	}
		break;
	case 2:
	{
		m_pSubTransformCom->Set_MatrixState(CTransform::STATE_POS,
			m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * m_ArrTextureDir[iIndex].x * 1.91f * m_tMGBDesc.vScale.x +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * (m_ArrTextureDir[iIndex].y - 0.0575f) * 1.91f* m_tMGBDesc.vScale.z +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 1.325f *m_tMGBDesc.vScale.y);

		m_pSubTransformCom->LookDir(-XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * m_ArrTextureDir[iIndex].x * m_tMGBDesc.vScale.x +
			m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * m_ArrTextureDir[iIndex].y * m_tMGBDesc.vScale.y));

		m_pSubTransformCom->Scaled_All(_float3(2.875f* m_tMGBDesc.vScale.z, 1.375f* m_tMGBDesc.vScale.y, 1.f));
	}
		break;
	case 3:
	{
		m_pSubTransformCom->Set_MatrixState(CTransform::STATE_POS,
			m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * (m_ArrTextureDir[iIndex].x)* 1.91f * m_tMGBDesc.vScale.x +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * m_ArrTextureDir[iIndex].y * 1.91f* m_tMGBDesc.vScale.z +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 1.305f *m_tMGBDesc.vScale.y);

		m_pSubTransformCom->LookDir(-XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * m_ArrTextureDir[iIndex].x * m_tMGBDesc.vScale.x +
			m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * m_ArrTextureDir[iIndex].y * m_tMGBDesc.vScale.y));

		m_pSubTransformCom->Scaled_All(_float3(2.825f* m_tMGBDesc.vScale.x, 1.33f* m_tMGBDesc.vScale.y, 1.f));
	}
		break;
	default:
		break;
	}



	


	FAILED_CHECK(m_pSubTransformCom->Bind_OnShader(m_pSubShaderCom, "g_WorldMatrix"));


	FAILED_CHECK(m_pTexture->Bind_OnShader(m_pSubShaderCom, "g_SourTexture", 1));
	FAILED_CHECK(m_pSubShaderCom->Set_Texture("g_DiffuseTexture", m_vecMGBTargets[iIndex].pSRV));
	FAILED_CHECK(m_pVIBufferCom->Render(m_pSubShaderCom, 10));

	return S_OK;
}

HRESULT CMiniGameBuilding::Ready_MiniGameScreenTarget_For_Prototype()
{
	m_pDevice_ForStatic = m_pDevice;
	m_pDeviceContext_ForStatic = m_pDeviceContext;

	for (auto& pDesc : m_vecMGBTargets)
	{
		Safe_Release(pDesc.pSRV);
		Safe_Release(pDesc.pRTV);
		Safe_Release(pDesc.pTexture2D);
	}
	m_vecMGBTargets.clear();
	m_vecMGBTargets.reserve(4);

	for (_uint i = 0 ; i < 4 ; i ++)
	{
		MGBTARGET tMGBDesc;
		tMGBDesc.vClearColor = _float4(0.f, 0.f, 0.f, 0.f);

		/* 1. */

		D3D11_TEXTURE2D_DESC	TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = g_iWinCX;
		TextureDesc.Height = g_iWinCY;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;

		FAILED_CHECK(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &tMGBDesc.pTexture2D));

		/* 2. ·»´õÅ¸°Ùºä */
		FAILED_CHECK(m_pDevice->CreateRenderTargetView(tMGBDesc.pTexture2D, nullptr, &tMGBDesc.pRTV));

		/* 3. ¼ÎÀÌ´õ¸®¼Ò½ººä*/
		FAILED_CHECK(m_pDevice->CreateShaderResourceView(tMGBDesc.pTexture2D, nullptr, &tMGBDesc.pSRV));

		m_vecMGBTargets.push_back(tMGBDesc);
	}




	m_pFullScreenWVP;

	XMStoreFloat4x4(&m_pFullScreenWVP.WorldMatrix, XMMatrixIdentity());

	m_pFullScreenWVP.WorldMatrix._11 = g_iWinCX;
	m_pFullScreenWVP.WorldMatrix._22 = g_iWinCY;
	m_pFullScreenWVP.WorldMatrix._33 = 1.f;
	memcpy(&m_pFullScreenWVP.WorldMatrix.m[3][0], &_float4(g_iWinCX * 0.5f - (g_iWinCX * 0.5f), -g_iWinCY * 0.5f + (g_iWinCY * 0.5f), 0.f, 1.f), sizeof(_float4));

	XMStoreFloat4x4(&m_pFullScreenWVP.WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_pFullScreenWVP.WorldMatrix)));
	XMStoreFloat4x4(&m_pFullScreenWVP.ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_pFullScreenWVP.ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	




	return S_OK;
}

HRESULT CMiniGameBuilding::Copy_NowScreenToBuliding(_uint iIndex, ID3D11ShaderResourceView* pTargetSRV)
{

	//ClearRenderTarget
	NULL_CHECK_RETURN(m_pDeviceContext_ForStatic, E_FAIL);
	m_pDeviceContext_ForStatic->ClearRenderTargetView(m_vecMGBTargets[iIndex].pRTV, (_float*)&m_vecMGBTargets[iIndex].vClearColor);

	//////////////////////////////////////////////////////////////////////////

	//Begin
	ID3D11RenderTargetView*			m_pOldRTV = nullptr;
	ID3D11DepthStencilView*			m_pOriginalDSV = nullptr;

	{
		m_pDeviceContext_ForStatic->OMGetRenderTargets(1, &m_pOldRTV, &m_pOriginalDSV);

		_uint iNumViews = 0;
		ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };
		pRenderTargets[iNumViews++] = m_vecMGBTargets[iIndex].pRTV;
		m_pDeviceContext_ForStatic->OMSetRenderTargets(iNumViews, pRenderTargets, m_pOriginalDSV);


		ID3D11ShaderResourceView* pSRV[8] = { nullptr };
		m_pDeviceContext_ForStatic->PSSetShaderResources(0, 8, pSRV);
	}
	//////////////////////////////////////////////////////////////////////////

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	CShader* pShader = pUtil->Get_RectShader();
	NULL_CHECK_RETURN(pShader,E_FAIL);

	CVIBuffer_Rect* pVIBufferRect = pUtil->Get_VIBuffer_Rect();
	NULL_CHECK_RETURN(pVIBufferRect, E_FAIL);

	if (pTargetSRV == nullptr)
	{
		FAILED_CHECK(pShader->Set_Texture("g_DiffuseTexture", g_pGameInstance->Get_SRV(TEXT("Target_Defferred"))));
	}
	else
	{
		FAILED_CHECK(pShader->Set_Texture("g_DiffuseTexture", pTargetSRV));
	}


	FAILED_CHECK(pShader->Set_RawValue("g_WorldMatrix", &m_pFullScreenWVP.WorldMatrix, sizeof(_float4x4)));
	FAILED_CHECK(pShader->Set_RawValue("g_ViewMatrix", &m_pFullScreenWVP.ViewMatrix, sizeof(_float4x4)));
	FAILED_CHECK(pShader->Set_RawValue("g_ProjMatrix", &m_pFullScreenWVP.ProjMatrix, sizeof(_float4x4)));


	FAILED_CHECK(pVIBufferRect->Render(pShader, 11));

	//////////////////////////////////////////////////////////////////////////

	//End
	{
		_uint iNumViews = 1;
		m_pDeviceContext_ForStatic->OMSetRenderTargets(iNumViews, &m_pOldRTV, m_pOriginalDSV);
		Safe_Release(m_pOldRTV);
		Safe_Release(m_pOriginalDSV);
		ID3D11ShaderResourceView* pSRV[8] = { nullptr };
		m_pDeviceContext_ForStatic->PSSetShaderResources(0, 8, pSRV);
	}
	//////////////////////////////////////////////////////////////////////////



	return S_OK;
}




CMiniGameBuilding * CMiniGameBuilding::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMiniGameBuilding*	pInstance = NEW CMiniGameBuilding(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMiniGameBuilding");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMiniGameBuilding::Clone(void * pArg)
{
	CMiniGameBuilding*	pInstance = NEW CMiniGameBuilding(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMiniGameBuilding");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMiniGameBuilding::Free()
{
	__super::Free();

	if (!m_bIsClone)
	{
		for (auto& pDesc : m_vecMGBTargets)
		{
			Safe_Release(pDesc.pSRV);
			Safe_Release(pDesc.pRTV);
			Safe_Release(pDesc.pTexture2D);
		}
		m_vecMGBTargets.clear();
	}

	Safe_Release(m_pRendererCom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);

	Safe_Release(m_pSubTransformCom);
	Safe_Release(m_pSubShaderCom);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBufferCom);


}
