#include "stdafx.h"
#include "..\public\Chiedtuan_Weapon.h"



CChiedtuan_Weapon::CChiedtuan_Weapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CChiedtuan_Weapon::CChiedtuan_Weapon(const CChiedtuan_Weapon & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CChiedtuan_Weapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CChiedtuan_Weapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	m_pTransformCom->Scaled_All(_float3(1.f, 1.f, 1.f));

	m_pModel->Change_AnimIndex(0);

	m_fAttackCoolTime = 5.f;
	m_fSkillCoolTime = 8.f;
	m_bIsHit = false;

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum,
		TEXT("Layer_Player"));

	return S_OK;
}

_int CChiedtuan_Weapon::Update(_double fDeltaTime)
{
	return _int();
}

_int CChiedtuan_Weapon::LateUpdate(_double fDeltaTime)
{
	return _int();
}

_int CChiedtuan_Weapon::Render()
{
	return _int();
}

_int CChiedtuan_Weapon::LateRender()
{
	return _int();
}

HRESULT CChiedtuan_Weapon::SetUp_Components()
{
	return E_NOTIMPL;
}

HRESULT CChiedtuan_Weapon::Adjust_AnimMovedTransform(_double fDeltatime)
{
	return E_NOTIMPL;
}

CChiedtuan_Weapon * CChiedtuan_Weapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	return nullptr;
}

CGameObject * CChiedtuan_Weapon::Clone(void * pArg)
{
	return nullptr;
}

void CChiedtuan_Weapon::Free()
{
}
