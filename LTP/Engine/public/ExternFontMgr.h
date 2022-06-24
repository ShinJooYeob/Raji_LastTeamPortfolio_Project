#pragma once

#include "Base.h"

BEGIN(Engine)

class CExternFontMgr final : public CBase
{
	DECLARE_SINGLETON(CExternFontMgr)
public:
	CExternFontMgr();
	virtual ~CExternFontMgr() = default;

public:
	HRESULT Initialize_FontMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

public:
	HRESULT Add_Font( const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fVector vColor, _float fAngle,_float2 vScale);

private:
	map<const _tchar*, class CExternFont*>			m_Fonts;
	typedef map<const _tchar*, class CExternFont*>	FONTS;


	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

private:
	class CExternFont* Find_SpriteFont(const _tchar* pFontTag);

public:
	virtual void Free() override;
};

END