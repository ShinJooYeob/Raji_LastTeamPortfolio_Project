#include "stdafx.h"
#include "../public/BeachBall.h"
#include "GameInstance.h"
#include "Shader.h"
#include "MiniGame_Jino_Player.h"
#include "Scene_MiniGame_Jino.h"

CBeachBall::CBeachBall(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CBeachBall::CBeachBall(const CBeachBall & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBeachBall::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CBeachBall::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	_float3 vPos = *(static_cast<_float3*>(pArg));
	vPos.y += 0.4f;
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	FAILED_CHECK(SetUp_Etc());
	FAILED_CHECK(SetUp_Colliders());

	return S_OK;
}

_int CBeachBall::Update(_double fDeltaTime)
{
	if (false == m_bActive)
	{
		return 0;
	}

	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_eCurState)
	{
	case STATE_SPAWN:
	{
		Update_Spawn(fDeltaTime);
		break;
	}
	case STATE_RIDE:
	{
		Update_Ride(fDeltaTime);
		break;
	}
	case STATE_LOSE:
	{
		Update_Lose(fDeltaTime);
		break;
	}
	case STATE_LOSE_R:
	{
		Update_Lose_R(fDeltaTime);
		break;
	}
	}

	Update_Colliders();

	return _int();
}

_int CBeachBall::LateUpdate(_double fDeltaTimer)
{
	if (false == m_bActive)
	{
		return 0;
	}

	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider_RideArea));
#endif	// _DEBUG

	return _int();
}

_int CBeachBall::Render()
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

_int CBeachBall::LateRender()
{
	return _int();
}

void CBeachBall::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	switch (m_eCurState)
	{
	case STATE_SPAWN:
	{
		if (pMyCollider == m_pCollider)
		{
			CMiniGame_Jino_Player* pPlayer = static_cast<CMiniGame_Jino_Player*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player)));
			pPlayer->Set_State_LoseBeachBall();
			Set_State_Lose_R();
		}
		else if (pMyCollider == m_pCollider_RideArea)
		{
			if (4 >= iConflictedObjColliderIndex)
			{
				CMiniGame_Jino_Player* pPlayer = static_cast<CMiniGame_Jino_Player*>(pConflictedObj);
				if (true == pPlayer->Is_Falling())
				{
					static_cast<CMiniGame_Jino_Player*>(pConflictedObj)->Set_BeachBall(this);
					Set_State_Ride();
					g_pGameInstance->Play3D_Sound(TEXT("Jino_MiniGame_OnBall.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNEL_PLAYER);
				}
			}
		}
		break;
	}
	}
}

void CBeachBall::SetUp_PlayerTransform(CTransform* pPlayerTransform)
{
	m_pPlayerTransform = pPlayerTransform;
}

void CBeachBall::Set_Active(_bool bActive)
{
	if (bActive == false)
	{
		int a = 10;
	}
	m_bActive = bActive;
}

void CBeachBall::Set_Position(_float3 vPos)
{
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPos);
}

void CBeachBall::Update_Spawn(_double fDeltaTime)
{
	m_pTransformCom->MovetoDir(XMVectorSet(-1.f, 0.f, 0.f, 0.f), fDeltaTime);
	m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), fDeltaTime);

	_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
	_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	_float fPlayerPos_X = XMVectorGetX(vPlayerPos);
	_float fMyPos_X = XMVectorGetX(vMyPos);

	_float fDist = fMyPos_X - fPlayerPos_X;

	if (-7.f >= fDist || 3.5f >= fMyPos_X || 28.f <= fDist)
	{
		m_bActive = false;
	}
}

void CBeachBall::Update_Ride(_double fDeltaTime)
{

}

void CBeachBall::Update_Lose(_double fDeltaTime)
{
	m_pTransformCom->MovetoDir(XMVectorSet(-1.f, 0.f, 0.f, 0.f), fDeltaTime);
	m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 0.f, 1.f, 0.f), fDeltaTime);

	m_fDelayTime -= (_float)fDeltaTime;
	if (0.f >= m_fDelayTime)
	{
		m_fDelayTime = 3.f;
		m_bActive = false;
		static_cast<CScene_MiniGame_Jino*>(g_pGameInstance->Get_NowScene())->Push_BeachBall(this);
	}
}

void CBeachBall::Update_Lose_R(_double fDeltaTime)
{
	m_pTransformCom->MovetoDir(XMVectorSet(1.f, 0.f, 0.f, 0.f), fDeltaTime);
	m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), fDeltaTime);

	m_fDelayTime -= (_float)fDeltaTime;
	if (0.f >= m_fDelayTime)
	{
		m_fDelayTime = 3.f;
		m_bActive = false;
		static_cast<CScene_MiniGame_Jino*>(g_pGameInstance->Get_NowScene())->Push_BeachBall(this);
	}
}

void CBeachBall::Update_Colliders()
{
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	_Matrix mat = m_pTransformCom->Get_WorldMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	if (STATE_RIDE == m_eCurState)
	{
		m_pCollider->Update_Transform(0, mat); 
		g_pGameInstance->Add_CollisionGroup(CollisionType_MiniGame_BALL_1, this, m_pCollider);
	}
	else if (STATE_SPAWN == m_eCurState)
	{
		m_pCollider->Update_Transform(0, mat);

		mat = XMMatrixIdentity();
		mat.r[3] = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
		m_pCollider_RideArea->Update_Transform(0, mat);

		g_pGameInstance->Add_CollisionGroup(CollisionType_MiniGame_BALL_2, this, m_pCollider);
		g_pGameInstance->Add_CollisionGroup(CollisionType_MiniGame_RideArea, this, m_pCollider_RideArea);

	}
}

CBeachBall::EBeachBallState CBeachBall::Get_CurState()
{
	return m_eCurState;
}

CTransform * CBeachBall::Get_TransformCom()
{
	return m_pTransformCom;
}

void CBeachBall::Set_State_Spawn()
{
	m_eCurState = STATE_SPAWN;
	m_pTransformCom->Set_MoveSpeed(4.f);
	m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(500.f));
}

void CBeachBall::Set_State_Ride()
{
	m_eCurState = STATE_RIDE;
	m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(600.f));
}

void CBeachBall::Set_State_Lose()
{
	m_eCurState = STATE_LOSE;
	m_pTransformCom->Set_MoveSpeed(15.f);
	m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(600.f));

	m_fDelayTime = 3.f;
}

void CBeachBall::Set_State_Lose_R()
{
	m_eCurState = STATE_LOSE_R;
	m_pTransformCom->Set_MoveSpeed(15.f);
	m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(600.f));

	m_fDelayTime = 3.f;
}

HRESULT CBeachBall::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_BeachBall), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 15.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360.f);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CBeachBall::SetUp_Colliders()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.9f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider_1), (CComponent**)&m_pCollider_RideArea));
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.4f, 0.1f, 0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pCollider_RideArea->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));


	return S_OK;
}

HRESULT CBeachBall::SetUp_Etc()
{
	Set_State_Ride();
	Set_IsOcllusion(false);
	m_pTransformCom->Scaled_All(_float3(0.03f));
	m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(600.f));

	return S_OK;
}

CBeachBall * CBeachBall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CBeachBall*	pInstance = NEW CBeachBall(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CBeachBall");
		Safe_Release(pInstance);
	}
	return pInstance;	
}

CGameObject * CBeachBall::Clone(void * pArg)
{
	CBeachBall*	pInstance = NEW CBeachBall(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CBeachBall");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBeachBall::Free()
{
	__super::Free();

	Safe_Release(m_pCollider_RideArea);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pModel);
}
