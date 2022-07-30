#include "stdafx.h"
#include "..\public\Mahabalasura_Weapon.h"

CMahabalasura_Weapon::CMahabalasura_Weapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CMahabalasura_Weapon::CMahabalasura_Weapon(const CMahabalasura_Weapon & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CMahabalasura_Weapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMahabalasura_Weapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_WeaponDesc, sizeof(WEAPONDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_WeaponDesc, pArg, sizeof(WEAPONDESC));
	}

	if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_INSTANCE)
	{
		CGameObject* BossObj = g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Boss), 0);
		CTransform* BossTransform = (CTransform*)BossObj->Get_Component(TAG_COM(Com_Transform));


		m_pBossPos = m_WeaponDesc.Pos;
		m_mBossMatrix = BossTransform->Get_WorldMatrix();
		m_bIsAttack = true;
	}

	FAILED_CHECK(SetUp_Components());

	if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_BOSS)
	{
		m_pTransformCom->Scaled_All(_float3(0.8f));
		m_bIsAttack = false;
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_SKILL)
	{
		m_pTransformCom->Scaled_All(XMVectorSet(100.f, 200.f, 150.f, 0.f));

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_WeaponDesc.Pos);
		m_bIsAttack = true;

		//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
	}


	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));
	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	m_PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS); 

	//m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(170.f));
	return S_OK;
}

_int CMahabalasura_Weapon::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	m_pDissolveCom->Update_Dissolving(fDeltaTime);

	if (m_pDissolveCom->Get_IsFadeIn() == false && m_pDissolveCom->Get_DissolvingRate() >= 1.0)
	{
		Set_IsDead();
	}


	if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_SKILL)
	{
		if (m_dAliveTime > 4)
		{
			Set_IsDead();
			return _int();
		}
		m_dAliveTime += fDeltaTime;
		m_fUpSpeed += (_float)fDeltaTime * 30.0f;
		_float3 fThisPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		fThisPos.y = m_PlayerPos.y - 3.0f;
		
		_float3 Pos = g_pGameInstance->Easing_Vector(TYPE_Linear, fThisPos, m_PlayerPos, m_fUpSpeed, 4.f);
		if (m_fUpSpeed > 4.f)
		{
			m_PlayerPos.y = -1.f;
			Pos = m_PlayerPos;
		}
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_INSTANCE)
	{
			m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));
	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	m_PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

		if (m_bIsAliveTimeStart)
		{
			if (m_dAliveTime > 3)
			{
				Set_IsDead();
				return _int();
			}

			m_dAliveTime += fDeltaTime;
		}

		if (m_bIsStab && !m_bIsAliveTimeStart)
		{
			for (_int i = 0; i < m_vInstanceTransformComs.size(); ++i)
			{
				_float3 Pos = m_vInstanceTransformComs[i]->Get_MatrixState(CTransform::STATE_POS);

				if (m_PlayerPos.y + 2 < Pos.y)
				{
					Pos.y -= 2.f;
					m_vInstanceTransformComs[i]->Set_MatrixState(CTransform::STATE_POS, Pos);
				}
				else
				{
					Pos.y = m_PlayerPos.y;

					if(i== m_vInstanceTransformComs.size()-1)
						m_bIsAliveTimeStart = true;
				}
			}
		}
	}

	if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_BOSS)
	{
		m_pCollider->Update_ConflictPassedTime(fDeltaTime);

		_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

		_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_WeaponDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix();

		mat.r[0] = XMVector3Normalize(mat.r[0]);
		mat.r[1] = XMVector3Normalize(mat.r[1]);
		mat.r[2] = XMVector3Normalize(mat.r[2]);

		for (_uint i = 0; i < iNumCollider; i++)
			m_pCollider->Update_Transform(i, mat);

		if (m_bIsAttack)
		{
			FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));
		}

	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_SKILL)
	{

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
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_INSTANCE)
	{
		m_pCollider->Update_ConflictPassedTime(fDeltaTime);

		_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

		for (_uint i = 0; i < iNumCollider; i++)
		{
			if (i == 0)
			{
				_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_WeaponDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix();

				mat.r[0] = XMVector3Normalize(mat.r[0]);
				mat.r[1] = XMVector3Normalize(mat.r[1]);
				mat.r[2] = XMVector3Normalize(mat.r[2]);

				m_pCollider->Update_Transform(i, mat);
				continue;
			}

			_Matrix mat = m_vInstanceTransformComs[i-1]->Get_WorldMatrix();
			mat.r[0] = XMVector3Normalize(mat.r[0]);
			mat.r[1] = XMVector3Normalize(mat.r[1]);
			mat.r[2] = XMVector3Normalize(mat.r[2]);
			m_pCollider->Update_Transform(i, mat);

		}

		if (m_bIsAttack)
		{
			FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));
		}
	}

	if (m_bIsDissolveStart)
		m_pDissolveCom->Set_DissolveOn(false, 6.4f);

	return _int();
}

_int CMahabalasura_Weapon::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_BOSS)
	{
		Update_AttachMatrix();
		m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &m_fAttachedMatrix));
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
		FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_SKILL)
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
		FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_INSTANCE)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
		FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	}

	return _int();
}

