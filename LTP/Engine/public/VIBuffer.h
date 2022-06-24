#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;


public:
	virtual HRESULT Render(class CShader* pShader, _uint iPassIndex);

protected:
	ID3D11Buffer*				m_pVB = nullptr;
	D3D11_BUFFER_DESC			m_VBDesc;
	D3D11_SUBRESOURCE_DATA		m_VBSubResourceData;
	//정점의 개수
	_uint						m_iNumVertices = 0;
	//버텍스 버퍼가 몇개인가(한번에 여러개의 버퍼를 바인드하여 그릴수있다)
	_uint						m_iNumVertexBuffers = 0;


	_float3*					m_pVertices = nullptr;

protected:
	ID3D11Buffer*				m_pIB = nullptr;
	D3D11_BUFFER_DESC			m_IBDesc;
	D3D11_SUBRESOURCE_DATA		m_IBSubResourceData;

	//인덱스(삼각형)의 총 개수
	_uint						m_iNumPrimitive = 0;
	//한번 그릴때 몇개의 인덱스를 뽑아서 그리는가
	_uint						m_iNumIndicesPerPrimitive = 0;
	//인덱스 형식 (ex : DXGI_FORMAT_R16_UINT)
	DXGI_FORMAT					m_eIndexFormat;
	//그리는 방식 (ex : D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;


protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END