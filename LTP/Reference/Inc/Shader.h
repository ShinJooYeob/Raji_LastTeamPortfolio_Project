#pragma once

#include "Component.h"



BEGIN(Engine)
class ENGINE_DLL CShader final : public CComponent
{
public:
	typedef struct tagShaderDesc
	{
		ID3DX11EffectPass*			pPass;
		ID3D11InputLayout*			pInputLayout;
	} SHADERDESC;

private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

private:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize_Clone(void * pArg)override;

public:
	HRESULT Apply(_uint iPassIndex);
	HRESULT Set_InputLayout(_uint iPassIndex);
	HRESULT Set_RawValue(const char* pValueName, const void* pData, _uint iLength);
	HRESULT Set_Texture(const char* pValueName, ID3D11ShaderResourceView* pShaderResourceView);


private:
	ID3DX11Effect*				m_pEffect = nullptr;
	vector<SHADERDESC*>			m_ShaderDescList;


public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END