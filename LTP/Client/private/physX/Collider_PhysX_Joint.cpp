#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Joint.h"


_Sfloat3	myOffset[8];
_float		myOffsetScale[8];
_float4x4	myHM[8];

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

	// 머리의 뼈의 오프셋 값

	BlenderMat[0] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(-0.024161f, -0.5f , -127.25f);


	BlenderMat[1] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(0, 5.7499f, -133.2f);


	BlenderMat[2] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(0, 10.315f, -133.2f);


	BlenderMat[3] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(0, 14.229f, -133.21f);


	BlenderMat[4] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(0, 23.251f, -133.25f);


	BlenderMat[5] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(0, 35.752f, -133.28f);


	BlenderMat[6] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(0, 52.818f, -133.21f);


	BlenderMat[7] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(0, 64.212f, -133.21f);


	BlenderMat[8] = XMMatrixScaling(1, 1, 1)
		*XMMatrixRotationX(XMConvertToRadians(0))
		*XMMatrixRotationY(XMConvertToRadians(0))
		*XMMatrixRotationZ(XMConvertToRadians(0))
		* XMMatrixTranslation(0, 79.901f, -133.14f);






	return S_OK;
}


HRESULT CCollider_PhysX_Joint::Update_BeforeSimulation()
{
	FAILED_CHECK(__super::Update_BeforeSimulation());

	if (mMainTransform == nullptr)
		return E_FAIL;

	//	PxVec3 Value1;
//	PxVec3 Value2;

//	memcpy((_float3*)(&Value1), &DEBUGVALUE1, sizeof(_float3));
//	memcpy((_float3*)(&Value2), &DEBUGVALUE2, sizeof(_float3));


	//PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)mMain_Actor, Value2.x, &Value1);


	if (mType == 0)
	{


		_Matrix HM = mVecHier[0]->Get_UpdatedMatrix();
		_Matrix WorldHM = BlenderMat[0].XMatrix() * HM * mAttachModel->Get_DefaiultPivotMat().XMatrix()
			* mMainTransform->Get_WorldMatrix();

		PxVec3 pxvec = FLOAT3TOPXVEC3(WorldHM.r[3]);
		mMain_Actor->setGlobalPose(PxTransform(pxvec));



#pragma region 먼저 머리위치 맞추기

		// 뼈들 위치로 보내기
		for (_uint i = 1; i < mVecActors.size(); ++i)
		{

			PxTransform trans = mVecActors[i]->getGlobalPose();
			PxMat44 px4 = PxMat44(trans);
			_Matrix mat = (PXMATTOMAT4x4(px4)).XMatrix();


			_Matrix HM = mVecHier[i]->Get_UpdatedMatrix();
			_Matrix WorldHM = BlenderMat[i].XMatrix() * HM * mAttachModel->Get_DefaiultPivotMat().XMatrix() 
				* mMainTransform->Get_WorldMatrix();


			//if (g_pGameInstance->Get_DIKeyState(DIK_Z)&DIS_Down)
			//{
			//	_float4 Pos = WorldHM.r[3];
			//	wstring str = to_wstring(i) + L" -> Hair : " + to_wstring(Pos.x) + L" , " + to_wstring(Pos.y) + L" , " + to_wstring(Pos.z) + L" \n";
			//	OutputDebugStringW(str.c_str());

			//	Pos = mat.r[3];
			//	str = to_wstring(i) + L" -> Px : " + to_wstring(Pos.x) + L" , " + to_wstring(Pos.y) + L" , " + to_wstring(Pos.z) + L" \n";
			//	OutputDebugStringW(str.c_str());
			//}

			//mat.r[0] = XMVector3Normalize(mat.r[0]) * XMVector3Length(WorldHM.r[0]);
			//mat.r[1] = XMVector3Normalize(mat.r[1]) * XMVector3Length(WorldHM.r[1]);
			//mat.r[2] = XMVector3Normalize(mat.r[2]) * XMVector3Length(WorldHM.r[2]);


			mat.r[0] =WorldHM.r[0];
			mat.r[1] =WorldHM.r[1];
			mat.r[2] =WorldHM.r[2];

			mat = XMMatrixInverse(nullptr, BlenderMat[i].XMatrix()) *
				(
				(mat) *

					(mMainTransform->Get_InverseWorldMatrix())
					) * mAttachModel->Get_DefaiultPivotMat().InverseXMatrix();

			mVecHier[i]->Set_UpdateTransform(mat);

		}
		
#pragma endregion 먼저 머리위치 맞추기

	}



#pragma region 참고 코드


	//	mVecActors[0]->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

//	mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());
//	PxTransform trans = PxTransform(mPxMainMatrix4x4);
	//PxTransform tt = mVecActors[0]->getGlobalPose();
//	trans.p += PxVec3(0, 1.5f, 0);
//	mVecActors[0]->setGlobalPose(trans);
			//memcpy((_float3*)(&ffMat.m[1][0]), &DEBUGVALUE2, sizeof(_float3));
	//memcpy((_float3*)(&ffMat.m[2][0]), &DEBUGVALUE3, sizeof(_float3));
	//memcpy((_float3*)(&ffMat.m[3][0]), &DEBUGVALUE4, sizeof(_float3));

	// 뼈위치만 업데이트
	//PxVec3 APos = mVecActors[1]->getGlobalPose().p;
	//PxVec3 BPos = mVecActors[0]->getGlobalPose().p;
	//PxVec3 DIr = APos - BPos;
	//DIr = DIr.getNormalized();
	//_Sfloat4x4 newmat = mVecHier[1]->Get_UpdatedMatrix();
	//int Debug = 5;

	// 뼈 회전만 실행
		// _Sfloat4x4 rotmat = _Sfloat4x4();
		// PxRigidDynamic* act = mVecActors[i];
		// _Squternion quternian = _Squternion(act->getGlobalPose().q.x, act->getGlobalPose().q.y, act->getGlobalPose().q.z, act->getGlobalPose().q.w);
		// _float4x4 mat =  _Sfloat4x4::CreateFromQuaternion(quternian);
		// 
		// mVecHier[i]->Set_UpdateTransform(mat.XMatrix());


		//_Sfloat4x4 mat = mVecHier[i]->Get_UpdatedMatrix();
		//PxVec3 APos = mVecActors[i - 1]->getGlobalPose().p;
		//PxVec3 BPos = mVecActors[i]->getGlobalPose().p;
		//PxVec3 DIr = APos - BPos;
		//DIr *= ffMat._11;

		//_Sfloat4x4 DirMat = _Sfloat4x4::CreateTranslation(PXVEC3TOFLOAT3(DIr));
		//mVecHier[i]->Set_UpdateTransform(DirMat* mat);

#pragma endregion 참고 코드


	return S_OK;
}
PxTransform New_WorldPxTransform[8];
HRESULT CCollider_PhysX_Joint::Update_AfterSimulation()
{

	FAILED_CHECK(__super::Update_AfterSimulation());


	if (mType == 1)
	{
		// 계산된 위치와 다음위치를 바로전 위치의 방향을 구해서 
		_Matrix HM = mVecHier[0]->Get_UpdatedMatrix();
		_Matrix WorldHM = BlenderMat[0].XMatrix() * HM * mAttachModel->Get_DefaiultPivotMat().XMatrix()
			* mMainTransform->Get_WorldMatrix();

		PxVec3 pxvec = FLOAT3TOPXVEC3(WorldHM.r[3]);
		mMain_Actor->setGlobalPose(PxTransform(pxvec));

		New_WorldPxTransform[8];

		for (_uint i = 0; i < mVecActors.size(); ++i)
		{
			New_WorldPxTransform[i] = mVecActors[i]->getGlobalPose();
		}
	

	//	return S_OK;

		_float3 value1;  
		memcpy((_float3*)(&value1), &DEBUGVALUE1, sizeof(_float3));

		// 테스트로 엑터 1과 2를 보정
		PxVec3 PreDir;
		//	PreDir = New_WorldPxTransform[2].p - New_WorldPxTransform[1].p;
		//	PreDir = PreDir.getNormalized();
		//	New_WorldPxTransform[2].p = New_WorldPxTransform[1].p + PreDir * value1.x;;


		for (_uint i = 1; i < mVecActors.size(); ++i)
		{
			// 보간한 위치의 회전이 안먹인다
		//	PreDir = New_WorldPxTransform[i+1].p - New_WorldPxTransform[i].p;
		//	PreDir = PreDir.getNormalized();
		//	New_WorldPxTransform[i+1].p = New_WorldPxTransform[i].p + PreDir * myOffsetScale[i]* value1.x;;
			//_Squternion a = _Squternion::Slerp(*(_Squternion*)&New_WorldPxTransform[i].q, *(_Squternion*)&New_WorldPxTransform[i + 1].q, value1.y);
			//New_WorldPxTransform[i].q = *(PxQuat*)&a;


			PxTransform trans = New_WorldPxTransform[i];
			PxMat44 px4 = PxMat44(trans);

			// 다음 위치를 변환 위치로 새로 만들어야한다.


			_Matrix mat = (PXMATTOMAT4x4(px4)).XMatrix() ;
			_Vector Pos = mat.r[3];

			mat.r[3] = XMVectorSet(0, 0, 0, 1);
			mat =  XMMatrixRotationX(XMConvertToRadians(-90)) * mat ;
			//* 
			

			_Matrix HM = mVecHier[i]->Get_UpdatedMatrix();
			_Matrix WorldHM = BlenderMat[i].XMatrix() * HM * mAttachModel->Get_DefaiultPivotMat().XMatrix()
				* mMainTransform->Get_WorldMatrix();

			XMStoreFloat4x4(&myHM[i], WorldHM);

			mat.r[0] = XMVector3Normalize(mat.r[0]) * XMVector3Length(WorldHM.r[0]);
			mat.r[1] = XMVector3Normalize(mat.r[1]) * XMVector3Length(WorldHM.r[1]);
			mat.r[2] = XMVector3Normalize(mat.r[2]) * XMVector3Length(WorldHM.r[2]);
			mat.r[3] = Pos;
			mat = XMMatrixInverse(nullptr, BlenderMat[i].XMatrix()) *
				(
				(mat) *

					(mMainTransform->Get_InverseWorldMatrix())
					) * mAttachModel->Get_DefaiultPivotMat().InverseXMatrix();

			mVecHier[i]->Set_UpdateTransform(mat);

		}


		// 뼈들 위치로 보내기
		//_float NewRatio = 0.5f;

		//for (_uint i = 1; i < mVecActors.size(); ++i)
		//{
		//	PxTransform trans = mVecActors[i]->getGlobalPose();
		//	PxMat44 px4 = PxMat44(trans);

		//	if (i != 1)
		//	{
		//		_Sfloat3 inver = -myOffset[i-1];
		//		PxVec4 NewOffset = PxVec4(FLOAT3TOPXVEC3(inver),0);
		//		px4.column3 += NewOffset;
		//	}
		//

		//	_Matrix mat = (PXMATTOMAT4x4(px4)).XMatrix();


		//	_Matrix HM = mVecHier[i]->Get_UpdatedMatrix();
		//	_Matrix WorldHM = BlenderMat[i].XMatrix() * HM * mAttachModel->Get_DefaiultPivotMat().XMatrix()
		//		* mMainTransform->Get_WorldMatrix();


		//	mat.r[0] = WorldHM.r[0];
		//	mat.r[1] = WorldHM.r[1];
		//	mat.r[2] = WorldHM.r[2];

		//	mat = XMMatrixInverse(nullptr, BlenderMat[i].XMatrix()) *
		//		(
		//		(mat) *

		//			(mMainTransform->Get_InverseWorldMatrix())
		//			) * mAttachModel->Get_DefaiultPivotMat().InverseXMatrix();

		//	mVecHier[i]->Set_UpdateTransform(mat);

		//}


	}
	





#pragma region 이전코드

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
#pragma endregion 이전코드

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

	//mRenderColor = DirectX::Colors::Blue;
	PxShape* shapes[1];

	//mMain_Actor->getShapes(shapes, 1);
	//const PxShape& shape = *shapes[0];
	//const PxMat44 shpaeWorld(PxShapeExt::getGlobalPose(shape, *mMain_Actor));
	//const PxGeometryHolder h = shape.getGeometry();
	//RenderShape(h, shpaeWorld, mRenderColor); 
	
	mRenderColor = DirectX::Colors::Blue;

	// 액터 위치
	for (auto actor : mVecActors)
	{
		actor->getShapes(shapes, 1);
		const PxShape& shape = *shapes[0];
		const PxMat44 shpaeWorld(PxShapeExt::getGlobalPose(shape, *actor));
		const PxGeometryHolder h = shape.getGeometry();
		RenderShape(h, shpaeWorld, mRenderColor);

	}

	mRenderColor = DirectX::Colors::Red;

	// 액터 위치
	//for (PxTransform newPxTrans : New_WorldPxTransform)
	//{
	//	
	//	RenderDebugSphere(PXMATTOMAT4x4(PxMat44(newPxTrans)), _float3(0.2f, 0.1f, 0.3f),mRenderColor,PxGeometryType::eBOX);
	//}


	for (_float4x4 mat : myHM)
	{

		RenderDebugSphere(mat, _float3(1, 1, 1), mRenderColor, PxGeometryType::eBOX);
	}

	



	m_pBatch->End();

	return S_OK;
}

