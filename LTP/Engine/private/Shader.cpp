#include "..\public\Shader.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_ShaderDescList(rhs.m_ShaderDescList)
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElements)
{
	FAILED_CHECK(__super::Initialize_Prototype(nullptr));

	_tchar szShaderFullPath[MAX_PATH] = L"../Bin/ShaderFiles/";

	lstrcat(szShaderFullPath, pShaderFilePath);

	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif
	ID3DBlob*			pShaderBinaryCodes = nullptr;
	ID3DBlob*			pErrorMessage = nullptr;

	FAILED_CHECK(D3DCompileFromFile(szShaderFullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pShaderBinaryCodes, &pErrorMessage));
	//컴파일러를 이용해 hlsl 파일을 바이너리 코드로 읽어둠

	FAILED_CHECK(D3DX11CreateEffectFromMemory(pShaderBinaryCodes->GetBufferPointer(), pShaderBinaryCodes->GetBufferSize(), 0, m_pDevice, &m_pEffect));
	//읽어둔 바이너리 파일을 cpp에서 사용할 수 있는 Effect객체로 치환해둠

	ID3DX11EffectTechnique*			pTechnique = m_pEffect->GetTechniqueByIndex(0);
	NULL_CHECK_RETURN(pTechnique, E_FAIL);
	//치환된 Effect객체를 통해 테크니커(pass의 집합)에 대한 정보를 받아옴
	
	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	FAILED_CHECK(pTechnique->GetDesc(&TechniqueDesc));
	//테크니커 Desc를 받아온다(해당 테크니커 안의 pass의 총 개수를 알고싶어서)


	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		SHADERDESC*		pShaderDesc = new SHADERDESC;
		ZeroMemory(pShaderDesc, sizeof(SHADERDESC));

		pShaderDesc->pPass = pTechnique->GetPassByIndex(i);
		NULL_CHECK_RETURN(pShaderDesc->pPass, E_FAIL);

		D3DX11_PASS_DESC		PassDesc;
		ZeroMemory(&PassDesc, sizeof(D3DX11_PASS_DESC));
		pShaderDesc->pPass->GetDesc(&PassDesc);

		/*
		D3D11_INPUT_ELEMENT_DESC 란 우리가 실제 오브젝트에서 사용할의 정보에 대해 알려주는 구조체이다.
		또한 배열로 던저야 하기 때문에 iNumElements 을 같이 던져서 몇개의 정점 정보를 사용하는지 알려줘야한다.
		
		추가적으로 9 에서까지는 VS_IN과 실제 사용할 정점의 정보가 다르더라도 내부적으로 알아서 던져주게 되었는데
		11부터는 다르면 터지게 된다. 
		그래서 1,2 인자는 우리가 사용하는 정점에 대한 정보
		2,3 인자는 pass에서 VS가 Input으로 받아들이는 정점 정보에 대한 정보를 넘겨주는 것

		*/
		FAILED_CHECK(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &(pShaderDesc->pInputLayout)));
		//이렇게 레이아웃(쉐이더에게 넘겨줄 정점의 규격들에 대한 정보)를 만듬


		m_ShaderDescList.push_back(pShaderDesc);
	}

	return S_OK;
}

HRESULT CShader::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	return S_OK;
}

HRESULT CShader::Apply(_uint iPassIndex)
{
	if (iPassIndex >= m_ShaderDescList.size())
		return E_FAIL;

	return m_ShaderDescList[iPassIndex]->pPass->Apply(0, m_pDeviceContext);
}

HRESULT CShader::Set_InputLayout(_uint iPassIndex)
{
	if (iPassIndex >= m_ShaderDescList.size())
		return E_FAIL;

	m_pDeviceContext->IASetInputLayout(m_ShaderDescList[iPassIndex]->pInputLayout);

	return S_OK;
}

HRESULT CShader::Set_RawValue(const char * pValueName, const void * pData, _uint iLength)
{
	ID3DX11EffectVariable*		pValue = m_pEffect->GetVariableByName(pValueName);
	NULL_CHECK_RETURN(pValue, E_FAIL);

	return pValue->SetRawValue(pData, 0, iLength);
}

HRESULT CShader::Set_Texture(const char * pValueName, ID3D11ShaderResourceView * pShaderResourceView)
{
	ID3DX11EffectVariable*		pValue = m_pEffect->GetVariableByName(pValueName);
	NULL_CHECK_RETURN(pValue, E_FAIL);

	ID3DX11EffectShaderResourceVariable*	pShaderResourceVariable = pValue->AsShaderResource();
	NULL_CHECK_RETURN(pShaderResourceVariable, E_FAIL);

	return pShaderResourceVariable->SetResource(pShaderResourceView);
}

CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElements)
{
	CShader*	pInstance = new CShader(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
	{
		MSGBOX("Failed to Creating CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*	pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	if (false == m_bIsClone)
	{
		for (auto& pPassDesc : m_ShaderDescList)
		{
			Safe_Release(pPassDesc->pPass);
			Safe_Release(pPassDesc->pInputLayout);

			Safe_Delete(pPassDesc);
		}

		m_ShaderDescList.clear();
	}


	Safe_Release(m_pEffect);
}
