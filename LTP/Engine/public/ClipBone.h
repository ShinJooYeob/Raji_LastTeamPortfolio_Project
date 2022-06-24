#pragma once



#include "Base.h"


//특정 애니메이션 클립에서 사용되는 뼈
BEGIN(Engine)

class CHierarchyNode;

class CClipBone final : public CBase
{
private:
	CClipBone();
	virtual ~CClipBone() = default;

public:
	void Reserve(_uint iNumKeyFrames) { m_vecKeyFrames.reserve(iNumKeyFrames);	m_iNumKeyFrames = iNumKeyFrames; };
	void Add_KeyFrame(KEYFRAME* pKeyFrame) { m_vecKeyFrames.push_back(pKeyFrame); };


	const vector<KEYFRAME*>*	Get_KeyFrameContainor() { return &m_vecKeyFrames; };
	//_uint Get_CurrentKeyFrame() const {	return m_iCurrentKeyFrame;}
	//void Set_CurrentKeyFrame(_uint iKeyFrameIndex);

	void Set_TransformationMatrix_ToHierarchyNode(_fMatrix& TransformationMatrix, const vector<CHierarchyNode*>* pVecHierarchyNodes);

public:
	HRESULT Initialize_ClipBone(const char* pClipBoneName, _int HierarchyNodeIndex);


private:
	string						m_szClipBoneName = "";



	_uint						m_iNumKeyFrames = 0;
	vector<KEYFRAME*>			m_vecKeyFrames;
	typedef vector<KEYFRAME*>	KEYFRAME;

	_int					m_iHierarchyNodeIndex = -1;


public:
	static CClipBone* Create(const char* pClipBoneName, _int HierarchyNodeIndex);
	virtual void Free() override;
};


END