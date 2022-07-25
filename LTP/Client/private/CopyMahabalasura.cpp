#include "stdafx.h"
#include "..\public\CopyMahabalasura.h"
#include "Mahabalasura.h"

CCopyMahabalasura::CCopyMahabalasura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}


CCopyMahabalasura::CCopyMahabalasura(const CCopyMahabalasura & rhs)
	: CBoss(rhs)
{
}

HRESULT CCopyMahabalasura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CCopyMahabalasura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));

	m_pBossObj = (CMahabalasura*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Boss));

	FAILED_CHECK(SetUp_Components());

	m_fAttackCoolTime = 4.f;

	m_bIsHit = false;

	//m_pTransformCom->Scaled_All(_float3(1.5f));

	m_iRandomIndex = rand() % 16;

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
	m_startPos = PlayerPos;

	return S_OK;
}

_int CCopyMahabalasura::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	m_fAttackCoolTime -= (_float)fDeltaTime;

	if (m_fAttackCoolTime <= 0 && !m_bIsAttack)
	{
		m_pModel->Change_AnimIndex_ReturnTo(1, 0);

	}

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
	_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{

		m_vecInstancedTransform[i]->LookAt(XMLoadFloat3(&m_startPos));

		if (i % 2 == 0)
		{
			m_vecInstancedTransform[i]->Move_Left(fDeltaTime);
		}
		else if (i % 2 == 1)
		{
			m_vecInstancedTransform[i]->Move_Right(fDeltaTime);
		}
	}

	//나중에 충돌로 해당 인덱스일때 다 사라지게 하고 보스를 그위치에서 나오게끔한다.
	if (g_pGameInstance->Get_DIKeyState(DIK_5 & DIS_Down))
	{
		CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
		_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

		_float3 CopyPos = m_vecInstancedTransform[m_iRandomIndex]->Get_MatrixState(CTransform::STATE_POS);

		BossTransform->Set_MatrixState(CTransform::STATE_POS, CopyPos);
		m_pBossObj->Set_CopyOff(false);
		m_pBossObj->Set_Hit();

		Set_IsDead();
	}
	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	m_pAttackCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pCollider->Update_ConflictPassedTime(1);

	_Matrix mat = PlayerTransform->Get_WorldMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	m_pAttackCollider->Update_Transform(0, mat);
	m_pCollider->Update_Transform(0, mat);
	

	if (m_bIsAttack)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pAttackCollider));
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pCollider));

	return _int();
}

_int CCopyMahabalasura::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);



	m_vecRenderInstanceTransform.clear();

	sort(m_vecColliderIndexs.begin(), m_vecColliderIndexs.end());

	_int ColliderIndex = 0;

	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{

		if (m_vecColliderIndexs.size() > ColliderIndex && m_vecColliderIndexs[ColliderIndex] == i)
		{
			++ColliderIndex;
			continue;
		}
		else
		{
			m_vecRenderInstanceTransform.push_back(m_vecInstancedTransform[i]);
		}
		//_bool bState = false;
		//for (_int j = 0; j < m_vecColliderIndexs.size(); ++j)
		//{
		//	if (i == m_vecColliderIndexs[j])
		//	{
		//		bState = true;
		//		break;
		//	}
		//}
		//if(!bState)
		//	m_vecRenderInstanceTransform.push_back(m_vecInstancedTransform[i]);
	}

	ColliderIndex = 0;

	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{

		if (m_vecColliderIndexs.size() > ColliderIndex && m_vecColliderIndexs[ColliderIndex] == i)
		{
			++ColliderIndex;
			continue;
		}

		m_vecInstancedTransform[i]->LookAtExceptY(XMLoadFloat3(&PlayerPos));


		_Matrix mat = m_vecInstancedTransform[i]->Get_WorldMatrix();
		mat.r[0] = XMVector3Normalize(mat.r[0]);
		mat.r[1] = XMVector3Normalize(mat.r[1]);
		mat.r[2] = XMVector3Normalize(mat.r[2]);
		mat.r[3] = mat.r[3] + m_vecInstancedTransform[i]->Get_MatrixState(CTransform::STATE_LOOK);
		m_pAttackCollider->Update_Transform(i + 1, mat);
		m_pCollider->Update_Transform(i + 1, m_vecInstancedTransform[i]->Get_WorldMatrix());

	}



	//FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	//m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pAttackCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CCopyMahabalasura::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);





	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecRenderInstanceTransform));


	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	//if(i == 10)continue;

	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}

	return _int();
}

