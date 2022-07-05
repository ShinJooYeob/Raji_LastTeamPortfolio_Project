#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Base.h"
#include "DebugDraw.h"


#define  MAX_NUM_ACTOR_SHAPES 128


CCollider_PhysX_Base::CCollider_PhysX_Base(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CCollider_PhysX_Base::CCollider_PhysX_Base(const CCollider_PhysX_Base & rhs)
	: CComponent(rhs)
#ifdef _DEBUG
	,m_pBasicEffect(rhs.m_pBasicEffect),
	m_pBatch(rhs.m_pBatch),
	m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}


HRESULT CCollider_PhysX_Base::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

#ifdef _DEBUG
	m_pBasicEffect = new BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	FAILED_CHECK(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout));

	NULL_CHECK_RETURN(m_pInputLayout, E_FAIL);

	m_pBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);
	NULL_CHECK_RETURN(m_pBatch, E_FAIL);

#endif // _DEBUG


	return S_OK;
}

HRESULT CCollider_PhysX_Base::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	mMain_Actor = nullptr;

	return S_OK;
}


HRESULT CCollider_PhysX_Base::Update_BeforeSimulation()
{

	// 시뮬레이션 전의 충돌체 위치
	// 현재 오브젝트 위치를 받아온다.
	if (mbPhysXUpdate == false)
		return S_OK;

	if (mMainTransform == nullptr)
		return E_FAIL;

		mPxTransform = PxTransform(*(PxVec3*)&mMainTransform->Get_MatrixState_Float3(CTransform::STATE_POS));

	//if (mePhysxType == E_PHYSXTYPE_JOINT)
	//{
	//	_float4x4 mat =  mAttachDesc.Caculate_AttachedBoneMatrix();
	//	mPxTransform = PxTransform(MAT4X4TOPXMAT(mat));
	//}

	return S_OK;
}

HRESULT CCollider_PhysX_Base::Update_AfterSimulation()
{
	if (mbPhysXUpdate == false)
		return S_OK;

	if (mMainTransform == nullptr)
		return E_FAIL;

	//if (mePhysxType == E_PHYSXTYPE_JOINT)
	//	Trans = mAttachDesc.Get_AttachObjectTransform();

	//if (mePhysxType == E_PHYSXTYPE_JOINT)
	//{
	//	//mPxTransform = PxTransform(*(PxVec3*)&Trans->Get_MatrixState_Float3(CTransform::STATE_POS));
	//	//

	//	//_float4x4 mat = mAttachDesc.Caculate_AttachedBoneMatrix();
	//	//mPxTransform = PxTransform(MAT4X4TOPXMAT(mat));

	//	mRigActor->setGlobalPose(mPxTransform);

	//	return S_OK;
	//}


	// 시뮬레이션 후 충돌체를 업데이트 해준다.
	//if (mbKeyUpdate)
	//{
	//	
	//	mPxTransform = PxTransform(*(PxVec3*)&Trans->Get_MatrixState_Float3(CTransform::STATE_POS));
	//	mRigActor->setGlobalPose(mPxTransform);
	//}

		// Pos
		mPxTransform = mMain_Actor->getGlobalPose();
		_float3 vec3 = *(_float3*)&mPxTransform.p;
		mMainTransform->Set_MatrixState(CTransform::STATE_POS, vec3);
	

	return S_OK;
}

//HRESULT CCollider_PhysX_Base::Set_PhysXScale(_float3 scaleVec)
//{
//	mActorScale  =  FLOAT3TOPXVEC3(scaleVec);
//	return S_OK;
//}
//
//HRESULT CCollider_PhysX_Base::CreateDynamicActor(PxVec3 scale)
//{
//	if (mMain_Actor)
//		return S_FALSE;
//
//	mMain_Actor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(mPxTransform, PxSphereGeometry(scale.x));
//
//	return S_OK;
//}
//
//HRESULT CCollider_PhysX_Base::CreateStaticActor(PxVec3 scale)
//{
//	if (mMain_Actor)
//		return S_FALSE;
//
//	mePhysxType = CCollider_PhysX_Base::E_PHYSXTYPE_STATIC;
//
//	mMain_Actor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(mPxTransform, PxBoxGeometry(scale));
//
//	return S_OK;
//
//}
//
//HRESULT CCollider_PhysX_Base::CreateChain(const PxTransform & t, PxU32 length, const PxGeometry & g, PxReal separation, JointCreateFunction createJoint)
//{
//	if (mMain_Actor)
//		return S_FALSE;
//	mePhysxType = CCollider_PhysX_Base::E_PHYSXTYPE_STATIC;
//	mMain_Actor = GetSingle(CPhysXMgr)->CreateChain(t, length, g, separation, createJoint);
//	return S_OK;
//}
//
//
//HRESULT CCollider_PhysX_Base::CreateChain(ATTACHEDESC attach, PxU32 length, const PxGeometry & g, PxReal separation, JointCreateFunction createJoint)
//{
//	if (mMain_Actor)
//		return S_FALSE;
//
//	mePhysxType = CCollider_PhysX_Base::E_PHYSXTYPE_JOINT;
//	ATTACHEDESC mAttachDesc;
//	mAttachDesc = attach;
//	PxTransform t = PxTransform(FLOAT3TOPXVEC3(mAttachDesc.Get_AttachObjectTransform()->Get_MatrixState(CTransform::STATE_POS)));
//	mMain_Actor = GetSingle(CPhysXMgr)->CreateChain(t, length, g, separation, createJoint);
//	return S_OK;
//}



