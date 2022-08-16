#include "stdafx.h"
#include "..\public\GoalTrigger.h"
#include "Scene_Loading.h"
#include "Scene_MiniGame_Jino.h"

CGoalTrigger::CGoalTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CGoalTrigger::CGoalTrigger(const CGoalTrigger & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGoalTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CGoalTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	_float3 vPos = *(static_cast<_float3*>(pArg));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_Etc());

	return S_OK;
}

_int CGoalTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	Update_Colliders();

	return _int();
}

_int CGoalTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CGoalTrigger::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CGoalTrigger::LateRender()
{
	return _int();
}

void CGoalTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (false == m_bActive && eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player)
	{
		static_cast<CScene_MiniGame_Jino*>(g_pGameInstance->Get_NowScene())->GameClear();
		m_bActive = true;
	}
}

void CGoalTrigger::Update_Colliders()
{
	m_pCollider->Update_Transform(0, m_pTransformCom->Get_WorldMatrix());
	m_pCollider->Update_Transform(1, m_pTransformCom->Get_WorldMatrix());

	g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pCollider);
}

HRESULT CGoalTrigger::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CGoalTrigger::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

HRESULT CGoalTrigger::SetUp_Etc()
{
	return S_OK;
}

CGoalTrigger * CGoalTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGoalTrigger*	pInstance = NEW CGoalTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGoalTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGoalTrigger::Clone(void * pArg)
{
	CGoalTrigger*	pInstance = NEW CGoalTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGoalTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoalTrigger::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
}