#endif


const PxVec3 HairSize(0.3f, 0.3f, 0.15f);
const PxReal Size(0.15f);

HRESULT CCollider_PhysX_Joint::Set_ColiiderDesc2(PHYSXDESC_JOINT desc)
{
	mType = 0;

	// 충돌 모델 초기화
	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC_JOINT));

	if (desc.mAttachModel == nullptr || desc.mLength == 0 || desc.mBoneNames == nullptr || desc.mGameObject == nullptr)
		return E_FAIL;

	// 0 Head ~ Hair_End
	for (int i = 0; i < int(desc.mLength); ++i)
	{
		CHierarchyNode* findBone = desc.mAttachModel->Find_HierarchyNode(desc.mBoneNames[i].c_str());
		NULL_CHECK_BREAK(findBone);
		mVecHier.push_back(findBone);
	}

	mAttachModel = desc.mAttachModel;
	mAttachModel->Update_AnimationClip(g_fDeltaTime, true);
	mMainGameObject = desc.mGameObject;
	mMainTransform = (CTransform*)mMainGameObject->Get_Component(TAG_COM(Com_Transform));

	if (mMain_Actor)
		return E_FAIL;

	PxGeometry* mainGemo = nullptr;
	PxGeometry* gemo = nullptr;
	mainGemo = Create_Geometry(E_GEOMAT_TYPE::E_GEOMAT_SPEHE, mPhysXDesc.mMainScale);
	gemo = Create_Geometry(desc.eShapeType, mPhysXDesc.mActorScale);
	NULL_CHECK_BREAK(mainGemo);
	NULL_CHECK_BREAK(gemo);

 	PxVec3 pos = FLOAT3TOPXVEC3(mMainTransform->Get_MatrixState(CTransform::STATE_POS));
	PxTransform pxtrans(pos);
	// 기본 엑터를 생성후에 여기에 관절을 달아야한다.
	mMain_Actor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(pxtrans, *mainGemo, 100);
	Create_DemoChain2((PxRigidDynamic*)mMain_Actor, pxtrans, desc.mLength, *gemo, desc.mSeparation, CreateMYJoint);
	
	((PxRigidDynamic*)mMain_Actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	NULL_CHECK_BREAK(mMain_Actor);
	Safe_Delete(gemo);
	Safe_Delete(mainGemo);

	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Set_ColiderDesc_Test(PHYSXDESC_JOINT desc)
{
	// 질량이 같은 예제로 예제로 테스트 용

	mType = 1;

	// 충돌 모델 초기화
	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC_JOINT));

	if (desc.mAttachModel == nullptr || desc.mLength == 0 || desc.mBoneNames == nullptr || desc.mGameObject == nullptr)
		return E_FAIL;

	// 0 Head ~ Hair_End
	for (int i = 0; i < int(desc.mLength); ++i)
	{
		CHierarchyNode* findBone = desc.mAttachModel->Find_HierarchyNode(desc.mBoneNames[i].c_str());
		NULL_CHECK_BREAK(findBone);
		mVecHier.push_back(findBone);
	}

	mAttachModel = desc.mAttachModel;
	mAttachModel->Update_AnimationClip(g_fDeltaTime, true);
	mMainGameObject = desc.mGameObject;
	mMainTransform = (CTransform*)mMainGameObject->Get_Component(TAG_COM(Com_Transform));

	if (mMain_Actor)
		return E_FAIL;

	PxGeometry* mainGemo = nullptr;
	PxGeometry* gemo = nullptr;
	mainGemo = Create_Geometry(E_GEOMAT_TYPE::E_GEOMAT_SPEHE, mPhysXDesc.mMainScale);
	gemo = Create_Geometry(desc.eShapeType, mPhysXDesc.mActorScale);
	NULL_CHECK_BREAK(mainGemo);
	NULL_CHECK_BREAK(gemo);

	// 기본 엑터를 생성후에 여기에 관절을 달아야한다.
	PxTransform dummyTransform = PxTransform(0, 0, 0);
	mMain_Actor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(dummyTransform, *mainGemo, 100);

	Create_DemoChain_Test((PxRigidDynamic*)mMain_Actor, PxTransform(0,0,0), desc.mLength-1, *gemo, desc.mSeparation, CreateD6Joint);

	((PxRigidDynamic*)mMain_Actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	NULL_CHECK_BREAK(mMain_Actor);
	Safe_Delete(gemo);
	Safe_Delete(mainGemo);
	return S_OK;
}

