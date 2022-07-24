#include "..\Public\Cell.h"
#include "VIBuffer_Triangle.h"
#include "Shader.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

void CCell::Set_ReLocation(_float3 PointPos, _float3 RePointPos)
{
	for (_int i = 0; i < POINT_END; ++i)
	{
		if (m_vPoints[i] == PointPos)
			m_vPoints[i] = RePointPos;
	}

	XMStoreFloat3(&m_vLineDir[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLineDir[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLineDir[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));
}

void CCell::Set_CellOption(CELL_OPTION eCellOption)
{
	m_eCellOption = eCellOption;

	switch (m_eCellOption)
	{
	case Engine::CCell::CELL_NOMAL:
		m_vColor = _float4(1.f, 1.f, 1.f, 1.f);
		break;
	case Engine::CCell::CELL_DROP:
		m_vColor = _float4(0.f, 0.f, 1.f, 1.f);
		break;
	case Engine::CCell::CELL_JUMPZONE:
		m_vColor = _float4(0.f,1.f,1.f,1.f);
		break;
	case Engine::CCell::CELL_BLOCKZONE:
		m_vColor = _float4(0.75f,0.f,1.f,1.f);
		break;

	case Engine::CCell::CELL_HIDE_ON_BUSH:
		m_vColor = _float4(0, 1.f, 0, 1.f);
		break;

	}
}

void CCell::PrevOptionColor()
{
	Set_CellOption(m_eCellOption);
}

HRESULT CCell::NativeConstruct(const _float3 * pPoints, _uint iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	XMStoreFloat3(&m_vLineDir[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLineDir[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLineDir[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	m_iIndex = iIndex;

	return S_OK;
}

_bool CCell::Compare_Points(_fVector vSourPoint, _fVector vDestPoint)
{

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fVector vPosition, _int* pNeighborIndex, _int* iLineNumber)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_Vector		vDestDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_Vector		vSourDir = XMVector3Normalize(XMLoadFloat4(&_float4(m_vLineDir[i].z * -1.f, 0.0f, m_vLineDir[i].x, 0.f)));

		if (0 < XMVectorGetX(XMVector3Dot(vDestDir, vSourDir)))
		{
			if (0 <= m_iNeighbor[i])
				*pNeighborIndex = m_iNeighbor[i];

			*iLineNumber = i;

			return false;
		}
	}

	return true;
}

#ifdef _DEBUG
HRESULT CCell::Render(CVIBuffer_Triangle* pVIBuffer, CShader* pShader, _float4	vColor)
{
	pVIBuffer->Update(m_vPoints);

	pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));

	if (FAILED(pVIBuffer->Render(pShader, 0)))
		return E_FAIL;

	return S_OK;
}
#endif // _DEBUG

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _float3 * pPoints, _uint iIndex)
{
	CCell*	pInstance = new CCell(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pPoints, iIndex)))
	{
		MSGBOX("Failed to Creating CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
