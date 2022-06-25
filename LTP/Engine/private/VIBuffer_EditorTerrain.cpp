#include "..\Public\VIBuffer_EditorTerrain.h"
#include "Shader.h"

CVIBuffer_EditorTerrain::CVIBuffer_EditorTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext),
	m_pHeightMapSRV(nullptr)
{

}

CVIBuffer_EditorTerrain::CVIBuffer_EditorTerrain(const CVIBuffer_EditorTerrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_fMinMapSize(rhs.m_fMinMapSize)
	,m_pHeightMapSRV(rhs.m_pHeightMapSRV)
{
	Safe_AddRef(m_pHeightMapSRV);
}

HRESULT CVIBuffer_EditorTerrain::Initialize_Prototype(const _tchar* pHeightMap)
{
	FAILED_CHECK(__super::Initialize_Prototype(nullptr));

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	_tchar szHeightMapPath[MAX_PATH] = L"../Bin/Resources/Textures/HeightMap/";

	lstrcat(szHeightMapPath, pHeightMap);

	//if (m_pHeightMapSRV)Safe_Release(m_pHeightMapSRV);
	//FAILED_CHECK(CreateWICTextureFromFile(m_pDevice, szHeightMapPath, nullptr, &m_pHeightMapSRV));
	//NULL_CHECK_RETURN(m_pHeightMapSRV, E_FAIL);



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
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXNORTEX);

	m_pKeepVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(m_pKeepVertices, sizeof(VTXNORTEX) * m_iNumVertices);
	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);


	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;
			//_float ValueY = (pPixel[iIndex] & 0x000000ff) / 10.f;			

			if ((_ulong((pPixel[iIndex] & 0xff000000) >> 24)) == 0)
			{
				m_pKeepVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), -FLT_MAX, _float(i));

			}
			else
			{

				_float fInt = _float(_ulong((pPixel[iIndex] & 0x0000ff00)>> 8) );
				if (fInt > 255)fInt = 255;
				_float fFlt = _float(_ulong(pPixel[iIndex] & 0x000000ff))*0.01f;
				
				m_pKeepVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), fInt+ fFlt, _float(i));

				//m_pKeepVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), _float(_ulong(pPixel[iIndex] & 0x000000ff)*0.1f), _float(i));
			}

			//m_pKeepVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), (pPixel[iIndex] & 0x000000ff) / 1.f, _float(i));

			m_pKeepVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			m_pKeepVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
			//m_pNaviTerrain[iIndex] = Tile_Movable;
		}
	}


	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//락언락으로 값을 채워주는 것이 이제는 불가능하기 떄문에
	//D3D11_SUBRESOURCE_DATA 구조체에 값을 던져서 해당 값을 복사해서 생성하도록 한다.
	//이렇게 되면 우리는 해당 정점에 접근하고 싶다면?
	m_VBSubResourceData.pSysMem = m_pKeepVertices;

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

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._0].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._0].vNormal) + vNorm)));

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._1].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._1].vNormal) + vNorm)));

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._2].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._2].vNormal) + vNorm)));

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

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._0].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._0].vNormal) + vNorm)));

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._1].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._1].vNormal) + vNorm)));

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._2].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._2].vNormal) + vNorm)));
			}

			++iNumFace;
		}
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

#pragma  endregion 

#pragma region VERTEX_BUFFER


	FAILED_CHECK(Create_VertexBuffer());


#pragma endregion


#pragma region INDEX_BUFFER


	FAILED_CHECK(Create_IndexBuffer());
	Safe_Delete_Array(pIndices);

#pragma endregion
	_float2 vTerrainSize = Get_NumVerticesXY();
	_float fCrossSize = vTerrainSize.Get_Lenth();
	m_fMinMapSize = fCrossSize / 5;
	return S_OK;
}

