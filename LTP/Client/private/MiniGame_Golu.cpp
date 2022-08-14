#include "stdafx.h"
#include "..\public\MiniGame_Golu.h"
#include "HpUI.h"
#include "Camera_Main.h"
#include "Golu_Bullet.h"
#include "InstanceMonsterBatchTrigger.h"

/*
1. Main Cam -> FocusTarget Settomg
2. Target Obj->Set_AttachCamPos()
3. Main Cam->Lock_CamLook(true, Look)
4. Main Cam->Set_TargetArmLength, Max, Min, Or Cur
*/

CMiniGame_Golu::CMiniGame_Golu(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMiniGame_Golu::CMiniGame_Golu(const CMiniGame_Golu & rhs)
	: CGameObject(rhs)
{
}


HRESULT CMiniGame_Golu::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMiniGame_Golu::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));


	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(180));




	//m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));


	SetUp_Info();

	//네비메쉬 47 이후 인덱스부터 아무거나 쓰면 되겠끔 만들자

	return S_OK;
}

_int CMiniGame_Golu::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;


	//FAILED_CHECK(Ready_TriggerObject(L"Stage_MiniGame1_InstanceMonsterTrigger1.dat", SCENE_MINIGAME1, TAG_LAY(Layer_ColTrigger)));


	m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 0.f));

	if (m_fHP <= 0)
	{
		m_pDissolve->Update_Dissolving(dDeltaTime);
		m_pDissolve->Set_DissolveOn(false, 2.f);

		m_dDissolveTime += dDeltaTime;

		//if (m_bDieSound == false && m_dDissolveTime >= 1.)
		//{
		//	g_pGameInstance->Play3D_Sound(TEXT("EH_Wave_Tezabsura_Damage_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
		//	m_bDieSound = true;
		//}

		if (m_dDissolveTime >= 2)
		{
			Set_IsDead();
		}
	}

	Camera_Walking(dDeltaTime);
	Play_MiniGame(dDeltaTime);

	m_pModel->Change_AnimIndex(0/*여기에 애니메이션 인덱스 ㄱㄱㄱ*/, 0.f);




	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);


	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));


	if (m_pHPUI != nullptr)
		m_pHPUI->Update(dDeltaTime);

	Update_Collider(dDeltaTime);

	return _int();
}

_int CMiniGame_Golu::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolve));

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
#endif

	if (m_pHPUI != nullptr)
		m_pHPUI->LateUpdate(dDeltaTime);

	return _int();
}

_int CMiniGame_Golu::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolve->Render(3)); //디졸브 내부에서 밑의 머테리얼을 찾아주고 있음

#ifdef _DEBUG
	FAILED_CHECK(m_pNavigationCom->Render());
#endif
	return _int();
}

_int CMiniGame_Golu::LateRender()
{
	return _int();
}

void CMiniGame_Golu::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

_float CMiniGame_Golu::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	m_pHPUI->Set_ADD_HitCount((_int)fDamageAmount);
	m_fHP += -fDamageAmount;


	if (0 >= m_fHP)
	{
		return -1.f;
	}

	return _float();
}

void CMiniGame_Golu::Update_AttachCamPos()
{
	_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_float3 vBonePos = Get_BonePos("skd_hip");
	m_fAttachCamPos_Offset.y = 7.f;
	m_fAttachCamPos_Offset.x = 0.f;
	m_fAttachCamPos_Offset.z = -3.f;
	m_pMainCamera->Set_TargetArmLength(0.f);
	XMStoreFloat3(&m_fAttachCamPos, vMyPos + m_fAttachCamPos_Offset.XMVector());
	//m_fAttachCamPos_Offset으로 피벗 설정

	CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	_float _fCameraArmLength = pMainCam->Get_TargetArmLength();
	CTransform* pCamTransform = pMainCam->Get_Camera_Transform(); 

	XMStoreFloat3(&m_fAttachCamPos, m_fAttachCamPos.XMVector() - pCamTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * _fCameraArmLength);

	//m_fAttachCamLook = XMVector3Normalize((XMVectorSetW(vBonePos.XMVector() - m_fAttachCamPos.XMVector(), 0.f))) + m_fAttachCamLook_Offset.XMVector();
	//카메라가 바라보는 방향은 윗 주석으로 알려주고 밑처럼 걍 절대값 넣어줘도 됨
	//m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, -0.8f, 0.1f,0.f));

	//Bone is Look
	//m_pMainCamera->Lock_CamLook(true,XMVector3Normalize(vBonePos.XMVector()- m_fAttachCamPos.XMVector()));

	m_pMainCamera->Lock_CamLook(true, XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) - m_fAttachCamPos.XMVector()));


	/*m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	m_fAttachCamPos.y += 2.f;
	m_fAttachCamPos.z -= 4.f;*/
}

