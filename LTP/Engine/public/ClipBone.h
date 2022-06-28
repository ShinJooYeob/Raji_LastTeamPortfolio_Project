#pragma once



#include "Base.h"


//특정 애니메이션 클립에서 사용되는 뼈
BEGIN(Engine)

class CHierarchyNode;

class ENGINE_DLL CClipBone final : public CBase
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

	const char* Get_Name() { return m_szClipBoneName.c_str(); }
	_uint Get_Frames() { return m_iNumKeyFrames; };
	_int Get_HierNodeIndex() { return m_iHierarchyNodeIndex; }



public:
	HRESULT Initialize_ClipBone(const char* pClipBoneName, _int HierarchyNodeIndex);
	HRESULT Initialize_ClipBone(ANIBONES* bone);


private:
	string						m_szClipBoneName = "";



	_uint						m_iNumKeyFrames = 0;
	vector<KEYFRAME*>			m_vecKeyFrames;
	typedef vector<KEYFRAME*>	KEYFRAMES;

	_int					m_iHierarchyNodeIndex = -1;


public:
	static CClipBone* Create(const char* pClipBoneName, _int HierarchyNodeIndex);
	static CClipBone* Create(ANIBONES* bone);

	virtual void Free() override;
};


END