#include "..\public\Texture.h"
#include "TextureLayer.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs),
	m_mapTextureLayers(rhs.m_mapTextureLayers)
{
	for (auto& pair : m_mapTextureLayers)
	{
		Safe_AddRef(pair.second);
	}

	auto iter = m_mapTextureLayers.begin();
	m_pBindedTextureLayer = iter->second;
	m_TagNowTexture = iter->first.c_str();
	m_fFrameTime = 0;
	m_fFramePerSec = 6;
	m_iNumMaxTexture = m_pBindedTextureLayer->Get_TextureListSize();

	Safe_AddRef(m_pBindedTextureLayer);

}

HRESULT CTexture::Initialize_Prototype(const _tchar * pTextureFilePath)
{
	FAILED_CHECK(__super::Initialize_Prototype(nullptr));

	FAILED_CHECK(Read_TextFile(pTextureFilePath));

	return S_OK;
}

HRESULT CTexture::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype(nullptr));

	m_pBindedTextureLayer = nullptr;
	m_TagNowTexture = nullptr;

	m_fFrameTime = 0;
	m_fFramePerSec = 6.;
	m_iNumMaxTexture = 0;

	return S_OK;
}

HRESULT CTexture::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	return S_OK;
}

HRESULT CTexture::NullCheckTexture(_uint iIndex)
{
	if (m_pBindedTextureLayer == nullptr)
		return E_FAIL;

	return m_pBindedTextureLayer->NullCheckTexture(iIndex);
}

HRESULT CTexture::Bind_OnShader(CShader * pShader, const char * pValueName, _uint iTextureIndex)
{
	if (pShader == nullptr || m_pBindedTextureLayer == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	return m_pBindedTextureLayer->Bind_OnShader(pShader, pValueName, iTextureIndex);
}

HRESULT CTexture::Bind_OnShader_AutoFrame(CShader * pShader, const char * pValueName, _double fTimeDelta, _double * pOutFrameCount)
{
	if (pShader == nullptr || m_pBindedTextureLayer == nullptr)
		return E_FAIL;

	m_fFrameTime += m_fFramePerSec * fTimeDelta;

	if (m_fFrameTime >= m_iNumMaxTexture)
		m_fFrameTime = 0;

	if (pOutFrameCount != nullptr)
		*pOutFrameCount = m_fFrameTime;

	return m_pBindedTextureLayer->Bind_OnShader(pShader, pValueName, _uint(m_fFrameTime));
}

HRESULT CTexture::Change_TextureLayer(const _tchar * tagTexureLayer, _double fFramePerSec)
{
	if (!lstrcmp(m_TagNowTexture, tagTexureLayer))
	{
		m_fFramePerSec = fFramePerSec;
		return S_FALSE;
	}

	auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagStringFinder(tagTexureLayer));

	if (iter == m_mapTextureLayers.end())
		return E_FAIL;


	Safe_Release(m_pBindedTextureLayer);
	m_pBindedTextureLayer = iter->second;
	Safe_AddRef(m_pBindedTextureLayer);

	m_fFrameTime = 0;
	m_TagNowTexture = (iter->first).c_str();
	m_iNumMaxTexture = m_pBindedTextureLayer->Get_TextureListSize();


	return S_OK;
}

HRESULT CTexture::Insert_Empty_TextureLayer(_tchar * szStateKey)
{
	auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagStringFinder(szStateKey));


	if (iter != m_mapTextureLayers.end())
	{
		OutputDebugString(TEXT("Already Exist Texture Layer\n"));
		__debugbreak();
		return E_FAIL;
	}

	CTextureLayer* pTextureLayer = CTextureLayer::Create(m_pDevice, m_pDeviceContext);

	NULL_CHECK_RETURN(pTextureLayer, E_FAIL);

	m_mapTextureLayers.emplace(wstring(szStateKey), pTextureLayer);

	FAILED_CHECK( Change_TextureLayer(szStateKey));

	return S_OK;
}

HRESULT CTexture::Insert_Texture_On_BindedLayer(_uint iIndex,  _tchar * szFilePath)
{
	NULL_CHECK_RETURN(m_pBindedTextureLayer, E_FAIL);

	return m_pBindedTextureLayer->Add_Model_Texture(iIndex,szFilePath);
}





HRESULT CTexture::Read_TextFile(const _tchar * szFilePath)
{

	wifstream		fin;

	_tchar FullFilePath[MAX_PATH] = L"../Bin/Resources/Data/TexturePath/";

	lstrcat(FullFilePath, szFilePath);

	fin.open(FullFilePath);

	if (!fin.fail())
	{
		_tchar	szPadding[64] = L"";
		_tchar	szPath[MAX_PATH] = L"";
		_tchar	szStateKey[MAX_PATH] = L"";
		_tchar	szCount[MAX_PATH] = L"";

		while (true)
		{

			// '|' 단위로 끊어서 문자열 입력 처리

			fin.getline(szPadding, MAX_PATH, '|');
			fin.getline(szStateKey, MAX_PATH, '|');
			fin.getline(szCount, MAX_PATH, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			// 문자열을 정수로 변환 함수
			_int iCount = _ttoi(szCount);

			FAILED_CHECK(Insert_TextureLayer(szPath, szStateKey, iCount));


			if (fin.eof())
				break;
		}

		fin.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}


	auto iter = m_mapTextureLayers.begin();

	//m_pBindedTextureLayer = iter->second;
	//m_TagNowTexture = iter->first.c_str();
	//Safe_AddRef(m_pBindedTextureLayer);

	//m_fFrameTime = 0;
	//m_iNumMaxTexture = m_pBindedTextureLayer->Get_TextureNum();

	return S_OK;


	return S_OK;
}

HRESULT CTexture::Insert_TextureLayer(_tchar * szFilePath, _tchar * szStateKey, _int iNumTextureCount)
{


	auto iter = find_if(m_mapTextureLayers.begin(), m_mapTextureLayers.end(), CTagStringFinder(szStateKey));

	if (iter != m_mapTextureLayers.end()) {
		FAILED_CHECK((iter)->second->Add_Another_Texture(szFilePath, iNumTextureCount))
	}
	else
	{

		CTextureLayer* pTextureLayer = CTextureLayer::Create(m_pDevice, m_pDeviceContext, szFilePath, iNumTextureCount);

		NULL_CHECK_RETURN(pTextureLayer, E_FAIL);

		m_mapTextureLayers.emplace(szStateKey, pTextureLayer);
	}

	return S_OK;
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pTextureFilePath)
{
	CTexture*	pInstance = new CTexture(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTexture*	pInstance = new CTexture(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Failed to Creating 3D Model Texture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*	pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();


	Safe_Release(m_pBindedTextureLayer);

	for (auto& pair : m_mapTextureLayers)
		Safe_Release(pair.second);
	
	m_mapTextureLayers.clear();

}