PxJoint * CCollider_PhysX_Joint::CreateMYJoint(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	// 관절의 제약 조건 설정

	// PxFixedJoint 고정 관절 생성 두 엑터가 붙어있다.
	//PxFixedJoint* j = PxFixedJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	//j->setProjectionLinearTolerance(0.01f);
	//j->setProjectionAngularTolerance(0.01f);

	//j->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	//j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
	//j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);

	// D6
	//	PxD6Joint* j = PxD6JointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	//	j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLIMITED);
	//	j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLIMITED);
	//	j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLIMITED);
	//	j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(0, 1000, FLT_MAX, true));
	//	j->setDistanceLimit(
	//		PxJointLinearLimit(CPhysXMgr::gToleranceScale, 100.f, 15.f));

	//j->setConstraintFlag(PxConstraintFlag::ePROJECT_TO_ACTOR0, true);
	//j->setConstraintFlag(PxConstraintFlag::ePROJECT_TO_ACTOR1, false);
	//j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);		 
	//j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
																	 
	//	j->setTwistLimit(
	//		PxJointLinearLimit(CPhysXMgr::gToleranceScale, 100.f, 15.f));
	
	//j->setProjectionAngularTolerance()


	//PxD6Joint* j = PxD6JointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	//j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
	//j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLIMITED);
	//j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLIMITED);
	//j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(0, 1, 0.01f, true));



	// 회전 조인트 프레임의 원점과 x축을 함께 유지하고 이 공통 축을 중심으로 자유 회전을 허용합니다.
	//PxRevoluteJoint* j = PxRevoluteJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	//j->setDriveForceLimit(3);
	//j->setDriveVelocity(5);
	//j->setDriveGearRatio(0.5f);

	//j->setInvMassScale0(1000);
	//j->setInvMassScale1(1000);

	//j->setProjectionLinearTolerance(1000);
	//j->setProjectionAngularTolerance(200);

	//j->setConstraintFlag(PxConstraintFlag::Enum::ePROJECTION, true);
	//j->setConstraintFlag(PxConstraintFlag::Enum::eCOLLISION_ENABLED, false);
	//j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
	//j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);



	// 프리즘 방향을 동일하게 유지하지만 각 프레임의 원점이 공통 x축을 따라 자유롭게 미끄러지도록 허용합니다.
//	PxPrismaticJoint* j = PxPrismaticJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);

	//j->setInvMassScale0(0.01f);
	//j->setInvMassScale1(0.01f);

	//j->setInvInertiaScale0(5);
	//j->setInvInertiaScale1(0.1f);
	//j->setProjectionLinearTolerance(0.001f);



	// 거리 조절
	//PxD6Joint* j = PxD6JointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	//j->setMotion(PxD6Axis::eX, PxD6Motion::eLIMITED);
	//j->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
	//j->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
	//j->setDistanceLimit(PxJointLinearLimit(1, PxSpring(5000, 5)));


	//PxDistanceJoint* j = PxDistanceJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	//j->setMaxDistance(100.0f);
	//j->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED,true);

	//j->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	//j->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	//j->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE);
	

	PxFixedJoint* j = PxFixedJointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);

	j->setInvMassScale0(1000);
	j->setInvMassScale1(1000);

	j->setProjectionLinearTolerance(1000);
	j->setProjectionAngularTolerance(200);




	return j;
}


