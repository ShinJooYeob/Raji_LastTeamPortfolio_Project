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

	if (pArg != nullptr)
		memcpy(&mColDesc, pArg, sizeof(CColider_PX_Desc));



	return S_OK;
}


HRESULT CCollider_PhysX_Joint::Update_BeforeSimulation()
{
	FAILED_CHECK(__super::Update_BeforeSimulation());

	if (mMainTransform == nullptr)
		return E_FAIL;

	if (mType == 0)
	{

		mVecActors[0]->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

		mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());
		PxTransform trans = PxTransform(mPxMainMatrix4x4);
		//PxTransform tt = mVecActors[0]->getGlobalPose();
		trans.p += PxVec3(0, 1, 0);
		mVecActors[0]->setGlobalPose(trans);

		for (_uint i = 1; i < mVecActors.size(); ++i)
		{
			//if(i==0)
			//	continue;;
			PxMat44 mat = PxMat44(mVecActors[i]->getGlobalPose());
			//	_Sfloat4x4 idy = _Sfloat4x4::Identity;
			_fMatrix getPos = (PXMATTOMAT4x4(mat)).XMatrix();// *  mDebugMat.XMatrix();
		//	mVecHier[i]->Set_UpdateTransform(idy);
		}

		_Sfloat4x4 ffMat = _Sfloat4x4::Identity;
		memcpy((_float3*)(&ffMat.m[0][0]), &DEBUGVALUE1, sizeof(_float3));
		memcpy((_float3*)(&ffMat.m[1][0]), &DEBUGVALUE2, sizeof(_float3));
		memcpy((_float3*)(&ffMat.m[2][0]), &DEBUGVALUE3, sizeof(_float3));
		memcpy((_float3*)(&ffMat.m[3][0]), &DEBUGVALUE4, sizeof(_float3));

		// 뼈위치만 업데이트
		for (_uint i = 1; i < mVecActors.size(); ++i)
		{

			_Sfloat4x4 mat = mVecHier[i]->Get_UpdatedMatrix();

			PxVec3 APos = mVecActors[i - 1]->getGlobalPose().p;
			PxVec3 BPos = mVecActors[i]->getGlobalPose().p;

			PxVec3 DIr = APos - BPos;
			DIr = DIr.getNormalized() * ffMat._11;

			_Sfloat4x4 DirMat = _Sfloat4x4::CreateTranslation(PXVEC3TOFLOAT3(DIr));

			mVecHier[i]->Set_UpdateTransform(DirMat* mat);



			// 로컬위치 보내기
			//_float4x4 mat = PXMATTOMAT4x4(px4);
			//mat = (mat.XMatrix() * mMainTransform->Get_InverseWorldMatrix())*
			//	mAttachModel->Get_DefaiultPivotMat().InverseXMatrix()
			//	* XMMatrixScaling(0.01f, 0.01f, 0.01f);


			//mMainTransform;
			//mAttachModel->Get_DefaiultPivotMat();

			//_Sfloat4x4(mMainTransform->Get_WorldFloat4x4());
			//mMatSocket = (*mBoneMatrixPtr.pCombinedMatrix) * mSocketTransformMatrix * mSocketDESC.mTransform->GetWorldMatrix();
			//mVecBoneTestMat.push_back(mMatSocket);
			//mVecHier[i]->Set_UpdateTransform(mMatSocket);

		}
	}



	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Update_AfterSimulation()
{
	FAILED_CHECK(__super::Update_AfterSimulation());
	_float3 offset(-1, 0, 0);

	if (mType == 0)
	{
		//	actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);


		//mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());
		//PxTransform trans = PxTransform(mPxMainMatrix4x4);
		//
		//PxVec3 offset(mPhysXDesc.mSeparation / 2, 0, 0);
		//PxTransform localTm(offset);
		//mVecActors[0]->setGlobalPose(trans);

		//for (_uint i=0; i<mVecActors.size();++i)
		//{
		//	if(i==0)
		//		mVecActors[i]->setGlobalPose(trans);
		//	else
		//		mVecActors[i]->setGlobalPose(trans*localTm);
		//
		//}


	//	mAttachDesc.Set_DefaultBonePivot(_float3 (0), _float3 (90,0,0), _float3 (0.f, 132.0f, -5.80000f ));;
		//mDebugMat = mAttachDesc.Caculate_AttachedBoneMatrix();
	//	mDebugMat = XMMatrixIdentity();
	//	memcpy(&mDebugMat._41, &_float3(mAttachDesc.Get_AttachedBoneWorldPosition()), sizeof(_float3));

		// 뼈에 업데이트
		//_uint size = mVecActors.size();
		//for (int i = 0; i < size; ++i)
		//{
		//	//	mMain_Actor->setGlobalPose(mPxMainTransform);
		//	// PxMat44(PxMat33(t.q), t.p); // 회전 적용하는 법

		//	PxMat44 mat = PxMat44(mVecActors[i]->getGlobalPose());
		//	_fMatrix getPos =(PXMATTOMAT4x4(mat)).XMatrix() *  mDebugMat.XMatrix();
		//	mVecHier[i]->Set_UpdateTransform(getPos);
		//}
		//int Debug = 5;


		// 1 첫번쨰 뼈만 업데이트
	//	CHierarchyNode* hier1  = mVecHier.front();
	//	mMain_Actor->getGlobalPose()

	//	mDebugMat = mMainTransform->Get_WorldMatrix() * mAttachModel->Caculate_AttachedBone(mVecHier.front());
	//	mAttachDesc.Get_AttachedBoneWorldPosition();


		//PxMat44 mat = PxMat44(mVecActors[0]->getGlobalPose());
		//PxMat44 mm = MAT4X4TOPXMAT(mMainTransform->Get_InverseWorldMatrix())*mat;
		//_Sfloat4x4 getPos = PXMATTOMAT4x4(mm);

		//mVecHier[0]->Set_UpdateTransform(getPos);
	}

	// #TEST Bone Test Code

	//CHierarchyNode* hier1 = mVecHier[0];
	//CHierarchyNode* hier2 = mVecHier[1];
	//CHierarchyNode* hier3 = mVecHier[2];
	//_float4x4 ffMat = _Sfloat4x4::Identity;
	//
	//memcpy((_float3*)(&ffMat.m[0][0]), &DEBUGVALUE1, sizeof(_float3));
	//memcpy((_float3*)(&ffMat.m[1][0]), &DEBUGVALUE2, sizeof(_float3));
	//memcpy((_float3*)(&ffMat.m[2][0]), &DEBUGVALUE3, sizeof(_float3));
	//memcpy((_float3*)(&ffMat.m[3][0]), &DEBUGVALUE4, sizeof(_float3));


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

	// 연결된 본 다 그리기

	// 모양에 따라 드로잉
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pBasicEffect->SetWorld(XMMatrixIdentity());



	//	m_pBasicEffect->SetWorld(mDebugMat.XMatrix());
	m_pBasicEffect->SetView(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_VIEW));
	m_pBasicEffect->SetProjection(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_PROJ));
	m_pBasicEffect->Apply(m_pDeviceContext);


	m_pBatch->Begin();

	PxShape* shapes[1];
	XMVECTORF32 color = DirectX::Colors::Blue;

	// 액터 위치
	for (auto actor : mVecActors)
	{
		actor->getShapes(shapes, 1);
		const PxShape& shape = *shapes[0];
		const PxMat44 shpaeWorld(PxShapeExt::getGlobalPose(shape, *actor));
		const PxGeometryHolder h = shape.getGeometry();
		RenderShape(h, shpaeWorld, color);

	}

	// 뼈 위치;
	color = DirectX::Colors::Red;

	for (auto& mat : mVecBoneTestMat)
	{
		RenderDebugSphere(mat, 0.1f, color);
	}
	mVecBoneTestMat.clear();

	m_pBatch->End();


	return S_OK;
}

