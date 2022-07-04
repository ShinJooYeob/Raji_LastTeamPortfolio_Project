#include "MotionTrailBuffer.h"
#include "Model.h"
#include "HierarchyNode.h"
#include "MeshContainer.h"
#include "Shader.h"


CMotionTrailBuffer::CMotionTrailBuffer()
{
}



HRESULT CMotionTrailBuffer::Initialize_Prototype(CModel* pOriginalModel)
{
	m_pOriginalModel = pOriginalModel;

	m_DefaultPivotMatrix = m_pOriginalModel->Get_DefaiultPivotMat();

	m_vecMeshContainerArr = m_pOriginalModel->Get_MeshContainerArr();
	m_iNumMeshContainers = _uint(m_vecMeshContainerArr->size());

	m_iNumMaterials = m_pOriginalModel->Get_NumMaterial();
	return S_OK;
}

HRESULT CMotionTrailBuffer::Initialize_Clone(void * pArg)
{


	//FAILED_CHECK(Ready_Copy_OriginModel());


	return S_OK;
}



HRESULT CMotionTrailBuffer::Start_MotionTrail(_float4x4 WorldMatrix, _float4 vColor, _double TargetTime)
{
	m_vColor = vColor;
	m_vColor.w = 1.f;
	m_TargetTime = TargetTime;
	m_PassedTime = 0;


	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix.TransposeXMatrix());

	FAILED_CHECK(Ready_Copy_OriginModel());

	return S_OK;
}

_int CMotionTrailBuffer::Update_MotionBuffer(_double fDeltaTime)
{
	m_PassedTime += fDeltaTime;
	m_vColor.w = _float((m_TargetTime - m_PassedTime) / m_TargetTime);

	return _int();
}

HRESULT CMotionTrailBuffer::Render(CShader * pShader, _uint iPassIndex,  const char* szBoneValueName)
{
	
	FAILED_CHECK(pShader->Set_RawValue("g_fEmissive", &m_fEmisive, sizeof(_float)));
	FAILED_CHECK(pShader->Set_RawValue("g_vLimLight", &m_vColor, sizeof(_float4)));
	FAILED_CHECK(pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4)));
	
	for (_uint i = 0 ; i< m_iNumMaterials; i++)
	{
		NULL_CHECK_RETURN(szBoneValueName, E_FAIL);
		_float4x4		BoneMatrices[150];
		_Matrix matDefualtPivot = m_DefaultPivotMatrix.XMatrix();

		for (auto& pMeshContainer : m_vecMeshContainerArr[i])
		{
			FAILED_CHECK(pMeshContainer->Bind_AffectingBones_OnShader(pShader, matDefualtPivot, BoneMatrices, szBoneValueName, &m_vecUpdatedMatrix, true));
			FAILED_CHECK(pMeshContainer->Render(pShader, iPassIndex));
		}

	}
		


	return S_OK;
}



HRESULT CMotionTrailBuffer::Ready_Copy_OriginModel()
{
	vector<CHierarchyNode*>* OriginObjNodeContainer = m_pOriginalModel->Get_HierarchyContainer();
	_uint iSize = (_uint)OriginObjNodeContainer->size();
	m_vecUpdatedMatrix.clear();
	m_vecUpdatedMatrix.reserve(iSize + 1 );

	_float4x4 Mat;
	for (_uint i = 0 ;i<iSize;i++)
	{
		Mat = (*OriginObjNodeContainer)[i]->Get_UpdatedMatrix();
		m_vecUpdatedMatrix.push_back(Mat);
	}


	return S_OK;	
}

CMotionTrailBuffer * CMotionTrailBuffer::Create(CModel* pOriginalModel)
{
	CMotionTrailBuffer*	pInstance = new CMotionTrailBuffer();

	if (FAILED(pInstance->Initialize_Prototype(pOriginalModel)))
	{
		MSGBOX("Failed to Created CMotionTrailBuffer");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CMotionTrailBuffer::Free()
{

}
