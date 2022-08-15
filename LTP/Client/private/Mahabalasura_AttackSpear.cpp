#include "stdafx.h"
#include "..\public\Mahabalasura_AttackSpear.h"

#define MBSAtkSpearSize 0.285f,0.285f,0.35f

CMahabalasura_AttackSpear::CMahabalasura_AttackSpear(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CMahabalasura_AttackSpear::CMahabalasura_AttackSpear(const CMahabalasura_AttackSpear & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CMahabalasura_AttackSpear::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMahabalasura_AttackSpear::Initialize_Clone(void * pArg)
{

	NULL_CHECK_RETURN(pArg, E_FAIL);

	FAILED_CHECK(__super::Initialize_Clone(pArg));

	memcpy(&m_tDesc, pArg ,sizeof(MBSASD));


	FAILED_CHECK(SetUp_Components());


	CGameObject* pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));
	m_pPlayerTrs = (CTransform*)pPlayerObj->Get_Component(TAG_COM(Com_Transform));;



	return S_OK;
}

_int CMahabalasura_AttackSpear::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	m_fPassedTime += (_float)fDeltaTime;

	_int iCount = 0;

	_Vector DefaultPivot = XMVectorSet(100.f, 34.225f, 321.f, 1);

	m_vecForRenderTransform.clear();
	m_vecLimLight.clear();
	m_vecEmissive.clear();
	m_vecForRenderTransform.reserve(m_vecInstancedTransform.size());




	for (auto& tDesc : m_vecInstancedTransform)
	{
		tDesc.fPassedTime += (_float)fDeltaTime;


		if (tDesc.bIsDead)
		{
			iCount++;
			continue;
		}
		else
		{
			if (tDesc.pTransform->Get_MatrixState_Float3(CTransform::STATE_POS).y < 20.f)
			{
				tDesc.bIsDead = true;
				iCount++;
				continue;
			}
			else
			{
				if (tDesc.fPassedTime > 0)
				{
					tDesc.pTransform->Turn_CW(tDesc.pTransform->Get_MatrixState(CTransform::STATE_LOOK), fDeltaTime);
					tDesc.pTransform->Move_Backward(fDeltaTime);
					m_vecForRenderTransform.push_back(tDesc.pTransform);
					m_vecLimLight.push_back(_float4(0.3f, 0.2f,.8f, 1.f));
					m_vecEmissive.push_back(_float4(1.f, 0.5f, 1.f, 0));
				}
			}

		}



	}
	m_pCollider->Update_ConflictPassedTime(fDeltaTime);
	_Matrix mat = m_pPlayerTrs->Get_WorldMatrix();

	m_pCollider->Update_Transform(0, mat);

	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{
		mat = m_vecInstancedTransform[i].pTransform->Get_WorldMatrix();

		m_pCollider->Update_Transform(i+1, mat);
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	if (iCount >= 128)
	{
		Set_IsDead();
		m_vecLimLight.clear();
		m_vecEmissive.clear();
		m_vecForRenderTransform.clear();
	}




	return _int();
}

_int CMahabalasura_AttackSpear::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (m_bIsDead) return 0;
	//if (iDeadCount >= AtkArmTotalCount)Set_IsDead();

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_NONANIMINSTANCE, this, &m_vecForRenderTransform, m_pModelInstance,m_pShaderCom,m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CMahabalasura_AttackSpear::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));



	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture

	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", 381));
	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture",57));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &_float2(0,1), sizeof(_float2)));






	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 3, &m_vecForRenderTransform,0,&m_vecLimLight,&m_vecEmissive));

	return _int();
}

_int CMahabalasura_AttackSpear::LateRender()
{
	return _int();
}

