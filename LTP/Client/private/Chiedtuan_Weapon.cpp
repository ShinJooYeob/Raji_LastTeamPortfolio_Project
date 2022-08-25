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

	ZeroMemory(&m_WeaponDesc, sizeof(WEAPOPNDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_WeaponDesc, pArg, sizeof(WEAPOPNDESC));
	}

	FAILED_CHECK(SetUp_Components());

	//m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(170.f));
	m_pTransformCom->Scaled_All(_float3(0.8f));
	return S_OK;
}

_int CChiedtuan_Weapon::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;



	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, m_AttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	if(m_bIsAttack)
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	if (m_StopTrailTimer > 0)
	{
		m_StopTrailTimer -= _float(fDeltaTime);

		_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_WeaponDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix();
		m_pSwordTrail->Update_SwordTrail(mat.r[3], mat.r[3] + XMVector3Normalize(mat.r[1])  * m_SwordTrailLength, g_fDeltaTime);

		if (m_StopTrailTimer <= 0)
		{
			m_pSwordTrail->Set_TrailTurnOn(false, _float3(0), _float3(0));
		}
	}
	else
	{
		 _Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_WeaponDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix();
		 m_pSwordTrail->Update_SwordTrail(mat.r[3], mat.r[3] + XMVector3Normalize(mat.r[1])  * m_SwordTrailLength, g_fDeltaTime);

	}
	return _int();
}

_int CChiedtuan_Weapon::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	Update_AttachMatrix();
	//m_pTransformCom->Scaled_All(_float3(1.f, 1.f, 1.f));


	//_Matrix mat = m_fAttachedMatrix.XMatrix();

	//mat.r[0] = XMVector3Normalize(mat.r[0]);
	//mat.r[1] = XMVector3Normalize(mat.r[1]);
	//mat.r[2] = XMVector3Normalize(mat.r[2]);


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &_float4x4(m_fAttachedMatrix),m_pDissolveCom));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_SWORD, m_pSwordTrail));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();




	return _int();
}

_int CChiedtuan_Weapon::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(8));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();
	//
	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//	{
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
	//	}
	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 8, i));
	//}

	return _int();
}

_int CChiedtuan_Weapon::LateRender()
{
	return _int();
}

void CChiedtuan_Weapon::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

void CChiedtuan_Weapon::SwordTrailOnOff(_bool bBool)
{
	if (bBool)
	{
		m_StopTrailTimer = -1.f;
		m_pSwordTrail->Set_TrailTurnOn(false, _float3(0), _float3(0));
		_Vector vDir = XMVector3Normalize(m_pCollider->Get_ColliderPosition(3).XMVector() - m_pCollider->Get_ColliderPosition(1).XMVector());
		m_pSwordTrail->Set_TrailTurnOn(bBool, m_pCollider->Get_ColliderPosition(1), m_pCollider->Get_ColliderPosition(1).XMVector() + vDir * m_SwordTrailLength);
	}
	else
	{
		_Vector vDir = XMVector3Normalize(m_pCollider->Get_ColliderPosition(3).XMVector() - m_pCollider->Get_ColliderPosition(1).XMVector());
		m_ArrStopSwordTrailPos[0] = m_pCollider->Get_ColliderPosition(1);
		m_ArrStopSwordTrailPos[1] = m_pCollider->Get_ColliderPosition(1).XMVector() + vDir * m_SwordTrailLength;
		m_StopTrailTimer = 0.25f;
	}

}

void CChiedtuan_Weapon::Set_Dissolve(_bool FadeIn, _double Time)
{
	m_pDissolveCom->Set_DissolveOn(FadeIn, Time);
}

void CChiedtuan_Weapon::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_WeaponDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix();
}

HRESULT CChiedtuan_Weapon::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	CSwordTrail::TRAILDESC tSwordDesc;
	tSwordDesc.iPassIndex = 0;
	tSwordDesc.vColor = _float4(1.f, 0.2f, 0.1f, 1.f);
	tSwordDesc.iTextureIndex = 1;
	tSwordDesc.NoiseSpeed = 0;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_SwordTrail), TAG_COM(Com_SwordTrail), (CComponent**)&m_pSwordTrail, &tSwordDesc));

	if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_TR)
	{
		m_SwordTrailLength = 7.f;
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_TL)
	{
		m_SwordTrailLength = 5.65f;
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon2), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_BR)
	{
		m_SwordTrailLength = 6.f;
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon3), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_BL)
	{
		m_SwordTrailLength = 7.f;
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon4), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));



	CDissolve::DISSOLVEDESC	tDissolveDesc;

	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM_ATTACHED;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_TR)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(11.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, -3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		ATTACHEDESC tAttachedDesc;
		tAttachedDesc.Initialize_AttachedDesc(m_WeaponDesc.BossObj, "middle_metacarpal_r", _float3(1), _float3(0), _float3(-2.03656f, -0.227399f, -2.99337f));
		m_AttachedDesc = tAttachedDesc;

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, -1.5f, 1.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, -3.5f, 1.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, -6.f, 1.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_TL)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(11.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, -3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		ATTACHEDESC tAttachedDesc;
		tAttachedDesc.Initialize_AttachedDesc(m_WeaponDesc.BossObj, "middle_metacarpal_l", _float3(1), _float3(0), _float3(2.03655f, -0.227398f, -2.99338f));
		m_AttachedDesc = tAttachedDesc;

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, -1.5f, 1.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, -3.5f, 1.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, -6.f, 1.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_BR)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(13.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		ATTACHEDESC tAttachedDesc;
		tAttachedDesc.Initialize_AttachedDesc(m_WeaponDesc.BossObj, "middle_02_r_01", _float3(1), _float3(0), _float3(-1.53831f, -0.850419f, -2.53354f));
		m_AttachedDesc = tAttachedDesc;

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.5f, 1.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 6.f, 6.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else if (m_WeaponDesc.m_KatanaPOS == CChiedtuan_Weapon::KATANA_BL)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(13.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		ATTACHEDESC tAttachedDesc;
		tAttachedDesc.Initialize_AttachedDesc(m_WeaponDesc.BossObj, "middle_02_l_01", _float3(1), _float3(0), _float3(1.53831f, -0.850418f, -2.53354f));
		m_AttachedDesc = tAttachedDesc;

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.5f, 1.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 3.5f, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 6.f, 6.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}


	return S_OK;
}

CChiedtuan_Weapon * CChiedtuan_Weapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CChiedtuan_Weapon*	pInstance = NEW CChiedtuan_Weapon(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChiedtuan_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CChiedtuan_Weapon::Clone(void * pArg)
{
	CChiedtuan_Weapon*	pInstance = NEW CChiedtuan_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CChiedtuan_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChiedtuan_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pSwordTrail);
	
}