PxJoint* CCollider_PhysX_Joint::CreateD6Joint(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
{
	PxD6Joint* j = PxD6JointCreate(*GetSingle(CPhysXMgr)->gPhysics, a0, t0, a1, t1);
	j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
	j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(0, 1000, FLT_MAX, true));
	return j;

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

PxRigidDynamic* CCollider_PhysX_Joint::Create_DemoChain(PxRigidDynamic* actor,const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
{
	PxVec3 offset(0, 0, separation);
	PxTransform localTm(offset);
	PxRigidDynamic* prev = actor;


	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*mPhysics, t*localTm, g, *mNormalMaterial, 1);
		current->setMass(0.05f);
		current->setSleepThreshold(PX_MAX_F32);
		current->setStabilizationThreshold(PX_MAX_F32);
		current->setWakeCounter(PX_MAX_F32);
		current->setLinearDamping(5.f);


		(*createJoint)(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		GetSingle(CPhysXMgr)->Get_PhysicsScene()->addActor(*current);
		prev = current;
		localTm.p.z += separation;
	}
	return nullptr;
}

PxRigidDynamic* CCollider_PhysX_Joint::Create_DemoChain2(PxRigidDynamic* mainactor, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
{
	
	_float3 WorldPos[9];
	_float3 distarr[8];
	_float3	fdistarr[8];

	for (_uint i = 0; i < 9; i++)
	{
		_Matrix HM = mVecHier[i]->Get_UpdatedMatrix();
		_Matrix WorldHM = BlenderMat[i].XMatrix() * HM * mAttachModel->Get_DefaiultPivotMat().XMatrix()
			* mMainTransform->Get_WorldMatrix();

		WorldPos[i] = WorldHM.r[3];
	}

	for (_uint i = 0; i < 8; i++)
	{
		distarr[i] = WorldPos[i + 1 ].XMVector() - WorldPos[i].XMVector();
		fdistarr[i] = _float3(XMVectorGetX(XMVector3Length(distarr[i].XMVector())), 0, 0);
	}



	PxRigidDynamic* prev = mainactor;

	mVecJoints.clear();

//	mainactor->setMass(0);
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*mPhysics, PxTransform(FLOAT3TOPXVEC3(WorldPos[i+1])), g, *mNormalMaterial, 1.0f);
		mVecActors.push_back(current);

		PxJoint* j = (*createJoint)(prev, prev ? PxTransform(FLOAT3TOPXVEC3(distarr[i])) : t, current, PxTransform(FLOAT3TOPXVEC3(-distarr[i].XMVector())));
		mVecJoints.push_back(j);

		// 모든 변환 각도 설정 구동: 
		j->setConstraintFlag(PxConstraintFlag::Enum::ePROJECTION, true);
		j->setConstraintFlag(PxConstraintFlag::Enum::eCOLLISION_ENABLED, false);
		j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
		j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);


		GetSingle(CPhysXMgr)->Get_PhysicsScene()->addActor(*current);
		prev = current;
	}

	return mainactor;
}