HRESULT CMiniGame_Golu::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Golu), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	CHpUI::HPDesc HpDesc;
	HpDesc.m_HPType = CHpUI::HP_RAJI;
	HpDesc.m_pObjcect = this;
	HpDesc.m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	HpDesc.m_Dimensions = 1.2f;
	m_fMaxHP = 9;
	m_fHP = m_fMaxHP;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);


	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 4;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));


	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentIndex = 0;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	SetUp_Collider();

	return S_OK;
}

HRESULT CMiniGame_Golu::Adjust_AnimMovedTransform(_double dDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 1://애니메이션 인덱스마다 잡아주면 됨
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴
			{

				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate > 0.7666666666666666) //특정 프레임 플레이 레이트이후에 들어오면실행
			{


				m_iAdjMovedIndex++;
			}

			break;
		case 2:

			break;
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

HRESULT CMiniGame_Golu::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	m_pMainCamera = reinterpret_cast<CCamera_Main*>(pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC,TAG_LAY(Layer_Camera_Main)));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMiniGame_Golu::Play_MiniGame(_double dDeltaTime)
{
	Keyboard_Input(dDeltaTime);
	return S_OK;
}

HRESULT CMiniGame_Golu::Keyboard_Input(_double dDeltatime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		m_pTransformCom->MovetoDir(XMVectorSet(0.f, 0.f, 1.f, 0.f), dDeltatime, m_pNavigationCom);
		//m_pTransformCom->Move_Forward(dDeltatime, m_pNavigationCom);
	}
	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		m_pTransformCom->MovetoDir(XMVectorSet(0.f, 0.f, -1.f, 0.f), dDeltatime, m_pNavigationCom);
		//m_pTransformCom->Move_Backward(dDeltatime, m_pNavigationCom);
	}
	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		m_pTransformCom->MovetoDir(XMVectorSet(-1.f, 0.f, 0.f, 0.f), dDeltatime, m_pNavigationCom);
		//m_pTransformCom->Move_Left(dDeltatime, m_pNavigationCom);
	}
	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		m_pTransformCom->MovetoDir(XMVectorSet(1.f, 0.f, 0.f, 0.f), dDeltatime, m_pNavigationCom);
		//m_pTransformCom->Move_Right(dDeltatime, m_pNavigationCom);
	}


	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));

	RELEASE_INSTANCE(CGameInstance);


	SkillNumber_Input(dDeltatime);
	Mouse_Input(dDeltatime);



	return S_OK;
}

HRESULT CMiniGame_Golu::SkillNumber_Input(_double dDeltatime)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_1) & DIS_Down)
	{
		m_dSkillNumber = 1;
	}

	if (pGameInstance->Get_DIKeyState(DIK_2) & DIS_Down)
	{
		m_dSkillNumber = 2;
	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMiniGame_Golu::Mouse_Input(_double dDeltatime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		CGolu_Bullet::GOLU_BULLETDESC Golu_BulletDesc;

		Golu_BulletDesc.iGoluBulletNumber = CGolu_Bullet::FIREBALL;
		Golu_BulletDesc.fScale = _float3(1.f, 1.f, 1.f);
		Golu_BulletDesc.fPositioning = _float3(0.f, 0.3f, 0.f);
		Golu_BulletDesc.fSpeed = 5.f;
		Golu_BulletDesc.dDuration = 100;

		Golu_BulletDesc.pObject = this;

		_float3 MousePickingPos;
		MousePickingPos = Check_MousePicking();
		MousePickingPos.y += 0.3f;
		Golu_BulletDesc.fDestinationPos = MousePickingPos;

		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Golu_Bullet), TAG_OP(Prototype_Object_Golu_Bullet), &Golu_BulletDesc));
	}





	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMiniGame_Golu::LookAt_MousePos(_float fWeight)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	_Vector vCursorPos = XMVectorSet(
		(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
		(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
		0, 1.f);

	_Matrix InvProjMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_PROJ));

	_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

	_Matrix InvViewMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
	vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


	CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	_Vector vCamPos = pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

	_float3 fResult;
	if (XMVectorGetY(vCamPos) * XMVectorGetY(vRayDir) < 0)
	{
		_float fPos_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		_float Scale = (XMVectorGetY(vCamPos) - fPos_Y) / -(XMVectorGetY(vRayDir));

		_float3 vTargetPos = vCamPos + (Scale)* vRayDir;

		fResult.x = vTargetPos.x;
		fResult.y = fPos_Y;
		fResult.z = vTargetPos.z;

		_Vector vTargetDir = XMVector3Normalize(XMLoadFloat3(&fResult) - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		m_pTransformCom->Turn_Dir(vTargetDir, fWeight);
	}

	return S_OK;
}