void CMahabalasura_AttackSpear::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		pConflictedObj->Take_Damage(this, 1.f, XMVector3Normalize(pMyCollider->Get_ColliderPosition(iMyColliderIndex).XMVector() - pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector()), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

HRESULT CMahabalasura_AttackSpear::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Spear_UltiEffect), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CModelInstance::MODELINSTDESC tDesc;
	tDesc.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_ModelInstance_128), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tDesc));


	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	_Vector DefaultPivot = XMVectorSet(100.f, 34.225f, 321.f, 1);

	CTransform*	pGuideTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(pGuideTransform, E_FAIL);
	pGuideTransform->Set_MatrixState(CTransform::STATE_POS, DefaultPivot);
	_float DefulatAngle = pUtil->RandomFloat(0, 360.f);

	if (m_tDesc.iKinds)
	{
		/*
		for (_uint i = 0; i < 128; i++)
		{

			TTD tTTD;

			pGuideTransform->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(_float(i % 32) * 11.25f + DefulatAngle));

			_Vector TargetPosition = DefaultPivot + pGuideTransform->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * (35.f - 0.546875f * ((_float)(i)));

			tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
			NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

			tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
			//
			tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
			tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

			tTTD.pTransform->Scaled_All(_float3(0.35f, 0.35f, 0.35f));
			tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
			tTTD.bIsDead = false;
			tTTD.fPassedTime = -(_float)(((_float)(i) / 128.f)) * 2.9583333f;


			m_vecInstancedTransform.push_back(tTTD);

		}
		*/
		for (_uint i = 0; i < 16; i++)
		{

			{
				TTD tTTD;

				_Vector TargetPosition = DefaultPivot + XMVectorSet(0.5f,0, 0.5f,0)*2.f * (5.f + 45.f * (_float)(i / 32.f));

				tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
				NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

				tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
				//
				tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
				tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

				tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
				tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
				tTTD.bIsDead = false;
				tTTD.fPassedTime = -(_float)((_float)(i / 32.f)) * 1.47916665f;

				m_vecInstancedTransform.push_back(tTTD);
			}
			{
				TTD tTTD;

				_Vector TargetPosition = DefaultPivot + XMVectorSet(0.5f, 0, -0.5f, 0)*2.f * (5.f + 45.f* (_float)(i / 32.f));

				tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
				NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

				tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
				//
				tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
				tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

				tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
				tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
				tTTD.bIsDead = false;
				tTTD.fPassedTime = -(_float)((_float)(i / 32.f)) *1.47916665f;

				m_vecInstancedTransform.push_back(tTTD);
			}
			{
				TTD tTTD;

				_Vector TargetPosition = DefaultPivot + XMVectorSet(-0.5f, 0, -0.5f, 0)*2.f * (5.f + 45.f* (_float)(i / 32.f));

				tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
				NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

				tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
				//
				tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
				tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

				tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
				tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
				tTTD.bIsDead = false;
				tTTD.fPassedTime = -(_float)((_float)(i / 32.f)) *1.47916665f;

				m_vecInstancedTransform.push_back(tTTD);
			}
			{
				TTD tTTD;

				_Vector TargetPosition = DefaultPivot + XMVectorSet(-0.5f, 0, 0.5f, 0)*2.f * (5.f + 45.f* (_float)(i / 32.f));

				tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
				NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

				tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
				//
				tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
				tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

				tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
				tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
				tTTD.bIsDead = false;
				tTTD.fPassedTime = -(_float)((_float)(i / 32.f)) * 1.47916665f;

				m_vecInstancedTransform.push_back(tTTD);
			}

			{
				TTD tTTD;

				_Vector TargetPosition = DefaultPivot + XMVectorSet(0.707f, 0, 0, 0)*2.f * (5.f + 45.f * (_float)(i / 32.f));

				tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
				NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

				tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
				//
				tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
				tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

				tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
				tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
				tTTD.bIsDead = false;
				tTTD.fPassedTime = -(_float)((_float)(i / 32.f)) * 1.47916665f;

				m_vecInstancedTransform.push_back(tTTD);
			}
			{
				TTD tTTD;

				_Vector TargetPosition = DefaultPivot + XMVectorSet(0, 0, 0.707f, 0)*2.f * (5.f + 45.f* (_float)(i / 32.f));

				tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
				NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

				tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
				//
				tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
				tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

				tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
				tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
				tTTD.bIsDead = false;
				tTTD.fPassedTime = -(_float)((_float)(i / 32.f)) *1.47916665f;

				m_vecInstancedTransform.push_back(tTTD);
			}
			{
				TTD tTTD;

				_Vector TargetPosition = DefaultPivot + XMVectorSet(-0.707f, 0, 0, 0)*2.f * (5.f + 45.f* (_float)(i / 32.f));

				tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
				NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

				tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
				//
				tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
				tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

				tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
				tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
				tTTD.bIsDead = false;
				tTTD.fPassedTime = -(_float)((_float)(i / 32.f)) *1.47916665f;

				m_vecInstancedTransform.push_back(tTTD);
			}
			{
				TTD tTTD;

				_Vector TargetPosition = DefaultPivot + XMVectorSet(0, 0, -0.707f, 0) *2.f* (5.f + 45.f* (_float)(i / 32.f));

				tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
				NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

				tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
				//
				tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
				tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

				tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
				tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
				tTTD.bIsDead = false;
				tTTD.fPassedTime = -(_float)((_float)(i / 32.f)) * 1.47916665f;

				m_vecInstancedTransform.push_back(tTTD);
			}
		}
	}
	else
	{
		_Vector PlayerPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_PLAYER);
		_float Length = max(min(XMVectorGetX(XMVector3Length(DefaultPivot - PlayerPos)) * 1.25f, 35.f),20.f);
		_float Interval = Length / 4.f;

		for (_uint i = 0; i < 128; i++)
		{

			TTD tTTD;

			pGuideTransform->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(_float(i % 32) *11.25f + DefulatAngle));

			_Vector TargetPosition = DefaultPivot + pGuideTransform->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * (Interval*4.f - Interval/*0.46875f */* (_float)(i / 32));

			tTTD.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
			NULL_CHECK_RETURN(tTTD.pTransform, E_FAIL);

			tTTD.pTransform->Set_MoveSpeed(pUtil->RandomFloat(30, 45));
			//
			tTTD.pTransform->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(TargetPosition, 70.f));
			tTTD.pTransform->LookDir_ver2(XMVector3Normalize(-(TargetPosition)+tTTD.pTransform->Get_MatrixState(CTransform::STATE_POS)));

			tTTD.pTransform->Scaled_All(_float3(MBSAtkSpearSize));
			tTTD.pTransform->Set_TurnSpeed(XMConvertToRadians(1080));
			tTTD.bIsDead = false;
			tTTD.fPassedTime = -(_float)((_float)(i / 32)) * 0.39444444f;


			m_vecInstancedTransform.push_back(tTTD);
		}
	}

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(100.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	for (_int i = 0; i < 128; ++i)
	{
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(8.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}

	Safe_Release(pGuideTransform);

	return S_OK;
}

HRESULT CMahabalasura_AttackSpear::Adjust_AnimMovedTransform(_double fDeltatime)
{

	return S_OK;
}

CMahabalasura_AttackSpear * CMahabalasura_AttackSpear::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura_AttackSpear*	pInstance = NEW CMahabalasura_AttackSpear(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMahabalasura_AttackSpear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura_AttackSpear::Clone(void * pArg)
{
	CMahabalasura_AttackSpear*	pInstance = NEW CMahabalasura_AttackSpear(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMahabalasura_AttackSpear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura_AttackSpear::Free()
{
	__super::Free();

	for (auto& Desc: m_vecInstancedTransform)
		Safe_Release(Desc.pTransform);
	m_vecInstancedTransform.clear();


	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pModelInstance);
	Safe_Release(m_pCollider);
}
