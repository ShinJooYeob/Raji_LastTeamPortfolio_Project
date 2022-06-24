#pragma once

#include "Base.h"

BEGIN(Engine)

class CExternFont final : public CBase
{
private:
	CExternFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CExternFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pText, _float2 vPosition, _fVector vColor,_float fAngle, _float2 vScale);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

private:
	SpriteBatch*			m_pBatch = nullptr;
	SpriteFont*				m_pFont = nullptr;


public:
	static CExternFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontFilePath);
	virtual void Free() override;
};

END