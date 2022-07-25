#include "stdafx.h"
#include "..\public\Monster_Spider.h"

CMonster_Spider::CMonster_Spider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Spider::CMonster_Spider(const CMonster_Spider & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Spider::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	for (auto& p : m_pModel)	p = nullptr;
	for (auto& p : m_pModelInstance)	p = nullptr;



	return S_OK;
}

HRESULT CMonster_Spider::Initialize_Clone(void * pArg)
{

	for (auto& p : m_pModel)	p = nullptr;
	for (auto& p : m_pModelInstance)	p = nullptr;


	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (nullptr != pArg)
	{
		memcpy(&m_Instance_Info, pArg, sizeof(INSTANCE_INFO));
	}
	else {
		MSGBOX("CMonster_Spider의 pArg가 nullptr입니다.");
	}


	FAILED_CHECK(SetUp_Components());

	return S_OK;
}

_int CMonster_Spider::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;


	FollowMe(dDeltaTime);

	for (_int i = 0; i < ANIM_END; i++)
	{
		if (i >= ANIM_ATTACK_Frame1 && i <= ANIM_ATTACK_Frame5)
		{
			m_pModel[i]->Update_AnimationClip(dDeltaTime * m_dAcceleration);
		}
		else {
			m_pModel[i]->Update_AnimationClip(dDeltaTime);
		}
	}



	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	Update_Collider(dDeltaTime);

	return _int();
}

_int CMonster_Spider::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;



	for (_int i = 0; i < ANIM_END; i++)
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this, &m_ModelTransGroup[i], m_pModelInstance[i], m_pShaderCom, m_pModel[i]));
	}


	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

#ifdef _DEBUG

	for (_uint i = 0; i < m_vecInstancedTransform.size(); i++)
	{
		FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_vecInstancedTransform[i].pCollider));
	}
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pAttackColliderCom));
#endif

	return _int();
}

_int CMonster_Spider::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_DissolveTex_OnShader(m_pShaderCom, 1));

	for (_int i = 0; i < ANIM_END; i++)
	{
		if (m_ModelTransGroup[i].size() == 0)
			continue;

		FAILED_CHECK(m_pModelInstance[i]->Render(m_pShaderCom, 2, &m_ModelTransGroup[i], 0, &m_vecRimLight[i], &m_vecEmissive[i], &m_vecDissolve[i]));
	}



	return _int();
}

_int CMonster_Spider::LateRender()
{
	return _int();
}

void CMonster_Spider::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}

	if (CollisionTypeID::CollisionType_PlayerWeapon == eConflictedObjCollisionType)
	{
		for (_uint i = 0; i < m_vecInstancedTransform.size(); i++)
		{
			if (pMyCollider == m_vecInstancedTransform[i].pCollider)
			{
				if (m_vecInstancedTransform[i].bHit == false)
				{
					m_vecInstancedTransform[i].iRenderType = RENDER_HIT;
					m_vecInstancedTransform[i].iHp += -1;
					m_vecInstancedTransform[i].dTime = 0;
					m_vecInstancedTransform[i].bHit = true;

					m_vecInstancedTransform[i].fRimRight.w = 1;
					m_vecInstancedTransform[i].pCollider->Set_Conflicted(0.f);
					if (m_vecInstancedTransform[i].iHp <= 0)
					{
						m_vecInstancedTransform[i].iRenderType = RENDMER_DIE;
					}
					break;
				}
				else {
					break;
				}
			}
		}
	}
}

_float CMonster_Spider::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{

	//m_pColliderCom->Set_Conflicted(0.f);

	return _float();
}

