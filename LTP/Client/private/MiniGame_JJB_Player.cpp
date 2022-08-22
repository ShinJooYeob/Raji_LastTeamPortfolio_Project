#include "stdafx.h"
#include "..\public\MiniGame_JJB_Player.h"
#include "PlayerWeapon_Sword.h"

CMiniGame_JJB_Player::CMiniGame_JJB_Player(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMiniGame_JJB_Player::CMiniGame_JJB_Player(const CMiniGame_JJB_Player & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMiniGame_JJB_Player::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMiniGame_JJB_Player::Initialize_Clone(void * pArg)
{
	CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;

	SetUp_Components();

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(-24.5f, -2.f, -1.f));
	m_pTransformCom->Scaled_All(_float3(0.4f));
	m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

	//////// Create Player Weapon Sword //
	//eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	//eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_Object_MiniGame_PlayerWeapon), &eWeaponDesc));
	//m_pPlayerWeapons = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 0));

	return S_OK;
}

_int CMiniGame_JJB_Player::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)		return -1;


	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(-24.5f, -2.f, -1.f));
	m_pModel->Change_AnimIndex(0);
	//m_pTransformCom->Scaled_All(_float3(0.4f));
	//m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);

	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));
	Adjust_AnimMovedTransform(fDeltaTime);

	//m_pTransformCom->Scaled_All()

	return _int();
}

_int CMiniGame_JJB_Player::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)		return -1;

	m_fRenderSortValue = -2.f;
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this));

	return _int();
}

_int CMiniGame_JJB_Player::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPEFORENGINE(j)));
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	}
	return _int();
}

_int CMiniGame_JJB_Player::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

HRESULT CMiniGame_JJB_Player::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MiniGame_JJB_Player), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(1));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CMiniGame_JJB_Player::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;

	if (PlayRate <= 0.98)
	{

	}
	else
	{

	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMiniGame_JJB_Player * CMiniGame_JJB_Player::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMiniGame_JJB_Player*	pInstance = NEW CMiniGame_JJB_Player(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_JJB_Player");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMiniGame_JJB_Player::Clone(void * pArg)
{
	CMiniGame_JJB_Player*	pInstance = NEW CMiniGame_JJB_Player(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_JJB_Player");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMiniGame_JJB_Player::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModel);
}
