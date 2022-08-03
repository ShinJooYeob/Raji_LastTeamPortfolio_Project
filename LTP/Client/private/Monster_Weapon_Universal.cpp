#include "stdafx.h"
#include "..\public\Monster_Weapon_Universal.h"
#include "Monster_Gadasura_Black.h"
#include "Monster_Gadasura_Rage.h"

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
	m_fAttachedMatrix = XMMatrixIdentity();


	return S_OK;
}

_int CMonster_Weapon_Universal::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	if (m_pMonster_Object->Get_NowHP() <= 0)
	{
		m_pDissolve->Update_Dissolving(dDeltaTime);
		m_pDissolve->Set_DissolveOn(false, 2.f);
	}

	Update_AttachMatrix();


	Update_Weapon(dDeltaTime);


	Update_Collider(dDeltaTime);

	return _int();
}

_int CMonster_Weapon_Universal::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	/*FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));*/


	_Matrix mat = m_fAttachedMatrix.XMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &_float4x4(mat), m_pDissolve));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

	if(m_pColliderCom != nullptr)
		FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
	//Caculate_AttachedBone //디폴트피벗*  뼈의 디폴트 뼈 *  뼈의 오픈셋 * 컴바인드 * 부모의 트랜스폼,

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
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolve->Render(8)); //디졸브 내부에서 밑의 머테리얼을 찾아주고 있음


	//_uint NumMaterial = m_pModel->Get_NumMaterial();

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

void CMonster_Weapon_Universal::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMonster_Weapon_Universal::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
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

	m_pMonster_Object = static_cast<CGameObject*>(m_Monster_Weapon_UniversalDesc.Object);
	m_pMonster_Model = static_cast<CModel*>(m_pMonster_Object->Get_Component(TAG_COM(Com_Model)));


	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM_ATTACHED;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));


	SetUp_Collider();

	return S_OK;
}

HRESULT CMonster_Weapon_Universal::SetUp_Info()
{
	return S_OK;
}

HRESULT CMonster_Weapon_Universal::SetUp_Collider()
{
	switch (m_Monster_Weapon_UniversalDesc.iMonsterWeaponMeshNumber)
	{
	case CMonster_Weapon_Universal::GADASURA_BLACK_WEAPON:
	{
		CMonster_Gadasura_Black* Gadasura_Black = static_cast<CMonster_Gadasura_Black*>(m_Monster_Weapon_UniversalDesc.Object);
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

		///////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(10.f, 10.f, 10.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		ATTACHEDESC tAttachedDesc;
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Black, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		tAttachedDesc = ATTACHEDESC();
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Black, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);
		m_pColliderCom->Set_ParantBuffer();


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(-0.55f, -1.1f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		tAttachedDesc = ATTACHEDESC();
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Black, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);
		m_pColliderCom->Set_ParantBuffer();


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(-0.6f, -1.8f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		tAttachedDesc = ATTACHEDESC();
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Black, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);
		m_pColliderCom->Set_ParantBuffer();


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(-0.7f, -2.7f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		tAttachedDesc = ATTACHEDESC();
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Black, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);
		m_pColliderCom->Set_ParantBuffer();
		break;
	}
	case CMonster_Weapon_Universal::GADASURA_RAGE_WEAPON:
	{
		CMonster_Gadasura_Rage* Gadasura_Rage = static_cast<CMonster_Gadasura_Rage*>(m_Monster_Weapon_UniversalDesc.Object);
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

		///////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(10.f, 10.f, 10.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		ATTACHEDESC tAttachedDesc;
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Rage, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		tAttachedDesc = ATTACHEDESC();
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Rage, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);
		m_pColliderCom->Set_ParantBuffer();


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(-0.55f, -1.1f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		tAttachedDesc = ATTACHEDESC();
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Rage, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);
		m_pColliderCom->Set_ParantBuffer();


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(-0.6f, -1.8f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		tAttachedDesc = ATTACHEDESC();
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Rage, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);
		m_pColliderCom->Set_ParantBuffer();


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(-0.7f, -2.7f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		tAttachedDesc = ATTACHEDESC();
		tAttachedDesc.Initialize_AttachedDesc(Gadasura_Rage, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-1.5748f, 0.075877f, -2.0999f));
		m_vecAttachedDesc.push_back(tAttachedDesc);
		m_pColliderCom->Set_ParantBuffer();
	}
	default:
		break;
	}

	return S_OK;
}