HRESULT CVIBuffer_EditorTerrain::Initialize_Prototype(_uint iNumWidthPixelX, _uint iNumWidthPixelY)
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
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXNORTEX);

	m_pKeepVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(m_pKeepVertices, sizeof(VTXNORTEX) * m_iNumVertices);
	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);


	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			m_pKeepVertices[iIndex].vPosition = m_pVertices[iIndex] = _float3(_float(j), 0, _float(i));
			m_pKeepVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			m_pKeepVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}


	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//락언락으로 값을 채워주는 것이 이제는 불가능하기 떄문에
	//D3D11_SUBRESOURCE_DATA 구조체에 값을 던져서 해당 값을 복사해서 생성하도록 한다.
	//이렇게 되면 우리는 해당 정점에 접근하고 싶다면?
	m_VBSubResourceData.pSysMem = m_pKeepVertices;

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

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._0].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._0].vNormal) + vNorm)));

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._1].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._1].vNormal) + vNorm)));

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._2].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._2].vNormal) + vNorm)));

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

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._0].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._0].vNormal) + vNorm)));

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._1].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._1].vNormal) + vNorm)));

				XMStoreFloat3(&m_pKeepVertices[pIndices[iNumFace]._2].vNormal,
					(XMVector3Normalize(XMLoadFloat3(&m_pKeepVertices[pIndices[iNumFace]._2].vNormal) + vNorm)));
			}

			++iNumFace;
		}
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

#pragma  endregion 

#pragma region CREATE_BUFFER


	FAILED_CHECK(Create_VertexBuffer());


	FAILED_CHECK(Create_IndexBuffer());
	Safe_Delete_Array(pIndices);

#pragma endregion
	_float2 vTerrainSize = Get_NumVerticesXY();
	_float fCrossSize = vTerrainSize.Get_Lenth();
	m_fMinMapSize = fCrossSize / 5;
	

	return S_OK;
}

HRESULT CVIBuffer_EditorTerrain::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	return S_OK;
}


_Vector CVIBuffer_EditorTerrain::Caculate_TerrainY(_bool* pbIsOnTerrain ,_float3 PosOnTerrainLocal, _float3 OldPosOnTerrainLocal, _float3* vLocalPlaneNormVector)
{
	

	_float CacluatedOld = EquationPlane(pbIsOnTerrain, OldPosOnTerrainLocal);
	//if (*pbIsOnTerrain == false || CacluatedOld < 0)
	//{
	//	*pbIsOnTerrain = false;
	//	return _float3(PosOnTerrainLocal.x, -FLT_MAX, PosOnTerrainLocal.z);
	//}

	_float CaculatedY = 0;
	_float CacluatedNow = EquationPlane(pbIsOnTerrain, PosOnTerrainLocal, &CaculatedY, vLocalPlaneNormVector);

	if (*pbIsOnTerrain == false ) {
		*pbIsOnTerrain = false;
		return PosOnTerrainLocal.XMVector();
	}

	if (PosOnTerrainLocal.y <= CaculatedY && CacluatedOld >= -0.2f)
	{
		*pbIsOnTerrain = true;
		return _float3(PosOnTerrainLocal.x, CaculatedY, PosOnTerrainLocal.z).XMVector();

	}
	else {
		*pbIsOnTerrain = false;
		return PosOnTerrainLocal.XMVector();
	}
}


