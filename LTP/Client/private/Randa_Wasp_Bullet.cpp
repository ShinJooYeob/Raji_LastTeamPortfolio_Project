#include "stdafx.h"
#include "..\public\Randa_Wasp_Bullet.h"

CRanda_Wasp_Bullet::CRanda_Wasp_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRanda_Wasp_Bullet::CRanda_Wasp_Bullet(const CRanda_Wasp_Bullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRanda_Wasp_Bullet::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}
HRESULT CRanda_Wasp_Bullet::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
		m_vStartPosition = *(_float3*)pArg;
	

	FAILED_CHECK(SetUp_Components());


	//if (pArg != nullptr)
	//{
	//	_float3 Pos = *(_float3*)pArg;
	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);

	//}
	//m_iPassIndex = 3;

	//m_pTransformCom->Scaled_All(_float3(2.f));
	//m_pTransformCom->Rotation_CCW(XMVectorSet(0,1.f,0,0),XMConvertToRadians(90.f));

	return S_OK;
}

_int CRanda_Wasp_Bullet::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pModel->Update_AnimationClip(g_fDeltaTime));

	//m_pDissolveCom->Update_Dissolving(fDeltaTime);
	m_vecRenderVector.clear();
	m_vecLimLight.clear();
	m_vecEmissive.clear();
	m_vecRenderVector.reserve(m_vecRWB.size());
	_uint iDeathCount = 0; 

	for (auto& tDesc : m_vecRWB)
	{
		tDesc.bEachLifeTime += (_float)fDeltaTime;
		
		if (tDesc.bIsDead)
		{
			iDeathCount++;
			continue;
		}


		if (tDesc.pTransform->Get_MatrixState_Float3(CTransform::STATE_POS).y < 95.4)
		{
			tDesc.bIsDead = true;
			iDeathCount++;
			continue;
		}
		else if (tDesc.bEachLifeTime > 0)
		{
			tDesc.pTransform->Move_Forward(fDeltaTime);
			m_vecRenderVector.push_back(tDesc.pTransform);
			m_vecLimLight.push_back(_float4(0.2f, 1.f, 0.2f, 0.3f));
			m_vecEmissive.push_back(_float4(1.f, 1.f, 1.0f, 1.f));
		}
		

	}

	if (iDeathCount >= m_vecRWB.size() - 1.f)
		Set_IsDead();


	CGameObject* PlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));
	CTransform* PlayerTransform = (CTransform*)PlayerObj->Get_Component(TAG_COM(Com_Transform));
	_Matrix mat = PlayerTransform->Get_WorldMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	m_pCollider->Update_Transform(0, mat);

	for (_uint i = 0; i < m_vecRenderVector.size(); i++)
	{
		m_pCollider->Update_Transform(i+1, m_vecRenderVector[i]->Get_WorldMatrix());
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));

	return _int();
}

_int CRanda_Wasp_Bullet::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this,
		&m_vecRenderVector, m_pModelInstance, m_pShaderCom, m_pModel));


	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	

	return _int();
}

_int CRanda_Wasp_Bullet::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	
		
	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecRenderVector,0,&m_vecLimLight,&m_vecEmissive));


	return 0;
}

_int CRanda_Wasp_Bullet::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CRanda_Wasp_Bullet::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

HRESULT CRanda_Wasp_Bullet::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Wasp), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	FAILED_CHECK(m_pModel->Update_AnimationClip(g_fDeltaTime));

	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_256), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));
	
	CGameInstance* pInstance = g_pGameInstance;
	CUtilityMgr*	pUtil = GetSingle(CUtilityMgr);
	_Vector vPlayerPosition = pInstance->Get_TargetPostion_Vector(PLV_PLAYER);



	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fMovePerSec = 30.f;

	for (_uint i = 0 ; i < 256; i++)
	{
		RWBDESC tDesc;
		tDesc.pTransform = (CTransform*)(pInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), &tTransformDesc));
		NULL_CHECK_RETURN(tDesc.pTransform, E_FAIL);

		tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, m_vStartPosition);
		tDesc.pTransform->LookAt(XMVectorSetY(pUtil->RandomFloat3(100.f, 200.f).XMVector(), 94.5f));
		tDesc.pTransform->Scaled_All(_float3(2.f));

	

		tDesc.bIsDead = false;
		tDesc.bEachLifeTime =- (_float (i) / 256.f) * 2.f;
		m_vecRWB.push_back(tDesc);
	}

	COLLIDERDESC			ColliderDesc;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(100.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	for (_uint i = 0; i < 256; i++)
	{
		//피격 충돌체
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(0.8f, 0.8f, 0.8f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.9f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}


	return S_OK;
}

CRanda_Wasp_Bullet * CRanda_Wasp_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRanda_Wasp_Bullet*	pInstance = NEW CRanda_Wasp_Bullet(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRanda_Wasp_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRanda_Wasp_Bullet::Clone(void * pArg)
{
	CRanda_Wasp_Bullet*	pInstance = NEW CRanda_Wasp_Bullet(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CRanda_Wasp_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRanda_Wasp_Bullet::Free()
{
	__super::Free();

	for (auto& pDesc : m_vecRWB)
		Safe_Release(pDesc.pTransform);
	m_vecRWB.clear();


	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pModelInstance);



	
}
