#include "stdafx.h"
#include "..\public\DynamicPlatform.h"
#include "Player.h"
#include "Golu.h"
#include "RepelWall.h"
#include "Scene_Laboratory_Jino.h"

CDynamicPlatform::CDynamicPlatform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CDynamicPlatform::CDynamicPlatform(const CDynamicPlatform & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDynamicPlatform::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CDynamicPlatform::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	DYNAMICPLATFORMDESC tSpawnDesc;
	ZeroMemory(&tSpawnDesc, sizeof(DYNAMICPLATFORMDESC));
	memcpy(&tSpawnDesc, pArg, sizeof(DYNAMICPLATFORMDESC));

	_float3 vPos = tSpawnDesc.fSpawnPos;
	vPos.y -= 1.f;
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	m_eOnTarget = tSpawnDesc.eOnTarget;

	m_ePattern = (EPATTERN)(rand() % PATTERN_END);

	m_iIndex = tSpawnDesc.iIndex;

	m_bEndPlatform = tSpawnDesc.bEndPlatform;

	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_Patterns());
	FAILED_CHECK(SetUp_Etc());

	Set_IsOcllusion(false);

	return S_OK;
}

_int CDynamicPlatform::Update(_double fDeltaTime)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_eOnTarget)
	{
	case EONTARGET::TARGET_RAJI:
		Update_OnTarget_Raji(fDeltaTime);
		break;
	case EONTARGET::TARGET_GOLU:
		Update_OnTarget_Golu(fDeltaTime);
		break;
	case EONTARGET::TARGET_BOTH:
		Update_OnTarget_Raji(fDeltaTime);
		Update_OnTarget_Golu(fDeltaTime);
		break;
	}

	if (false == m_bEndPlatform)
	{
		switch (m_ePattern)
		{
		case EPATTERN::PATTERN_1:
			Update_Pattern_1(fDeltaTime);
			break;
		case EPATTERN::PATTERN_2:
			Update_Pattern_2(fDeltaTime);
			break;
		case EPATTERN::PATTERN_3:
			Update_Pattern_3(fDeltaTime);
			break;
		case EPATTERN::PATTERN_4:
			Update_Pattern_4(fDeltaTime);
			break;
		case EPATTERN::PATTERN_5:
			Update_Pattern_5(fDeltaTime);
			break;
		case EPATTERN::PATTERN_6:
			Update_Pattern_6(fDeltaTime);
			break;
		case EPATTERN::PATTERN_7:
			Update_Pattern_7(fDeltaTime);
			break;
		}
	}
	Update_Colliders();
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pCollider));

	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));
	return _int();
}

_int CDynamicPlatform::LateUpdate(_double fDeltaTimer)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	
	return _int();
}

_int CDynamicPlatform::Render()
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

_int CDynamicPlatform::LateRender()
{
	return _int();
}

void CDynamicPlatform::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (true == m_bEndPlatform)
	{
		if (eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player)
		{
			CollisionTriger_OnTarget_Raji(pMyCollider, iMyColliderIndex, pConflictedObj, pConflictedCollider, iConflictedObjColliderIndex, eConflictedObjCollisionType);
		}
		else if (eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player2)
		{
			CollisionTriger_OnTarget_Golu(pMyCollider, iMyColliderIndex, pConflictedObj, pConflictedCollider, iConflictedObjColliderIndex, eConflictedObjCollisionType);
		}
	}
	else
	{
		switch (m_eOnTarget)
		{
		case EONTARGET::TARGET_RAJI:
		{
			if (eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player)
			{
				CollisionTriger_OnTarget_Raji(pMyCollider, iMyColliderIndex, pConflictedObj, pConflictedCollider, iConflictedObjColliderIndex, eConflictedObjCollisionType);
			}
		}
		break;
		case EONTARGET::TARGET_GOLU:
		{
			if (eConflictedObjCollisionType == CollisionTypeID::CollisionType_Player2)
			{
				CollisionTriger_OnTarget_Golu(pMyCollider, iMyColliderIndex, pConflictedObj, pConflictedCollider, iConflictedObjColliderIndex, eConflictedObjCollisionType);
			}
		}
		break;
		}
	}
}

