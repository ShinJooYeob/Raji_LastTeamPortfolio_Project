#include "..\Public\ExternFont.h"
#include "PipeLineMgr.h"

CExternFont::CExternFont(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CExternFont::Initialize(const _tchar * pFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pDeviceContext);
	if (nullptr == m_pBatch)
		return E_FAIL;

	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);
	if (nullptr == m_pFont)
		return E_FAIL;

	return S_OK;
}

HRESULT CExternFont::Render(const _tchar* pText, _float2 vPosition, _fVector vColor , _float fAngle,_float2 vScale)
{
	CPipeLineMgr*		pPipeLine = GetSingle(CPipeLineMgr);

	_Matrix			ViewMatrix = pPipeLine->Get_Transform_Matrix(PLM_VIEW);
	_Matrix			ProjMatrix = pPipeLine->Get_Transform_Matrix(PLM_PROJ);

	m_pBatch->Begin();	
	m_pFont->DrawString(m_pBatch, pText, XMVectorSet(vPosition.x, vPosition.y, 0.f, 1.f), vColor, XMConvertToRadians(fAngle), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(vScale.x , vScale.y, 1.f, 0.f));
	m_pBatch->End();


	return S_OK;
}

CExternFont * CExternFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontFilePath)
{
	CExternFont*	pInstance = new CExternFont(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		MSGBOX("Failed to Created CFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CExternFont::Free()
{

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
