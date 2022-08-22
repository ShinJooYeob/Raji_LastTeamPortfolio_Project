#include "stdafx.h"
#include "..\public\PackMen.h"
#include "Scene.h"
#include "MiniGameBuilding.h"


CPackMen::CPackMen(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPackMen::CPackMen(const CPackMen & rhs)
	: CGameObject(rhs)
{
}


HRESULT CPackMen::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CPackMen::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
		m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	}



	return S_OK;
}

_int CPackMen::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	if (m_BustTimer > 0) m_BustTimer -= (_float)dDeltaTime;

	//0  : _float3(-22.586998	, 0.800010, 5.757167)
	//1  : _float3(-17.669325	, 0.800010, 5.757167)
	//2  : _float3(-14.834543	, 0.800010, 5.757167)
	//3  : _float3(-9.166116	, 0.800010, 5.757167)
	//4  : _float3(-6.165494	, 0.800010, 5.757167)
	//5  : _float3(-1.413313	, 0.800010, 5.757167)
	//6  : _float3(1.339999	, 0.800010, 5.757167)
	//7  : _float3(6.841322	, 0.800010, 5.757167)
	//8  : _float3(9.847200	, 0.800010, 5.757167)
	//9  : _float3(14.683441	, 0.800010, 5.757167)
	//10 : _float3(17.768784	, 0.800010, 5.757167)
	//11 : _float3(23.354647	, 0.800010, 5.757167)
	//12 : _float3(-9.166116, 0.800010, -0.583812)
	//13 : _float3(-6.165494, 0.800010, -0.583812)
	//14 : _float3(-1.413313, 0.800010, -0.583812)
	//15 : _float3(1.339999, 0.800010, -0.583812)
	//16 : _float3(6.841322, 0.800010, -0.583812)
	//17 : _float3(9.847200, 0.800010, -0.583812)
	//18 : _float3(-22.586998, 0.800010, -6.442939)
	//19 : _float3(-17.669325, 0.800010, -6.442939)
	//20 : _float3(-14.834543, 0.800010, -6.442939)
	//21 : _float3(-9.166116, 0.800010, -6.442939)
	//22 : _float3(-6.165494, 0.800010, -6.442939)
	//23 : _float3(-1.413313, 0.800010, -6.442939)
	//24 : _float3(1.339999, 0.800010, -6.442939)
	//25 : _float3(6.841322, 0.800010, -6.442939)
	//26 : _float3(9.847200, 0.800010, -6.442939)
	//27 : _float3(14.683441, 0.800010, -6.442939)
	//28 : _float3(17.768784, 0.800010, -6.442939)
	//29 : _float3(23.354647, 0.800010, -6.442939)


	FAILED_CHECK(Keyboard_Input(dDeltaTime));


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);

	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));


	Update_Collider(dDeltaTime);

	return _int();
}

_int CPackMen::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolve));

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));

	return _int();
}

_int CPackMen::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolve->Render(3)); //디졸브 내부에서 밑의 머테리얼을 찾아주고 있음



	//_float4x4 mat = XMMatrixIdentity();
	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &mat, sizeof(_float4x4)));


	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		if (m_iLifeCount > 0)
		{

			m_pSubTransform->Set_Matrix(XMMatrixScaling(2, 2, 2)*
				XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(90)) *
				XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(90)) *
				XMMatrixTranslation(-25, 5, 11));

			FAILED_CHECK(m_pSubTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

			FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));


			if (m_iLifeCount > 1)
			{


				m_pSubTransform->Set_Matrix(XMMatrixScaling(2, 2, 2)*
					XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(90)) *
					XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(90)) *
					XMMatrixTranslation(-23, 5, 11));

				FAILED_CHECK(m_pSubTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
				FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));


				if (m_iLifeCount > 2)
				{

					m_pSubTransform->Set_Matrix(XMMatrixScaling(2, 2, 2)*
						XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(90)) *
						XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(90)) *
						XMMatrixTranslation(-21, 5, 11));

					FAILED_CHECK(m_pSubTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
					FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));

				}
			}
		}
	}






#ifdef _DEBUG
	//FAILED_CHECK(m_pNavigationCom->Render());
#endif
	return _int();
}

_int CPackMen::LateRender()
{
	return _int();
}

HRESULT CPackMen::Added_Damaged()
{

	m_iLifeCount--;

	if (m_iLifeCount > 0)
	{
		CUtilityMgr*	pUtil = GetSingle(CUtilityMgr);
		pUtil->Set_HitEffect(0.3f, 1.f, _float3(1, 0, 0));

		m_BustTimer = 3.f;

	}
	else
	{
		CMiniGameBuilding::Copy_NowScreenToBuliding(CMiniGameBuilding::MINIGAME_PACKMAN);

		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
		g_pGameInstance->Get_NowScene()->Set_SceneChanging(SCENE_MINIGAME_PM);

	}


	g_pGameInstance->Play3D_Sound(TEXT("PM_Dead.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_PLAYER, 0.7f);

	return S_OK;
}

void CPackMen::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

HRESULT CPackMen::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PackMen), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_SubTransform), (CComponent**)&m_pSubTransform, &tDesc));
	



	m_pSubTransform->Set_Matrix(XMMatrixScaling(3, 3, 3)*
		XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(90)) *
		XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(90)) *
		XMMatrixTranslation(-25, 5, 12));

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


	FAILED_CHECK(SetUp_Collider());

	return S_OK;
}

HRESULT CPackMen::Adjust_AnimMovedTransform(_double dDeltatime)
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


HRESULT CPackMen::Keyboard_Input(_double dDeltatime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	static _double SoundTimer = 0;

	if (SoundTimer > 0)
		SoundTimer -= dDeltatime;


	_Vector vPressDir= 	(pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press) ? XMVectorSet(0.f, 0.f, 1.f, 0.f) :
		(pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press) ? XMVectorSet(0.f, 0.f, -1.f, 0.f) :
		(pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press) ? XMVectorSet(-1.f, 0.f, 0.f, 0.f) :
		(pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press) ? XMVectorSet(1.f, 0.f, 0.f, 0.f) : 
		XMVectorSet(0.f, 0.f, 0.f, 0.f);


	if (XMVectorGetX(XMVector3Length(vPressDir)) < 1)
	{
		m_pModel->Change_AnimIndex(0);
	}
	else
	{
		if (SoundTimer <= 0)
		{
			
			SoundTimer = 0.548f;
			g_pGameInstance->Play3D_Sound(TEXT("PM_Walk.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_PLAYER, 0.35f);
		}
		m_pModel->Change_AnimIndex(1);
		m_pTransformCom->Turn_Dir(vPressDir, 0.9f);
		m_pTransformCom->MovetoDir(vPressDir, dDeltatime, m_pNavigationCom);
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, 
			m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(0,0.5f,0,0)));
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPackMen::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));


	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));



	return S_OK;
}

HRESULT CPackMen::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Player, this, m_pColliderCom));

	return S_OK;
}


CPackMen * CPackMen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPackMen*	pInstance = NEW CPackMen(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPackMen");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPackMen::Clone(void * pArg)
{
	CPackMen*	pInstance = NEW CPackMen(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPackMen");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPackMen::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pSubTransform);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);
	
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
} 