void CDynamicPlatform::Set_Appear(_bool bAppear)
{
	if (true == bAppear && true == m_bDisappear)
	{
		g_pGameInstance->PlaySoundW(L"Jino_Teleport.wav", CHANNEL_PLAYER);
		m_pDissolveCom->Set_DissolveOn(true, 0.5f);		// Appear
		m_bDisappear = false;

		for (auto& iter : m_pRepelWalls)
		{
			iter->Set_Appear(true);
		}
	}
	else if(false == bAppear && false == m_bDisappear)
	{
		m_pDissolveCom->Set_DissolveOn(false, 0.5f);	// Disappear
		m_bDisappear = true;

		for (auto& iter : m_pRepelWalls)
		{
			iter->Set_Appear(false);
		}
	}
}

_int CDynamicPlatform::Update_OnTarget_Raji(_double fDeltaTime)
{
	if (true == m_bOnPlayer)
	{
		_Vector vUp = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP));
		_Vector vDot = XMVector3Dot(vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_LABORATORY_JINO, TAG_LAY(Layer_Player)));

		if (0.85f > XMVectorGetX(vDot) && false == m_bOnceSwitch2)
		{
			m_bOnceSwitch2 = true;
		}

		CTransform* pPlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
		_Vector PlayerPos = pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
		XMVectorSetZ(PlayerPos, 1.f);
		_Vector MyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		PlayerPos = XMVectorSetY(PlayerPos, XMVectorGetY(MyPos));

		if (XMVector3NotEqual(PlayerPos, MyPos) && false == m_bEndPlatform)
		{
			_Vector vLookDir = XMVector3Normalize(PlayerPos - MyPos);
			_Vector RotAxis = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir);
			m_pTransformCom->Turn_CW(RotAxis, fDeltaTime * 0.015f);

			for (auto& iter : m_pRepelWalls)
			{
				iter->Get_TransformCom()->Turn_CW(RotAxis, fDeltaTime * 0.015f);
			}
		}

		_Vector vT1 = XMVector3TransformCoord(XMVectorSet(-0.5f, 0.f, 0.5f, 1.f), m_pTransformCom->Get_WorldMatrix());
		_Vector vT2 = XMVector3TransformCoord(XMVectorSet(0.5f, 0.f, 0.5f, 1.f), m_pTransformCom->Get_WorldMatrix());
		_Vector vT3 = XMVector3TransformCoord(XMVectorSet(0.5f, 0.f, -0.5f, 1.f), m_pTransformCom->Get_WorldMatrix());

		_Vector		vPosition = pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
		if (true == m_bOnceSwitch2)
		{
			_Vector vHigher = (XMVectorGetY(vT1) >= XMVectorGetY(vT2) ? ((XMVectorGetY(vT1) >= XMVectorGetY(vT3) ? vT1 : vT3)) : (XMVectorGetY(vT2) >= XMVectorGetY(vT3) ? vT2 : vT3));
			_Vector vDir = XMVector3Normalize(vPosition - vHigher);
			m_fFallingAcc += (_float)g_fDeltaTime * 0.5f;
			vPosition = vPosition + (vDir * m_fFallingAcc);
		}


		_float	fY = 0.f;
		_Vector	vPlane;

		vPlane = XMPlaneFromPoints(vT1, vT2, vT3);

		fY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition)
			- XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition)
			- XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);


		if (false == m_bEndPlatform)
		{
			vPosition = XMVectorSetY(vPosition, fY + 0.6f);
		}
		else
		{
			vPosition = XMVectorSetY(vPosition, fY + 1.65f);
		}


		pPlayerTransform->Set_MatrixState(CTransform::TransformState::STATE_POS, vPosition);
	}

	return _int();
}