_int CCopyMahabalasura::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CCopyMahabalasura::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	switch (eConflictedObjCollisionType)
	{
	case Engine::CollisionType_Player:
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
		break;
	case Engine::CollisionType_PlayerWeapon:
		if (m_iRandomIndex == iMyColliderIndex - 1)
		{
			CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
			_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

			_float3 CopyPos = m_vecInstancedTransform[m_iRandomIndex]->Get_MatrixState(CTransform::STATE_POS);

			BossTransform->Set_MatrixState(CTransform::STATE_POS, CopyPos);
			m_pBossObj->Set_CopyOff(false);
			m_pBossObj->Set_Hit();

			Set_IsDead();
		}
		else
		{
			for (_int i = 0; i < m_vecColliderIndexs.size(); ++i)
			{
				if (m_vecColliderIndexs[i] == _uint(iMyColliderIndex - 1))
					return;
			}
			m_pCollider->Delete_ChildeBuffer(0,iMyColliderIndex);
			m_vecColliderIndexs.push_back(_uint(iMyColliderIndex - 1));
		}
		break;
	}

	//if (pMyCollider == m_pAttackCollider)
	//{

	//}
	//else if (pMyCollider == m_pCollider)
	//{
	//
	//}
}

_float CCopyMahabalasura::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

HRESULT CCopyMahabalasura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasurCopy), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pAttackCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(100.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pAttackCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(100.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	for (_uint i = 0; i < 16; i++)
	{
		CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(pTransform, E_FAIL);

		pTransform->Set_MoveSpeed(1.5f);

		CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
		_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

		pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(GetSingle(CUtilityMgr)->RandomFloat(-10.5f, 10.5f), BossPos.y, GetSingle(CUtilityMgr)->RandomFloat(-10.5f, 10.5f)));
		//pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(rand()& 6+1 * iTemp, BossPos.y, rand() & 6 + 1 * iTemp));
		m_vecInstancedTransform.push_back(pTransform);

		//Attack충돌체
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.3f, 1.6f, 1);
		FAILED_CHECK(m_pAttackCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pAttackCollider->Set_ParantBuffer();

		//피격 충돌체
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(3.5f, 3.5f, 3.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}

	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_16), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));


	//CTransform::TRANSFORMDESC tDesc = {};

	//tDesc.fMovePerSec = 5;
	//tDesc.fRotationPerSec = XMConvertToRadians(60);
	//tDesc.fScalingPerSec = 1;
	//tDesc.vPivot = _float3(0, 0, 0);

	//FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CCopyMahabalasura::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 0:
			break;
		case 1:
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.233644859)
			{
				m_bIsAttack = true;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 1 && PlayRate > 0.2803738)
			{
				m_bIsAttack = false;;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate > 0.392523)
			{
				m_bIsAttack = true;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 3 && PlayRate > 0.47663551)
			{
				m_bIsAttack = false;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 4 && PlayRate > 0.56074766)
			{
				m_bIsAttack = true;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 5 && PlayRate > 0.6074766)
			{
				m_bIsAttack = false;
				++m_iAdjMovedIndex;
			}
			break;

		case 5:

		break;


		}
	}
	else
	{
		if (iNowAnimIndex == 0)
		{
		}
		if (iNowAnimIndex == 1)
		{
			m_bIsAttack = false;
			m_fAttackCoolTime = 4.f;
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CCopyMahabalasura * CCopyMahabalasura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CCopyMahabalasura*	pInstance = NEW CCopyMahabalasura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CCopyMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCopyMahabalasura::Clone(void * pArg)
{
	CCopyMahabalasura*	pInstance = NEW CCopyMahabalasura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CCopyMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCopyMahabalasura::Free()
{
	__super::Free();

	//Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pModelInstance);
	Safe_Release(m_pAttackCollider);
	Safe_Release(m_pCollider);

	for (auto& pTransform : m_vecInstancedTransform)
		Safe_Release(pTransform);
	m_vecInstancedTransform.clear();
}
