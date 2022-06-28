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

HRESULT CClipBone::Initialize_ClipBone(ANIBONES * bone)
{
	m_szClipBoneName = bone->mBoneName;
	m_iHierarchyNodeIndex = bone->mHierarchyNodeIndex;
	m_iNumKeyFrames = bone->mNumKeyFrames;

	
	// #BUG
	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME*	KeyFrames = NEW KEYFRAME;
		memcpy(KeyFrames, &bone->mKeyFrames[i], sizeof(KEYFRAME));
		Add_KeyFrame(KeyFrames);
	}


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

CClipBone * CClipBone::Create(ANIBONES * bone)
{
	CClipBone*	pInstance = new CClipBone();

	if (FAILED(pInstance->Initialize_ClipBone(bone)))
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