HRESULT CMonster_Spider::SetUp_Info()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	if (m_Instance_Info.fValueMat.m[0][1] <= 1)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_1);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 1 && m_Instance_Info.fValueMat.m[0][1] <= 2)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_2);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 2 && m_Instance_Info.fValueMat.m[0][1] <= 4)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_4);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 4 && m_Instance_Info.fValueMat.m[0][1] <= 8)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_8);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 8 && m_Instance_Info.fValueMat.m[0][1] <= 16)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_16);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 16 && m_Instance_Info.fValueMat.m[0][1] <= 32)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_32);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 32 && m_Instance_Info.fValueMat.m[0][1] <= 64)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_64);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 64 && m_Instance_Info.fValueMat.m[0][1] <= 128)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_128);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 128 && m_Instance_Info.fValueMat.m[0][1] <= 256)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_256);
	}
	else if (m_Instance_Info.fValueMat.m[0][1] > 256 && m_Instance_Info.fValueMat.m[0][1] <= 512)
	{
		m_charModellInstanceType = TAG_CP(Prototype_ModelInstance_512);
	}

	for (_uint i = 0; i < m_Instance_Info.fValueMat.m[0][1]; i++)
	{
		TRANSFORM_STATE tDesc;
		tDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(tDesc.pTransform, E_FAIL);
		tDesc.iAnimType = rand() % 2;

		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		_float fSpeed = pUtil->RandomFloat(2, 3);

		tDesc.pTransform->Set_MoveSpeed(fSpeed);


		for (_uint j = (_uint)m_Instance_Info.fValueMat.m[0][3]; j < (_uint)m_Instance_Info.fValueMat.m[0][2]; i++)
		{
			_uint X = j / 4;
			_uint Y = j % 4;
			if (m_Instance_Info.fSubValueMat.m[X][Y] > 0)
			{
				tDesc.iCellIndex = m_Instance_Info.fSubValueMat.m[X][Y];
				m_Instance_Info.fValueMat.m[0][3]++;

				if ((_uint)m_Instance_Info.fValueMat.m[0][3] + 1 >= (_uint)m_Instance_Info.fValueMat.m[0][2])
					m_Instance_Info.fValueMat.m[0][3] = 0;
				break;
			}
			else {
				MSGBOX("CMonster_Spider의 m_Instance_Info.fSubValueMat.m[X][Y]가 0 이하입니다.");
			}
		}


		tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_IndexPosition(tDesc.iCellIndex));


		/////////////////////////////////////////////PlayerPosition Create
		//_Vector vDis = (m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1));

		//_Vector PlayerPos = m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS);

		//PlayerPos = PlayerPos + (XMVector3Normalize(vDis) * pUtil->RandomFloat(2, 5));

		//tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, PlayerPos);
		///////////////////////////////////////////////


		//////////Navigation
		CNavigation::NAVIDESC		NaviDesc;
		NaviDesc.iCurrentIndex = tDesc.iCellIndex;

		tDesc.pNavigation = (CNavigation*)g_pGameInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), &NaviDesc);

		//tDesc.pNavigation->FindCellIndex(tDesc.pTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));
		///////////////

		/////////////////////////////////////Collider
		tDesc.pCollider = (CCollider*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Collider));

		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1);
		FAILED_CHECK(tDesc.pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		///////////////////

		/////////////////////////////////////AttackCollider
		COLLIDERDESC			AttackColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		AttackColliderDesc.vScale = _float3(1.5f);
		AttackColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		AttackColliderDesc.vPosition = _float4(0.f, 1.f, 0.75f, 1);
		FAILED_CHECK(m_pAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &AttackColliderDesc));
		//m_pAttackColliderCom->Delete_ChildeBuffer(0,i+1);

		///////////////////

		m_vecInstancedTransform.push_back(tDesc);
	}

	for (_uint i = 0; i < ANIM_END; i++)
	{
		CModel* pModel = (CModel*)g_pGameInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Spider));
		NULL_CHECK_RETURN(pModel, E_FAIL);
		m_pModel[i] = pModel;

		CModelInstance::MODELINSTDESC tModelIntDsec;
		tModelIntDsec.m_pTargetModel = pModel;

		CModelInstance* pModelInstance = (CModelInstance*)g_pGameInstance->Clone_Component(m_eNowSceneNum, m_charModellInstanceType , &tModelIntDsec);
		NULL_CHECK_RETURN(pModelInstance, E_FAIL);
		m_pModelInstance[i] = pModelInstance;
	}


	for (_int i = 0; i < ANIM_END; i++)
	{
		if (i == ANIM_RUN_Frame1 && i <= ANIM_RUN_Frame2)
		{
			m_pModel[i]->Change_AnimIndex(0);
		}
		else if (i >= ANIM_ATTACK_Frame1 && i <= ANIM_ATTACK_Frame5)
		{
			m_pModel[i]->Change_AnimIndex(1);
		}
	}

	_int	iNumber = 0;
	_double	dpercent = 0.54;

	for (_uint i = ANIM_RUN_Frame1; i <= ANIM_RUN_Frame2; i++)
	{
		m_pModel[i]->Update_AnimationClip(iNumber * dpercent);

		iNumber++;
	}

	iNumber = 0;
	dpercent = 0.334;
	for (_uint i = ANIM_ATTACK_Frame1; i <= ANIM_ATTACK_Frame5; i++)
	{
		m_pModel[i]->Update_AnimationClip(iNumber * dpercent);

		iNumber++;
	}

	return S_OK;
}

