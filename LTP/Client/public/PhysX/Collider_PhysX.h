#pragma once

#include "Component.h"

BEGIN(Client)



class CCollider_PhysX : public CComponent
{
public:
	enum E_PHYSXTYPE
	{
		E_PHYSXTYPE_COLLIDER,
		E_PHYSXTYPE_JOINT,
		E_PHYSXTYPE_END,

	};

private:
	explicit CCollider_PhysX(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCollider_PhysX(const CCollider_PhysX& rhs);
	virtual ~CCollider_PhysX() = default;

public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

	HRESULT Update_BeforeSimulation(CTransform* objTransform);
	HRESULT Update_AfterSimulation(CTransform* objTransform);
	HRESULT Set_PhysXScale(_float3 pxvec);


public:
 	HRESULT CreateDynamicActor(PxVec3 scale = PxVec3(1,1,1));
	HRESULT CreateStaticActor(PxVec3 scale = PxVec3(1, 1, 1));
	HRESULT CreateChain(const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);


	HRESULT Add_Shape(PxGeometry& gemo, PxTransform trans = PxTransform());


	PxRigidActor*	Get_ColliderActor() const {return mRigActor; }
	void			Set_Postiotn(_float3 positiotn);



public:
	// CreateLimitedSpherical
	static PxJoint* CreateLimitedSpherical(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	static PxJoint* CreateBreakableFixed(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	static PxJoint* CreateDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);


public:
#ifdef _DEBUG
	virtual HRESULT Render() override;
	HRESULT CCollider_PhysX::RenderShape(const PxGeometryHolder & h, const PxMat44& world, XMVECTORF32 color = DirectX::Colors::White);

#endif // _DEBUG

private:
	PxRigidActor*				mRigActor = nullptr;
	PxTransform					mPxTransform;
	PxVec3						mActorScale = PxVec3(1,1,1);
	E_PHYSXTYPE					mePhysxType = E_PHYSXTYPE_END;

//private:
//	_bool						m_bIsConflicted = false;
//	_double						m_ConflictedPassedTime = 0;


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