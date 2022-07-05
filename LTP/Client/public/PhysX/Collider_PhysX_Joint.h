#pragma once

#include "Collider_PhysX_Base.h"

BEGIN(Client)
class CCollider_PhysX_Joint: public CCollider_PhysX_Base
{
private:
	explicit CCollider_PhysX_Joint(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCollider_PhysX_Joint(const CCollider_PhysX_Joint& rhs);
	virtual ~CCollider_PhysX_Joint() = default;

public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

	virtual HRESULT Update_BeforeSimulation()override;
	virtual HRESULT Update_AfterSimulation()override;

public:
	const PHYSXDESC_JOINT& Get_PhysXDesc() const { return mPhysXDesc; }

public:
	HRESULT CreateChain(const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);
	HRESULT CreateChain(ATTACHEDESC attach, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);

	static PxJoint* CreateLimitedSpherical(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	static PxJoint* CreateBreakableFixed(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	static PxJoint* CreateDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);


public:
#ifdef _DEBUG
	virtual HRESULT Render() override;

#endif // _DEBUG
	HRESULT Set_ColiiderDesc(PHYSXDESC_JOINT desc);




protected:
	PHYSXDESC_JOINT mPhysXDesc;

public:
	static CCollider_PhysX_Joint* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

	// CCollider_PhysX_Base을(를) 통해 상속됨
};

END