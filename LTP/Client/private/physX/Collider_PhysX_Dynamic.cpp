#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Dynamic.h"


CCollider_PhysX_Dynamic ::CCollider_PhysX_Dynamic (ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCollider_PhysX_Base(pDevice, pDeviceContext)
{
	mePhysX_ID = E_PHYTYPE_DYNAMIC;
}

CCollider_PhysX_Dynamic ::CCollider_PhysX_Dynamic (const CCollider_PhysX_Dynamic  & rhs)
	: CCollider_PhysX_Base(rhs)
{

}

HRESULT CCollider_PhysX_Dynamic ::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollider_PhysX_Dynamic ::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;
	

	// 키나 외부 힘이 작용했을떄 예외처리
	//PxRigidDynamic* dynamic;
	//dynamic->setKinematicTarget();
	return S_OK;
}


HRESULT CCollider_PhysX_Dynamic ::Update_BeforeSimulation()
{
	if (FAILED(__super::Update_BeforeSimulation()))
		return E_FAIL;



	return S_OK;
}

HRESULT CCollider_PhysX_Dynamic ::Update_AfterSimulation()
{
	if (FAILED(__super::Update_AfterSimulation()))
		return E_FAIL;

	PxTransform trans =  mMain_Actor->getGlobalPose();
	mMainTransform->Set_MatrixState(CTransform::STATE_POS, PXVEC3TOFLOAT3(trans.p));
//	mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());

	return S_OK;
}


#ifdef _DEBUG

HRESULT CCollider_PhysX_Dynamic ::Render()
{
	FAILED_CHECK(__super::Render());

	mPxMainMatrix4x4 = mMain_Actor->getGlobalPose();
	mMainTransform->Set_Matrix(PXMATTOMAT4x4(mPxMainMatrix4x4));

	return S_OK;
}

#endif


HRESULT CCollider_PhysX_Dynamic::Set_ColiiderDesc(PHYSXDESC_DYNAMIC desc)
{
	// 충돌 모델 초기화
	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC_DYNAMIC));

	// 위 정보로 콜라이더 초기화
	// 위치 / 스케일 / 엑터 타입 / 모양을 지정헤서 콜라이더 컴포넌트 생성
	if (mMain_Actor)
		return E_FAIL;


	PxGeometry* gemo = nullptr;
	mMainTransform = mPhysXDesc.mTrnasform;

	_float3 scale = mMainTransform->Get_Scale();
	_float3 pos = mMainTransform->Get_MatrixState(CTransform::STATE_POS);

	gemo = Create_Geometry(desc.eShapeType, scale);
	NULL_CHECK_BREAK(gemo);

	// 초기휘치 오류
	mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());
	PxTransform nomalTransform = GetPxTransform(mPxMainMatrix4x4);

	PxReal density = 5.f;

	mMain_Actor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(nomalTransform, *gemo, density, FLOAT3TOPXVEC3(mPhysXDesc.mVelocity));
	NULL_CHECK_BREAK(mMain_Actor);

	Safe_Delete(gemo);
	mMain_Actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	mMain_Actor->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
	
	return S_OK;
}


CCollider_PhysX_Dynamic  * CCollider_PhysX_Dynamic ::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CCollider_PhysX_Dynamic * pInstance = new CCollider_PhysX_Dynamic (pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_PhysX_Dynamic ::Clone(void * pArg)
{
	CCollider_PhysX_Dynamic * pInstance = new CCollider_PhysX_Dynamic ((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_PhysX_Dynamic ::Free()
{
	__super::Free();




}
