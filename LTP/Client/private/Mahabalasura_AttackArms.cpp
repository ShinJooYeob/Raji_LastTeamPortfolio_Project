#include "stdafx.h"
#include "..\public\Mahabalasura_AttackArms.h"

CMahabalasura_AttackArms::CMahabalasura_AttackArms(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CMahabalasura_AttackArms::CMahabalasura_AttackArms(const CMahabalasura_AttackArms & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CMahabalasura_AttackArms::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMahabalasura_AttackArms::Initialize_Clone(void * pArg)
{

	NULL_CHECK_RETURN(pArg, E_FAIL);

	FAILED_CHECK(__super::Initialize_Clone(pArg));

	memcpy(&m_tDesc, pArg, sizeof(ATADESC));

	FAILED_CHECK(SetUp_Components());

	CTransform* BossTransform = ((CTransform*)g_pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Boss), TAG_COM(Com_Transform)));
	_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);
	m_BossMatrix = BossTransform->Get_WorldFloat4x4();
	m_pPlayerTrs = (CTransform*)g_pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"), TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pPlayerTrs, E_FAIL);
	m_PlayerPos = (m_pPlayerTrs)->Get_MatrixState(CTransform::STATE_POS);

	CUtilityMgr* pUtil =  GetSingle(CUtilityMgr);
	CGameInstance* pIs = g_pGameInstance;

	 CTransform* pGuideTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	 pGuideTransform->Set_TurnSpeed(1);


	for (_int i = 0; i < AtkArmTotalCount; ++i)
	{
		TTD tDesc;

		_float Angle = (_float)i*(180.f / ((_float)AtkArmTotalCount));

		pGuideTransform->LookDir_ver2(m_tDesc.vRotAxis.XMVector());
		pGuideTransform->Turn_CW(pGuideTransform->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(Angle));

		//_Matrix Mat = XMMatrixRotationAxis(m_tDesc.vRotAxis.XMVector(), XMConvertToRadians(Angle));

		_float Value = (1.f - (fabs(_float(i) - (AtkArmTotalCount * 0.5f)) / (AtkArmTotalCount * 0.5f))) * 1.618f + 1.f;
		_float3 Pos = BossPos.XMVector() + (pGuideTransform->Get_MatrixState(CTransform::STATE_RIGHT) * m_tDesc.fAtkArmLength * Value);
		Pos.y += 0.809f;

		tDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(tDesc.pTransform, E_FAIL);

		tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);
		tDesc.pTransform->Scaled_All(_float3(3.f));

		tDesc.bIsDead = false;
		tDesc.fPassedTime = -1.f+ (_float)( (_float)i / _float(AtkArmTotalCount));

		m_StartPos[i] = Pos;
		m_vecInstancedTransform.push_back(tDesc);

		COLLIDERDESC			ColliderDesc;
		//�浹ü
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 1.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}

	Safe_Release(pGuideTransform);
	//Set_LimLight_N_Emissive(_float4(255.f, 0.f, 20.f, 255.f), _float4(0));

	m_fTotalTime = 0.61804697156f;
	return S_OK;
}

