#include "..\Public\RenderTargetMgr.h"
#include "RenderTargetLayer.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CRenderTargetMgr)

CRenderTargetMgr::CRenderTargetMgr()
{
}

ID3D11ShaderResourceView * CRenderTargetMgr::Get_SRV(const _tchar * pTargetTag) const
{
	auto	iter = find_if(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CTagFinder(pTargetTag));

	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second->Get_SRV();	
}

HRESULT CRenderTargetMgr::Initialize_RenderTargetMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);

	NULL_CHECK_RETURN(m_pShader, E_FAIL);
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderTargetMgr::Add_RenderTarget(const _tchar * pRenderTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	//NULL_CHECK_RETURN(!(Find_RenderTarget(pRenderTargetTag)), E_FAIL);
	if (Find_RenderTarget(pRenderTargetTag) != nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	CRenderTargetLayer*		pRenderTarget = CRenderTargetLayer::Create(m_pDevice, m_pDeviceContext, iSizeX, iSizeY, eFormat, vClearColor);

	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	m_mapRenderTarget.emplace(pRenderTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetMgr::Add_MRT(const _tchar * pMRTTag, const _tchar * pRenderTargetTag)
{
	CRenderTargetLayer*		pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);


	list<CRenderTargetLayer*>*		pMRTGroup = Find_MRT(pMRTTag);

	if (nullptr == pMRTGroup)
	{
		list<CRenderTargetLayer*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_mapMRTGroup.emplace(pMRTTag, MRTList);
	}
	else
		pMRTGroup->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);	

	return S_OK;
}

HRESULT CRenderTargetMgr::Begin(const _tchar * pMRTTag)
{
	list<CRenderTargetLayer*>*	pMRTList = Find_MRT(pMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	m_pDeviceContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOriginalDSV);

	_uint iNumViews = 0;

	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTargets[iNumViews++] = pRenderTarget->Get_RTV();
	}

	m_pDeviceContext->OMSetRenderTargets(iNumViews, pRenderTargets, m_pOriginalDSV);


	ID3D11ShaderResourceView* pSRV[8] = { nullptr };
	m_pDeviceContext->PSSetShaderResources(0, 8, pSRV);

	return S_OK;
}

HRESULT CRenderTargetMgr::Begin(const _tchar * pMRTTag, ID3D11DepthStencilView * DepthStencil)
{
	list<CRenderTargetLayer*>*	pMRTList = Find_MRT(pMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	m_pDeviceContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOriginalDSV);

	_uint iNumViews = 0;

	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTargets[iNumViews++] = pRenderTarget->Get_RTV();
	}

	m_pDeviceContext->OMSetRenderTargets(iNumViews, pRenderTargets, DepthStencil);


	ID3D11ShaderResourceView* pSRV[8] = { nullptr };
	m_pDeviceContext->PSSetShaderResources(0, 8, pSRV);
	return S_OK;
}

HRESULT CRenderTargetMgr::Begin_WithBackBuffer(const _tchar * pMRTTag)
{
	list<CRenderTargetLayer*>*	pMRTList = Find_MRT(pMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	m_pDeviceContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOriginalDSV);

	_uint iNumViews = 1;

	ID3D11RenderTargetView*		pRenderTargets[8] = {  nullptr };

	pRenderTargets[0] = m_pOldRTV;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTargets[iNumViews++] = pRenderTarget->Get_RTV();
	}

	m_pDeviceContext->OMSetRenderTargets(iNumViews, pRenderTargets, m_pOriginalDSV);


	ID3D11ShaderResourceView* pSRV[8] = { nullptr };
	m_pDeviceContext->PSSetShaderResources(0, 8, pSRV);
	return S_OK;
}

HRESULT CRenderTargetMgr::End(const _tchar * pMRTTag)
{
	_uint		iNumViews = 1;

	m_pDeviceContext->OMSetRenderTargets(iNumViews, &m_pOldRTV, m_pOriginalDSV);

	Safe_Release(m_pOldRTV);
	Safe_Release(m_pOriginalDSV);

	ID3D11ShaderResourceView* pSRV[8] = { nullptr };
	m_pDeviceContext->PSSetShaderResources(0, 8, pSRV);

	return S_OK;
}

HRESULT CRenderTargetMgr::Clear_All_RenderTargetColor()
{

	for (auto& pRenderTarget : m_mapRenderTarget)
		pRenderTarget.second->Clear();

	return S_OK;
}



#ifdef _DEBUG

HRESULT CRenderTargetMgr::Ready_DebugDesc(const _tchar * pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTargetLayer*		pRenderTarget = Find_RenderTarget(pTargetTag);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	return pRenderTarget->Ready_DebugDesc(fX, fY, fSizeX, fSizeY);	
}

HRESULT CRenderTargetMgr::Render_DebugBuffer(const _tchar * pMRTTag)
{
	list<CRenderTargetLayer*>*		pMRTList = Find_MRT(pMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);


	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render_DebugBuffer(m_pShader, m_pVIBuffer);
	}

	return S_OK;
}

#endif // _DEBUG



CRenderTargetLayer * CRenderTargetMgr::Find_RenderTarget(const _tchar * pRenderTargetTag)
{
	auto	iter = find_if(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CTagFinder(pRenderTargetTag));
	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

list<CRenderTargetLayer*>* CRenderTargetMgr::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_mapMRTGroup.begin(), m_mapMRTGroup.end(), CTagFinder(pMRTTag));

	if (iter == m_mapMRTGroup.end())
		return nullptr;

	return &iter->second;
}

void CRenderTargetMgr::Free()
{
#ifdef _DEBUG

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
#endif // _DEBUG

	for (auto& Pair : m_mapMRTGroup)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			Safe_Release(pRenderTarget);
		}
		Pair.second.clear();
	}

	m_mapMRTGroup.clear();

	for (auto& Pair : m_mapRenderTarget)
		Safe_Release(Pair.second);
	m_mapRenderTarget.clear();


	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
