#include "..\Public\VIBuffer_Model_Instance.h"
#include "Shader.h"
#include "MeshContainer.h"
#include "Transform.h"
#include "Model.h"
#include "FrustumMgr.h"

CVIBuffer_Model_Instance::CVIBuffer_Model_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_Model_Instance::CVIBuffer_Model_Instance(const CVIBuffer_Model_Instance & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_VBInstSubResource(rhs.m_VBInstSubResource)
	, m_iNumInstance(rhs.m_iNumInstance)	
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Model_Instance::Initialize_Prototype(_uint iNumInstance)
{
#pragma region INSTANCE_BUFFER

	m_iNumInstance = iNumInstance;

	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));	

	m_VBInstDesc.ByteWidth = sizeof(VTXINSTMATRIX) * m_iNumInstance;
	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstDesc.StructureByteStride = sizeof(VTXINSTMATRIX);
	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VTXINSTMATRIX*		pInstanceMatrix = new VTXINSTMATRIX[m_iNumInstance];
	ZeroMemory(pInstanceMatrix, sizeof(VTXINSTMATRIX) * m_iNumInstance);


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceMatrix[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstanceMatrix[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstanceMatrix[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstanceMatrix[i].vPosition = _float4(0, 0.f, 0, 1.f);

		pInstanceMatrix[i].vLimLight = _float4(0);
		pInstanceMatrix[i].vEmissive= _float4(0);
	}	

	ZeroMemory(&m_VBInstSubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBInstSubResource.pSysMem = pInstanceMatrix;

	FAILED_CHECK(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResource, &m_pVBInstance));

	Safe_Delete_Array(pInstanceMatrix);

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Model_Instance::Initialize_Clone(void * pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);


	CModel*	pModel = (CModel*)pArg;

	_uint iNumNumMaterial = pModel->Get_NumMaterial();

	if (iNumNumMaterial >= MAXMATERAILCOUNT)
	{
		__debugbreak();
		MSGBOX("Need to Rise Count MAXMATERAILCOUNT Num");
		return E_FAIL;
	}

	vector<CMeshContainer*>* pvecOriginMeshContainerArr = pModel->Get_MeshContainerArr();
	
	for (_uint j = 0; j < iNumNumMaterial; j++)
	{
		m_vecIndexBufferArr[j].reserve(pvecOriginMeshContainerArr[j].size());

		for (_uint i = 0; i < pvecOriginMeshContainerArr[j].size(); i++)
		{
			FORINDEXDATA OriginIndexData;
			(pvecOriginMeshContainerArr[j])[i]->Get_IndexBufferData(&OriginIndexData);

			INDEXBUFFERDESC tDesc;

			tDesc.iIndexCountPerInstance = OriginIndexData.iNumPrimitive * 3;
			tDesc.iNumPrimitive = OriginIndexData.iNumPrimitive * m_iNumInstance;
			tDesc.eIndexFormat = OriginIndexData.eIndexFormat;
			tDesc.eTopology = OriginIndexData.eTopology;

			ZeroMemory(&tDesc.IBDesc, sizeof(D3D11_BUFFER_DESC));

			tDesc.IBDesc.ByteWidth = sizeof(FACEINDICES32) * tDesc.iNumPrimitive;
			tDesc.IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
			tDesc.IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			FACEINDICES32*	pIndices = new FACEINDICES32[tDesc.iNumPrimitive];
			ZeroMemory(pIndices, sizeof(FACEINDICES32) * tDesc.iNumPrimitive);

			for (_uint i = 0; i < m_iNumInstance; i++)
			{
				memcpy((pIndices + (OriginIndexData.iNumPrimitive * i)), OriginIndexData.pIndices, sizeof(FACEINDICES32) * OriginIndexData.iNumPrimitive);
			}


			ZeroMemory(&tDesc.IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			tDesc.IBSubResourceData.pSysMem = pIndices;

			FAILED_CHECK(m_pDevice->CreateBuffer(&tDesc.IBDesc, &tDesc.IBSubResourceData, &tDesc.pIB));

			m_vecIndexBufferArr[j].push_back(tDesc);

			Safe_Delete_Array(pIndices);
		}


	}
	return S_OK;
}

HRESULT CVIBuffer_Model_Instance::Render(CShader* pShader, _uint iPassIndex, _uint iMatreialIndex, _uint iMeshContainerIndex ,
	CMeshContainer* pMeshContainer, vector<CTransform*>* pvecWorldMatrixs ,_float fFrustumsize, vector<_float4>*  pvecLimLight, vector<_float4>*  pvecEmissive)
{
	if (nullptr == m_pDeviceContext)
	{
		__debugbreak();
		return E_FAIL;
	}

	if(pvecLimLight && pvecLimLight->size() != (*pvecWorldMatrixs).size())
	{
		__debugbreak();
		return E_FAIL;
	}

	if (pvecEmissive && pvecEmissive->size() != (*pvecWorldMatrixs).size())
	{
		__debugbreak();
		return E_FAIL;
	}

	_uint iNeedToRenderingCount = ((*pvecWorldMatrixs).size() < m_iNumInstance) ? (_uint)(*pvecWorldMatrixs).size() : m_iNumInstance;
	
	CFrustumMgr*	pFrustum = GetSingle(CFrustumMgr);



	if (fFrustumsize > 0)
	{
		vector<CTransform*> TempVec;
		TempVec.reserve(pvecWorldMatrixs->size());

		_uint MaxCount = iNeedToRenderingCount;
		iNeedToRenderingCount = 0;

		for (_uint i = 0 ; i < MaxCount; i++)
		{
			if (pFrustum->IsNeedToRender((*pvecWorldMatrixs)[i]->Get_MatrixState(CTransform::STATE_POS), fFrustumsize))
			{
				TempVec.push_back((*pvecWorldMatrixs)[i]);
				iNeedToRenderingCount++;
			}
		}
		
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

		for (_uint i = 0; i < iNeedToRenderingCount; ++i)
		{
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vRight), TempVec[i]->Get_MatrixState(CTransform::STATE_RIGHT));
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vUp), TempVec[i]->Get_MatrixState(CTransform::STATE_UP));
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLook), TempVec[i]->Get_MatrixState(CTransform::STATE_LOOK));
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vPosition), TempVec[i]->Get_MatrixState(CTransform::STATE_POS));

			if (pvecLimLight)
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLimLight), (*pvecLimLight)[i].XMVector());
			else
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLimLight), XMVectorSet(0, 0, 0, 0));

			if (pvecEmissive)
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vEmissive), (*pvecEmissive)[i].XMVector());
			else
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vEmissive), XMVectorSet(0, 0, 0, 0));



		}
		m_pDeviceContext->Unmap(m_pVBInstance, 0);
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

		for (_uint i = 0; i < iNeedToRenderingCount; ++i)
		{
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vRight), (*pvecWorldMatrixs)[i]->Get_MatrixState(CTransform::STATE_RIGHT));
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vUp), (*pvecWorldMatrixs)[i]->Get_MatrixState(CTransform::STATE_UP));
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLook), (*pvecWorldMatrixs)[i]->Get_MatrixState(CTransform::STATE_LOOK));
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vPosition), (*pvecWorldMatrixs)[i]->Get_MatrixState(CTransform::STATE_POS));


			if (pvecLimLight)
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLimLight), (*pvecLimLight)[i].XMVector());
			else
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLimLight), XMVectorSet(0, 0, 0, 0));

			if (pvecEmissive)
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vEmissive), (*pvecEmissive)[i].XMVector());
			else
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vEmissive), XMVectorSet(0, 0, 0, 0));
		}
		m_pDeviceContext->Unmap(m_pVBInstance, 0);
	}


	
	FORINSTDATA tInstancingData;
	pMeshContainer->Get_InstancingData(&tInstancingData);

	
	ID3D11Buffer*	pVertexBuffers[] = {
		tInstancingData.pVB,
		m_pVBInstance
	};

	_uint			iStrides[] = {
		tInstancingData.VBDesc.StructureByteStride,
		m_VBInstDesc.StructureByteStride
	};

	_uint			iOffsets[] = {
		0,
		0
	};

	

	m_pDeviceContext->IASetVertexBuffers(0, 2, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer((m_vecIndexBufferArr[iMatreialIndex])[iMeshContainerIndex].pIB, (m_vecIndexBufferArr[iMatreialIndex])[iMeshContainerIndex].eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology((m_vecIndexBufferArr[iMatreialIndex])[iMeshContainerIndex].eTopology);

	FAILED_CHECK(pShader->Set_InputLayout(iPassIndex));
	FAILED_CHECK(pShader->Apply(iPassIndex));

	m_pDeviceContext->DrawIndexedInstanced((m_vecIndexBufferArr[iMatreialIndex])[iMeshContainerIndex].iIndexCountPerInstance, iNeedToRenderingCount, 0, 0, 0);
	//m_pDeviceContext->DrawIndexedInstanced(3 * asdasdasd, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer_Model_Instance::Render_float4x4(CShader * pShader, _uint iPassIndex, _uint iMatreialIndex, _uint iMeshContainerIndex, CMeshContainer * pMeshContainer, 
	vector<_float4x4>* pvecWorldMatrixs, _float fFrustumsize, vector<_float4>*  pvecLimLight, vector<_float4>*  pvecEmissive)
{
	if (nullptr == m_pDeviceContext)
	{
		__debugbreak();
		return E_FAIL;
	}
	if (pvecLimLight && pvecLimLight->size() != (*pvecWorldMatrixs).size())
	{
		__debugbreak();
		return E_FAIL;
	}

	if (pvecEmissive && pvecEmissive->size() != (*pvecWorldMatrixs).size())
	{
		__debugbreak();
		return E_FAIL;
	}

	_uint iNeedToRenderingCount = ((*pvecWorldMatrixs).size() < m_iNumInstance) ? (_uint)(*pvecWorldMatrixs).size() : m_iNumInstance;
	CFrustumMgr*	pFrustum = GetSingle(CFrustumMgr);


	{
		D3D11_MAPPED_SUBRESOURCE		SubResource;

		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

		for (_uint i = 0; i < iNeedToRenderingCount; ++i)
		{
			_Matrix mat = (*pvecWorldMatrixs)[i].XMatrix();

			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vRight),		mat.r[0]);
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vUp),		mat.r[1]);
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLook),		mat.r[2]);
			XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vPosition),	mat.r[3]);


			if (pvecLimLight)
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLimLight), (*pvecLimLight)[i].XMVector());
			else
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vLimLight), XMVectorSet(0,0,0,0));

			if (pvecEmissive)
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vEmissive), (*pvecEmissive)[i].XMVector());
			else
				XMStoreFloat4(&(((VTXINSTMATRIX*)SubResource.pData)[i].vEmissive), XMVectorSet(0, 0, 0, 0));
		}
		m_pDeviceContext->Unmap(m_pVBInstance, 0);
	}



	FORINSTDATA tInstancingData;
	pMeshContainer->Get_InstancingData(&tInstancingData);


	ID3D11Buffer*	pVertexBuffers[] = {
		tInstancingData.pVB,
		m_pVBInstance
	};

	_uint			iStrides[] = {
		tInstancingData.VBDesc.StructureByteStride,
		m_VBInstDesc.StructureByteStride
	};

	_uint			iOffsets[] = {
		0,
		0
	};



	m_pDeviceContext->IASetVertexBuffers(0, 2, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer((m_vecIndexBufferArr[iMatreialIndex])[iMeshContainerIndex].pIB, (m_vecIndexBufferArr[iMatreialIndex])[iMeshContainerIndex].eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology((m_vecIndexBufferArr[iMatreialIndex])[iMeshContainerIndex].eTopology);

	FAILED_CHECK(pShader->Set_InputLayout(iPassIndex));
	FAILED_CHECK(pShader->Apply(iPassIndex));

	m_pDeviceContext->DrawIndexedInstanced((m_vecIndexBufferArr[iMatreialIndex])[iMeshContainerIndex].iIndexCountPerInstance, iNeedToRenderingCount, 0, 0, 0);
	//m_pDeviceContext->DrawIndexedInstanced(3 * asdasdasd, m_iNumInstance, 0, 0, 0);

	return S_OK;
}


CVIBuffer_Model_Instance * CVIBuffer_Model_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumInstance)
{
	CVIBuffer_Model_Instance*	pInstance = new CVIBuffer_Model_Instance(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		MSGBOX("Failed to Created CVIBuffer_Model_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Model_Instance::Clone(void * pArg)
{
	CVIBuffer_Model_Instance*	pInstance = new CVIBuffer_Model_Instance(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CVIBuffer_Model_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Model_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);

	for (_uint i = 0; i < MAXMATERAILCOUNT; i++)
	{
		for (auto& pDesc : m_vecIndexBufferArr[i])
			Safe_Release(pDesc.pIB);
		m_vecIndexBufferArr[i].clear();
	}
	
}
