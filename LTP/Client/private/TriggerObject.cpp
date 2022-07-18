#include "stdafx.h"
#include "..\public\TriggerObject.h"


CTriggerObject::CTriggerObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CTriggerObject::CTriggerObject(const CTriggerObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTriggerObject::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CTriggerObject::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));


	COLLIDERDESC         ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.4147592f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();


	return S_OK;
}

_int CTriggerObject::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;



	for (_uint i = 0; i < m_pColliderCom->Get_NumColliderBuffer(); i++)
		m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());



	return _int();
}

_int CTriggerObject::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;



#ifdef _DEBUG
	if (g_pGameInstance->Get_NowSceneNum() == SCENE_EDIT)
		FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CTriggerObject::Render()
{
	if (__super::Render() < 0)      return -1;

	return _int();
}

_int CTriggerObject::LateRender()
{
	return _int();
}

_int CTriggerObject::Active_Trigger(CGameObject* pTarget, _double fDeltaTime)
{
	return _int();
}

CTriggerObject::EParkourTriggerType  CTriggerObject::Get_ParkourTriggerType()
{
	return EParkourTriggerType();
}

void CTriggerObject::Free()
{
	__super::Free();

}
