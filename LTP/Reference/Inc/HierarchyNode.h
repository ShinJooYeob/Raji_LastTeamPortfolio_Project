#pragma once

#include "Base.h"


BEGIN(Engine)

class CHierarchyNode final :public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	HRESULT Initialize_HierarchyNode(CHierarchyNode* pParent, const char* pName, _float4x4* TransformationMatrix, _uint iDepth);

public:/*GetSet*/
	_uint			Get_Depth() const { return m_iDepth; };
	const char*		Get_Name() const { return m_szName.c_str(); };


	_fMatrix Get_UpdatedMatrix() { return m_matUpdatedTransform.XMatrix(); };
	_fMatrix Get_CombinedMatrix() { return m_matCombinedTransformation.XMatrix(); };
	_float4x4* Get_UpdatedTransformMat() { return &m_matUpdatedTransform; };
	void Set_OffsetMatrix(_float4x4* matOffSet) { 	XMStoreFloat4x4(&m_matOffset, XMMatrixTranspose(XMLoadFloat4x4(matOffSet)));};
	void Set_Transformation(_fMatrix& matTransformation) { m_matTransformation = matTransformation; };
	void Update_CombinedMatrix();

	

	_float4x4	Get_OffsetMatrix() { return m_matOffset.TransposeXMatrix(); }

private:
	CHierarchyNode*				m_pParent = nullptr;
	string						m_szName = "";
	_float4x4					m_matOffset;
	_float4x4					m_matTransformation;
	_float4x4					m_matCombinedTransformation;
	_float4x4					m_matUpdatedTransform;


	_uint						m_iDepth;
public:
	static CHierarchyNode* Create(CHierarchyNode* pParent, const char* pName, _float4x4* TransformationMatrix, _uint iDepth);
	virtual void Free() override;
};

END	