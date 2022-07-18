#include "stdafx.h"
#include "..\public\JumpTrigger.h"
#include "Player.h"

CJumpTrigger::CJumpTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CJumpTrigger::CJumpTrigger(const CJumpTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CJumpTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CJumpTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_EtcInfo());
	Set_IsOcllusion(false);

	return S_OK;
}

_int CJumpTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
//		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Trigger, this, m_pCollider));
	}

	return _int();
}

_int CJumpTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	return _int();
}

_int CJumpTrigger::Render()
{
	return _int();
}

_int CJumpTrigger::LateRender()
{
	return _int();
}

_int CJumpTrigger::Active_Trigger(CGameObject* pTarget, _double fDeltaTime)
{
	static_cast<CPlayer*>(pTarget)->Set_JumpPower(m_fJumpPower);

	return _int();
}

_int CJumpTrigger::DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime)
{
	return _int();
}

void CJumpTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		static_cast<CPlayer*>(pConflictedObj)->Set_JumpPower(m_fJumpPower);
	}
}

_float CJumpTrigger::Get_JumpPower()
{
	return m_fJumpPower;
}

void CJumpTrigger::Set_JumpPower(_float fJumpPower)
{
	m_fJumpPower = fJumpPower;
}

HRESULT CJumpTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CJumpTrigger::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

HRESULT CJumpTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CJumpTrigger * CJumpTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CJumpTrigger*	pInstance = NEW CJumpTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CJumpTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CJumpTrigger::Clone(void * pArg)
{
	CJumpTrigger*	pInstance = NEW CJumpTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CJumpTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJumpTrigger::Free()
{
	Safe_Release(m_pCollider);
}
