#pragma once

#include "Component.h"



BEGIN(Engine)
class CShader;
class ENGINE_DLL CTextureLayer final : public CComponent
{

private:
	CTextureLayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTextureLayer(const CTextureLayer& rhs);
	virtual ~CTextureLayer() = default;


public:
	HRESULT Bind_OnShader(CShader* pShader, const char* pValueName , _uint iTextureIndex = 0);

	HRESULT Add_Another_Texture(const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	HRESULT Add_Model_Texture(_uint iIndex, const _tchar* pTextureFilePath);

	_uint Get_TextureListSize() { return (_uint)(m_vecTextures.size()); }
	HRESULT NullCheckTexture(_uint iIndex);
private:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void * pArg)override;

private:
	vector<ID3D11ShaderResourceView*>				m_vecTextures;
	typedef vector<ID3D11ShaderResourceView*>		TEXTURES;


public:
	//일반 텍스처용
	static CTextureLayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	//3d 모델용 텍스처
	static CTextureLayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END