HRESULT CMonster_Weapon_Universal::SetUp_BoneMatrix()
{
	return S_OK;
}

HRESULT CMonster_Weapon_Universal::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_Monster_Weapon_UniversalDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();



//	_Matrix tt = m_pTransformCom->Get_WorldMatrix()  * m_Monster_Weapon_UniversalDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
//	_Matrix t2 = m_fAttachedMatrix.TransposeXMatrix();
//
//#define Rate 0.8f
//
//
//	tt.r[0] = tt.r[0] * (1.f - Rate) + t2.r[0] * Rate;
//	tt.r[1] = tt.r[1] * (1.f - Rate) + t2.r[1] * Rate;
//	tt.r[2] = tt.r[2] * (1.f - Rate) + t2.r[2] * Rate;
//	tt.r[3] = tt.r[3];
//
//	m_fAttachedMatrix = tt;

	return S_OK;
}

HRESULT CMonster_Weapon_Universal::Update_Collider(_double dDeltaTime)
{

	switch (m_Monster_Weapon_UniversalDesc.iMonsterWeaponMeshNumber)
	{
	case GADASURA_BLACK_WEAPON:
	{
		CMonster_Gadasura_Black* Gadasura_Black = static_cast<CMonster_Gadasura_Black*>(m_Monster_Weapon_UniversalDesc.Object);
		m_WeaponAttackOn = Gadasura_Black->Get_WeaponAttackSwitch();

		m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

		if (m_WeaponAttackOn == true)
		{
			_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
			for (_uint i = 0; i < iNumCollider; i++)
			{
				m_pColliderCom->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());
			} 
			FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pColliderCom));
		}
		break;
	}
	case GADASURA_RAGE_WEAPON:
	{
		CMonster_Gadasura_Rage* Gadasura_Rage = static_cast<CMonster_Gadasura_Rage*>(m_Monster_Weapon_UniversalDesc.Object);
		m_WeaponAttackOn = Gadasura_Rage->Get_WeaponAttackSwitch();

		m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

		if (m_WeaponAttackOn == true)
		{
			_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
			for (_uint i = 0; i < iNumCollider; i++)
			{
				m_pColliderCom->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());
			}
			FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pColliderCom));
		}
		break;
	}
	default:
		break;
	}

	return S_OK;
}