PxRigidDynamic* CCollider_PhysX_Joint::Create_DemoChain_Test(PxRigidDynamic* mainactor, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
{
	// 예제코드로 테스트
	// 머리 1에 박는다.

	_float3 WorldPos[9];
	myOffset[8]; // 방향
	myOffsetScale[8]; // 방향 크기
	_float ratio = 2.0f;


	for (_uint i = 0; i < 9; i++)
	{
		// Head~ HairEnd까지의 world 위치
		_Matrix HM = mVecHier[i]->Get_UpdatedMatrix();
		_Matrix WorldHM = BlenderMat[i].XMatrix() * HM * mAttachModel->Get_DefaiultPivotMat().XMatrix()
			* mMainTransform->Get_WorldMatrix();
		WorldPos[i] = WorldHM.r[3];
	}

	// 거리 계산
	for (_uint i = 0; i < 8; i++)
	{
		myOffset[i] = WorldPos[i + 1].XMVector() - WorldPos[i].XMVector();
		myOffsetScale[i] = myOffset[i].Length();
		myOffset[i] *= ratio;
	//	fdistarr[i] = _float3(XMVectorGetX(XMVector3Length(distarr[i].XMVector())), 0, 0);
	}

	// mainActor = Hair1
	PxTransform trans = PxTransform(FLOAT3TOPXVEC3(WorldPos[1]));
	mainactor->setGlobalPose(trans);


	PxRigidDynamic* prev = mainactor;

	mVecJoints.clear();
	mVecActors.push_back(mainactor);

	//	mainactor->setMass(0);
	for (PxU32 i = 1; i < length; i++)
	{
		// 그 다음 액터 생성

		PxRigidDynamic* current = PxCreateDynamic(*mPhysics, PxTransform(FLOAT3TOPXVEC3(WorldPos[i + 1])), g, *mNormalMaterial, 1.0f);
	//	current->setMass(0.05f);
	//	current->setSleepThreshold(PX_MAX_F32);
	//	current->setStabilizationThreshold(PX_MAX_F32);
		current->setWakeCounter(PX_MAX_F32);
	//	current->setLinearDamping(5.f);

		mVecActors.push_back(current);

		PxTransform newoffset = PxTransform(FLOAT3TOPXVEC3(myOffset[i]));

		PxJoint* j = (*createJoint)(prev, newoffset, current, newoffset.getInverse());
		mVecJoints.push_back(j);
		if (i == 0)
		{
			j->setInvMassScale0(10);
		}

		// 모든 변환 각도 설정 구동: 
		j->setConstraintFlag(PxConstraintFlag::Enum::ePROJECTION, true);
		j->setConstraintFlag(PxConstraintFlag::Enum::eCOLLISION_ENABLED, false);
		j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
		j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);

		GetSingle(CPhysXMgr)->Get_PhysicsScene()->addActor(*current);
		prev = current;
	}

	return mainactor;
}