_int CMahabalasura_AttackArms::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;



	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);



	_int iCount = 0;
	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{
		m_vecInstancedTransform[i].fPassedTime += (_float)fDeltaTime;
		_float Length = m_StartPos[i].Get_Distance(m_DestPos[i].XMVector());
		_float Rate = (1.0f - max((10.0f - Length), 0) * 0.1f) * 1.618f;


		
		
		if (m_vecInstancedTransform[i].fPassedTime <= 0)
		{
			continue;
		}
		else
		{
			if (m_vecInstancedTransform[i].fPassedTime - (_float)fDeltaTime <= 0)
			{
				if (m_iSoundCount == iCount)
				{
					++m_iSoundCount;
					g_pGameInstance->Play3D_Sound(L"JJB_MrM_Teleport.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				}

				m_DestPos[i] = m_pPlayerTrs->Get_MatrixState(CTransform::STATE_POS)
					+ XMVectorSetY(pUtil->RandomFloat3(-20.f, 20.f).XMVector(), 0);

				m_vecInstancedTransform[i].pTransform->LookAt(m_pPlayerTrs->Get_MatrixState(CTransform::STATE_POS));

				m_vecInstancedTransform[i].vSubTarget = XMVectorSetY(m_vecInstancedTransform[i].pTransform->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) 
					* pUtil->RandomFloat(-Rate * 5.f, Rate * 5.f), 0);
				m_vecInstancedTransform[i].vSubTarget.y = pUtil->RandomFloat(0, 2.427f);

				m_vecInstancedTransform[i].pTransform->Turn_CCW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(20.f));

			}
			else
			{
				
				if (m_vecInstancedTransform[i].fPassedTime > m_fTotalTime * Rate + 1.f)
				{

					m_vecInstancedTransform[i].bIsDead = true;
					continue;
				}
			}

		}


		_float3 Pos;

		if (m_vecInstancedTransform[i].fPassedTime < 1)
		{
			m_DestPos[i] = m_pPlayerTrs->Get_MatrixState(CTransform::STATE_POS)
				+ XMVectorSetY(pUtil->RandomFloat3(-Rate * 3.f, Rate * 3.f).XMVector(), 0);

			m_vecInstancedTransform[i].pTransform->LookAt(m_pPlayerTrs->Get_MatrixState(CTransform::STATE_POS));
			m_vecInstancedTransform[i].pTransform->Turn_CCW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(20.f));

		}
		else
		{

			m_bIsAttack = true;



			if (m_vecInstancedTransform[i].fPassedTime - 1 < m_fTotalTime * 0.4 *Rate)
			{

				Pos = g_pGameInstance->Easing_Vector(TYPE_SinOut, m_StartPos[i],
					_float3(m_StartPos[i].x + m_vecInstancedTransform[i].vSubTarget.x , m_StartPos[i].y + m_StartPos[i].Get_Distance(m_DestPos[i].XMVector()) * 0.101125f * m_vecInstancedTransform[i].vSubTarget.y,
						m_StartPos[i].z + m_vecInstancedTransform[i].vSubTarget.z), m_vecInstancedTransform[i].fPassedTime - 1, m_fTotalTime *0.4f *Rate);

			}	
			else
			{
				Pos = g_pGameInstance->Easing_Vector(TYPE_SinIn, _float3(m_StartPos[i].x + m_vecInstancedTransform[i].vSubTarget.x, m_StartPos[i].y + m_StartPos[i].Get_Distance(m_DestPos[i].XMVector()) * 0.101125f * m_vecInstancedTransform[i].vSubTarget.y,
						m_StartPos[i].z + m_vecInstancedTransform[i].vSubTarget.z), m_DestPos[i],
					m_vecInstancedTransform[i].fPassedTime - 1 - (m_fTotalTime *0.4f)*Rate, m_fTotalTime *0.6f * Rate);

			}


			_Vector OldPos = m_vecInstancedTransform[i].pTransform->Get_MatrixState(CTransform::STATE_POS);
			m_vecInstancedTransform[i].pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);

			
			m_vecInstancedTransform[i].pTransform->LookDir_ver2(
				(m_vecInstancedTransform[i].pTransform->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.9f + XMVector3Normalize(Pos.XMVector() - OldPos) * 0.1f));
			//m_vecInstancedTransform[i].pTransform->LookAt(m_DestPos[i].XMVector());
			//m_vecInstancedTransform[i].pTransform->Turn_CCW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(20.f));
		}
		iCount++;
	}




	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple)));


	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

	for (_uint i = 0; i < iNumCollider; i++)
	{
		if (i == 0)
		{
			_Matrix mat = m_BossMatrix.XMatrix();

			mat.r[0] = XMVector3Normalize(mat.r[0]);
			mat.r[1] = XMVector3Normalize(mat.r[1]);
			mat.r[2] = XMVector3Normalize(mat.r[2]);

			m_pCollider->Update_Transform(i, mat);
			continue;
		}

		_Matrix mat = m_vecInstancedTransform[i - 1].pTransform->Get_WorldMatrix();
		mat.r[0] = XMVector3Normalize(mat.r[0]);
		mat.r[1] = XMVector3Normalize(mat.r[1]);
		mat.r[2] = XMVector3Normalize(mat.r[2]);
		m_pCollider->Update_Transform(i, mat);

	}

	if (m_bIsAttack)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));
	}

	return _int();
}

