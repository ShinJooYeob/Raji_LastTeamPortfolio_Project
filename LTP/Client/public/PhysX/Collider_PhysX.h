#pragma once

#include "Component.h"

BEGIN(Client)

class CCollider_PhysX : public CComponent
{
public:

private:
	explicit CCollider_PhysX(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCollider_PhysX(const CCollider_PhysX& rhs);
	virtual ~CCollider_PhysX() = default;

public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

	HRESULT Update_Trans2Px();
	HRESULT Update_Px2Trans();
	HRESULT Set_PxScale(PxVec3 pxvec);


public:
 	HRESULT CreateDynamicActor();
	HRESULT CreateStaticActor();
	HRESULT Add_Shape(PxGeometry& gemo);



	// 충돌 체크 자식 충돌 체크 랜더링

	//_bool Inspect_Collision(CCollider* pTargetCollider, _uint iBufferIndex = 0, _uint iTargetIndex = 0, _uint2* pOutIndex = nullptr);

	//HRESULT Set_ParantBuffer(_uint iParantIndex = 0, _int iIndex = -1);

	//_uint Get_NumColliderBuffer() { return _uint(m_vecColliderBuffer.size()); };
	//_int Add_ColliderBuffer(COLLIDERTYPE eColliderType, COLLIDERDESC* pColliderDesc = nullptr);
	//void Set_Conflicted(_double TotalTime = 0.5);
	//_bool Get_Conflicted() { return m_bIsConflicted; }

	//_float3 Get_ColliderPosition(_uint iIndex = 0);

	////Edit
	//BoundingSphere*				Get_Sphere_Transform();
	//CColliderBuffer*			Get_Edit_ColliderBuffer();

public:
#ifdef _DEBUG
	virtual HRESULT Render() override;
	HRESULT CCollider_PhysX::RenderShape(const PxGeometryHolder & h, const PxMat44& world,const PxVec4& color = PxVec4(0,0,0,0));

#endif // _DEBUG

//private:
//	vector<CColliderBuffer*>    m_vecColliderBuffer;
//	vector<CColliderBuffer*>	m_vecPrototypeCollideBuffer;
//

private:
	PxRigidActor*				mRigActor = nullptr;
//	CTransform*					mTransform;
	PxTransform					mPxTransform;

private:
	_bool						m_bIsConflicted = false;
	_double						m_ConflictedPassedTime = 0;

private:
//	_bool Inspect_ChildBuffer(_uint iBufferIndex, CCollider* pTargetCollider, _uint iTargetIndex, _uint2* pOutIndex);

#ifdef _DEBUG
private:
	BasicEffect*									m_pBasicEffect = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = nullptr;
#endif // _DEBUG





public:
	static CCollider_PhysX* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;
};

END