_Vector CVIBuffer_EditorTerrain::Caculate_Terrain_Pick_byRay(_bool* pbIsOnTerrain, _float3 PosOnTerrainLocal, _float3 OldPosOnTerrainLocal, _float3* vLocalPlaneNormVector)
{


	_float CacluatedOld = EquationPlane(pbIsOnTerrain, OldPosOnTerrainLocal);
	//if (*pbIsOnTerrain == false || CacluatedOld < 0)
	//{
	//	*pbIsOnTerrain = false;
	//	return _float3(PosOnTerrainLocal.x, -FLT_MAX, PosOnTerrainLocal.z);
	//}

	_float CaculatedY = 0;
	_float CacluatedNow = EquationPlane(pbIsOnTerrain, PosOnTerrainLocal, &CaculatedY, vLocalPlaneNormVector);

	if (*pbIsOnTerrain == false) {
		*pbIsOnTerrain = false;
		return PosOnTerrainLocal.XMVector();
	}

	if (PosOnTerrainLocal.y <= CaculatedY && CacluatedOld >= -0.2f)
	{

		RECT IndexRange = { 0 };

		if (OldPosOnTerrainLocal.x < PosOnTerrainLocal.x)
		{
			IndexRange.left = LONG(OldPosOnTerrainLocal.x);
			IndexRange.right = LONG(PosOnTerrainLocal.x);
		}
		else {
			IndexRange.left = LONG(PosOnTerrainLocal.x);
			IndexRange.right = LONG(OldPosOnTerrainLocal.x);
		}

		if (OldPosOnTerrainLocal.z < PosOnTerrainLocal.z)
		{
			IndexRange.bottom = LONG(OldPosOnTerrainLocal.z);
			IndexRange.top = LONG(PosOnTerrainLocal.z);
		}
		else
		{
			IndexRange.bottom = LONG(PosOnTerrainLocal.z);
			IndexRange.top = LONG(OldPosOnTerrainLocal.z);
		}

		IndexRange.left -= 1;
		IndexRange.bottom -= 1;
		IndexRange.right += 1;
		IndexRange.top += 1;

		if (IndexRange.left < 0)IndexRange.left = 0;
		if (IndexRange.bottom < 0)IndexRange.bottom = 0;
		if (IndexRange.right > _int(m_iNumVerticesX))IndexRange.right = m_iNumVerticesX;
		if (IndexRange.top > _int(m_iNumVerticesZ))IndexRange.top = m_iNumVerticesZ;

		vector<_float2> vecAvailIndex;
		vecAvailIndex.reserve(20);

		for (_uint i = IndexRange.left; _int(i) <= IndexRange.right; i++)
		{
			for (_uint j = IndexRange.bottom; _int(j) <= IndexRange.top; j++)
			{
				vecAvailIndex.push_back(_float2(_float(i), _float(j)));
			}
		}

		_bool IsPiecked = false;
		_Vector CalculatedLocalPos;


		_Vector vRayPos = OldPosOnTerrainLocal.XMVector();
		_Vector vRayDir = XMVector3Normalize(PosOnTerrainLocal.XMVector() - vRayPos);


		for (auto& AvailIndex : vecAvailIndex)
		{
			CalculatedLocalPos = Pick_ByRay(vRayPos, vRayDir, AvailIndex,&IsPiecked);

			if (IsPiecked)
			{
				*pbIsOnTerrain = true;
				return CalculatedLocalPos;
			}
		}


		*pbIsOnTerrain = false;
		return PosOnTerrainLocal.XMVector();

	}
	else {
		*pbIsOnTerrain = false;
		return PosOnTerrainLocal.XMVector();
	}
}