_int CMahabalasura_AttackArms::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	m_vecForRenderTransform.clear();
	m_vecForLimLight.clear();
	m_vecForEmissive.clear();

	m_vecForRenderTransform.reserve(m_vecInstancedTransform.size());
	m_vecForLimLight.reserve(m_vecInstancedTransform.size());
	m_vecForEmissive.reserve(m_vecInstancedTransform.size());

	_uint iDeadCount = 0;
	CGameInstance* pInstance = g_pGameInstance;

	for (auto& tDesc : m_vecInstancedTransform)
	{

		if (tDesc.fPassedTime <= 0)
		{
			continue;
		}
		if (tDesc.bIsDead)
		{
			iDeadCount++;
			continue;
		}

		m_vecForRenderTransform.push_back(tDesc.pTransform);


		_float Value = (tDesc.fPassedTime - _uint(tDesc.fPassedTime))*0.5f + 0.25f ;

		Value = max(min((Value), 1.f), 0);

		//_float3 ColorValue = pInstance->Easing_Vector(TYPE_Linear,_float3(1),_float3(0.3125f, 0.734375f, 0.87109375f), Value,1.f);

		m_vecForLimLight.push_back(_float4(_float3(XMVectorSet(0.06640625f, 0.1484375f, 0.30859375f, 0)* Value), 1.f/** 0.003f*/));
		//m_vecForLimLight.push_back(_float4(_float3(XMVectorSet(0.3125f, 0.734375f, 0.87109375f, 0)* Value), 1.f/** 0.003f*/));
		//m_vecForLimLight.push_back(_float4(_float3(XMVectorSet(0.3125f, 0.734375f, 0.87109375f, 0)* Value), Value /** 0.003f*/));
		m_vecForEmissive.push_back(_float4(1.f, 0.5f, 1.f, 0));
	}

	if (iDeadCount >= AtkArmTotalCount)
	{
		Set_IsDead();
		return 0;
	}


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this, &m_vecForRenderTransform, m_pModelInstance,m_pShaderCom,m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	return _int();
}

_int CMahabalasura_AttackArms::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecForRenderTransform,0,&m_vecForLimLight,&m_vecForEmissive));

	return _int();
}

_int CMahabalasura_AttackArms::LateRender()
{
	return _int();
}

void CMahabalasura_AttackArms::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

HRESULT CMahabalasura_AttackArms::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasuraAttackArms), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CModelInstance::MODELINSTDESC tDesc;
	tDesc.m_pTargetModel = m_pModel;

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_ModelInstance_8), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tDesc));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(100.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	return S_OK;
}

HRESULT CMahabalasura_AttackArms::Adjust_AnimMovedTransform(_double fDeltatime)
{
	//_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	//_double PlayRate = m_pModel->Get_PlayRate();

	//if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	//	m_iAdjMovedIndex = 0;


	//if (PlayRate <= 0.98)
	//{

	//}
	//else
	//{

	//}

	//m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMahabalasura_AttackArms * CMahabalasura_AttackArms::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura_AttackArms*	pInstance = NEW CMahabalasura_AttackArms(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMahabalasura_AttackArms");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura_AttackArms::Clone(void * pArg)
{
	CMahabalasura_AttackArms*	pInstance = NEW CMahabalasura_AttackArms(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMahabalasura_AttackArms");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura_AttackArms::Free()
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
