#pragma once

#include "Component.h"



BEGIN(Engine)

class CTextureLayer;

class ENGINE_DLL CTexture final : public CComponent
{

private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

private:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath);
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void * pArg)override;

public:
	HRESULT NullCheckBinedTextureLayer() { if (m_pBindedTextureLayer == nullptr)return E_FAIL; return S_OK; };
	HRESULT NullCheckTexture(_uint iIndex);


	//바인드 관련
	HRESULT Bind_OnShader(class CShader* pShader, const char* pValueName, _uint iTextureIndex = 0);
	HRESULT Bind_OnShader_AutoFrame(class CShader* pShader, const char* pValueName, _double fTimeDelta, _double* pOutFrameCount = nullptr);


	//체인지 관련
	HRESULT Change_TextureLayer(const _tchar* tagTexureLayer, _double fFramePerSec = 6.f);

	//3D 모델 레이어 추가용
	HRESULT Insert_Empty_TextureLayer(_tchar* szStateKey);
	HRESULT Insert_Texture_On_BindedLayer(_uint iIndex, _tchar* szFilePath);

private:
	map<wstring, CTextureLayer*>				m_mapTextureLayers;
	typedef map<wstring, CTextureLayer*>		TEXTURELAYERS;


	//얘는 다른 클론들과 공유 X
	CTextureLayer*								m_pBindedTextureLayer = nullptr;
	const _tchar*								m_TagNowTexture = nullptr;

	_double										m_fFrameTime = 0;
	_double										m_fFramePerSec = 6.;
	_uint										m_iNumMaxTexture = 0;

private:
	HRESULT Read_TextFile(const _tchar* szFilePath);
	HRESULT Insert_TextureLayer( _tchar* szFilePath, _tchar* szStateKey, _int iNumTextureCount);


public:
	//일반 텍스처용
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pTextureFilePath);
	//3D 모델용 빈 텍스처
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END