_float CVIBuffer_EditorTerrain::EquationPlane(_bool * pbIsOnTerrain, _float3 PosOnTerrainLocal, _float* pCaculateY, _float3* vNormVector)
{
	if (PosOnTerrainLocal.x < 0 || PosOnTerrainLocal.x >= m_iNumVerticesX ||
		PosOnTerrainLocal.z < 0 || PosOnTerrainLocal.z >= m_iNumVerticesZ)
	{
		*pbIsOnTerrain = false;
		return -FLT_MAX;
	}

	_uint iIndex = _uint(_uint(PosOnTerrainLocal.z) * m_iNumVerticesX + PosOnTerrainLocal.x);

	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex };

	if (m_pVertices[iIndices[0]].y < -99999.f || m_pVertices[iIndices[2]].y < -99999.f)
	{
		*pbIsOnTerrain = false;
		return -FLT_MAX;
	}

	_float4 Plane;

	if (PosOnTerrainLocal.x - m_pVertices[iIndices[0]].x < m_pVertices[iIndices[0]].z - PosOnTerrainLocal.z)
	{//아래 023
		if (m_pVertices[iIndices[3]].y < -99999.f)
		{
			*pbIsOnTerrain = false;
			return -FLT_MAX;
		}

		Plane = XMPlaneFromPoints(XMLoadFloat3(&m_pVertices[iIndices[0]]),
			XMLoadFloat3(&m_pVertices[iIndices[2]]), XMLoadFloat3(&m_pVertices[iIndices[3]]));


		if (vNormVector != nullptr)
		{
			*vNormVector = XMVector3Normalize(XMVector3Cross(
				(m_pVertices[iIndices[2]].XMVector()) - (m_pVertices[iIndices[0]].XMVector()),
				(m_pVertices[iIndices[3]].XMVector()) - (m_pVertices[iIndices[0]].XMVector())));
		}
	}
	else
	{//위 012
		if (m_pVertices[iIndices[1]].y < -99999.f)
		{
			*pbIsOnTerrain = false;
			return -FLT_MAX;
		}

		Plane = XMPlaneFromPoints(XMLoadFloat3(&m_pVertices[iIndices[0]]),
			XMLoadFloat3(&m_pVertices[iIndices[1]]), XMLoadFloat3(&m_pVertices[iIndices[2]]));

		if (vNormVector != nullptr)
		{
			*vNormVector = XMVector3Normalize(XMVector3Cross(
				(m_pVertices[iIndices[1]].XMVector()) - (m_pVertices[iIndices[0]].XMVector()),
				(m_pVertices[iIndices[2]].XMVector()) - (m_pVertices[iIndices[0]].XMVector())));
		}

	}

	if (pCaculateY != nullptr)
		*pCaculateY = ((Plane.x * PosOnTerrainLocal.x + Plane.z * PosOnTerrainLocal.z + Plane.w) / -Plane.y);


	*pbIsOnTerrain = true;
	return (Plane.x * PosOnTerrainLocal.x + Plane.y * PosOnTerrainLocal.y + Plane.z * PosOnTerrainLocal.z + Plane.w);
}


_Vector CVIBuffer_EditorTerrain::Pick_ByRay(_fVector vRayPos, _fVector vRayDir, _float2 vIndex, _bool * bIsPieck)
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

	for (_uint i = 0 ; i<4; i++)
	{
		if (iIndices[i] >= m_iNumVertices)
		{
			*bIsPieck = false;
			return _Vector();
		}
	}

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

_float CVIBuffer_EditorTerrain::Get_NowValueY(_float2 vIndex)
{
	if (vIndex.x < 0 || vIndex.x >= m_iNumVerticesX ||
		vIndex.y < 0 || vIndex.y >= m_iNumVerticesZ)
	{
		return NOT_EXIST_FLOAT;
	}

	_uint iIndex = _uint(_uint(vIndex.y) * m_iNumVerticesX + vIndex.x);

	return m_pKeepVertices[iIndex].vPosition.y;
}



CVIBuffer_EditorTerrain * CVIBuffer_EditorTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pHeightMap)
{
	CVIBuffer_EditorTerrain*	pInstance = new CVIBuffer_EditorTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMap)))
	{
		MSGBOX("Failed to Created CVIBuffer_EditorTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBuffer_EditorTerrain * CVIBuffer_EditorTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumWidthPixelX, _uint iNumWidthPixelY)
{
	CVIBuffer_EditorTerrain*	pInstance = new CVIBuffer_EditorTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumWidthPixelX, iNumWidthPixelY)))
	{
		MSGBOX("Failed to Created CVIBuffer_EditorTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_EditorTerrain::Clone(void * pArg)
{
	CVIBuffer_EditorTerrain*	pInstance = new CVIBuffer_EditorTerrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_EditorTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_EditorTerrain::Free()
{
	__super::Free();

	if (!m_bIsClone)
	{
		Safe_Delete_Array(m_pKeepVertices);
	}
		Safe_Release(m_pHeightMapSRV);
}