HRESULT CMonster_Spider::FollowMe(_double dDeltaTime)
{
	for (_int i = 0; i < ANIM_END; i++)
	{
		m_ModelTransGroup[i].clear();
		m_ModelTransGroup[i].reserve(m_vecInstancedTransform.size());

		m_vecRimLight[i].clear();
		m_vecRimLight[i].reserve(m_vecInstancedTransform.size());

		m_vecEmissive[i].clear();
		m_vecEmissive[i].reserve(m_vecInstancedTransform.size());

		m_vecDissolve[i].clear();
		m_vecDissolve[i].reserve(m_vecInstancedTransform.size());
	}


	m_iTempAnimNumber = ANIM_ATTACK_Frame1;
	for (_uint i = ANIM_ATTACK_Frame2; i <= ANIM_ATTACK_Frame5; i++)
	{
		if (m_pModel[m_iTempAnimNumber]->Get_PlayRate() < m_pModel[i]->Get_PlayRate())
		{
			m_iTempAnimNumber = m_iTempAnimNumber;
		}
		else {
			m_iTempAnimNumber = i;
		}
	}

	for (auto& MeshInstance : m_vecInstancedTransform)
	{

		_Vector vTarget = XMVector3Normalize(m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS) - MeshInstance.pTransform->Get_MatrixState(CTransform::STATE_POS));

		MeshInstance.pTransform->Turn_Dir(vTarget, 0.9f);
		_float fDistance = MeshInstance.pTransform->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS));


		MeshInstance.pTransform->Set_MatrixState(CTransform::STATE_POS, MeshInstance.pNavigation->Get_NaviPosition(MeshInstance.pTransform->Get_MatrixState(CTransform::STATE_POS)));

		//////////////////////RenderType

		if (MeshInstance.bHit == true && MeshInstance.iHp > 0)
		{
			MeshInstance.dTime += dDeltaTime;
			MeshInstance.iRenderType = RENDER_HIT;

			if (MeshInstance.dTime >= 0.2)
			{
				MeshInstance.bHit = false;
				MeshInstance.iRenderType = RENDER_IDLE;
				MeshInstance.fRimRight.w = 1;
			}
		}
		else if (MeshInstance.iHp <= 0)
		{
			MeshInstance.iRenderType = RENDMER_DIE;
			MeshInstance.iAnimType = rand() % 2;
		}
		else {
			MeshInstance.iRenderType = RENDER_IDLE;
		}

		////////////////////////AnimType
		if (fDistance < 2.0)
		{
			if (MeshInstance.iAnimType >= ANIM_ATTACK_Frame1&& MeshInstance.iAnimType <= ANIM_ATTACK_Frame5)
				continue;
			MeshInstance.iAnimType = m_iTempAnimNumber;
		}
		else {
			if (MeshInstance.iAnimType >= ANIM_ATTACK_Frame1&& MeshInstance.iAnimType <= ANIM_ATTACK_Frame5)
			{
				if (m_pModel[MeshInstance.iAnimType]->Get_PlayRate() > 0.95)
				{
					MeshInstance.iAnimType = rand() % 2;
				}
			}
		}

	}

	Update_VectorGroup(dDeltaTime);


	return S_OK;
}

