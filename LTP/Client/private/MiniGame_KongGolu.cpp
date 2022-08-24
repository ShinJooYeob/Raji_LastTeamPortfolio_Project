#include "stdafx.h"
#include "..\public\MiniGame_KongGolu.h"
#include "Scene.h"


CMiniGame_KongGolu::CMiniGame_KongGolu(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMiniGame_KongGolu::CMiniGame_KongGolu(const CMiniGame_KongGolu & rhs)
	: CGameObject(rhs)
{
}


HRESULT CMiniGame_KongGolu::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMiniGame_KongGolu::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));


	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(180));




	//m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));


	SetUp_Info();


	m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, -1.f, 0.f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(38.513f, 46.53f, 39.5f, 1.f));

	return S_OK;
}

_int CMiniGame_KongGolu::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	if (m_bClear == true)
	{
		m_dClearTime += dDeltaTime;

		if (m_bSoundIndex == 0)
		{
			g_pGameInstance->Play3D_Sound(TEXT("EH_DonkeyKong_Clear.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
			m_bSoundIndex++;
		}

		if (m_dClearTime > 2)
		{
			g_pGameInstance->Get_NowScene()->Set_SceneChanging(SCENE_LOBY);
		}
	}

	Play_MiniGame(dDeltaTime);


	m_pModel->Change_AnimIndex(0, 0.f);


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);


	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	Update_Collider(dDeltaTime);

	return _int();
}

_int CMiniGame_KongGolu::LateUpdate(_double dDeltaTime)
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

	return _int();
}

_int CMiniGame_KongGolu::Render()
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
	//FAILED_CHECK(m_pNavigationCom->Render());
#endif
	return _int();
}

_int CMiniGame_KongGolu::LateRender()
{
	return _int();
}

void CMiniGame_KongGolu::Set_IsDead()
{
	__super::Set_IsDead();

}

void CMiniGame_KongGolu::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	m_bClear = true;
}

_float CMiniGame_KongGolu::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	//m_pHPUI->Set_ADD_HitCount((_int)fDamageAmount);
	//m_fHP += -fDamageAmount;


	if (0 >= m_fHP)
	{
		return -1.f;
	}

	return _float();
}

HRESULT CMiniGame_KongGolu::SetUp_Components()
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

HRESULT CMiniGame_KongGolu::Adjust_AnimMovedTransform(_double dDeltatime)
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

HRESULT CMiniGame_KongGolu::SetUp_Info()
{
	return S_OK;
}

HRESULT CMiniGame_KongGolu::Play_MiniGame(_double dDeltaTime)
{
	return S_OK;
}

HRESULT CMiniGame_KongGolu::SetUp_Collider()
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

HRESULT CMiniGame_KongGolu::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pColliderCom));

	return S_OK;
}

CMiniGame_KongGolu * CMiniGame_KongGolu::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMiniGame_KongGolu*	pInstance = NEW CMiniGame_KongGolu(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_KongGolu");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMiniGame_KongGolu::Clone(void * pArg)
{
	CMiniGame_KongGolu*	pInstance = NEW CMiniGame_KongGolu(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_KongGolu");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMiniGame_KongGolu::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
}
