#include "..\public\ClipBone.h"
#include "HierarchyNode.h"



CClipBone::CClipBone()
{
}

//void CClipBone::Set_CurrentKeyFrame(_uint iKeyFrameIndex)
//{
//	if (iKeyFrameIndex >= m_vecKeyFrames.size()) __debugbreak();
//
//	m_iCurrentKeyFrame = iKeyFrameIndex;
//
//}

void CClipBone::Set_TransformationMatrix_ToHierarchyNode(_fMatrix& TransformationMatrix, const vector<CHierarchyNode*>* pVecHierarchyNodes)
{
	if (m_iHierarchyNodeIndex == -1)
	{
		__debugbreak();
		return;
	}

	(*pVecHierarchyNodes)[m_iHierarchyNodeIndex]->Set_Transformation(TransformationMatrix);

}

HRESULT CClipBone::Initialize_ClipBone(const char * pClipBoneName, _int HierarchyNodeIndex)
{
	m_szClipBoneName = pClipBoneName;
	m_iHierarchyNodeIndex = HierarchyNodeIndex;


	return S_OK;
}

CClipBone * CClipBone::Create(const char * pClipBoneName, _int HierarchyNodeIndex)
{
	CClipBone*	pInstance = new CClipBone();

	if (FAILED(pInstance->Initialize_ClipBone(pClipBoneName, HierarchyNodeIndex)))
	{
		MSGBOX("Failed to Created CClipBone");
		Safe_Release(pInstance);
	}
	return pInstance;;
}

void CClipBone::Free()
{
	for (auto& pKeyFrame : m_vecKeyFrames)
		Safe_Delete(pKeyFrame);

	m_vecKeyFrames.clear();

}