HRESULT CMonster_Spider::Update_Collider(_double dDeltaTime)
{
	//////////////Collider
	m_pAttackColliderCom->Update_ConflictPassedTime(dDeltaTime);

	m_pAttackColliderCom->Update_Transform(0, m_pPlayerTransformCom->Get_WorldMatrix());


	for (_int i = 0; i < m_vecInstancedTransform.size(); i++)
	{
		if (m_vecInstancedTransform[i].iAnimType >= ANIM_RUN_Frame1 && m_vecInstancedTransform[i].iAnimType <= ANIM_RUN_Frame2)
			FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_vecInstancedTransform[i].pTransform, 0.5f, m_vecInstancedTransform[i].pNavigation));

		m_pAttackColliderCom->Update_Transform(i + 1, m_vecInstancedTransform[i].pTransform->Get_WorldMatrix());

		m_vecInstancedTransform[i].pCollider->Update_ConflictPassedTime(dDeltaTime);
		m_vecInstancedTransform[i].pCollider->Update_Transform(0, m_vecInstancedTransform[i].pTransform->Get_WorldMatrix());
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_vecInstancedTransform[i].pCollider));

	}
	if (m_bAttackOn == true)
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pAttackColliderCom));
	//////////////////////////


	///////////////AttackCollider
	///////////////
	return S_OK;
}

HRESULT CMonster_Spider::Update_Render(_double dDeltaTime)
{
	return S_OK;
}

HRESULT CMonster_Spider::Update_VectorGroup(_double dDeltaTime)
{

	for (_uint i = 0; i < m_vecInstancedTransform.size(); i++)
	{
		switch (m_vecInstancedTransform[i].iRenderType)
		{
		case RENDER_IDLE:
			m_vecInstancedTransform[i].fRimRight = _float4(0.2f, 0.2f, 0.2f, 1.f);
			m_vecInstancedTransform[i].fEmissive = _float4(1.f, 0.5f, 1.f, 1.f);
			if (m_vecInstancedTransform[i].fDissolve.w == 1)
			{
				m_vecInstancedTransform[i].fDissolve = _float4(m_vecInstancedTransform[i].fDissolve.x + (_float)dDeltaTime, 1.f, 0.4f, 1.f);

				if (m_vecInstancedTransform[i].fDissolve.x >= 1)
				{
					m_vecInstancedTransform[i].fDissolve.w = 0;
				}
			}
			else {
				m_vecInstancedTransform[i].fDissolve = _float4(0.2f, 0.2f, 0.2f, 0.f);
			}
			break;
		case RENDER_HIT:
			m_vecInstancedTransform[i].fRimRight = _float4(0.875f, 0.0234375f, 0.18359375f, m_vecInstancedTransform[i].fRimRight.w - (_float)dDeltaTime);
			m_vecInstancedTransform[i].fEmissive = _float4(1.f, 0.5f, 1.f, 1.f);
			m_vecInstancedTransform[i].fDissolve = _float4(0.2f, 0.2f, 0.2f, 0.f);
			m_vecInstancedTransform[i].bHit;
			break;
		case RENDMER_DIE:
			m_vecInstancedTransform[i].fRimRight = _float4(0.f, 0.f, 0.f, 0.f);
			m_vecInstancedTransform[i].fEmissive = _float4(1.f, 0.5f, 1.f, 1.f);
			m_vecInstancedTransform[i].fDissolve = _float4(m_vecInstancedTransform[i].fDissolve.x + (_float)dDeltaTime, 1.f, 0.4f, 2.f);
			break;
		default:
			break;
		}

		switch (m_vecInstancedTransform[i].iAnimType)
		{
		case ANIM_RUN_Frame1:
			m_ModelTransGroup[ANIM_RUN_Frame1].push_back(m_vecInstancedTransform[i].pTransform);
			m_vecRimLight[ANIM_RUN_Frame1].push_back(m_vecInstancedTransform[i].fRimRight);
			m_vecEmissive[ANIM_RUN_Frame1].push_back(m_vecInstancedTransform[i].fEmissive);
			m_vecDissolve[ANIM_RUN_Frame1].push_back(m_vecInstancedTransform[i].fDissolve);
			break;
		case ANIM_RUN_Frame2:
			m_ModelTransGroup[ANIM_RUN_Frame2].push_back(m_vecInstancedTransform[i].pTransform);
			m_vecRimLight[ANIM_RUN_Frame2].push_back(m_vecInstancedTransform[i].fRimRight);
			m_vecEmissive[ANIM_RUN_Frame2].push_back(m_vecInstancedTransform[i].fEmissive);
			m_vecDissolve[ANIM_RUN_Frame2].push_back(m_vecInstancedTransform[i].fDissolve);
			break;
		case ANIM_ATTACK_Frame1:
			m_ModelTransGroup[ANIM_ATTACK_Frame1].push_back(m_vecInstancedTransform[i].pTransform);
			m_vecRimLight[ANIM_ATTACK_Frame1].push_back(m_vecInstancedTransform[i].fRimRight);
			m_vecEmissive[ANIM_ATTACK_Frame1].push_back(m_vecInstancedTransform[i].fEmissive);
			m_vecDissolve[ANIM_ATTACK_Frame1].push_back(m_vecInstancedTransform[i].fDissolve);
			break;
		case ANIM_ATTACK_Frame2:
			m_ModelTransGroup[ANIM_ATTACK_Frame2].push_back(m_vecInstancedTransform[i].pTransform);
			m_vecRimLight[ANIM_ATTACK_Frame2].push_back(m_vecInstancedTransform[i].fRimRight);
			m_vecEmissive[ANIM_ATTACK_Frame2].push_back(m_vecInstancedTransform[i].fEmissive);
			m_vecDissolve[ANIM_ATTACK_Frame2].push_back(m_vecInstancedTransform[i].fDissolve);
			break;
		case ANIM_ATTACK_Frame3:
			m_ModelTransGroup[ANIM_ATTACK_Frame3].push_back(m_vecInstancedTransform[i].pTransform);
			m_vecRimLight[ANIM_ATTACK_Frame3].push_back(m_vecInstancedTransform[i].fRimRight);
			m_vecEmissive[ANIM_ATTACK_Frame3].push_back(m_vecInstancedTransform[i].fEmissive);
			m_vecDissolve[ANIM_ATTACK_Frame3].push_back(m_vecInstancedTransform[i].fDissolve);
			break;
		case ANIM_ATTACK_Frame4:
			m_ModelTransGroup[ANIM_ATTACK_Frame4].push_back(m_vecInstancedTransform[i].pTransform);
			m_vecRimLight[ANIM_ATTACK_Frame4].push_back(m_vecInstancedTransform[i].fRimRight);
			m_vecEmissive[ANIM_ATTACK_Frame4].push_back(m_vecInstancedTransform[i].fEmissive);
			m_vecDissolve[ANIM_ATTACK_Frame4].push_back(m_vecInstancedTransform[i].fDissolve);
			break;
		case ANIM_ATTACK_Frame5:
			m_ModelTransGroup[ANIM_ATTACK_Frame5].push_back(m_vecInstancedTransform[i].pTransform);
			m_vecRimLight[ANIM_ATTACK_Frame5].push_back(m_vecInstancedTransform[i].fRimRight);
			m_vecEmissive[ANIM_ATTACK_Frame5].push_back(m_vecInstancedTransform[i].fEmissive);
			m_vecDissolve[ANIM_ATTACK_Frame5].push_back(m_vecInstancedTransform[i].fDissolve);
			break;
		default:
			break;
		}


	}

	return S_OK;
}



