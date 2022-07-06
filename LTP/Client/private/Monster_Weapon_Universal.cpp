#include "stdafx.h"
#include "..\public\Monster_Weapon_Universal.h"

const _tchar* m_pMonster_Weapon_UniversalTag[CMonster_Weapon_Universal::MONSTER_WEAPON_UNIVERSAL_END]
{
	TEXT("Gadesura_Black_Waepon.fbx"),
	TEXT("Gadesura_Rage_Waepon.fbx"),
};

CMonster_Weapon_Universal::CMonster_Weapon_Universal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CMonster_Weapon_Universal::CMonster_Weapon_Universal(const CMonster_Weapon_Universal & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CMonster_Weapon_Universal::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Weapon_Universal::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Monster_Weapon_UniversalDesc, pArg, sizeof(Monster_Weapon_UniversalDesc));
	}
	else {
		MSGBOX("Monster_Bullet의 pArg가 Nullptr입니다.");
	}


	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());


	SetUp_Info();
	SetUp_BoneMatrix();



	return S_OK;
}

_int CMonster_Weapon_Universal::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	return _int();
}

_int CMonster_Weapon_Universal::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));

	return _int();
}

_int CMonster_Weapon_Universal::Render()
{
	if (__super::Render() < 0)
		return -1;
	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint iNumMaterials = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModel->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModel->Render(m_pShaderCom, 3, i);
	}

	return _int();
}

_int CMonster_Weapon_Universal::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

void CMonster_Weapon_Universal::Set_IsDead()
{


	m_bIsDead = true;
}

HRESULT CMonster_Weapon_Universal::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, m_pMonster_Weapon_UniversalTag[m_Monster_Weapon_UniversalDesc.iMonsterWeaponMeshNumber], TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 100;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CMonster_Weapon_Universal::SetUp_BoneMatrix()
{
	return S_OK;
}

HRESULT CMonster_Weapon_Universal::SetUp_Info()
{

	return S_OK;
}

CMonster_Weapon_Universal * CMonster_Weapon_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Weapon_Universal*	pInstance = new CMonster_Weapon_Universal(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Weapon_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Weapon_Universal::Clone(void * pArg)
{
	CMonster_Weapon_Universal*	pInstance = new CMonster_Weapon_Universal(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Weapon_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Weapon_Universal::Free()
{

	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}