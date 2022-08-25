#include "stdafx.h"
#include "..\public\Goldfish.h"
#include "InstanceMonsterBatchTrigger.h"
#include "Player.h"

CGoldfish::CGoldfish(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CGoldfish::CGoldfish(const CGoldfish & rhs)
	: CMonster(rhs)
{
}

HRESULT CGoldfish::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	for (auto& p : m_pModel)	p = nullptr;
	for (auto& p : m_pModelInstance)	p = nullptr;



	return S_OK;
}

HRESULT CGoldfish::Initialize_Clone(void * pArg)
{

	for (auto& p : m_pModel)	p = nullptr;
	for (auto& p : m_pModelInstance)	p = nullptr;
	ZeroMemory(&m_Instance_Info, sizeof(INSTANCE_INFO));


	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (nullptr != pArg)
	{
		memcpy(&m_Instance_Info, pArg, sizeof(INSTANCE_INFO));
	}
	else {
		MSGBOX("CGoldfish의 pArg가 nullptr입니다.");
	}


	FAILED_CHECK(SetUp_Components());

	m_pBatchTrigger = static_cast<CInstanceMonsterBatchTrigger*>(m_Instance_Info.Object);

	return S_OK;
}

_int CGoldfish::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	if (m_pBatchTrigger->Get_MonsterAllDie() == true)
	{
		for (_uint i = 0; i < m_Instance_Info.fValueMat.m[0][1]; i++)
		{
			m_vecInstancedTransform[i].iHp = -10;
			//m_pBatchTrigger->Set_IsDead();
		}
	}


	FollowMe(dDeltaTime);

	for (_int i = 0; i < ANIM_END; i++)
	{
		m_pModel[i]->Update_AnimationClip(dDeltaTime * m_dAcceleration);
	}



	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	for (_int i = 0; i < m_vecInstancedTransform.size(); i++)
	{
		FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_vecInstancedTransform[i].pTransform, 0.5f));
	}

	return _int();
}

_int CGoldfish::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;



	for (_int i = 0; i < ANIM_END; i++)
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this, &m_ModelTransGroup[i], m_pModelInstance[i], m_pShaderCom, m_pModel[i], nullptr, &m_vecDissolve[i]));
	}


	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CGoldfish::Render()
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

_int CGoldfish::LateRender()
{
	return _int();
}

void CGoldfish::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		pConflictedObj->Take_Damage_Instance(this, 1.f, XMVector3Normalize(XMVectorSetW(g_pGameInstance->Get_TargetPostion_Vector(PLV_PLAYER) -
			pMyCollider->Get_ColliderPosition(iMyColliderIndex).XMVector(), 0)), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CGoldfish::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{

	//m_pColliderCom->Set_Conflicted(0.f);

	return _float();
}

HRESULT CGoldfish::SetUp_Info()
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

	Instance_Route();

	for (_uint i = 0; i < m_Instance_Info.fValueMat.m[0][1]; i++)
	{
		TRANSFORM_STATE tDesc;

		ZeroMemory(&tDesc, sizeof(TRANSFORM_STATE));

		tDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(tDesc.pTransform, E_FAIL);
		tDesc.iAnimType = rand() % 2;

		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		_float fSpeed = pUtil->RandomFloat(1, 2);

		tDesc.pTransform->Scaled_All(_float3(0.1f,0.1f,0.1f));
		tDesc.pTransform->Set_MoveSpeed(fSpeed);


		tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, m_fRoute[i][0]);

		tDesc.iMyIndex = i;
		tDesc.bDirChangeOn = false;
		tDesc.iHp = 5;

		m_vecInstancedTransform.push_back(tDesc);
	}

	for (_uint i = 0; i < ANIM_END; i++)
	{
		CModel* pModel = (CModel*)g_pGameInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Goldfish));
		NULL_CHECK_RETURN(pModel, E_FAIL);
		m_pModel[i] = pModel;

		CModelInstance::MODELINSTDESC tModelIntDsec;
		tModelIntDsec.m_pTargetModel = pModel;

		CModelInstance* pModelInstance = (CModelInstance*)g_pGameInstance->Clone_Component(m_eNowSceneNum, m_charModellInstanceType, &tModelIntDsec);
		NULL_CHECK_RETURN(pModelInstance, E_FAIL);
		m_pModelInstance[i] = pModelInstance;
	}


	_int	iNumber = 0;
	_double	dpercent = 0.24;

	for (_int i = 0; i < ANIM_END; i++)
	{
		m_pModel[i]->Change_AnimIndex(0);

		m_pModel[i]->Update_AnimationClip(iNumber * dpercent);

		iNumber++;
	}
	return S_OK;
}

