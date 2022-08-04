#include "..\public\VIBuffer_Plat.h"



CVIBuffer_Plat::CVIBuffer_Plat(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Plat::CVIBuffer_Plat(const CVIBuffer_Plat & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Plat::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

#pragma region VERTEX_BUFFER
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumVertices = 4;
	m_iNumVertexBuffers = 1;

	m_VBDesc.ByteWidth = sizeof(VTXTEX) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXTEX);

	VTXTEX*		pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.f, 0.5f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.f, 0.5f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.f, -0.5f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, 0.f, -0.5f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//락언락으로 값을 채워주는 것이 이제는 불가능하기 떄문에
	//D3D11_SUBRESOURCE_DATA 구조체에 값을 던져서 해당 값을 복사해서 생성하도록 한다.
	//이렇게 되면 우리는 해당 정점에 접근하고 싶다면?
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion


#pragma region INDEX_BUFFER
	m_iNumPrimitive = 2;
	m_iNumIndicesPerPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	// m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACEINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	FACEINDICES16*	pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Plat::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	return S_OK;
}

_float CVIBuffer_Plat::Get_OnPlatHeight(_fVector vPos)
{
//	_Vector		vPosition = vTargetPos;

	_float	fY = 0.f;
//	_Vector	vPlane;

	/*vPlane = XMPlaneFromPoints(m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_A), m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_B), m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_C));

	fY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition)
		- XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition)
		- XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);*/

	return fY;
}

CVIBuffer_Plat * CVIBuffer_Plat::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CVIBuffer_Plat*	pInstance = new CVIBuffer_Plat(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Plat");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Plat::Clone(void * pArg)
{
	CVIBuffer_Plat*	pInstance = new CVIBuffer_Plat(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Plat");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Plat::Free()
{
	__super::Free();
}
