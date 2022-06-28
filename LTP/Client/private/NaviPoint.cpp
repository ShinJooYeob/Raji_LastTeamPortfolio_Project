#include "stdafx.h"
#include "..\public\NaviPoint.h"


CNaviPoint::CNaviPoint(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CNaviPoint::CNaviPoint(const CNaviPoint & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNaviPoint::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CNaviPoint::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	return S_OK;
}

_int CNaviPoint::Update(_double TimeDelta)
{
	if (__super::Update(TimeDelta) < 0)
		return -1;

	//m_pSphereCom->Update_Transform(m_pTransform->Get_WorldMatrix());
	m_pColliderCom->Update_Transform(0, m_pTransformCom->Get_WorldMatrix());

	return _int();
}

_int CNaviPoint::LateUpdate(_double TimeDelta)
{
	if (__super::LateUpdate(TimeDelta) < 0)
		return -1;

	return _int();
}

_int CNaviPoint::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return _int();
}

_int CNaviPoint::LateRender()
{
	return _int();
}

void CNaviPoint::CollisionTriger(_uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

void CNaviPoint::ReLocationCell(_float3 PointPos, _float3 RePointPos)
{
	for (_int i=0; i < m_vThisUsedCells.size(); ++i)
	{
		m_vThisUsedCells[i]->Set_ReLocation(PointPos, RePointPos);
	}
}

HRESULT CNaviPoint::SetUp_Components()
{
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(0);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));
	/* For.Com_SPHERE */
	COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	return S_OK;
}

CNaviPoint * CNaviPoint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg)
{
	CNaviPoint*	pInstance = new CNaviPoint(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CNaviPoint");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNaviPoint::Clone(void * pArg)
{
	CNaviPoint*	pInstance = new CNaviPoint(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CNaviPoint");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNaviPoint::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
}