#endif


HRESULT CCollider_PhysX_Joint::Set_ColiiderDesc(PHYSXDESC_JOINT desc)
{
	mType = 0;

	// 충돌 모델 초기화
	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC_JOINT));

	if (desc.mAttachModel == nullptr || desc.mLength == 0 || desc.mBoneNames == nullptr || desc.mGameObject == nullptr)
		return E_FAIL;

	mAttachModel = desc.mAttachModel;

	// 위 정보로 콜라이더 초기화
	// 위치 / 스케일 / 엑터 타입 / 모양을 지정헤서 콜라이더 컴포넌트 생성
	if (mMain_Actor)
		return E_FAIL;

	_float3 scale = mPhysXDesc.mScale;
	_float3 halfscale = _float3(scale.x*0.5f, scale.y*0.5f, scale.z*0.5f);

	for (int i = 0; i < int(desc.mLength); ++i)
	{
		CHierarchyNode* findBone = desc.mAttachModel->Find_HierarchyNode(desc.mBoneNames[i].c_str());
		NULL_CHECK_BREAK(findBone);
		mVecHier.push_back(findBone);
	}

	PxGeometry* gemo = nullptr;
	gemo = Create_Geometry(mPhysXDesc.eShapeType, scale);
	NULL_CHECK_BREAK(gemo);
	mMainTransform = (CTransform*)desc.mGameObject->Get_Component(TAG_COM(Com_Transform));
	mMainGameObject = desc.mGameObject;

	mMainBone = mVecHier.front();
	_float4x4 bonemat = mMainTransform->Get_WorldMatrix() * mMainBone->Get_OffsetMatrix().XMatrix();
	//	XMStoreFloat4x4(&bonemat, mMainBone->Get_OffsetMatrix());
	mPxMainMatrix4x4 = MAT4X4TOPXMAT(bonemat);

	// 기본 엑터를 생성후에 여기에 관절을 달아야한다.
	mMain_Actor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(PxTransform(mPxMainMatrix4x4), *gemo, 1.0f);
	CreateChain(mMain_Actor, mVecActors, PxTransform(mPxMainMatrix4x4), mPhysXDesc.mLength, *gemo, mPhysXDesc.mSeparation, CreateHairSpherical);

	NULL_CHECK_BREAK(mMain_Actor);
	Safe_Delete(gemo);

	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Set_NomalJoint(CTransform* trans, CGameObject* obj, _uint length)
{
	// 충돌 모델 초기화

	// 위 정보로 콜라이더 초기화
	// 위치 / 스케일 / 엑터 타입 / 모양을 지정헤서 콜라이더 컴포넌트 생성
	if (mMain_Actor)
		return E_FAIL;

	PxGeometry* gemo = nullptr;
	_float3 scale = _float3(2.0f);
	gemo = Create_Geometry(E_GEOMAT_SPEHE, scale);
	NULL_CHECK_BREAK(gemo);
	mMainTransform = trans;
	mMainGameObject = obj;
	mPxMainMatrix4x4 = PxTransform(MAT4X4TOPXMAT(mMainTransform->Get_WorldFloat4x4()));
	mMain_Actor = CreateChain(PxTransform(mPxMainMatrix4x4), 5, *gemo, 2, CreateHairSpherical);
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

PxJoint * CCollider_PhysX_Joint::CreateHairSpherical(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	//PxSphericalJoint* j = PxSphericalJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	//j->setLimitCone(PxJointLimitCone(PxPi / 4, PxPi / 4, PxPi / 4));
	//j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
	//j->setProjectionLinearTolerance(10.0f);

	PxD6Joint* j = PxD6JointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	j->setMotion(PxD6Axis::eX, PxD6Motion::eLOCKED);
	j->setMotion(PxD6Axis::eY, PxD6Motion::eLOCKED);
	j->setMotion(PxD6Axis::eZ, PxD6Motion::eLOCKED);

	j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
	j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
	j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
	j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(10, 10, 10, true));

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

	//j->setConstraintFlag(PxConstraintFlag::eBROKEN, false);
	//j->setConstraintFlag(PxConstraintFlag::ePROJECT_TO_ACTOR0, true);
	//j->setConstraintFlag(PxConstraintFlag::ePROJECT_TO_ACTOR1, true);
	j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);
	j->setConstraintFlag(PxConstraintFlag::eCOLLISION_ENABLED, false);
	j->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, false);
	j->setConstraintFlag(PxConstraintFlag::eIMPROVED_SLERP, false);
	j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);
	j->setConstraintFlag(PxConstraintFlag::eENABLE_EXTENDED_LIMITS, false);
	//j->setConstraintFlag(PxConstraintFlag::eGPU_COMPATIBLE, false);


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

	PxVec3 offset(-separation / 2, 0, 0);
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
	//mVecJoints.clear();
	//PxVec3 offset(separation / 2, 0, 0);
	//PxTransform localTm(offset);
	//PxRigidDynamic* prev = NULL;
	//PxRigidDynamic* first = NULL;

	//// N개의 관절 연결
	//// 엑터에 
	//for (PxU32 i = 0; i < length; i++)
	//{
	//	PxRigidDynamic* current = PxCreateDynamic(*mPhysics, pxTrans*localTm, g, *mNormalMaterial, 1.0f);
	//	PxJoint* j = (*createJoint)(prev, prev ? PxTransform(offset) : pxTrans, current, PxTransform(-offset));
	//	mVecJoints.push_back(j);

	//	mScene->addActor(*current);
	//	if (prev == nullptr)
	//		first = current;
	//	prev = current;
	//	localTm.p.x += separation;
	//}

	return nullptr;
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

PxRigidDynamic * CCollider_PhysX_Joint::CreateChain(PxRigidActor* baseActor, vector<PxRigidDynamic*>& listPxRig, const PxTransform & t, PxU32 length, const PxGeometry & g, PxReal separation, JointCreateFunction createJoint)
{
	// 기본 오브젝트에 관절오브젝트를 연결해서 사용해야
	// 관절 오브젝트 생성
	listPxRig.clear();
	PxVec3 offset(-2 / separation, 0, 0);
	PxTransform localTm(offset);
	PxRigidActor* prev = baseActor;

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

	return nullptr;
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