_int CDynamicPlatform::Update_OnTarget_Golu(_double fDeltaTime)
{
	if (true == m_bOnGolu)
	{
		_Vector vUp = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP));
		_Vector vDot = XMVector3Dot(vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		CGameObject* pPlayer = (CGolu*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_LABORATORY_JINO, TAG_LAY(Layer_NPC)));

		if (0.85f > XMVectorGetX(vDot) && false == m_bOnceSwitch2_Golu)
		{
			m_bOnceSwitch2_Golu = true;
		}

		CTransform* pPlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
		_Vector PlayerPos = pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
		XMVectorSetZ(PlayerPos, 1.f);
		_Vector MyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		PlayerPos = XMVectorSetY(PlayerPos, XMVectorGetY(MyPos));

		if (XMVector3NotEqual(PlayerPos, MyPos) && false == m_bEndPlatform)
		{
			_Vector vLookDir = XMVector3Normalize(PlayerPos - MyPos);
			_Vector RotAxis = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir);
			m_pTransformCom->Turn_CW(RotAxis, fDeltaTime * 0.015f);

			for (auto& iter : m_pRepelWalls)
			{
				iter->Get_TransformCom()->Turn_CW(RotAxis, fDeltaTime * 0.015f);
			}
		}

		_Vector vT1 = XMVector3TransformCoord(XMVectorSet(-0.5f, 0.f, 0.5f, 1.f), m_pTransformCom->Get_WorldMatrix());
		_Vector vT2 = XMVector3TransformCoord(XMVectorSet(0.5f, 0.f, 0.5f, 1.f), m_pTransformCom->Get_WorldMatrix());
		_Vector vT3 = XMVector3TransformCoord(XMVectorSet(0.5f, 0.f, -0.5f, 1.f), m_pTransformCom->Get_WorldMatrix());

		_Vector		vPosition = pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
		if (true == m_bOnceSwitch2_Golu)
		{
			_Vector vHigher = (XMVectorGetY(vT1) >= XMVectorGetY(vT2) ? ((XMVectorGetY(vT1) >= XMVectorGetY(vT3) ? vT1 : vT3)) : (XMVectorGetY(vT2) >= XMVectorGetY(vT3) ? vT2 : vT3));
			_Vector vDir = XMVector3Normalize(vPosition - vHigher);
			m_fFallingAcc += (_float)g_fDeltaTime * 0.5f;
			vPosition = vPosition + (vDir * m_fFallingAcc);
		}


		_float	fY = 0.f;
		_Vector	vPlane;

		vPlane = XMPlaneFromPoints(vT1, vT2, vT3);

		fY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition)
			- XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition)
			- XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

		if (false == m_bEndPlatform)
		{
			vPosition = XMVectorSetY(vPosition, fY + 0.55f);
		}
		else
		{
			vPosition = XMVectorSetY(vPosition, fY + 1.65f);
		}

		pPlayerTransform->Set_MatrixState(CTransform::TransformState::STATE_POS, vPosition);
	}

	return _int();
}

void CDynamicPlatform::Update_Colliders()
{
	_Matrix WorldMat = m_pTransformCom->Get_WorldMatrix();
	m_pCollider->Update_Transform(0, WorldMat);

	m_pCollider->Update_Transform(1, WorldMat);
	m_pCollider->Update_Transform(2, WorldMat);
	m_pCollider->Update_Transform(3, WorldMat);
	m_pCollider->Update_Transform(4, WorldMat);

	m_pCollider->Update_Transform(5, WorldMat);
	m_pCollider->Update_Transform(6, WorldMat);
	m_pCollider->Update_Transform(7, WorldMat);
	m_pCollider->Update_Transform(8, WorldMat);

	m_pCollider->Update_Transform(9, WorldMat);
}

