#pragma once
#include "Base.h"
BEGIN(Engine)

class CColliderBuffer final : public CBase
{
public:
	enum BOUNDING { BOUNDING_ORIGINAL, BOUNDING_TRANSFORM, BOUNDING_END };

private:
	CColliderBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CColliderBuffer(const CColliderBuffer& rhs);
	virtual ~CColliderBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDERTYPE eType);
	virtual HRESULT Initialize_Clone(COLLIDERDESC* pArg);

	void Update_Transform(_fMatrix& TransformMatrix);
	
public:
	_bool Collision_AABB(CColliderBuffer* pTargetColliderBuffer, _bool IsFinialBuffer = false);
	_bool Collision_OBB(CColliderBuffer* pTargetColliderBuffer, _bool IsFinialBuffer = false);
	_bool Collision_Sphere(CColliderBuffer* pTargetColliderBuffer, _bool IsFinialBuffer = false);
	_bool Collision_All(CColliderBuffer* pTargetColliderBuffer,_bool IsFinialBuffer = false);

	void Add_ChildBufferIndex(_uint iIndex);
	void Delete_ChildeBuffer(_uint iIndex);
	
	list<_uint>* Get_ChildIndexList() { return &m_ChildNodeIndexList; };
	_uint Get_NumChildBuffer() { return _uint(m_ChildNodeIndexList.size()); };
	void Set_IsConflicted(_bool bBool) { m_bIsConflicted = bBool; };

	_float3 Get_ColliderPosition();

	//EditBOUNDING_TRANSFORM
	BoundingSphere* Get_PointCollider() { return m_pSphere[BOUNDING_TRANSFORM]; }

#ifdef _DEBUG
#endif // _DEBUG
public:
	virtual _int Render();

private:
	BoundingBox*				m_pAABB[BOUNDING_END] = { nullptr };
	BoundingOrientedBox*		m_pOBB[BOUNDING_END] = { nullptr };
	BoundingSphere*				m_pSphere[BOUNDING_END] = { nullptr };
	COLLIDERTYPE				m_eColliderType = COLLIDERTYPE_END;
	COLLIDERDESC				m_ColliderDesc;

	_bool						m_bIsConflicted = false;

#ifdef _DEBUG
#endif // _DEBUG
private:
	BasicEffect*									m_pBasicEffect = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = nullptr;


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	_bool					m_bIsClone = false;

private:
	_Matrix Remove_Rotation(_fMatrix& TransformMatrix);
	list<_uint>	m_ChildNodeIndexList;



public:
	static CColliderBuffer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, COLLIDERTYPE eType);
	virtual CColliderBuffer* Clone(COLLIDERDESC* pArg);
	virtual void Free() override;
};

END