#pragma once

#include "Base.h"


BEGIN(Engine)

class ENGINE_DLL CHierarchyNode final :public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;



public:
	HRESULT Initialize_HierarchyNode(CHierarchyNode* pParent, const char* pName, _float4x4* TransformationMatrix, _uint iDepth);
	HRESULT Initialize_HierarchyNode(BONEDESC* desc);


public:/*GetSet*/
	_uint			Get_Depth() const { return m_iDepth; };
	const char*		Get_Name() const { return m_szName.c_str(); };
	void			Set_Parent(CHierarchyNode* node) { m_pParent = node; }
	const char*		Get_AssimpParentName() const
	{
		return m_szParentName.c_str();
	};


	_fMatrix Get_UpdatedMatrix() { return m_matUpdatedTransform.XMatrix(); };
	_fMatrix Get_CombinedMatrix() { return m_matCombinedTransformation.XMatrix(); };
	_float4x4* Get_UpdatedTransformMat() { return &m_matUpdatedTransform; };
	void Set_OffsetMatrix(_float4x4* matOffSet) { XMStoreFloat4x4(&m_matOffset, XMMatrixTranspose(XMLoadFloat4x4(matOffSet))); };
	void Set_Transformation(_fMatrix& matTransformation) { m_matTransformation = matTransformation; };
	void Set_UpdateTransform(_fMatrix& matTransformation) { m_matUpdatedTransform = matTransformation; };
	_float4x4 Get_Transformation() { return m_matTransformation; }
	void Update_CombinedMatrix();

	const char*		Get_ParentName() const { if (m_pParent == nullptr)return nullptr; return m_pParent->Get_Name(); };
	_float4x4	Get_OffsetMatrix() { return m_matOffset.TransposeXMatrix(); }

private:
	CHierarchyNode*				m_pParent = nullptr;
	string						m_szName = "";
	string						m_szParentName = ""; // For NoAssimp
	_float4x4					m_matOffset;
	_float4x4					m_matTransformation;
	_float4x4					m_matCombinedTransformation;
	_float4x4					m_matUpdatedTransform;


	_uint						m_iDepth;

public:
	static CHierarchyNode* Create(CHierarchyNode* pParent, const char* pName, _float4x4* TransformationMatrix, _uint iDepth);
	static CHierarchyNode* Create(BONEDESC* desc);

	virtual void Free() override;
};

END