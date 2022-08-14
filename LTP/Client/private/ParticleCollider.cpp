#include "stdafx.h"
#include "..\public\ParticleCollider.h"

CParticleCollider::CParticleCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CParticleCollider::CParticleCollider(const CParticleCollider & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticleCollider::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CParticleCollider::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
	{
		memcpy(&m_pColliderDesc, pArg, sizeof(SETTINGCOLLIDER));
	}
	Safe_AddRef(m_pColliderDesc.pTargetTransform);

	FAILED_CHECK(SetUp_Components());
	return S_OK;
}

_int CParticleCollider::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_pColliderDesc.pTargetTransform->Get_IsOwnerDead())
	{
		Set_IsDead();
	}

	_Matrix Matrix = XMMatrixIdentity();
	Matrix = m_pColliderDesc.pTargetTransform->Get_WorldMatrix(); /* * XMMatrixScaling(1.f, 3.f, 1.f);*/

	m_pCollider->Update_Transform(0, Matrix);
	m_pCollider->Update_ConflictPassedTime(1);
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	return _int();
}

_int CParticleCollider::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CParticleCollider::Render()
{
	if (__super::Render() < 0)
		return -1;

	return _int();
}

_int CParticleCollider::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

void CParticleCollider::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		//_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

HRESULT CParticleCollider::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(m_pColliderDesc.ColliderType, &m_pColliderDesc.ColliderDesc));

	return S_OK;
}

CParticleCollider * CParticleCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CParticleCollider*	pInstance = NEW CParticleCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CParticleCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CParticleCollider::Clone(void * pArg)
{
	CParticleCollider*	pInstance = NEW CParticleCollider(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CParticleCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticleCollider::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pColliderDesc.pTargetTransform);
}