PxRigidDynamic* CCollider_PhysX_Joint::Create_MatchForBonePos(PxRigidDynamic* mainactor, const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
{
	// 매치되는 뼈 위치 정확하게 생성하기
	_float3 WorldPos[9];
	_float3 distarr[8];
	_float3	fdistarr[8];

	for (_uint i = 0; i < 9; i++)
	{
		_Matrix HM = mVecHier[i]->Get_UpdatedMatrix();
		_Matrix WorldHM = BlenderMat[i].XMatrix() * HM * mAttachModel->Get_DefaiultPivotMat().XMatrix()
			* mMainTransform->Get_WorldMatrix();

		WorldPos[i] = WorldHM.r[3];
	}

	for (_uint i = 0; i < 8; i++)
	{
		distarr[i] = WorldPos[i + 1].XMVector() - WorldPos[i].XMVector();
		fdistarr[i] = _float3(XMVectorGetX(XMVector3Length(distarr[i].XMVector())), 0, 0);
	}



	PxRigidDynamic* prev = mainactor;

	mVecJoints.clear();
	PxJoint* joint = nullptr;

	//	mainactor->setMass(0);
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*mPhysics, PxTransform(FLOAT3TOPXVEC3(WorldPos[i + 1])), g, *mNormalMaterial, 1.0f);
		mVecActors.push_back(current);


		joint = (*createJoint)(prev, prev ? PxTransform(FLOAT3TOPXVEC3(distarr[i])) : t, current, PxTransform(FLOAT3TOPXVEC3(-distarr[i].XMVector())));
		mVecJoints.push_back(joint);
		GetSingle(CPhysXMgr)->Get_PhysicsScene()->addActor(*current);
		prev = current;
	}

	return mainactor;
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

