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

	mOffsetVec = PxVec3(0, 2, 0);

	return S_OK;
}


HRESULT CCollider_PhysX_Joint::Update_BeforeSimulation()
{
	FAILED_CHECK(__super::Update_BeforeSimulation());


	//if (mbUpdate_PhysX == false)
	//	return S_OK;
	//
	//if (mMainTransform == nullptr)
	//	return E_FAIL;

	//if (bTestflag == 0)
	//{
	////	mPxMainTransform = PxTransform(FLOAT3TOPXVEC3(mMainTransform->Get_MatrixState_Float3(CTransform::STATE_POS)));
	////	mPxMainTransform.p += mOffsetVec;
	////	mMain_Actor->setGlobalPose(mPxMainTransform);
	//}
	//else if (bTestflag == 1)
	//{
	//	mPxMainTransform = PxTransform(FLOAT3TOPXVEC3(mMainTransform->Get_MatrixState_Float3(CTransform::STATE_POS)));
	//	mMain_Actor->setGlobalPose(mPxMainTransform);
	//}
	//else if (bTestflag == 2)
	//{
	//	if (mbKeyUpdate)
	//	{
	//		mPxMainTransform = PxTransform(FLOAT3TOPXVEC3(mMainTransform->Get_MatrixState_Float3(CTransform::STATE_POS)));
	//		mMain_Actor->setGlobalPose(mPxMainTransform);
	//	}
	//	else
	//	{
	//		mMain_Actor->setGlobalPose(mPxMainTransform);

	//	//	mPxMainTransform = PxTransform(FLOAT3TOPXVEC3(mMainTransform->Get_MatrixState_Float3(CTransform::STATE_POS)));

	//	}


	//}

	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Update_AfterSimulation()
{
	FAILED_CHECK(__super::Update_AfterSimulation());


	// #TEST Bone Test Code

	//CHierarchyNode* hier1 = mVecHier[0];
	//CHierarchyNode* hier2 = mVecHier[1];
	//CHierarchyNode* hier3 = mVecHier[2];
	//_float4x4 ffMat = _Sfloat4x4::Identity;

	//memcpy((_float3*)(&ffMat.m[0][0]), &DEBUGVALUE1, sizeof(_float3));
	//memcpy((_float3*)(&ffMat.m[1][0]), &DEBUGVALUE2, sizeof(_float3));
	//memcpy((_float3*)(&ffMat.m[2][0]), &DEBUGVALUE3, sizeof(_float3));
	//memcpy((_float3*)(&ffMat.m[3][0]), &DEBUGVALUE4, sizeof(_float3));

	//
	//if (bTestflag == 0)
	//{
	////	PxMat44 mat = PxMat44(mMain_Actor->getGlobalPose());
	////	_Sfloat4x4 getPos = PXMATTOMAT4x4(mat);
	////	mMainBone->Set_UpdateTransform(getPos);
	//}
	//else if(bTestflag == 1)
	//{
	//	mPxMainTransform = mMain_Actor->getGlobalPose();
	//	_float3 vec3 = PXVEC3TOFLOAT3(mPxMainTransform.p);
	//	mMainTransform->Set_MatrixState(CTransform::STATE_POS, vec3);
	//
	//}
	//else if (bTestflag == 2)
	//{
	//
	//	if (mbKeyUpdate)
	//	{
	//		mMain_Actor->setGlobalPose(mPxMainTransform);
	//
	//		//mPxMainTransform = PxTransform(FLOAT3TOPXVEC3(mMainTransform->Get_MatrixState_Float3(CTransform::STATE_POS)));
	//		//mMain_Actor->setGlobalPose(mPxMainTransform);
	//		//_float3 vec3 = PXVEC3TOFLOAT3(mPxMainTransform.p);
	//	//	mMainTransform->Set_MatrixState(CTransform::STATE_POS, vec3);
	//	}
	//	else
	//	{
	//	//	mPxMainTransform = mMain_Actor->getGlobalPose();
	//	}
	//
	//	// _float3 vec3 = PXVEC3TOFLOAT3(mPxMainTransform.p);
	//	// mMainTransform->Set_MatrixState(CTransform::STATE_POS, vec3);
	//
	//}
	//int debig = 5;

	//_uint size = mVecActors.size();
	//for (int i =0; i<size;++i)
	//{
	//	PxMat44 mat = PxMat44(mVecActors[i]->getGlobalPose());
	//	_Sfloat4x4 getPos = PXMATTOMAT4x4(mat);
	//	mVecHier[i]->Set_UpdateTransform(getPos);
	//}

	//	_float4x4 DMat1 = _Sfloat4x4::CreateTranslation(DEBUGVALUE1);
//	_float4x4 DMat2 = _Sfloat4x4::CreateTranslation(DEBUGVALUE2);
//	_float4x4 DMat3 = _Sfloat4x4::CreateTranslation(DEBUGVALUE3);


	//	hier2->Set_UpdateTransform(XMLoadFloat4x4(&DMat2));
	//	hier3->Set_UpdateTransform(XMLoadFloat4x4(&DMat3));

	//	mPxMainTransform = mAttachDesc.Get_AttachObjectTransform();

	//mPxMainTransform = mMainTransform;

	//mPxTransform = PxTransform(*(PxVec3*)&Trans->Get_MatrixState_Float3(CTransform::STATE_POS));

	//_float4x4 mat = mAttachDesc.Caculate_AttachedBoneMatrix();
	//mPxTransform = PxTransform(MAT4X4TOPXMAT(mat));

	// Pos
	// #BUG 업데이트해도 같은 값이 나옴
	//PxVec3 pxpos = mMain_Actor->getGlobalPose().p;
	//
	//mPxMainTransform = mMain_Actor->getGlobalPose();
	//_float3 vec3 = *(_float3*)&mPxMainTransform.p;
	//mMainTransform->Set_MatrixState(CTransform::STATE_POS, vec3);

	// 애니메이션은 로테이션 영향을 받음
//	_float4x4 DMat = _Sfloat4x4::CreateRotationX(DEBUGVALUE1.x);

	//	_Sfloat4x4 DMat = _Sfloat4x4::CreateTranslation(DEBUGVALUE1);
	//	_Sfloat4x4 DMat = _Sfloat4x4::CreateTranslation(DEBUGVALUE1);
	//

	//	_float3 pos = PXVEC3TOFLOAT3(pxpos);
	//	_Sfloat4x4 matpos = _Sfloat4x4::CreateTranslation(pos);

	//	PxMat44 mat = PxMat44(mMain_Actor->getGlobalPose());
	//	_Sfloat4x4 getPos = PXMATTOMAT4x4(mat);


//	_uint size = mVecActors.size();
//	for (int i =0; i<size;++i)
//	{
//	//	mMain_Actor->setGlobalPose(mPxMainTransform);
//		// PxMat44(PxMat33(t.q), t.p); // 회전 적용하는 법
//		/*mVecHier[i]->set*/
//		PxMat44 mat = PxMat44(mVecActors[i]->getGlobalPose());
//		_Sfloat4x4 getPos = PXMATTOMAT4x4(mat);
//		mVecHier[i]->Set_UpdateTransform(getPos);
//	}



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

	bTestflag = 0;

//	mAttachDesc.Initialize_AttachedDesc(desc.mTargetObject, desc.mBoneName.c_str(), _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC_JOINT));

	if (desc.mAttachModel == nullptr || desc.mLength == 0 || desc.mBoneNames == nullptr || desc.mGameObject == nullptr)
		return E_FAIL;

	// 위 정보로 콜라이더 초기화
	// 위치 / 스케일 / 엑터 타입 / 모양을 지정헤서 콜라이더 컴포넌트 생성
	if (mMain_Actor)
		return E_FAIL;

	PxGeometry* gemo = nullptr;


	_float3 scale = mPhysXDesc.mScale;
	_float3 halfscale = _float3(scale.x*0.5f, scale.y*0.5f, scale.z*0.5f);

	for (int i = 0; i < int(desc.mLength); ++i)
	{
		CHierarchyNode* findBone = desc.mAttachModel->Find_HierarchyNode(desc.mBoneNames[i].c_str());
		NULL_CHECK_BREAK(findBone);
		mVecHier.push_back(findBone);
	}
	

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
	mMainTransform = (CTransform*)desc.mGameObject->Get_Component(TAG_COM(Com_Transform));

	mMainBone = mVecHier.front();
	_float4x4 bonemat;
	XMStoreFloat4x4(&bonemat, mMainBone->Get_UpdatedMatrix());
	mPxMainMatrix4x4 = MAT4X4TOPXMAT(bonemat);
	mMain_Actor = CreateChain(mVecActors, PxTransform(mPxMainMatrix4x4), mPhysXDesc.mLength, *gemo, mPhysXDesc.mSeparation, CreateNomalJoint);
	Safe_Delete(gemo);

	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Set_ColiiderDesc(PHYSXDESC_JOINT_TEST desc)
{
	bTestflag = 2;
	// 관절 테스트
	if (mMain_Actor)
		return E_FAIL;

	PxGeometry* gemo = nullptr;
	_float3 scale = desc.mScale;
	_float3 halfscale = _float3(scale.x*0.5f, scale.y*0.5f, scale.z*0.5f);
//	gemo = NEW PxBoxGeometry(FLOAT3TOPXVEC3(halfscale));
	gemo = NEW PxSphereGeometry(PxReal(halfscale.x));
	NULL_CHECK_BREAK(gemo);

	mMainTransform = desc.mTrnasform;
	mPxMainMatrix4x4 = PxTransform(MAT4X4TOPXMAT(mMainTransform->Get_WorldFloat4x4()));

	mMain_Actor = Create_TestJoint(PxTransform(mPxMainMatrix4x4), desc.mLength, *gemo, desc.mSeparation, CreateNomalJoint);
	Safe_Delete(gemo);
	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Set_NomalJoint(CTransform* trans, _uint length)
{
	// 충돌 모델 초기화

	bTestflag = 1;

	// 위 정보로 콜라이더 초기화
	// 위치 / 스케일 / 엑터 타입 / 모양을 지정헤서 콜라이더 컴포넌트 생성
	if (mMain_Actor)
		return E_FAIL;

	PxGeometry* gemo = nullptr;
	_float3 scale = _float3(2.0f);
	_float3 halfscale = _float3(scale.x*0.5f, scale.y*0.5f, scale.z*0.5f);

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
	mMainTransform = trans;

	mPxMainMatrix4x4 = PxTransform(MAT4X4TOPXMAT(mMainTransform->Get_WorldFloat4x4()));
	mMain_Actor = CreateChain(PxTransform(mPxMainMatrix4x4), 5, *gemo, 2, CreateLimitedSpherical);
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

PxJoint* CCollider_PhysX_Joint::CreateNomalJoint(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
{
	//	PxFixedJoint* j = PxFixedJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);


	PxRevoluteJoint* j = PxRevoluteJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	j->setDriveForceLimit(1.0f);
	j->setDriveGearRatio(0.5f);
	j->setDriveVelocity(0.1f);
	
	j->setConstraintFlag(PxConstraintFlag::eBROKEN, false);
	//j->setConstraintFlag(PxConstraintFlag::ePROJECT_TO_ACTOR0, true);
	//j->setConstraintFlag(PxConstraintFlag::ePROJECT_TO_ACTOR1, true);
	j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);
	j->setConstraintFlag(PxConstraintFlag::eCOLLISION_ENABLED, false);
	j->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, false);
	j->setConstraintFlag(PxConstraintFlag::eIMPROVED_SLERP, false);
	j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);
	j->setConstraintFlag(PxConstraintFlag::eENABLE_EXTENDED_LIMITS, false);
	j->setConstraintFlag(PxConstraintFlag::eGPU_COMPATIBLE, false);


	return j;

	
}


PxRigidDynamic* CCollider_PhysX_Joint::CreateChain(ATTACHEDESC attach, PxU32 length, const PxGeometry & g, PxReal separation, JointCreateFunction createJoint)
{
	//	if (mMain_Actor)
	//		return S_FALSE;
	//
	//	ATTACHEDESC mAttachDesc;
	//	mAttachDesc = attach;
	//	PxTransform t = PxTransform(FLOAT3TOPXVEC3(mAttachDesc.Get_AttachObjectTransform()->Get_MatrixState(CTransform::STATE_POS)));
	//	mMain_Actor = GetSingle(CPhysXMgr)->CreateChain(t, length, g, separation, createJoint);
	return nullptr;
}


PxRigidDynamic* CCollider_PhysX_Joint::CreateChain(
	const PxTransform& t, PxU32 length,
	const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
{
	// 관절 오브젝트 생성

	PxVec3 offset(separation / 2, 0, 0);
	PxTransform localTm(offset);
	PxRigidDynamic* prev = NULL;
	PxRigidDynamic* first = NULL;

	// N개의 관절 연결
	// 엑터에 
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*mPhysics, t*localTm, g, *mNormalMaterial, 1.0f);
		(*createJoint)(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		mScene->addActor(*current);
		if (prev == nullptr)
			first = current;
		prev = current;
		localTm.p.x += separation;
	}

	return first;

}

PxRigidDynamic* CCollider_PhysX_Joint::Create_TestJoint(const PxTransform& pxTrans, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
{
	// 관절 오브젝트 생성
	mVecJoints.clear();
	PxVec3 offset(separation / 2, 0, 0);
	PxTransform localTm(offset);
	PxRigidDynamic* prev = NULL;
	PxRigidDynamic* first = NULL;

	// N개의 관절 연결
	// 엑터에 
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*mPhysics, pxTrans*localTm, g, *mNormalMaterial, 1.0f);
		PxJoint* j = (*createJoint)(prev, prev ? PxTransform(offset) : pxTrans, current, PxTransform(-offset));
		mVecJoints.push_back(j);

		mScene->addActor(*current);
		if (prev == nullptr)
			first = current;
		prev = current;
		localTm.p.x += separation;
	}

	return first;
}


PxRigidDynamic* CCollider_PhysX_Joint::CreateChain(vector<PxRigidDynamic*>& listPxRig, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
{
	// 관절 오브젝트 생성
	listPxRig.clear();
	PxVec3 offset(separation / 2, 0, 0);
	PxTransform localTm(offset);
	PxRigidDynamic* prev = NULL;

	// N개의 관절 연결
	// 엑터에 
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*GetSingle(CPhysXMgr)->gPhysics, t*localTm, g, *GetSingle(CPhysXMgr)->gMaterial, 1.0f);
		(*createJoint)(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		GetSingle(CPhysXMgr)->Get_PhysicsScene()->addActor(*current);
		listPxRig.push_back(current);
		prev = current;
		localTm.p.x += separation;
	}

	return prev;
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