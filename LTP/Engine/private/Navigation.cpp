#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLineMgr.h"
#include "Transform.h"
#include "VIBuffer_Triangle.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pVIBuffer(rhs.m_pVIBuffer)
	, m_pShader(rhs.m_pShader)
#endif // _DEBUG
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pVIBuffer);
#endif // _DEBUG
}

HRESULT CNavigation::Initialize_Prototype(const _tchar * pNaviDataFilePath)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pNaviDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3];



	while (true)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
		CCell::CELL_OPTION CellOption = CCell::CELL_END;
		ReadFile(hFile, &CellOption, sizeof(CCell::CELL_OPTION), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, _uint(m_Cells.size()));
		if (nullptr == pCell)
			return E_FAIL;

		pCell->Set_CellOption(CellOption);

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	//m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXCOL_DECLARATION::Elements, VTXCOL_DECLARATION::iNumElements);
	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_Navigation.hlsl"), VTXCOL_DECLARATION::Elements, VTXCOL_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::Initialize_Clone(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_NaviDesc, pArg, sizeof(NAVIDESC));

	return S_OK;
}

_bool CNavigation::Move_OnNavigation(_fVector vPosition, _Vector vDir, _Vector* vSlidingVec)
{
	_int		iNeighborIndex = -1;
	_int		iLineNumber = 0;

	/* _fvector vPosition : 움직이고 난 후의 위치다. */
	if (false == m_Cells[m_NaviDesc.iCurrentIndex]->isIn(vPosition, &iNeighborIndex, &iLineNumber))
	{
		/* 나간쪽에 이웃이 있다라면. */
		if (0 <= iNeighborIndex)
		{
			/* if move to Block Zone Option Cell */
			if (CCell::CELL_OPTION::CELL_BLOCKZONE == m_Cells[iNeighborIndex]->Get_CellOption())
			{
				_Vector Line = m_Cells[m_NaviDesc.iCurrentIndex]->Get_LineDir(iLineNumber);
				Line = XMVector3Normalize(Line);
				vDir = XMVector3Normalize(vDir);

				_float CosValue = XMVectorGetX(XMVector3Dot(Line, vDir));
				*vSlidingVec = Line * CosValue;
			}

			while (true)
			{
				_int	iCurrentNeighborIndex = -1;

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iCurrentNeighborIndex, &iLineNumber))
				{
					m_NaviDesc.iCurrentIndex = iNeighborIndex;
					break;
				}
				else
				{
					if (-1 == iCurrentNeighborIndex)
					{
						_Vector Line = m_Cells[m_NaviDesc.iCurrentIndex]->Get_LineDir(iLineNumber);
						Line = XMVector3Normalize(Line);
						vDir = XMVector3Normalize(vDir);

						_float CosValue = XMVectorGetX(XMVector3Dot(Line, vDir));
						*vSlidingVec = Line * CosValue;


						return false;
					}
				}
				iNeighborIndex = iCurrentNeighborIndex;

			}
			return true;
		}
		/* 이웃이 없는 쪽으로 나갔다면. */
		else
		{
			/* 슬라이딩. */
			//_Vector SlidingVector;
			_Vector Line = m_Cells[m_NaviDesc.iCurrentIndex]->Get_LineDir(iLineNumber);
			Line = XMVector3Normalize(Line);
			vDir = XMVector3Normalize(vDir);

			_float CosValue = XMVectorGetX(XMVector3Dot(Line, vDir));
			*vSlidingVec = Line * CosValue;

			//_vector NomalVec = XMVectorSet(-XMVectorGetZ(Line), 0, XMVectorGetX(Line), 0);
			//_vector NomalVec = XMVector3Cross(Line, vDir);;
			//NomalVec = XMVector3Normalize(NomalVec);

			//NomalVec = XMVector3Cross(Line, NomalVec);
			//NomalVec = XMVector3Normalize(NomalVec);

			//_vector vDotVec = XMVector3Dot(-vDir, NomalVec);
			//vDotVec = XMVector3Normalize(vDotVec);

			//SlidingVector = vDir + NomalVec * vDotVec;
			//SlidingVector = XMVector3Normalize(SlidingVector);
			//SlidingVector = XMVectorSetW(SlidingVector, 1);

			//*vSlidingVec = SlidingVector;
			//_float Speed =  fabs(CosValue);

			//if(CosValue< 0)
			//	*vSlidingVec = -Line * Speed;
			//else
			//	*vSlidingVec = Line * Speed;

			return false;
		}
	}
	else
		return true;
}

