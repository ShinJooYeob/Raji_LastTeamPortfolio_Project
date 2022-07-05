#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Joint.h"


CCollider_PhysX_Joint::CCollider_PhysX_Joint(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCollider_PhysX_Base(pDevice, pDeviceContext)
{
	mePhysX_ID = E_PHYTYPE_JOINT;

}

CCollider_PhysX_Joint::CCollider_PhysX_Joint(const CCollider_PhysX_Joint & rhs)
	: CCollider_PhysX_Base(rhs)
{

}


HRESULT CCollider_PhysX_Joint::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	return S_OK;
}


HRESULT CCollider_PhysX_Joint::Update_BeforeSimulation()
{
	
	if (mbPhysXUpdate == false)
		return S_OK;

	if (mMainTransform == nullptr)
		return E_FAIL;

	mPxMainTransform = PxTransform(*(PxVec3*)&mMainTransform->Get_MatrixState_Float3(CTransform::STATE_POS));


	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Update_AfterSimulation()
{
	
	//	mPxMainTransform = mAttachDesc.Get_AttachObjectTransform();

	//mPxMainTransform = mMainTransform;

	//mPxTransform = PxTransform(*(PxVec3*)&Trans->Get_MatrixState_Float3(CTransform::STATE_POS));


	//_float4x4 mat = mAttachDesc.Caculate_AttachedBoneMatrix();
	//mPxTransform = PxTransform(MAT4X4TOPXMAT(mat));





	// Pos
	mMain_Actor->setGlobalPose(mPxMainTransform);
//	_float3 vec3 = *(_float3*)&mPxMainTransform.p;
//	mMainTransform->Set_MatrixState(CTransform::STATE_POS, vec3);


	return S_OK;
}


#ifdef _DEBUG

HRESULT CCollider_PhysX_Joint::Render()
{
	FAILED_CHECK(__super::Render());



	return S_OK;
}

#endif


HRESULT CCollider_PhysX_Joint::Set_ColiiderDesc(PHYSXDESC_JOINT desc)
{
	// 충돌 모델 초기화

	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC_JOINT));
	if (desc.mTargetObject == nullptr)
		return E_FAIL;

//	mAttachDesc.Initialize_AttachedDesc(desc.mTargetObject, desc.mBoneName.c_str(), _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));

	//// 위 정보로 콜라이더 초기화

	//// 위치 / 스케일 / 엑터 타입 / 모양을 지정헤서 콜라이더 컴포넌트 생성
	if (mMain_Actor)
		return E_FAIL;

	PxGeometry* gemo = nullptr;
	mMainTransform = (CTransform*)desc.mTargetObject->Get_Component(TAG_COM(Com_Transform)); 
	 //Get_AttachObjectTransform();

	_float3 scale = mPhysXDesc.mScale;
	_float3 halfscale = _float3(scale.x*0.5f, scale.y*0.5f, scale.z*0.5f);
	_float3 pos = mMainTransform->Get_MatrixState(CTransform::STATE_POS);

	switch (mPhysXDesc.eShapeType)
	{
	case Client::E_GEOMAT_BOX:
		gemo = NEW PxBoxGeometry(FLOAT3TOPXVEC3(halfscale));
		break;
	case Client::E_GEOMAT_SPEHE:
		gemo = NEW PxSphereGeometry(PxReal(halfscale.x));
		break;
	case Client::E_GEOMAT_CAPSULE:
		gemo = NEW PxCapsuleGeometry(PxReal(halfscale.x), PxReal(halfscale.y));
		break;
	case Client::E_GEOMAT_SHAPE:

		break;
	case Client::E_GEOMAT_VERTEX:
		break;
	case Client::E_GEOMAT_TRIANGLE:
		break;
	case Client::E_GEOMAT_END:
		break;

	default:
		break;
	}
	NULL_CHECK_BREAK(gemo);

	mPxMainTransform = PxTransform(FLOAT3TOPXVEC3(mMainTransform->Get_MatrixState(CTransform::STATE_POS)));
	mMain_Actor = GetSingle(CPhysXMgr)->CreateChain(mPxMainTransform, mPhysXDesc.mLength, *gemo, mPhysXDesc.mSeparation, CreateLimitedSpherical);
	NULL_CHECK_BREAK(mMain_Actor);
	Safe_Delete(gemo);
	return S_OK;
}


PxJoint * CCollider_PhysX_Joint::CreateLimitedSpherical(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	PxSphericalJoint* j = PxSphericalJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	j->setLimitCone(PxJointLimitCone(PxPi / 4, PxPi / 4, 0.1f));
	j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
	return j;
}

PxJoint * CCollider_PhysX_Joint::CreateBreakableFixed(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	PxFixedJoint* j = PxFixedJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	j->setBreakForce(1000, 100000);
	j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
	j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);
	return j;
}

PxJoint * CCollider_PhysX_Joint::CreateDampedD6(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	PxD6Joint* j = PxD6JointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
	j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(0, 1000, FLT_MAX, true));
	return j;
}

HRESULT CCollider_PhysX_Base::CreateChain(ATTACHEDESC attach, PxU32 length, const PxGeometry & g, PxReal separation, JointCreateFunction createJoint)
{
	if (mMain_Actor)
		return S_FALSE;

	ATTACHEDESC mAttachDesc;
	mAttachDesc = attach;
	PxTransform t = PxTransform(FLOAT3TOPXVEC3(mAttachDesc.Get_AttachObjectTransform()->Get_MatrixState(CTransform::STATE_POS)));
	mMain_Actor = GetSingle(CPhysXMgr)->CreateChain(t, length, g, separation, createJoint);
	return S_OK;
}




CCollider_PhysX_Joint * CCollider_PhysX_Joint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CCollider_PhysX_Joint* pInstance = new CCollider_PhysX_Joint(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_PhysX_Joint::Clone(void * pArg)
{
	CCollider_PhysX_Joint* pInstance = new CCollider_PhysX_Joint((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_PhysX_Joint::Free()
{
	__super::Free();

}