HRESULT CGoldfish::FollowMe(_double dDeltaTime)
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



	for (auto& MeshInstance : m_vecInstancedTransform)
	{
		_Vector vTarget = XMVector3Normalize( XMLoadFloat3(&m_fRoute[MeshInstance.iMyIndex][MeshInstance.iRouteIndex]) - MeshInstance.pTransform->Get_MatrixState(CTransform::STATE_POS));

		MeshInstance.pTransform->Turn_Dir(vTarget, 0.9f);

		MeshInstance.pTransform->MovetoDir(vTarget, dDeltaTime);
		//MeshInstance.pTransform->Set_MatrixState(CTransform::STATE_POS, MeshInstance.pNavigation->Get_NaviPosition(MeshInstance.pTransform->Get_MatrixState(CTransform::STATE_POS)));

		_float fDistance = MeshInstance.pTransform->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(XMLoadFloat3(&m_fRoute[MeshInstance.iMyIndex][MeshInstance.iRouteIndex]));

		if (fDistance < 0.5f)
		{
			if (MeshInstance.bDirChangeOn == false)
			{
				MeshInstance.iRouteIndex++;
				if (MeshInstance.iRouteIndex == 2)
				{
					MeshInstance.bDirChangeOn = true;
				}
			}
			else {
				MeshInstance.iRouteIndex--;
				if (MeshInstance.iRouteIndex == 0)
				{
					MeshInstance.bDirChangeOn = false;
				}
			}
		}
		//////////////////////RenderType
	}

	Update_VectorGroup(dDeltaTime);


	return S_OK;
}

HRESULT CGoldfish::Update_VectorGroup(_double dDeltaTime)
{

	for (_uint i = 0; i < m_vecInstancedTransform.size(); i++)
	{
		m_vecInstancedTransform[i].fRimRight = _float4(0.2f, 0.2f, 0.2f, 1.f);
		m_vecInstancedTransform[i].fEmissive = _float4(1.f, 0.5f, 1.f, 1.f);
		m_vecInstancedTransform[i].fDissolve = _float4(0.f,0.f,0.f,0.f);


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
		default:
			break;
		}


	}

	return S_OK;
}



HRESULT CGoldfish::SetUp_Components()
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

HRESULT CGoldfish::Adjust_AnimMovedTransform(_double dDeltatime)
{
	for (_int i = 0; i < m_vecInstancedTransform.size(); i++)
	{


	}
	return S_OK;
}

HRESULT CGoldfish::Instance_Route()
{
	_uint Index;

	Index = (_uint)m_Instance_Info.fValueMat.m[0][0];
	switch (Index)
	{
	case 0:
	{
		m_fRoute[0][0] = _float3(15.f,50.f,10.f);
		m_fRoute[0][1] = _float3(10.f,55.f,5.f);
		m_fRoute[0][2] = _float3(20.f,60.f,15.f);

		m_fRoute[1][0] = _float3(12.f, 55.f, 10.f);
		m_fRoute[1][1] = _float3(14.f, 57.f, 5.f);
		m_fRoute[1][2] = _float3(24.f, 70.f, 15.f);

		m_fRoute[2][0] = _float3(14.f, 60.f, 10.f);
		m_fRoute[2][1] = _float3(16.f, 65.f, 5.f);
		m_fRoute[2][2] = _float3(28.f, 50.f, 15.f);

		m_fRoute[3][0] = _float3(18.f, 50.f, 10.f);
		m_fRoute[3][1] = _float3(28.f, 55.f, 5.f);
		m_fRoute[3][2] = _float3(38.f, 50.f, 15.f);

	/*	m_fRoute[1][0] = _float3();
		m_fRoute[1][1] = _float3();
		m_fRoute[1][2] = _float3();

		m_fRoute[2][0] = _float3();
		m_fRoute[2][1] = _float3();
		m_fRoute[2][2] = _float3();*/
		break;
	}
	default:
		break;
	}
	return S_OK;
}

CGoldfish * CGoldfish::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGoldfish*	pInstance = NEW CGoldfish(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Spider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGoldfish::Clone(void * pArg)
{
	CGoldfish*	pInstance = NEW CGoldfish(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGoldfish");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGoldfish::Free()
{
	__super::Free();

	for (auto& pTransform : m_vecInstancedTransform)
	{
		Safe_Release(pTransform.pTransform);
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
