#include "..\Public\ExternFontMgr.h"
#include "ExternFont.h"

IMPLEMENT_SINGLETON(CExternFontMgr)

CExternFontMgr::CExternFontMgr()
{
}


HRESULT CExternFontMgr::Initialize_FontMgr(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CExternFontMgr::Add_Font(const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr != Find_SpriteFont(pFontTag))
	{
		__debugbreak();
		return E_FAIL;
	}

	CExternFont*	pSpriteFont = CExternFont::Create(m_pDevice, m_pDeviceContext, pFontFilePath);
	NULL_CHECK_RETURN(pSpriteFont, E_FAIL);

	m_Fonts.emplace(pFontTag, pSpriteFont);

	return S_OK;
}

HRESULT CExternFontMgr::Render_Font(const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _fVector vColor,_float fAngle,_float2 vScale)
{
	CExternFont*		pFont = Find_SpriteFont(pFontTag);

	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);

	return pFont->Render(pText, vPosition, vColor,fAngle, vScale);
}

CExternFont * CExternFontMgr::Find_SpriteFont(const _tchar * pFontTag)
{
	auto	iter = find_if(m_Fonts.begin(), m_Fonts.end(), CTagFinder(pFontTag));

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

void CExternFontMgr::Free()
{
	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