void CDynamicPlatform::Update_Pattern_1(_double fDeltaTime)
{
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 2.f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[0]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

void CDynamicPlatform::Update_Pattern_2(_double fDeltaTime)
{
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -2.f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[0]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

void CDynamicPlatform::Update_Pattern_3(_double fDeltaTime)
{
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * 2.f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[0]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

void CDynamicPlatform::Update_Pattern_4(_double fDeltaTime)
{
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * -2.f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[0]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

void CDynamicPlatform::Update_Pattern_5(_double fDeltaTime)
{
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
	
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 2.8f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[0]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -2.8f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[1]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * 2.8f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[2]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * -2.8f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[3]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

void CDynamicPlatform::Update_Pattern_6(_double fDeltaTime)
{
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));

	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 2.6f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[0]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -2.6f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[1]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * 2.6f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[2]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * -2.6f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[3]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

void CDynamicPlatform::Update_Pattern_7(_double fDeltaTime)
{
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));

	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 1.837f) + (vRight * 1.837f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[0]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -1.837f) + (vRight * -1.837f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[1]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 1.837f) + (vRight * -1.837f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[2]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -1.837f) + (vRight * 1.837f);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
	m_pRepelWalls[3]->Get_TransformCom()->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

void CDynamicPlatform::CollisionTriger_OnTarget_Raji(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (9 == iMyColliderIndex)
	{
		if (true == m_bOnPlayer)
		{
			static_cast<CPlayer*>(pConflictedObj)->Set_State_FallingStart(g_fDeltaTime);
			m_bOnPlayer = false;
		}
	}
	else
	{
		if (true == m_bDisappear)
		{
			static_cast<CPlayer*>(pConflictedObj)->Set_State_FallingStart(g_fDeltaTime);
			m_bOnPlayer = false;
		}
		else
		{
			if (false == m_bOnPlayer && false == m_bOnceSwitch)
			{
				static_cast<CPlayer*>(pConflictedObj)->Set_State_OnDynamicPlatformStart();
				m_bOnPlayer = true;
				m_bOnceSwitch = true;
				static_cast<CScene_Laboratory_Jino*>(g_pGameInstance->Get_NowScene())->Set_PlatformIndex_Raji(m_iIndex);
			}
		}
	}
}

void CDynamicPlatform::CollisionTriger_OnTarget_Golu(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (9 == iMyColliderIndex)
	{
		if (true == m_bOnGolu)
		{
			static_cast<CGolu*>(pConflictedObj)->Set_State_FallingStart(g_fDeltaTime);
			m_bOnGolu = false;
		}
	}
	else
	{
		if (true == m_bDisappear)
		{
			static_cast<CGolu*>(pConflictedObj)->Set_State_FallingStart(g_fDeltaTime);
			m_bOnGolu = false;
		}
		else
		{
			if (false == m_bOnGolu && false == m_bOnceSwitch_Golu)
			{
				static_cast<CGolu*>(pConflictedObj)->Set_State_IdleStart(g_fDeltaTime);
				m_bOnGolu = true;
				m_bOnceSwitch_Golu = true;
				static_cast<CScene_Laboratory_Jino*>(g_pGameInstance->Get_NowScene())->Set_PlatformIndex_Golu(m_iIndex);
			}
		}
	}
}

HRESULT CDynamicPlatform::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_DS_Platform_28), TAG_COM(Com_Model), (CComponent**)&m_pModel));

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