HRESULT CCollider_PhysX_Base::Setup_PhysXDesc(PHYSXDESC desc)
{
	// 충돌 모델 초기화

	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC));

	// 위 정보로 콜라이더 초기화

	// 위치 / 스케일 / 엑터 타입 / 모양을 지정헤서 콜라이더 컴포넌트 생성
	if (mMain_Actor)
		return E_FAIL;

	PxGeometry* gemo = nullptr;

	mMainTransform = mPhysXDesc.mTrnasform;

	_float3 scale = mMainTransform->Get_Scale();
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

	PxTransform pxtrans = PxTransform(FLOAT3TOPXVEC3(pos));
	_Sfloat4x4 float4x4 = mMainTransform->Get_WorldFloat4x4();
	_Squternion q = _Squternion::CreateFromRotationMatrix(float4x4);
	pxtrans.q = *(PxQuat*)&q;

	NULL_CHECK_BREAK(gemo);
	switch (mPhysXDesc.ePhyType)
	{
	case Client::E_PHYTYPE_STATIC:
		mMain_Actor = GetSingle(CPhysXMgr)->CreateStatic_BaseActor(pxtrans, *gemo);
		break;

	case Client::E_PHYTYPE_DYNAMIC:
		mMain_Actor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(pxtrans, *gemo,FLOAT3TOPXVEC3(mPhysXDesc.mVelocity));
		break;
	case Client::E_PHYTYPE_JOINT:
		break;
	default:
		break;

	}

	NULL_CHECK_BREAK(mMain_Actor);
	Safe_Delete(gemo);


	//mMain_Actor->attachShape(*shape);
	//shape->setLocalPose(offset);
	//NULL_CHECK_BREAK(shape);
	//mMain_Actor->attachShape(*shape);

	return S_OK;

}

HRESULT CCollider_PhysX_Base::Add_Shape(PxGeometry& gemo, PxTransform offset)
{
	// 메인 액터에 충돌체 추가
	if (mMain_Actor)
	{
		PxShape* shape = CPhysXMgr::gPhysics->createShape(gemo, *CPhysXMgr::gMaterial);
		shape->setLocalPose(offset);
		NULL_CHECK_BREAK(shape);
		mMain_Actor->attachShape(*shape);
	}

	return S_OK;
}

void CCollider_PhysX_Base::Set_Postiotn(_float3 positiotn)
{
	mMain_Actor->setGlobalPose(PxTransform(FLOAT3TOPXVEC3(positiotn)));
}

//PxJoint * CCollider_PhysX_Base::CreateLimitedSpherical(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
//{
//	PxSphericalJoint* j = PxSphericalJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
//	j->setLimitCone(PxJointLimitCone(PxPi / 4, PxPi / 4, 0.1f));
//	j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
//	return j;
//}
//
//PxJoint * CCollider_PhysX_Base::CreateBreakableFixed(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
//{
//	PxFixedJoint* j = PxFixedJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
//	j->setBreakForce(1000, 100000);
//	j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
//	j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);
//	return j;
//}
//
//PxJoint * CCollider_PhysX_Base::CreateDampedD6(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
//{
//	PxD6Joint* j = PxD6JointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
//	j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
//	j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
//	j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
//	j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(0, 1000, FLT_MAX, true));
//	return j;
//}

#ifdef _DEBUG

