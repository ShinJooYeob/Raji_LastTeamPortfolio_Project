#include "..\Public\VIBuffer_Terrain.h"
#include "Shader.h"

#define MovableHeight 0.5f


CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_fMinMapSize(rhs.m_fMinMapSize)
	, m_pNaviTerrain(rhs.m_pNaviTerrain)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMap)
{
	FAILED_CHECK(__super::Initialize_Prototype(nullptr));

	_tchar szHeightMapPath[MAX_PATH] = L"../Bin/Resources/Textures/HeightMap/";

	lstrcat(szHeightMapPath, pHeightMap);


	HANDLE		hFile = CreateFile(szHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong					dwByte = 0;
	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	 
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	_ulong*		pPixel = new _ulong[m_iNumVerticesX * m_iNumVerticesZ];
	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVerticesX * m_iNumVerticesZ, &dwByte, nullptr);



#pragma  region READY_VERTEX_BUFFER

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;

	m_VBDesc.ByteWidth = sizeof(VTXNORTEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXNORTEX);

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);
	m_pVertices = new _float3[m_iNumVertices];
	m_pNaviTerrain = new _float[m_iNumVertices];
	

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;
			//_float ValueY = (pPixel[iIndex] & 0x000000ff) / 10.f;			

			if ((_ulong((pPixel[iIndex] & 0xff000000) >> 24) ) == 0)
				pVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), -FLT_MAX, _float(i));
			else
			{
				_float fInt = _float(_ulong((pPixel[iIndex] & 0x0000ff00) >> 8));
				if (fInt > 255)fInt = 255;
				_float fFlt = _float(_ulong(pPixel[iIndex] & 0x000000ff))*0.01f;

				pVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), fInt + fFlt, _float(i));
				//pVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), _float((pPixel[iIndex] & 0x000000ff))*0.1f, _float(i));
			}

			//m_pKeepVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), (pPixel[iIndex] & 0x000000ff) / 1.f, _float(i));


			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
			m_pNaviTerrain[iIndex] = _float(_ulong((pPixel[iIndex] & 0x00ff0000) >> 16));
		}
	}



	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//락언락으로 값을 채워주는 것이 이제는 불가능하기 떄문에
	//D3D11_SUBRESOURCE_DATA 구조체에 값을 던져서 해당 값을 복사해서 생성하도록 한다.
	//이렇게 되면 우리는 해당 정점에 접근하고 싶다면?
	m_VBSubResourceData.pSysMem = pVertices;

	Safe_Delete_Array(pPixel);
	CloseHandle(hFile);

#pragma  endregion 

#pragma  region READY_INDEX_BUFFER
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	// m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	FACEINDICES32*	pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	_uint		iNumFace = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_Vector vSrc,vDest,vNorm;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[1];
			pIndices[iNumFace]._2 = iIndices[2];

			if (m_pVertices[pIndices[iNumFace]._0].y > -99999.f && m_pVertices[pIndices[iNumFace]._1].y > -99999.f && m_pVertices[pIndices[iNumFace]._2].y > -99999.f)
			{

				vSrc = (m_pVertices[pIndices[iNumFace]._1].XMVector()) - (m_pVertices[pIndices[iNumFace]._0].XMVector());
				vDest = (m_pVertices[pIndices[iNumFace]._2].XMVector()) - (m_pVertices[pIndices[iNumFace]._0].XMVector());
				vNorm = XMVector3Normalize(XMVector3Cross(vSrc, vDest));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._0].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._0].vNormal) + vNorm)));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._1].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._1].vNormal) + vNorm)));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._2].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._2].vNormal) + vNorm)));

			}

			++iNumFace;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[2];
			pIndices[iNumFace]._2 = iIndices[3];

			if (m_pVertices[pIndices[iNumFace]._0].y > -99999.f && m_pVertices[pIndices[iNumFace]._1].y > -99999.f && m_pVertices[pIndices[iNumFace]._2].y > -99999.f)
			{
				vSrc = (m_pVertices[pIndices[iNumFace]._1].XMVector()) - (m_pVertices[pIndices[iNumFace]._0].XMVector());
				vDest = (m_pVertices[pIndices[iNumFace]._2].XMVector()) - (m_pVertices[pIndices[iNumFace]._0].XMVector());
				vNorm = XMVector3Normalize(XMVector3Cross(vSrc, vDest));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._0].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._0].vNormal) + vNorm)));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._1].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._1].vNormal) + vNorm)));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._2].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._2].vNormal) + vNorm)));
			}

			++iNumFace;
		}
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

#pragma  endregion 

#pragma region VERTEX_BUFFER

	FAILED_CHECK(Create_VertexBuffer());
	Safe_Delete_Array(pVertices);


	FAILED_CHECK(Create_IndexBuffer());
	Safe_Delete_Array(pIndices);


	_float2 vTerrainSize = Get_NumVerticesXY();
	_float fCrossSize = vTerrainSize.Get_Lenth();
	m_fMinMapSize = fCrossSize / 5;

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumWidthPixelX, _uint iNumWidthPixelY)
{
	FAILED_CHECK(__super::Initialize_Prototype(nullptr));



#pragma  region READY_VERTEX_BUFFER

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	if (iNumWidthPixelY == 0)
	{
		m_iNumVerticesX = m_iNumVerticesZ = iNumWidthPixelX;
	}
	else {
		m_iNumVerticesX = iNumWidthPixelX;
		m_iNumVerticesZ = iNumWidthPixelY;
	}

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;

	m_VBDesc.ByteWidth = sizeof(VTXNORTEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXNORTEX);

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);
	m_pVertices = new _float3[m_iNumVertices];


	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), 0, _float(i));
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}


	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//락언락으로 값을 채워주는 것이 이제는 불가능하기 떄문에
	//D3D11_SUBRESOURCE_DATA 구조체에 값을 던져서 해당 값을 복사해서 생성하도록 한다.
	//이렇게 되면 우리는 해당 정점에 접근하고 싶다면?
	m_VBSubResourceData.pSysMem = pVertices;

