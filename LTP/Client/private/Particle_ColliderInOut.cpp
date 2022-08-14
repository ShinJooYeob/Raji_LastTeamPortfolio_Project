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
		memcpy(&m_pColliderDesc, pArg, sizeof(SETTINGCOLLIDER));
	}
	Safe_AddRef(m_pColliderDesc.pTargetTransform);

	FAILED_CHECK(SetUp_Components());
	return S_OK;
}

_int CParticle_ColliderInOut::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

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
}

HRESULT CParticle_ColliderInOut::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

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