#ifdef _DEBUG
HRESULT CNavigation::Render(CTransform* pTransform)
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &GetSingle(CPipeLineMgr)->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &GetSingle(CPipeLineMgr)->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4));

	_uint		iIndex = 0;

	//if (-1 == m_NaviDesc.iCurrentIndex)
	//{
		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
			{
				pCell->Render(m_pVIBuffer, m_pShader, iIndex++ == m_NaviDesc.iCurrentIndex ? _float4(1.f, 0.f, 0.f, 1.f) : _float4(1.f, 1.f, 1.f, 1.f));
			}
		}
	//}
	/*else
	{
		m_Cells[m_NaviDesc.iCurrentIndex]->Render(m_pVIBuffer, m_pShader, _float4(1.f, 0.f, 0.f, 1.f));
	}*/

	return S_OK;
}
#endif // _DEBUG

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_AB, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_BC, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_CA, pDestCell->Get_Index());
			}
		}
	}

	return S_OK;
}

_Vector CNavigation::Get_NaviPosition(_fVector vTargetPos)
{
	_Vector		vPosition = vTargetPos;

	_float	fY = 0.f;
	_Vector	vPlane;

	vPlane = XMPlaneFromPoints(m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_A), m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_B), m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_C));

	fY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition)
		- XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition)
		- XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

	vPosition = XMVectorSetY(vPosition, fY);

	return vPosition;
}

_float CNavigation::Get_NaviHeight(_fVector vTargetPos)
{
	_Vector		vPosition = vTargetPos;

	_float	fY = 0.f;
	_Vector	vPlane;

	vPlane = XMPlaneFromPoints(m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_A), m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_B), m_Cells[m_NaviDesc.iCurrentIndex]->Get_Point(CCell::POINT_C));

	fY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition)
		- XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition)
		- XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

	return fY;
}

HRESULT CNavigation::FindCellIndex(_Vector Pos)
{
	for (auto& Cell : m_Cells)
	{
		if (nullptr != Cell)
		{
			_int	NeighborIndex = -1;
			_int	Line[3] = { 1, 1, 1 };
			if (Cell->isIn(Pos, &NeighborIndex, Line))
			{
				_uint Index = Cell->Get_Index();
				m_NaviDesc.iCurrentIndex = Index;

				return S_OK;
			}
		}
	}
	return S_OK;
}


CCell::CELL_OPTION CNavigation::Get_CurCellOption()
{
		return m_Cells[m_NaviDesc.iCurrentIndex]->Get_CellOption();	
}

_uint CNavigation::Get_CurNavCellIndex()
{
	return m_NaviDesc.iCurrentIndex;
}

_Vector CNavigation::Get_IndexPosition(_uint iIndex)
{
	return (m_Cells[iIndex]->Get_Point(CCell::POINT_A) + m_Cells[iIndex]->Get_Point(CCell::POINT_B) + m_Cells[iIndex]->Get_Point(CCell::POINT_C)) / 3;

	//return XMPlaneFromPoints(m_Cells[iIndex]->Get_Point(CCell::POINT_A), m_Cells[iIndex]->Get_Point(CCell::POINT_B), m_Cells[iIndex]->Get_Point(CCell::POINT_C));
}

void CNavigation::Set_CurNavCellIndex(_uint iIndex)
{
	m_NaviDesc.iCurrentIndex = iIndex;
}

CCell::CELL_OPTION CNavigation::Get_CurCellOption()
{
	return m_Cells[m_NaviDesc.iCurrentIndex]->Get_CellOption();
}

_uint CNavigation::Get_CurNavCellIndex()
{
	return m_NaviDesc.iCurrentIndex;
}

void CNavigation::Set_CurNavCellIndex(_uint iIndex)
{
	m_NaviDesc.iCurrentIndex = iIndex;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pNaviDataFilePath)
{
	CNavigation*	pInstance = new CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pNaviDataFilePath)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
#endif // _DEBUG
}

