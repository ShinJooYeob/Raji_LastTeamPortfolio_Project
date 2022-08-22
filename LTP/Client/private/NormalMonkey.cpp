#include "stdafx.h"
#include "NormalMonkey.h"
#include "MiniGame_Jino_Player.h"
#include "Scene_MiniGame_Jino.h"

CNormalMonkey::CNormalMonkey(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CMiniGame_Jino_Monster(pDevice, pDeviceContext)
{
}

CNormalMonkey::CNormalMonkey(const CNormalMonkey & rhs)
	: CMiniGame_Jino_Monster(rhs)
{
}

HRESULT CNormalMonkey::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CNormalMonkey::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Colliders());

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
	}

	FAILED_CHECK(SetUp_Etc());

	return S_OK;
}

_int CNormalMonkey::Update(_double fDeltaTime)
{
	if (false == m_bActive)
		return 0;

	if (__super::Update(fDeltaTime) < 0) return -1;

	SetUp_MonsterType(m_eMonsterType);

	m_pTransformCom->LookDir(XMVectorSet(-1.f, 0.f, 0.f, 0.f));

	Update_Move(fDeltaTime);
	
	Update_Collider(fDeltaTime);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));

	return _int();
}

_int CNormalMonkey::LateUpdate(_double fDeltaTime)
{
	if (false == m_bActive)
		return 0;

	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolveCom));

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
#endif // _DEBUG

	return _int();
}

_int CNormalMonkey::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(13));

	return _int();
}

_int CNormalMonkey::LateRender()
{
	return _int();
}

void CNormalMonkey::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (eConflictedObjCollisionType == CollisionType_Player)
	{
		static_cast<CMiniGame_Jino_Player*>(pConflictedObj)->Set_State_DeadStart();
	}
}

_float CNormalMonkey::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

void CNormalMonkey::Dead()
{
	m_bActive = false;
	static_cast<CScene_MiniGame_Jino*>(g_pGameInstance->Get_NowScene())->Push_NormalMonkey(this);
}

void CNormalMonkey::SetUp_MonsterType(EMONSTER_TYPE eMonsterType)
{
	switch (eMonsterType)
	{
	case EMONSTER_TYPE::TYPE_WALKER:
		m_eMonsterType = eMonsterType;
		m_fAnimSpeed = 2.f;
		m_pModel->Change_AnimIndex(1);
		m_pTransformCom->Set_MoveSpeed(2.f);
		break;
	case EMONSTER_TYPE::TYPE_RUNNER:
		m_eMonsterType = eMonsterType;
		m_fAnimSpeed = 1.f;
		m_pModel->Change_AnimIndex(21);
		m_pTransformCom->Set_MoveSpeed(7.f);
		break;
	default:
		MSGBOX("eMonster Type is Unknown Monster Type ");
	}
}

void CNormalMonkey::Set_Active(_bool bActive)
{
	m_bActive = bActive;
}

void CNormalMonkey::Set_Position(_float3 vPos)
{
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

HRESULT CNormalMonkey::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Mahinasura_Minion), TAG_COM(Com_Model), (CComponent**)&m_pModel));
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
	DissolveDesc.RampTextureIndex = 0;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &DissolveDesc));

	return S_OK;
}

HRESULT CNormalMonkey::SetUp_Colliders()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -0.5f));
	m_vecAttachedDesc.push_back(tAttachedDesc);


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.8f, 0.8f, 0.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_head", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -0.8f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.8f, 0.8f, 0.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine_02", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.1f, -0.45f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_cloth_02", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.15f, -0.2f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

HRESULT CNormalMonkey::SetUp_Etc()
{
	m_pPlayerTransform = static_cast<CTransform*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player))->Get_Component(Tag_Component(Com_Transform)));
	m_bActive = false;
	m_pTransformCom->Scaled_All(_float3(1.5f));
	return S_OK;
}

HRESULT CNormalMonkey::Update_Move(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_float fDist = XMVectorGetX(vPos) - XMVectorGetX(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

	if (3.5f >= XMVectorGetX(vPos) || -7.f >= fDist || 28.f <= fDist)
	{
		Dead();
	}

	return S_OK;
}

HRESULT CNormalMonkey::Update_Collider(_double fDeltaTime)
{
	//Collider
	_uint	iNumCollider = m_pCollider->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	return S_OK;
}

CNormalMonkey * CNormalMonkey::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CNormalMonkey*	pInstance = NEW CNormalMonkey(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CNormalMonkey");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNormalMonkey::Clone(void * pArg)
{
	CNormalMonkey*	pInstance = NEW CNormalMonkey(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CNormalMonkey");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNormalMonkey::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pCollider);
}