_float3 CMiniGame_Golu::Check_MousePicking()
{
	_float3 fPickingPos = _float3(0.f, 0.f, 0.f);

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_Vector vCursorPos = XMVectorSet(
		(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
		(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
		0, 1.f);

	_Matrix InvProjMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_PROJ));

	_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

	_Matrix InvViewMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
	vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


	CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	_Vector vCamPos = pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);


	_float fPos_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
	_float Scale = (XMVectorGetY(vCamPos) - fPos_Y) / -(XMVectorGetY(vRayDir));

	_float3 vTargetPos = vCamPos + (Scale)* vRayDir;

	fPickingPos.x = vTargetPos.x;
	fPickingPos.y = fPos_Y + 0.001f;
	fPickingPos.z = vTargetPos.z;

	return fPickingPos;
}

HRESULT CMiniGame_Golu::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));


	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "Skd_Hips", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.29397f, -0.010983f));
	m_vecAttachedDesc.push_back(tAttachedDesc);



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "Skd_Hips", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.29397f, -0.010983f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();
	return S_OK;
}

HRESULT CMiniGame_Golu::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Player, this, m_pColliderCom));

	FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 1.f, m_pNavigationCom));


	return S_OK;
}

HRESULT CMiniGame_Golu::Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag)
{
	{

		CGameInstance* pInstance = g_pGameInstance;

		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/Trigger/";
		lstrcat(szFullPath, szTriggerDataName);


		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
		{
			__debugbreak();
			return E_FAIL;
		}

		DWORD	dwByte = 0;
		_int iIDLength = 0;




		while (true)
		{



			_uint eNumber = 0;
			_tchar eObjectID[MAX_PATH];
			_float4x4 WorldMat = XMMatrixIdentity();
			_float4x4 ValueData = XMMatrixIdentity();
			_float4x4 SubValueData = XMMatrixIdentity();

			ZeroMemory(eObjectID, sizeof(_tchar) * MAX_PATH);

			ReadFile(hFile, &(eNumber), sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &(eObjectID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);

			ReadFile(hFile, &(WorldMat), sizeof(_float4x4), &dwByte, nullptr);
			ReadFile(hFile, &(ValueData), sizeof(_float4x4), &dwByte, nullptr);
			ReadFile(hFile, &(SubValueData), sizeof(_float4x4), &dwByte, nullptr);
			if (0 == dwByte) break;



			FAILED_CHECK(pInstance->Add_GameObject_To_Layer(eSceneID, pLayerTag, eObjectID, &eNumber));

			CTriggerObject* pObject = (CTriggerObject*)(pInstance->Get_GameObject_By_LayerLastIndex(eSceneID, pLayerTag));

			NULL_CHECK_RETURN(pObject, E_FAIL);

			pObject->Set_eNumberNObjectID(eNumber, eObjectID);

			((CTransform*)pObject->Get_Component(TAG_COM(Com_Transform)))->Set_Matrix(WorldMat);

			pObject->Set_ValueMat(&ValueData);
			pObject->Set_SubValueMat(&SubValueData);

			pObject->After_Initialize();

		}

		CloseHandle(hFile);
	}








	return S_OK;

	return S_OK;
}

HRESULT CMiniGame_Golu::Ready_Round()
{
	return S_OK;
}

HRESULT CMiniGame_Golu::Camera_Walking(_double dDeltaTime)
{
	Update_AttachCamPos();
	return S_OK;
}

CMiniGame_Golu * CMiniGame_Golu::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMiniGame_Golu*	pInstance = NEW CMiniGame_Golu(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_Golu");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMiniGame_Golu::Clone(void * pArg)
{
	CMiniGame_Golu*	pInstance = NEW CMiniGame_Golu(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_Golu");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMiniGame_Golu::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pHPUI);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
}