_int CMahabalasura_Weapon::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_BOSS)
	{
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));

		FAILED_CHECK(m_pDissolveCom->Render(8));

		//_uint NumMaterial = m_pModel->Get_NumMaterial();

		//for (_uint i = 0; i < NumMaterial; i++)
		//{
		//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		//	{
		//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		//	}
		//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 8, i));
		//}
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_SKILL)
	{
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

		_uint NumMaterial = m_pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			{
				FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
			}
			FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i));
		}
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_INSTANCE)
	{
		FAILED_CHECK(m_pInstanceShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
		FAILED_CHECK(m_pInstanceShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

		FAILED_CHECK(m_pModelInstance->Render(m_pInstanceShaderCom, 2, &m_vInstanceTransformComs));
	}

	return _int();
}

_int CMahabalasura_Weapon::LateRender()
{
	return _int();
}

void CMahabalasura_Weapon::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	pConflictedCollider->Set_Conflicted();
}

HRESULT CMahabalasura_Weapon::Set_InstanceWeapon(_int iCount)
{
	COLLIDERDESC			ColliderDesc;

	if (iCount == 0)
	{
		for (_uint i = 0; i < 6; i++)
		{
			_float Angle = (_float)i*(360.f / 6.f);

			_Matrix Mat = XMMatrixRotationY(XMConvertToRadians(Angle));
			_float3 Pos = XMLoadFloat3(&m_pBossPos) + (Mat.r[2] * 4.f);
			Pos.y += 10.f;

			CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
			NULL_CHECK_RETURN(pTransform, E_FAIL);


			pTransform->Scaled_All(_float3(100, 200, 150));
			pTransform->Rotation_CW(XMVectorSet(0, 0, 1.f, 0), XMConvertToRadians(180.f));
			pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);

			m_vInstanceTransformComs.push_back(pTransform);

			//충돌체
			ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
			ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
			ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
			ColliderDesc.vPosition = _float4(0.f, 2.7f, 0.f, 1);
			FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
			m_pCollider->Set_ParantBuffer();
		}
	}
	else if (iCount == 1)
	{
		for (_uint i = 0; i < 12; i++)
		{
			_float Angle = (_float)i*(360.f / 12.f);

			_Matrix Mat = XMMatrixRotationY(XMConvertToRadians(Angle));
			_float3 Pos = XMLoadFloat3(&m_pBossPos) + (Mat.r[2] * 8.f);
			Pos.y += 10.f;

			CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
			NULL_CHECK_RETURN(pTransform, E_FAIL);
			pTransform->Scaled_All(_float3(100, 200, 150));
			pTransform->Rotation_CW(XMVectorSet(0, 0, 1.f, 0), XMConvertToRadians(180.f));
			pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);

			m_vInstanceTransformComs.push_back(pTransform);

			//충돌체
			ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
			ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
			ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
			ColliderDesc.vPosition = _float4(0.f, 2.7f, 0.f, 1);
			FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
			m_pCollider->Set_ParantBuffer();
		}
	}
	else if (iCount == 2)
	{
		for (_uint i = 0; i < 24; i++)
		{
			_float Angle = (_float)i*(360.f / 24.f);

			_Matrix Mat = XMMatrixRotationY(XMConvertToRadians(Angle));
			_float3 Pos = XMLoadFloat3(&m_pBossPos) + (Mat.r[2] * 12.f);
			Pos.y += 10.f;

			CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
			NULL_CHECK_RETURN(pTransform, E_FAIL);
			pTransform->Scaled_All(_float3(100, 200, 150));
			pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);
			pTransform->Rotation_CW(XMVectorSet(0, 0, 1.f, 0), XMConvertToRadians(180.f));
			m_vInstanceTransformComs.push_back(pTransform);

			//충돌체
			ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
			ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
			ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
			ColliderDesc.vPosition = _float4(0.f, 2.7f, 0.f, 1);
			FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
			m_pCollider->Set_ParantBuffer();
		}
	}
	return S_OK;
}

void CMahabalasura_Weapon::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_WeaponDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix();
}

HRESULT CMahabalasura_Weapon::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST), TAG_COM(Com_ShaderSub), (CComponent**)&m_pInstanceShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasurWeapon), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	DissolveDesc.pShader = m_pShaderCom;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &DissolveDesc));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_INSTANCE)
	{

		CModelInstance::MODELINSTDESC tModelIntDsec;
		tModelIntDsec.m_pTargetModel = m_pModel;
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_64), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));

	}


	if(m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_BOSS)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(5.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 2.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_SKILL)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(6.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 2.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else if (m_WeaponDesc.m_CloneType == CMahabalasura_Weapon::CLONE_INSTANCE)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(100.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	}

	return S_OK;
}

CMahabalasura_Weapon * CMahabalasura_Weapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura_Weapon*	pInstance = NEW CMahabalasura_Weapon(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChiedtuan_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura_Weapon::Clone(void * pArg)
{
	CMahabalasura_Weapon*	pInstance = NEW CMahabalasura_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMahabalasura_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pInstanceShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pModelInstance);
	Safe_Release(m_pCollider);
	Safe_Release(m_pDissolveCom);
	
	for (auto& pTransform : m_vInstanceTransformComs)
		Safe_Release(pTransform);
	m_vInstanceTransformComs.clear();
}