PxRigidDynamic * CCollider_PhysX_Joint::CreateChain_BaseActor(PxRigidActor* baseActor, vector<PxRigidDynamic*>& listPxRig, const PxTransform & t, PxU32 length, const PxGeometry & g, PxReal separation, JointCreateFunction createJoint)
{
	// 기본 오브젝트에 관절오브젝트를 연결해서 사용해야
	// 관절 오브젝트 생성
	listPxRig.clear();
	PxVec3 offset(0, 0, separation);
	PxTransform localTm(offset);
	PxRigidActor* prev = baseActor;
	((PxRigidDynamic*)baseActor)->setMass(0);

	// N개의 관절 연결
	// 엑터에 
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*GetSingle(CPhysXMgr)->gPhysics, t*localTm, g, *GetSingle(CPhysXMgr)->gMaterial, 1.f);
		current->setMass(1);
		current->setSleepThreshold(PX_MAX_F32);
		current->setStabilizationThreshold(PX_MAX_F32);
		current->setWakeCounter(PX_MAX_F32);
		current->setLinearDamping(15.f);

		(*createJoint)(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		GetSingle(CPhysXMgr)->Get_PhysicsScene()->addActor(*current);
		listPxRig.push_back(current);
		prev = current;
		localTm.p.z += separation;
	}
	return (PxRigidDynamic*)baseActor;
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