HRESULT CMonster_Weapon_Universal::Update_Weapon(_double dDeltaTime)
{
	switch (m_Monster_Weapon_UniversalDesc.iMonsterWeaponMeshNumber)
	{
	case GADASURA_BLACK_WEAPON:
		Gadasura_Black_Weapon(dDeltaTime);
		break;
	case GADASURA_RAGE_WEAPON:
		Gadasura_Rage_Weapon(dDeltaTime);
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CMonster_Weapon_Universal::Gadasura_Black_Weapon(_double dDeltaTime)
{

	_double PlayRate = m_pMonster_Model->Get_PlayRate();

	if (m_pMonster_Model->Get_NowAnimIndex()== 20)
	{
		/*
			//_Matrix mat = m_fAttachedMatrix.XMatrix();

			//mat.r[0] = XMVectorSet(1, 0, 0, 0) * XMVector3Length(mat.r[0]);
			//mat.r[1] = XMVectorSet(0, 1, 0, 0) * XMVector3Length(mat.r[1]);
			//mat.r[2] = XMVectorSet(0, 0, 1, 0) * XMVector3Length(mat.r[2]);

			//m_fAttachedMatrix = mat;

			//if (PlayRate <= 0.1588785046)
			//{

			//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(110));

			//}
			//else if (PlayRate < 0.214953271)
			//{

			//	_float Angle = g_pGameInstance->Easing(TYPE_Linear, 110, 0, PlayRate - 0.1588785046f, 0.0560747664);

			//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(Angle));

			//}
			//else if (PlayRate <= 0.654205607)
			//{
			//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(0));

			//}
			//else if (PlayRate <= 0.8130841121495327)
			//{
			//	_float Angle = g_pGameInstance->Easing(TYPE_SinInOut, 0, 110, PlayRate - 0.654205607, 0.1588785051495327);
			//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(Angle));
			//}
			//else
			//{
			//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(110));

			//}
			//m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() *mat;



			*/

		
		if (PlayRate > 0.1495327102803738 && PlayRate <= 0.7383177570093458)
		{
			_Matrix mat = m_fAttachedMatrix.XMatrix();

			mat.r[0] = XMVectorSet(1, 0, 0, 0) * XMVector3Length(mat.r[0]);
			mat.r[1] = XMVectorSet(0, 1, 0, 0) * XMVector3Length(mat.r[1]);
			mat.r[2] = XMVectorSet(0, 0, 1, 0) * XMVector3Length(mat.r[2]);


			m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()* mat;

		}

	}

	return S_OK;
}

HRESULT CMonster_Weapon_Universal::Gadasura_Rage_Weapon(_double dDeltaTime)
{
	_double PlayRate = m_pMonster_Model->Get_PlayRate();

	if (m_pMonster_Model->Get_NowAnimIndex() == 20)
	{
		/*
		//_Matrix mat = m_fAttachedMatrix.XMatrix();

		//mat.r[0] = XMVectorSet(1, 0, 0, 0) * XMVector3Length(mat.r[0]);
		//mat.r[1] = XMVectorSet(0, 1, 0, 0) * XMVector3Length(mat.r[1]);
		//mat.r[2] = XMVectorSet(0, 0, 1, 0) * XMVector3Length(mat.r[2]);

		//m_fAttachedMatrix = mat;

		//if (PlayRate <= 0.1588785046)
		//{

		//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(110));

		//}
		//else if (PlayRate < 0.214953271)
		//{

		//	_float Angle = g_pGameInstance->Easing(TYPE_Linear, 110, 0, PlayRate - 0.1588785046f, 0.0560747664);

		//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(Angle));

		//}
		//else if (PlayRate <= 0.654205607)
		//{
		//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(0));

		//}
		//else if (PlayRate <= 0.8130841121495327)
		//{
		//	_float Angle = g_pGameInstance->Easing(TYPE_SinInOut, 0, 110, PlayRate - 0.654205607, 0.1588785051495327);
		//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(Angle));
		//}
		//else
		//{
		//	m_pTransformCom->Rotation_CCW(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(110));

		//}
		//m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() *mat;



		*/


		if (PlayRate > 0.1495327102803738 && PlayRate <= 0.7383177570093458)
		{
			_Matrix mat = m_fAttachedMatrix.XMatrix();

			mat.r[0] = XMVectorSet(1, 0, 0, 0) * XMVector3Length(mat.r[0]);
			mat.r[1] = XMVectorSet(0, 1, 0, 0) * XMVector3Length(mat.r[1]);
			mat.r[2] = XMVectorSet(0, 0, 1, 0) * XMVector3Length(mat.r[2]);


			m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()* mat;

		}

	}
	return S_OK;
}

CMonster_Weapon_Universal * CMonster_Weapon_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Weapon_Universal*	pInstance = NEW CMonster_Weapon_Universal(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Weapon_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Weapon_Universal::Clone(void * pArg)
{
	CMonster_Weapon_Universal*	pInstance = NEW CMonster_Weapon_Universal(*this);

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

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);
}