HRESULT CCollider_PhysX_Base::Render()
{
	NULL_CHECK_RETURN(mMain_Actor, E_FAIL);

	// 모양에 따라 드로잉
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pBasicEffect->SetWorld(XMMatrixIdentity());

	m_pBasicEffect->SetView(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_VIEW));
	m_pBasicEffect->SetProjection(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_PROJ));
	m_pBasicEffect->Apply(m_pDeviceContext);


	//bool sleeping = mRigActor->is<PxRigidDynamic>() ? mRigActor->is<PxRigidDynamic>()->isSleeping() : false;

	//if (sleeping)
	//	return S_FALSE;

	m_pBatch->Begin();


	PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
	const PxU32 numShapes = mMain_Actor->getNbShapes();
	if (numShapes >= MAX_NUM_ACTOR_SHAPES)
		return E_FAIL;

	mMain_Actor->getShapes(shapes, numShapes);

	// 모양 마다 그려준다.
	XMVECTORF32 color = DirectX::Colors::Cyan;

	for (PxU32 j = 0; j < numShapes; j++)
	{
		const PxMat44 shpaeWorld(PxShapeExt::getGlobalPose(*shapes[j], *mMain_Actor));
		const PxGeometryHolder h = shapes[j]->getGeometry();
		RenderShape(h, shpaeWorld, color);
	}
	m_pBatch->End();

	return S_OK;
}


HRESULT CCollider_PhysX_Base::RenderShape(const PxGeometryHolder & h, const PxMat44& world, XMVECTORF32 color)
{
	const PxGeometry& geom = h.any();
	PxTransform worldTrans = PxTransform(world);
	_float3 worldpos = PXVEC3TOFLOAT3(worldTrans.p);


	switch (geom.getType())
	{
	case PxGeometryType::eSPHERE:
	{
		const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geom);
		BoundingSphere s = BoundingSphere(worldpos, sphereGeom.radius);
		DX::Draw(m_pBatch, s, color);
		break;
	}

	case PxGeometryType::eBOX:
	{
		const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
		_float3 halfextents = _float3(boxGeom.halfExtents.x, boxGeom.halfExtents.y, boxGeom.halfExtents.z);
	//	PxQuat q = worldTrans.q;
		_float4 q = *(_float4*)&worldTrans.q;

		BoundingOrientedBox obbBox = BoundingOrientedBox(worldpos, halfextents, q);

		DX::Draw(m_pBatch, obbBox, color);
		break;
	}
	case PxGeometryType::eCAPSULE:
	{
		const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(geom);

		BoundingBox box = BoundingBox(worldpos, _float3(capsuleGeom.radius, capsuleGeom.halfHeight, capsuleGeom.radius));
		_float3 upSph1 = worldpos; upSph1.y += capsuleGeom.halfHeight;
		_float3 upSph2 = worldpos; upSph2.y -= capsuleGeom.halfHeight;
		
		BoundingSphere s1 = BoundingSphere(upSph1, capsuleGeom.radius);
		BoundingSphere s2 = BoundingSphere(upSph2, capsuleGeom.radius);
		DX::Draw(m_pBatch, box);
		DX::Draw(m_pBatch, s1);
		DX::Draw(m_pBatch, s2);

	}
	break;

	//case PxGeometryType::eCONVEXMESH:
	//	PxConvexMeshGeometry
	//	break;

	//case PxGeometryType::eTRIANGLEMESH:
	//	PxTriangleMeshGeometry
	//	break;

	default:
		break;

	}

	return S_OK;


}
#endif

//CCollider_PhysX_Base * CCollider_PhysX_Base::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
//{
//
//	CCollider_PhysX_Base* pInstance = new CCollider_PhysX_Base(pDevice, pDeviceContext);
//
//	if (FAILED(pInstance->Initialize_Prototype(pArg)))
//	{
//		MSGBOX("Failed to Create Transform Prototype");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//CComponent * CCollider_PhysX_Base::Clone(void * pArg)
//{
//	CCollider_PhysX_Base* pInstance = new CCollider_PhysX_Base((*this));
//
//	if (FAILED(pInstance->Initialize_Clone(pArg)))
//	{
//		MSGBOX("Failed to Create Transform");
//		Safe_Release(pInstance);
//	}
//	return pInstance;
//}

void CCollider_PhysX_Base::Free()
{
	__super::Free();

#ifdef _DEBUG
	if (m_bIsClone == false)
	{
		Safe_Delete(m_pBasicEffect);
		Safe_Delete(m_pBatch);
	}

	Safe_Release(m_pInputLayout);

#endif // _DEBUG

	if (m_bIsClone)
	{
		auto scene = GetSingle(CPhysXMgr)->Get_PhysicsScene();
		if (scene)
			scene->removeActor(*mMain_Actor);
		mMain_Actor = nullptr;

	}


}
