#include "stdafx.h"
#include "..\public\Taiko_Monster.h"

CTaiko_Monster::CTaiko_Monster(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTaiko_Monster::CTaiko_Monster(const CTaiko_Monster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTaiko_Monster::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CTaiko_Monster::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	return S_OK;
}

_int CTaiko_Monster::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	return _int();
}

_int CTaiko_Monster::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	return _int();
}

_int CTaiko_Monster::Render()
{
	if (__super::Render() < 0)
		return -1;


	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolve->Render(3)); //디졸브 내부에서 밑의 머테리얼을 찾아주고 있음


	return _int();
}

_int CTaiko_Monster::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CTaiko_Monster::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

HRESULT CTaiko_Monster::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PMMonster), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));
	FAILED_CHECK(m_pModel->Update_AnimationClip(g_fDeltaTime, true));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = GetSingle(CUtilityMgr)->RandomFloat(2.5f, 5.f);
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 4;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));

	FAILED_CHECK(SetUp_Collider());

	return S_OK;
}

HRESULT CTaiko_Monster::Adjust_AnimMovedTransform(_double dDeltatime)
{
	return S_OK;
}

HRESULT CTaiko_Monster::SetUp_Collider()
{
	return S_OK;
}

HRESULT CTaiko_Monster::Update_Collider(_double dDeltaTime)
{
	return S_OK;
}

CTaiko_Monster * CTaiko_Monster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	return nullptr;
}

CGameObject * CTaiko_Monster::Clone(void * pArg)
{
	return nullptr;
}

void CTaiko_Monster::Free()
{
}