HRESULT CDynamicPlatform::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;

	if (false == m_bEndPlatform)
	{
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(30.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(7.f, 4.f, 21.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(7.f, 4.f, 21.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(20.f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(7.f, 4.f, 21.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(40.f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(7.f, 4.f, 21.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(80.f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		//************************************************************************//

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.2f, 4.f, 22.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(10.5f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.8f, 4.f, 22.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(29.7f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.98f, 4.f, 22.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(54.1f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.38f, 4.f, 22.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(152.f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		//************************************************************************//

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(28.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else
	{
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(30.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(7.f, 4.f, 21.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(7.f, 4.f, 21.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(20.f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(7.f, 4.f, 21.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(40.f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(7.f, 4.f, 21.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(80.f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		//************************************************************************//

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.2f, 4.f, 22.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(10.5f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.8f, 4.f, 22.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(29.7f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.98f, 4.f, 22.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(54.1f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.38f, 4.f, 22.f);
		ColliderDesc.vRotation = _float4(0.f, XMConvertToRadians(152.f), 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		//************************************************************************//

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(28.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}

	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Etc()
{
	if (false == m_bEndPlatform)
	{
		m_pTransformCom->Scaled_All(_float3(0.5f));
	}
	else
	{
		m_pTransformCom->Scaled_All(_float3(1.5f));
	}

	if (m_iIndex > 0)
	{
		m_pDissolveCom->Set_DissolveOn(false, 0.f);
		m_bDisappear = true;
		
		for (auto& iter : m_pRepelWalls)
		{
			iter->Set_Appear(false, 0.f);
		}

	}
	else
	{
		m_bDisappear = false;
	}

	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Patterns()
{
	switch (m_ePattern)
	{
	case EPATTERN::PATTERN_1:
		FAILED_CHECK(SetUp_Pattern_1());
		break;
	case EPATTERN::PATTERN_2:
		FAILED_CHECK(SetUp_Pattern_2());
		break;
	case EPATTERN::PATTERN_3:
		FAILED_CHECK(SetUp_Pattern_3());
		break;
	case EPATTERN::PATTERN_4:
		FAILED_CHECK(SetUp_Pattern_4());
		break;
	case EPATTERN::PATTERN_5:
		FAILED_CHECK(SetUp_Pattern_5());
		break;
	case EPATTERN::PATTERN_6:
		FAILED_CHECK(SetUp_Pattern_6());
		break;
	case EPATTERN::PATTERN_7:
		FAILED_CHECK(SetUp_Pattern_7());
		break;
	}
	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Pattern_1()
{
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
	CRepelWall::REPELWALLDESC tRepelWallSpawnDesc;
	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 2.f);
	tRepelWallSpawnDesc.fRotation = 0.f;
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	CGameObject* pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Pattern_2()
{
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
	CRepelWall::REPELWALLDESC tRepelWallSpawnDesc;
	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -2.f);
	tRepelWallSpawnDesc.fRotation = 0.f;
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	CGameObject* pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Pattern_3()
{
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	CRepelWall::REPELWALLDESC tRepelWallSpawnDesc;
	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * 2.f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(90.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	CGameObject* pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Pattern_4()
{
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	CRepelWall::REPELWALLDESC tRepelWallSpawnDesc;
	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * -2.f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(90.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	CGameObject* pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Pattern_5()
{
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	CRepelWall::REPELWALLDESC tRepelWallSpawnDesc;
	
	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(90.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	CGameObject* pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(90.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * 2.6f);
	tRepelWallSpawnDesc.fRotation = 0.f;
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * -2.6f);
	tRepelWallSpawnDesc.fRotation = 0.f;
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));
	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Pattern_6()
{
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	CRepelWall::REPELWALLDESC tRepelWallSpawnDesc;

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(45.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	CGameObject* pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(45.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * 2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(-45.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * -2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(-45.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	return S_OK;
}

HRESULT CDynamicPlatform::SetUp_Pattern_7()
{
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
	_Vector vRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	CRepelWall::REPELWALLDESC tRepelWallSpawnDesc;

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * 2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(45.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	CGameObject* pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vLook * -2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(45.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * 2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(-45.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	tRepelWallSpawnDesc.fSpawnPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (vRight * -2.6f);
	tRepelWallSpawnDesc.fRotation = XMConvertToRadians(-45.f);
	tRepelWallSpawnDesc.pAttachTarget = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MapObject), TAG_OP(Prototype_Object_RepelWall), &tRepelWallSpawnDesc));
	pRepelWall = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject));
	m_pRepelWalls.push_back(static_cast<CRepelWall*>(pRepelWall));

	return S_OK;
}

CDynamicPlatform * CDynamicPlatform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CDynamicPlatform*	pInstance = NEW CDynamicPlatform(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CDynamicPlatform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDynamicPlatform::Clone(void * pArg)
{
	CDynamicPlatform*	pInstance = NEW CDynamicPlatform(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CDynamicPlatform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamicPlatform::Free()
{
	__super::Free();
	 
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pModel);
}