HRESULT CMonster_Spider::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	//FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider2), TAG_COM(Com_Collider), (CComponent**)&m_pAttackColliderCom));
	m_pAttackColliderCom = (CCollider*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Collider));

	COLLIDERDESC			AttackColliderDesc;
	ZeroMemory(&AttackColliderDesc, sizeof(COLLIDERDESC));
	AttackColliderDesc.vScale = _float3(30.f);
	AttackColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	AttackColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &AttackColliderDesc));

	SetUp_Info();


	return S_OK;
}

HRESULT CMonster_Spider::Adjust_AnimMovedTransform(_double dDeltatime)
{
	for (_int i = 0; i < m_vecInstancedTransform.size(); i++)
	{
		if (m_vecInstancedTransform[i].iRenderType == RENDMER_DIE)
		{
			if (m_vecInstancedTransform[i].fDissolve.x > 1.5)
			{
				//////////////////////Old Pos
				/*CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
				CNavigation* pPlayerNavi = static_cast<CNavigation*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Navaigation)));

				RELEASE_INSTANCE(CGameInstance);

				_uint iPlayerIndex = pPlayerNavi->Get_CurNavCellIndex();

				_uint Random = (rand() % 6) - 3;


				_uint RandomPlayerIndex = iPlayerIndex + Random;

				m_vecInstancedTransform[i].pNavigation->Set_CurNavCellIndex(RandomPlayerIndex);
				m_vecInstancedTransform[i].pTransform->Set_MatrixState(CTransform::STATE_POS, pPlayerNavi->Get_IndexPosition(RandomPlayerIndex));*/
				///////////////////////////////

				m_vecInstancedTransform[i].pNavigation->Set_CurNavCellIndex(m_vecInstancedTransform[i].iCellIndex);
				m_vecInstancedTransform[i].pTransform->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_IndexPosition(m_vecInstancedTransform[i].iCellIndex));

				m_vecInstancedTransform[i].iRenderType = RENDER_IDLE;

				m_vecInstancedTransform[i].iHp = 3;
				m_vecInstancedTransform[i].dTime = 0;
				m_vecInstancedTransform[i].bHit = false;
				m_vecInstancedTransform[i].fDissolve.x = 0;
				m_vecInstancedTransform[i].fDissolve.w = 1; //Live

			}

			continue;
		}

		switch (m_vecInstancedTransform[i].iAnimType)
		{
		case ANIM_RUN_Frame1:
			m_vecInstancedTransform[i].pTransform->Move_Forward(dDeltatime, m_vecInstancedTransform[i].pNavigation);
			break;
		case ANIM_RUN_Frame2:
			m_vecInstancedTransform[i].pTransform->Move_Forward(dDeltatime, m_vecInstancedTransform[i].pNavigation);
			break;
		default:
		{
			if (m_pModel[m_vecInstancedTransform[i].iAnimType]->Get_PlayRate() >= 0.465 && m_pModel[m_vecInstancedTransform[i].iAnimType]->Get_PlayRate() < 0.52)
			{
				m_vecInstancedTransform[i].pTransform->Move_Forward(dDeltatime * 10, m_vecInstancedTransform[i].pNavigation);
			}
			if (m_vecInstancedTransform[i].iSwtichIndex == 0 && m_pModel[m_vecInstancedTransform[i].iAnimType]->Get_PlayRate() >= 0.465 && m_pModel[m_vecInstancedTransform[i].iAnimType]->Get_PlayRate() <= 0.5)
			{
				m_bAttackOn = true;
				m_pAttackColliderCom->Set_ParantBuffer(0, i + 1);

				m_vecInstancedTransform[i].iSwtichIndex++;
			}
			else if (m_vecInstancedTransform[i].iSwtichIndex == 1 && m_pModel[m_vecInstancedTransform[i].iAnimType]->Get_PlayRate() >= 0.83)
			{

				m_bAttackOn = false;
				m_pAttackColliderCom->Delete_ChildeBuffer(0, i + 1);

				m_vecInstancedTransform[i].iSwtichIndex = 0;
			}
			break;
		}
		}
	}

	//for (_uint i = ANIM_RUN_Frame1; i <= ANIM_RUN_Frame2; i++)
	//{
	//	for (auto& pObjectTransform : m_ModelTransGroup[i])
	//	{
	//		pObjectTransform->Move_Forward(dDeltatime, );
	//	}
	//}

	//for (_uint i = ANIM_ATTACK_Frame1; i <= ANIM_ATTACK_Frame5; i++)
	//{
	//	for (auto& pObjectTransform : m_ModelTransGroup[i])
	//	{
	//		if (m_pModel[i]->Get_PlayRate() > 0.4)
	//		{
	//			pObjectTransform->Move_Forward(dDeltatime);
	//		}
	//	}
	//}

	return S_OK;
}

CMonster_Spider * CMonster_Spider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Spider*	pInstance = NEW CMonster_Spider(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Spider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Spider::Clone(void * pArg)
{
	CMonster_Spider*	pInstance = NEW CMonster_Spider(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Spider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Spider::Free()
{
	__super::Free();

	for (auto& pTransform : m_vecInstancedTransform)
	{
		Safe_Release(pTransform.pTransform);
		Safe_Release(pTransform.pNavigation);
		Safe_Release(pTransform.pCollider);
	}
	m_vecInstancedTransform.clear();

	for (_int i = 0; i < ANIM_END; i++)
	{
		Safe_Release(m_pModelInstance[i]);
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pAttackColliderCom);

	for (_int i = 0; i < ANIM_END; i++)
	{
		Safe_Release(m_pModel[i]);
	}
}
