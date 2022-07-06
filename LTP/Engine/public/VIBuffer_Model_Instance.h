#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class CMeshContainer;
class CTransform;

#define MAXMATERAILCOUNT 64

class CVIBuffer_Model_Instance final : public CVIBuffer
{
private:
	typedef struct tagIndexBufferDesc
	{
		ID3D11Buffer*				pIB = nullptr;
		D3D11_BUFFER_DESC			IBDesc;
		DXGI_FORMAT					eIndexFormat;
		D3D11_PRIMITIVE_TOPOLOGY	eTopology;
		_uint						iNumPrimitive;
		D3D11_SUBRESOURCE_DATA		IBSubResourceData;

		_uint						iIndexCountPerInstance;
	}INDEXBUFFERDESC;


public:
	CVIBuffer_Model_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Model_Instance(const CVIBuffer_Model_Instance& rhs);
	virtual ~CVIBuffer_Model_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual HRESULT Render(class CShader* pShader, _uint iPassIndex, _uint iMatreialIndex ,_uint iMeshContainerIndex 
		,CMeshContainer* pMeshContainer, vector<CTransform*>* pvecWorldMatrixs, _float fFrustumsize);


private:
	/* 인스턴스하나당 표현되어야할 행렬을 정점으로서 인스턴스의 갯수만큼 가지고 있는 정점 버퍼 . */
	ID3D11Buffer*				m_pVBInstance;
	D3D11_BUFFER_DESC			m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA		m_VBInstSubResource;
	_uint						m_iNumInstance = 0;

	vector<INDEXBUFFERDESC>		m_vecIndexBufferArr[MAXMATERAILCOUNT];

public:
	static CVIBuffer_Model_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END