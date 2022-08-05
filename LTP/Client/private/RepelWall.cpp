#include "stdafx.h"
#include "..\public\RepelWall.h"
#include "Player.h"
#include "Golu.h"

CRepelWall::CRepelWall(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CRepelWall::CRepelWall(const CRepelWall & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRepelWall::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CRepelWall::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	memcpy(&m_tRepelWallDesc, pArg, sizeof(REPELWALLDESC));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Colliders());
	FAILED_CHECK(SetUp_Etc());

	Set_IsOcllusion(false);

	return S_OK;
}

_int CRepelWall::Update(_double fDeltaTime)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::Update(fDeltaTime) < 0) return -1;

	Update_Colliders();
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pCollider));

	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));
	return _int();
}

_int CRepelWall::LateUpdate(_double fDeltaTimer)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CRepelWall::Render()
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

_int CRepelWall::LateRender()
{
	return _int();
}

void CRepelWall::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player || eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player2)
	{
		_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
		_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));

		CTransform* pPlayerTransform = static_cast<CTransform*>(static_cast<CGameObject*>(pConflictedObj)->Get_Component(TAG_COM(Com_Transform)));
		_Vector vPlayerLook = XMVector3Normalize(pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
		
		_Vector vDot = XMVector3Dot(vLook, vPlayerLook);

		if (0.6f <= XMVectorGetX(vDot))
		{
			pPlayerTransform->MovetoDir_bySpeed(vLook * -1.f, 2.f, g_fDeltaTime);
		}
		else if (-0.6f >= XMVectorGetX(vDot) && -1.f <= XMVectorGetX(vDot))
		{
			pPlayerTransform->MovetoDir_bySpeed(vLook, 3.f, g_fDeltaTime);
		}
		else
		{
			vDot = XMVector3Dot(vRight, vPlayerLook);
			if (0 <= XMVectorGetX(vDot))
			{
				pPlayerTransform->MovetoDir_bySpeed(vRight * -1.f, 2.f, g_fDeltaTime);
			}
			else
			{
				pPlayerTransform->MovetoDir_bySpeed(vRight, 3.f, g_fDeltaTime);
			}
		}
	}
}

CTransform * CRepelWall::Get_TransformCom()
{
	return m_pTransformCom;
}

void CRepelWall::Set_Appear(_bool bAppear, _float fTargetTime)
{
	if (true == bAppear)
	{
		m_pDissolveCom->Set_DissolveOn(true, fTargetTime);		// Appear
		m_bDisappear = false;
	}
	else if (false == bAppear)
	{
		m_pDissolveCom->Set_DissolveOn(false, fTargetTime);	// Disappear
		m_bDisappear = true;
	}
}

void CRepelWall::Update_Colliders()
{
	_Matrix WorldMat = m_pTransformCom->Get_WorldMatrix();

	WorldMat.r[0] = XMVector3Normalize(WorldMat.r[0]);
	WorldMat.r[1] = XMVector3Normalize(WorldMat.r[1]);
	WorldMat.r[2] = XMVector3Normalize(WorldMat.r[2]);

	m_pCollider->Update_Transform(0, WorldMat);
	m_pCollider->Update_Transform(1, WorldMat);
}

HRESULT CRepelWall::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_F_BalconyPlant01), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CRepelWall::SetUp_Colliders()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.4f, 1.2f, 0.23f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

HRESULT CRepelWall::SetUp_Etc()
{
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, m_tRepelWallDesc.fSpawnPos);
	m_pTransformCom->Rotation_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_tRepelWallDesc.fRotation);
	m_pTransformCom->Scaled_All(_float3(0.2f, 0.1f, 0.5f));
	return S_OK;
}

CRepelWall * CRepelWall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRepelWall*	pInstance = NEW CRepelWall(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRepelWall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRepelWall::Clone(void * pArg)
{
	CRepelWall*	pInstance = NEW CRepelWall(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CRepelWall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRepelWall::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pModel);
}
