#pragma once

#include "Collider_PhysX_Base.h"

BEGIN(Engine)
class CHierarchyNode;
END
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
	const PHYSXDESC_JOINT& Get_PhysXDescHAIR() const { return mPhysXDesc; }


public:
	PxRigidDynamic* CreateChain(vector<PxRigidDynamic*>& listPxRig, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);
	PxRigidDynamic* CreateChain_BaseActor(PxRigidActor* baseActor, vector<PxRigidDynamic*>& listPxRig, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);
	PxRigidDynamic* CreateChain(const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);

	PxRigidDynamic* Create_DemoChain(PxRigidDynamic* actor, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);
	PxRigidDynamic* Create_DemoChain2(PxRigidDynamic* actor, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);
	PxRigidDynamic* Create_DemoChain_Test(PxRigidDynamic* actor, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);
	PxRigidDynamic* Create_MatchForBonePos(PxRigidDynamic* actor, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);
	
	
//	static PxJoint* createBreakableFixed(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	static PxJoint* CreateD6Joint(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	static PxJoint* CreateMYJoint(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
//	static PxJoint* CreateHairSpherical(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);




public:
#ifdef _DEBUG
	virtual HRESULT Render() override;

#endif // _DEBUG
	HRESULT Set_ColiiderDesc2(PHYSXDESC_JOINT desc);
	HRESULT Set_ColiderDesc_Test(PHYSXDESC_JOINT desc);


protected:
	PHYSXDESC_JOINT				mPhysXDesc;
	CHierarchyNode*				mMainBone = nullptr;
	CModel*						mAttachModel = nullptr;

	vector<CHierarchyNode*>		mVecHier;
	vector<PxRigidDynamic*>		mVecActors;
	vector<PxJoint*>			mVecJoints;

	PxVec3						mOffsetVec;
	ATTACHEDESC					mAttachDesc;

	_uint						mType = 0;

	_float4x4					BlenderMat[9];

//	vector<_float4x4>			mVecBoneTestMat;

	// DEBUG
	

public:
	static CCollider_PhysX_Joint* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

	// CCollider_PhysX_Base을(를) 통해 상속됨
};

END