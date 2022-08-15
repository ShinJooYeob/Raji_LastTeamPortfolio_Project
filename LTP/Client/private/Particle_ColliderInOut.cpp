#include "stdafx.h"
#include "..\public\Particle_ColliderInOut.h"

CParticle_ColliderInOut::CParticle_ColliderInOut(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CParticle_ColliderInOut::CParticle_ColliderInOut(const CParticle_ColliderInOut & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticle_ColliderInOut::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CParticle_ColliderInOut::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
	{
		memcpy(&m_pColliderDesc, pArg, sizeof(SETTINGCOLLIDERINOUT));
	}
	Safe_AddRef(m_pColliderDesc.pTargetTransform);

	FAILED_CHECK(SetUp_Components());
	return S_OK;
}

_int CParticle_ColliderInOut::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	//if (m_pColliderDesc.pTargetTransform->Get_IsOwnerDead())
	//{
	//	Set_IsDead();
	//}

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();
	m_pCollider->Update_ConflictPassedTime(1);

	_Matrix Matrix = XMMatrixIdentity();
	Matrix = m_pColliderDesc.pTargetTransform->Get_WorldMatrix(); /* * XMMatrixScaling(1.f, 3.f, 1.f);*/


	for (_uint i = 0; i < iNumCollider; i++)
	{
		m_pCollider->Update_Transform(i, Matrix);
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	return _int();
}

_int CParticle_ColliderInOut::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CParticle_ColliderInOut::Render()
{
	if (__super::Render() < 0)
		return -1;

	return _int();
}

_int CParticle_ColliderInOut::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CParticle_ColliderInOut::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (iMyColliderIndex == 2)
	{
		pConflictedObj->Take_Damage(this, 1.f, XMVector3Normalize(pMyCollider->Get_ColliderPosition(iMyColliderIndex).XMVector() - pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector()), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

HRESULT CParticle_ColliderInOut::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(m_pColliderDesc.ColliderDesc.vScale.x + 40.f, m_pColliderDesc.ColliderDesc.vScale.y + 40.f,
		m_pColliderDesc.ColliderDesc.vScale.z + 40.f);
	ColliderDesc.vRotation = m_pColliderDesc.ColliderDesc.vRotation;
	ColliderDesc.vPosition = m_pColliderDesc.ColliderDesc.vPosition;
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(m_pColliderDesc.ColliderType, &ColliderDesc));

	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(m_pColliderDesc.ColliderType, &m_pColliderDesc.ColliderDesc));
	m_pCollider->Set_ParantBuffer(0);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(m_pColliderDesc.ColliderDesc.vScale.x + 15.f, m_pColliderDesc.ColliderDesc.vScale.y + 15.f,
		m_pColliderDesc.ColliderDesc.vScale.z + 15.f);
	ColliderDesc.vRotation = m_pColliderDesc.ColliderDesc.vRotation;
	ColliderDesc.vPosition = m_pColliderDesc.ColliderDesc.vPosition;


	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(m_pColliderDesc.ColliderType, &ColliderDesc));
	m_pCollider->Set_ParantBuffer(0);


	return S_OK;
}

CParticle_ColliderInOut * CParticle_ColliderInOut::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CParticle_ColliderInOut*	pInstance = NEW CParticle_ColliderInOut(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CParticle_ColliderInOut");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CParticle_ColliderInOut::Clone(void * pArg)
{
	CParticle_ColliderInOut*	pInstance = NEW CParticle_ColliderInOut(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CParticle_ColliderInOut");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle_ColliderInOut::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pColliderDesc.pTargetTransform);	
}
