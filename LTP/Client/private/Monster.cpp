#include "stdafx.h"
#include "..\public\Monster.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice,pDeviceContext)
{
}

CMonster::CMonster(const CMonster & rhs)
	:CGameObject(rhs)
{
}

HRESULT CMonster::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentIndex = 0;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc));

	FAILED_CHECK(Ready_ParticleDesc());
	m_EffectAdjust = 0;

	m_bisSpawnParticle = false;
	m_bIsSpawnPartilceOne = false;
	m_bIsInitParticle = false;

	m_SpawnEffectAdjust = 0;


	return S_OK;
}

_int CMonster::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)
		return UPDATE_ERROR;

	if (m_bisSpawnParticle == false)
	{
		Play_SpawnEffect();

		if (m_bIsSpawnPartilceOne == false)
		{
			m_bIsSpawnPartilceOne = true;
		}

		if (m_SpawnDealytime < 0)
		{
			m_bisSpawnParticle = true;
			return UPDATE_NONE;
		}
		m_SpawnDealytime -= dDeltaTime;
		return UPDATE_SKIP;
	}

	FAILED_CHECK(Update_Particle(dDeltaTime));

	return UPDATE_NONE;
}

_int CMonster::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)
		return UPDATE_ERROR;

	if (m_bisSpawnParticle == false)
		return UPDATE_SKIP;

	return UPDATE_NONE;
}

_int CMonster::Render()
{
	if (__super::Render() < 0)
		return UPDATE_ERROR;

	if (m_bisSpawnParticle == false)
		return UPDATE_SKIP;

	return UPDATE_NONE;
}

_int CMonster::LateRender()
{
	if (__super::LateRender() < 0)
		return UPDATE_ERROR;

	if (m_bisSpawnParticle == false)
		return UPDATE_SKIP;

	return UPDATE_NONE;
}

HRESULT CMonster::Ready_ParticleDesc()
{
	// Particle Ready

	return S_OK;
}



HRESULT CMonster::Play_SpawnEffect()
{

	return S_OK;
}

HRESULT CMonster::Update_Particle(_double timer)
{
	return S_OK;
}

HRESULT CMonster::Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ type, CTransform* trans, bool * pb)
{
	NULL_CHECK_BREAK(trans);

	if (trans)
	{
		if (trans->Get_IsOwnerDead() == true)
		{
			trans->Set_IsOwnerDead(false);
		}

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc_Hard(type, trans);
		if (pb)
			*pb = true;
	}

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);

}
