#include "stdafx.h"
#include "..\public\Chiedtuan_2Page_Weapon.h"

CChiedtuan_2Page_Weapon::CChiedtuan_2Page_Weapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CChiedtuan_2Page_Weapon::CChiedtuan_2Page_Weapon(const CChiedtuan_2Page_Weapon & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CChiedtuan_2Page_Weapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CChiedtuan_2Page_Weapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_WeaponDesc, sizeof(WEAPOPNDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_WeaponDesc, pArg, sizeof(WEAPOPNDESC));
	}

	FAILED_CHECK(SetUp_Components());

	m_pTransformCom->Scaled_All(_float3(10.f));

	switch (m_WeaponDesc.KatanaPOSType)
	{
	case Client::CChiedtuan_2Page_Weapon::KATANA_TR:
		m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.f));
		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_TL:
		m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-45.f));
		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BR:
		m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f));
		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BL:
		m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(90.f));
		break;
	}

	return S_OK;
}

_int CChiedtuan_2Page_Weapon::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	//FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));

	//if (g_pGameInstance->Get_DIKeyState(DIK_Z) & DIS_Down)
	//{
	//	m_pDissolveCom->Set_DissolveOn(false, 5.5f);
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_X) & DIS_Down)
	//{
	//	m_pDissolveCom->Set_DissolveOn(true, 1.5f);
	//}

	Set_WeaponPosition();


	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

	_Matrix mat = m_pTransformCom->Get_WorldMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	_Vector vPos = mat.r[3];

	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, mat);

	if (m_bIsAttack)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));
	}

	return _int();
}

_int CChiedtuan_2Page_Weapon::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel,nullptr, m_pDissolveCom));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	//if(m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TL)
		FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CChiedtuan_2Page_Weapon::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	FAILED_CHECK(m_pDissolveCom->Render(10));

	return _int();
}

_int CChiedtuan_2Page_Weapon::LateRender()
{
	return _int();
}

void CChiedtuan_2Page_Weapon::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

void CChiedtuan_2Page_Weapon::Set_WeaponPosition()
{
	_float3 vMyNewPos;
	_float3 vRight;
	CTransform* BossTransform = (CTransform*)m_WeaponDesc.BossObj->Get_Component(TAG_COM(Com_Transform));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
	//m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));

	vRight = BossTransform->Get_MatrixState(CTransform::STATE_RIGHT);
	vRight = XMVector3Normalize(vRight.XMVector());

	switch (m_WeaponDesc.KatanaPOSType)
	{
	case Client::CChiedtuan_2Page_Weapon::KATANA_TR:
		//m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.f));

		vMyNewPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) +
			(XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -3.f) +
			(vRight.XMVector() * -3.f);
		vMyNewPos.y += 2.f;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(35.f));

		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_TL:
		//m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-45.f));
		
		vMyNewPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -3.f) + (vRight.XMVector() * -1.5f);
		vMyNewPos.y += 2.f;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(15.f));

		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BR:
		//m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f));

		vMyNewPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -3.f) + (vRight.XMVector() * 1.5f);
		vMyNewPos.y += 2.f;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(-15.f));

		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BL:
		m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(90.f));
		vMyNewPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -3.f) + (vRight.XMVector() * 3.f);
		vMyNewPos.y += 2.f;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(-35.f));

		break;
	}
}

HRESULT CChiedtuan_2Page_Weapon::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TR)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TL)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon2), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_BR)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon3), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_BL)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon4), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	CDissolve::DISSOLVEDESC	tDissolveDesc;

	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TR)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(11.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 6.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TL)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(11.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 6.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_BR)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(13.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 6.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_BL)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(13.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.5f,0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 6.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}

	return S_OK;
}

CChiedtuan_2Page_Weapon * CChiedtuan_2Page_Weapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CChiedtuan_2Page_Weapon*	pInstance = NEW CChiedtuan_2Page_Weapon(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChiedtuan_2Page_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CChiedtuan_2Page_Weapon::Clone(void * pArg)
{
	CChiedtuan_2Page_Weapon*	pInstance = NEW CChiedtuan_2Page_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CChiedtuan_2Page_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChiedtuan_2Page_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pCollider);
}