#pragma  endregion 

#pragma  region READY_INDEX_BUFFER
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	// m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES32) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	FACEINDICES32*	pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	_uint		iNumFace = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_Vector vSrc, vDest, vNorm;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[1];
			pIndices[iNumFace]._2 = iIndices[2];

			if (m_pVertices[pIndices[iNumFace]._0].y > -99999.f && m_pVertices[pIndices[iNumFace]._1].y > -99999.f && m_pVertices[pIndices[iNumFace]._2].y > -99999.f)
			{

				vSrc = (m_pVertices[pIndices[iNumFace]._1].XMVector()) - (m_pVertices[pIndices[iNumFace]._0].XMVector());
				vDest = (m_pVertices[pIndices[iNumFace]._2].XMVector()) - (m_pVertices[pIndices[iNumFace]._0].XMVector());
				vNorm = XMVector3Normalize(XMVector3Cross(vSrc, vDest));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._0].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._0].vNormal) + vNorm)));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._1].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._1].vNormal) + vNorm)));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._2].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._2].vNormal) + vNorm)));

			}

			++iNumFace;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[2];
			pIndices[iNumFace]._2 = iIndices[3];

			if (m_pVertices[pIndices[iNumFace]._0].y > -99999.f && m_pVertices[pIndices[iNumFace]._1].y > -99999.f && m_pVertices[pIndices[iNumFace]._2].y > -99999.f)
			{
				vSrc = (m_pVertices[pIndices[iNumFace]._1].XMVector()) - (m_pVertices[pIndices[iNumFace]._0].XMVector());
				vDest = (m_pVertices[pIndices[iNumFace]._2].XMVector()) - (m_pVertices[pIndices[iNumFace]._0].XMVector());
				vNorm = XMVector3Normalize(XMVector3Cross(vSrc, vDest));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._0].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._0].vNormal) + vNorm)));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._1].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._1].vNormal) + vNorm)));

				XMStoreFloat3(&pVertices[pIndices[iNumFace]._2].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[iNumFace]._2].vNormal) + vNorm)));
			}

			++iNumFace;
		}
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

#pragma  endregion 

#pragma region CREATE_BUFFER

	FAILED_CHECK(Create_VertexBuffer());
	Safe_Delete_Array(pVertices);


	FAILED_CHECK(Create_IndexBuffer());
	Safe_Delete_Array(pIndices);
#pragma endregion


	_float2 vTerrainSize = Get_NumVerticesXY();
	_float fCrossSize = vTerrainSize.Get_Lenth();
	m_fMinMapSize = fCrossSize / 5;


	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	return S_OK;
}



_Vector CVIBuffer_Terrain::Pick_ByRay(_fVector vRayPos, _fVector vRayDir, _float2 vIndex, _bool * bIsPieck)
{
	if (vIndex.x < 0 || vIndex.x >= m_iNumVerticesX ||
		vIndex.y < 0 || vIndex.y >= m_iNumVerticesZ)
	{
		*bIsPieck = false;
		return _Vector();
	}

	_uint iIndex = _uint(_uint(vIndex.y) * m_iNumVerticesX + vIndex.x);

	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,	iIndex };

	_Vector vVertex1, vVertex2, vVertex3;


		vVertex1 = XMLoadFloat3(&m_pVertices[iIndices[0]]);
		vVertex2 = XMLoadFloat3(&m_pVertices[iIndices[2]]);
		vVertex3  = XMLoadFloat3(&m_pVertices[iIndices[3]]);



	_float Dist;
	if (TriangleTests::Intersects(vRayPos, vRayDir, vVertex1, vVertex2, vVertex3, Dist))
	{

		*bIsPieck = true;
		return vRayPos + (vRayDir * Dist) + XMVectorSet(0,0.00001f,0,0);
	}


 	vVertex1 = XMLoadFloat3(&m_pVertices[iIndices[0]]);
	vVertex2 = XMLoadFloat3(&m_pVertices[iIndices[1]]);
	vVertex3 = XMLoadFloat3(&m_pVertices[iIndices[2]]);

	if (TriangleTests::Intersects(vRayPos, vRayDir, vVertex1, vVertex2, vVertex3, Dist))
	{

		*bIsPieck = true;
		return vRayPos + (vRayDir * Dist) + XMVectorSet(0, 0.00001f, 0, 0);
	}

	*bIsPieck = false;
	return _Vector();

}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pHeightMap)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMap)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumWidthPixelX, _uint iNumWidthPixelY)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumWidthPixelX, iNumWidthPixelY)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	if (!m_bIsClone)
		Safe_Delete_Array(m_pNaviTerrain);

}
