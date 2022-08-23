#include "stdafx.h"
#include "..\public\GaolCollider.h"
#include "Taiko_Monster.h"

CGaolCollider::CGaolCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CGaolCollider::CGaolCollider(const CGaolCollider & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGaolCollider::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CGaolCollider::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
	{
		memcpy(&m_GoalColliderDesc, pArg, sizeof(GOALCOLLIDERDESC));
	}

	FAILED_CHECK(SetUp_Components());

	m_pTransformCom->Scaled_All(_float3(1.f));
	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(-13.2f, 0.5f, -1.f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_GoalColliderDesc.vNotePosition);


	return S_OK;
}

_int CGaolCollider::Update(_double fDeltaTime)
{
	m_pTransformCom->Scaled_All(_float3(1.f));

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);
	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(-16.6f, 0.5f, -1.f));

	//Collider
	_uint	iNumCollider = m_pCollider->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
	{
		_Matrix mat = XMMatrixIdentity();
		mat.r[3] = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		m_pCollider->Update_Transform(i, mat);

	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider));
	return S_OK;

	return _int();
}

_int CGaolCollider::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CGaolCollider::Render()
{
	if (__super::Render() < 0)
		return -1;

	return _int();
}

_int CGaolCollider::LateRender()
{
	return _int();
}

void CGaolCollider::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (eConflictedObjCollisionType == CollisionType_Monster)
	{
		if (m_GoalColliderDesc.ColliderType == CGaolCollider::COLLIDER_DIE)
		{
			static_cast<CTaiko_Monster*>(pConflictedObj)->InitializeMonster();
		}
	}
}

HRESULT CGaolCollider::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	if (m_GoalColliderDesc.ColliderType == CGaolCollider::COLLIDER_GOOD)
	{
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);//2.8f
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	}
	else if (m_GoalColliderDesc.ColliderType == CGaolCollider::COLLIDER_DIE)
	{
		ColliderDesc.vScale = _float3(0.5f, 5.f, 1.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_AABB, &ColliderDesc));
	}

	return S_OK;
}

CGaolCollider* CGaolCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGaolCollider*	pInstance = NEW CGaolCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGaolCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGaolCollider::Clone(void * pArg)
{
	CGaolCollider*	pInstance = NEW CGaolCollider(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGaolCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGaolCollider::Free()
{
	__super::Free();

	Safe_Release(m_pCollider);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
