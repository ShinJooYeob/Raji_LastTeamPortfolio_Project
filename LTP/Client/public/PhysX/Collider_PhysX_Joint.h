#pragma once

#include "Collider_PhysX_Base.h"

BEGIN(Engine)
class CHierarchyNode;
END
BEGIN(Client)
class CCollider_PhysX_Joint: public CCollider_PhysX_Base
{
public:
	enum E_JOINT_TYPE
	{
		JOINT_HAIR,
		JOINT_END,

	};
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
	PxRigidDynamic* Create_HairJoint(PxRigidDynamic* actor, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint);
	
	
	static PxJoint* CreateD6Joint(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	static PxJoint* CreateMYJoint(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);



	void SetBodyCapsuleShape(PxVec3 scale);

public:
#ifdef _DEBUG
	virtual HRESULT Render() override;

#endif // _DEBUG
	HRESULT Set_ColiiderDesc2(PHYSXDESC_JOINT desc);
	HRESULT Set_ColiderDesc_Hair(PHYSXDESC_JOINT desc);

	HRESULT Add_ForceDir(_float3 dir,_float force);

protected:
	PHYSXDESC_JOINT				mPhysXDesc;
	CHierarchyNode*				mMainBone = nullptr;
	CModel*						mAttachModel = nullptr;

	PxRigidActor*				mBodyActor = nullptr;
	vector<CHierarchyNode*>		mVecHier;
	vector<PxRigidDynamic*>		mVecActors;
	vector<PxJoint*>			mVecJoints;

	PxVec3						mOffsetVec;
	ATTACHEDESC					mAttachDesc;
	_float4x4					BlenderMat[9];

	E_JOINT_TYPE				mType = JOINT_END;

	PxTransform					mNew_WorldPxTransform[8];
	_Sfloat3					mOffset[8];
	_float						mOffsetScale[8];

	// DEBUG
	const float					HairLength = 1.0f;


public:
	static CCollider_PhysX_Joint* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

	// CCollider_PhysX_Base을(를) 통해 상속됨
};

END