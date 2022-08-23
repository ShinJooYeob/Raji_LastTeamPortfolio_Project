#include "stdafx.h"
#include "..\public\Elevator.h"
#include "Player.h"

CElevator::CElevator(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CElevator::CElevator(const CElevator & rhs)
	: CGameObject(rhs)
{
}

HRESULT CElevator::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CElevator::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	memcpy(&m_tElevatorDesc, pArg, sizeof(ELEVATORDESC));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_Etc());

	Set_IsOcllusion(false);

	
	return S_OK;
}

_int CElevator::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (false == m_bEndInteract)
	{
		if (true == m_bActive)
		{
			m_pTransformCom->MovetoTarget_ErrRange(m_tElevatorDesc.fDestPos.XMVector(), fDeltaTime, 0.1f);

			if (XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS)) == m_tElevatorDesc.fDestPos.y)
			{
				m_pPlayer->Set_State_ElevatorEnd();
				m_bEndInteract = true;
				g_pGameInstance->Stop_ChannelSound(CHANNEL_TEMP);
				return _int();
			}
			else
			{
				m_pPlayer->Set_PosY(m_pCollider->Get_ColliderPosition(1).y);
			}
		}

		Update_Colliders();
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pCollider));
	}

	return _int();
}

_int CElevator::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	return _int();
}

_int CElevator::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPEFORENGINE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 2, i, "g_BoneMatrices"));
	}


	return _int();
}

_int CElevator::LateRender()
{
	return _int();
}

void CElevator::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player)
	{
		if (false == m_bActive)
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(pConflictedObj);
			m_pPlayer = pPlayer;
			m_pPlayer->Set_State_ElevatorStart();
			m_bActive = true;
			g_pGameInstance->PlaySoundW(L"Jino_Elevator_Moving.wav", CHANNEL_TEMP);
		}
	}
}

void CElevator::Update_Colliders()
{
	_Matrix mat = m_pTransformCom->Get_WorldMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	
	m_pCollider->Update_Transform(0, mat);
	m_pCollider->Update_Transform(1, mat);
}

HRESULT CElevator::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	
	switch (m_tElevatorDesc.iMeshType)
	{
	case 0:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_InteractObj_Elevator), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case 1:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Elevator), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CElevator::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;


	//_float fScale = -5.2f * (XMVectorGetX(m_pTransformCom->Get_Scale()) / 1.25f);
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, m_tElevatorDesc.fColliderOffset_Y, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(m_tElevatorDesc.fColliderScale);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, m_tElevatorDesc.fColliderOffset_Y, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

HRESULT CElevator::SetUp_Etc()
{
	m_pTransformCom->Set_MoveSpeed(m_tElevatorDesc.fMoveSpeed);
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, m_tElevatorDesc.fStartPos);
	m_pTransformCom->Scaled_All(m_tElevatorDesc.fScale);
	m_pTransformCom->Rotation_Multi(m_tElevatorDesc.fRotation);

	return S_OK;
}

CElevator * CElevator::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CElevator*	pInstance = NEW CElevator(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CElevator");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CElevator::Clone(void * pArg)
{
	CElevator*	pInstance = NEW CElevator(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CElevator");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CElevator::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pModel);
}
