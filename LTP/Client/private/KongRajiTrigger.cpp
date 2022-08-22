#include "stdafx.h"
#include "..\public\KongRajiTrigger.h"
#include "MiniGame_KongRaji.h"


CKongRajiTrigger::CKongRajiTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CKongRajiTrigger::CKongRajiTrigger(const CKongRajiTrigger & rhs)
	: CGameObject(rhs)
{
}

HRESULT CKongRajiTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CKongRajiTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	SetUp_Info();

	m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DEBUGCOLLIDER, true);



	return S_OK;
}

_int CKongRajiTrigger::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	Once_Trigger();


	Update_Trigger(dDeltaTime);
	Update_Collider(dDeltaTime);

	return _int();
}

_int CKongRajiTrigger::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));

	return _int();
}

_int CKongRajiTrigger::Render()
{
	if (__super::Render() < 0)
		return -1;

	return _int();
}

_int CKongRajiTrigger::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

void CKongRajiTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		if (m_pPlayer->Get_bJump() != true)
		{
			m_iColliderIndex = iMyColliderIndex;
			switch (iMyColliderIndex)
			{
			case 1:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex + 1);
				m_pPlayer->Set_NaviIndex(1);

				m_pPlayer->Set_HeightPos(m_fColliderPos);

				m_pPlayer->Set_MoveToWidthOn(true);
				break;
			}
			case 2:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex - 1);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_pPlayer->Set_HeightPos(m_fColliderPos);

				if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f <= m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(3);
					m_pPlayer->Set_MoveToWidthOn(true);
				}
				else if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f > m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(1);
				}
				break;
			}
			case 3:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex + 1);
				m_pPlayer->Set_NaviIndex(3);

				m_pPlayer->Set_HeightPos(m_fColliderPos);

				m_pPlayer->Set_MoveToWidthOn(true);
				break;
			}
			case 4:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex - 1);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_pPlayer->Set_HeightPos(m_fColliderPos);

				if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f <= m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(5);
					m_pPlayer->Set_MoveToWidthOn(true);
				}
				else if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f > m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(3);
				}
				break;
			}
			case 5:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex + 1);
				m_pPlayer->Set_NaviIndex(3);

				m_pPlayer->Set_HeightPos(m_fColliderPos);

				m_pPlayer->Set_MoveToWidthOn(true);
				break;
			}
			case 6:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex - 1);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_pPlayer->Set_HeightPos(m_fColliderPos);

				if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f <= m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(5);
					m_pPlayer->Set_MoveToWidthOn(true);
				}
				else if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f > m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(3);
				}
				break;
			}
			case 7:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex + 1);
				m_pPlayer->Set_NaviIndex(5);

				m_pPlayer->Set_HeightPos(m_fColliderPos);

				m_pPlayer->Set_MoveToWidthOn(true);
				break;
			}
			case 8:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex - 1);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_pPlayer->Set_HeightPos(m_fColliderPos);

				if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f <= m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(7);
					m_pPlayer->Set_MoveToWidthOn(true);
				}
				else if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f > m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(5);
				}
				break;
			}
			case 9:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex + 1);
				m_pPlayer->Set_NaviIndex(7);

				m_pPlayer->Set_HeightPos(m_fColliderPos);

				m_pPlayer->Set_MoveToWidthOn(true);
				break;
			}
			case 10:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex - 1);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_pPlayer->Set_HeightPos(m_fColliderPos);

				if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f <= m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(9);
					m_pPlayer->Set_MoveToWidthOn(true);
				}
				else if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f > m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(7);
				}
				break;
			}
			case 11:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex + 1);
				m_pPlayer->Set_NaviIndex(7);

				m_pPlayer->Set_HeightPos(m_fColliderPos);

				m_pPlayer->Set_MoveToWidthOn(true);
				break;
			}
			case 12:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex - 1);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_pPlayer->Set_HeightPos(m_fColliderPos);

				if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f <= m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(9);
					m_pPlayer->Set_MoveToWidthOn(true);
				}
				else if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f > m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(7);
				}
				break;
			}
			case 13:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex + 1);
				m_pPlayer->Set_NaviIndex(9);

				m_pPlayer->Set_HeightPos(m_fColliderPos);

				m_pPlayer->Set_MoveToWidthOn(true);
				break;
			}
			case 14:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex - 1);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_pPlayer->Set_HeightPos(m_fColliderPos);

				if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f <= m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(11);
					m_pPlayer->Set_MoveToWidthOn(true);
				}
				else if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f > m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(9);
				}
				break;
			}
			case 15:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex + 1);
				m_pPlayer->Set_NaviIndex(11);

				m_pPlayer->Set_HeightPos(m_fColliderPos);

				m_pPlayer->Set_MoveToWidthOn(true);
				break;
			}
			case 16:
			{
				m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex - 1);
				m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT] = pMyCollider->Get_ColliderPosition(iMyColliderIndex);
				m_pPlayer->Set_HeightPos(m_fColliderPos);

				if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f <= m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(13);
					m_pPlayer->Set_MoveToWidthOn(true);
				}
				else if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y - 0.3f > m_fKongRajiPos.y)
				{
					m_pPlayer->Set_NaviIndex(11);
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

_float CKongRajiTrigger::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

HRESULT CKongRajiTrigger::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	SetUp_Collider();

	return S_OK;
}

HRESULT CKongRajiTrigger::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

	COLLIDERDESC			ColliderDesc;

	//Index == 0;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(30.f, 30.f, 30.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(40.f, 40.f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	////////////////////////첫번째
	//Index == 1;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(50.25f, 33.437f, 40.f, 1.f);  //_float4(50.535f,33.442f,40.f,1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 2;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(50.25f, 35.388f, 40.f, 1.f); //_float4(50.535f, 35.388f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	////////////////////////

	////////////////////////두번째
	//Index == 3;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(39.417f,35.67f,40.f,1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 4;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(39.417f, 37.894f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	////////////////////////

	////////////////////////세번째
	//Index == 5;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(31.68f, 35.872f, 40.f, 1.f);  //_float4(31.413f, 35.879f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 6;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(31.68f, 37.734f, 40.f, 1.f);  //_float4(31.413f, 37.729f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	////////////////////////

	////////////////////////네번째
	//Index == 7;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(41.46f, 37.936f, 40.f, 1.f);  //_float4(41.499f, 37.936f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 8;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(41.46f, 40.316f, 40.f, 1.f);	//(41.499f, 40.315f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	////////////////////////

	////////////////////////다섯번째
	//Index == 9;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(36.56f, 40.438f, 40.f, 1.f);  //_float4(36.451f, 40.441f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 10;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(36.56f, 42.497f, 40.f, 1.f);  //_float4(36.451f, 42.557f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	////////////////////////

	////////////////////////여섯번째
	//Index == 11;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(30.72f, 40.585f, 40.f, 1.f);  //_float4(30.389f, 40.593f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 12;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(30.72f, 42.349f, 40.f, 1.f);  //_float4(30.389f, 42.43f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	////////////////////////

	////////////////////////일곱번째
	//Index == 13;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(46.32f, 42.745f, 40.f, 1.f); //_float4(46.475f, 42.768f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 14;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(46.32f, 44.617f, 40.f, 1.f);  //_float4(46.475f, 44.838f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	////////////////////////

	////////////////////////여덟번째
	//Index == 15;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(42.38f, 44.721f, 40.f, 1.f);  //_float4(42.474f, 44.891f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 16;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(42.38f, 46.68f, 40.f, 1.f);  //_float4(42.474f, 46.96f, 40.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	////////////////////////

	return S_OK;
}

HRESULT CKongRajiTrigger::SetUp_Info()
{
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

HRESULT CKongRajiTrigger::Update_Collider(_double dDeltaTime)
{
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);

	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	_uint iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
	{
		m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());
	}
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pColliderCom));


	switch (m_iColliderIndex)
	{
	case 2:
	{
		if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y <= m_fKongRajiPos.y)
		{
			m_pPlayer->Set_NaviIndex(3);
		}
		break;
	}
	case 4:
	{
		if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y <= m_fKongRajiPos.y)
		{
			m_pPlayer->Set_NaviIndex(5);
		}
		break;
	}
	case 6:
	{
		if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y <= m_fKongRajiPos.y)
		{
			m_pPlayer->Set_NaviIndex(5);
		}
		break;
	}
	case 8:
	{
		if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y <= m_fKongRajiPos.y)
		{
			m_pPlayer->Set_NaviIndex(7);
		}
		break;
	}
	case 10:
	{
		if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y <= m_fKongRajiPos.y)
		{
			m_pPlayer->Set_NaviIndex(9);
		}
		break;
	}
	case 12:
	{
		if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y <= m_fKongRajiPos.y)
		{
			m_pPlayer->Set_NaviIndex(9);
		}
		break;
	}
	case 14:
	{
		if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y <= m_fKongRajiPos.y)
		{
			m_pPlayer->Set_NaviIndex(11);
		}
		break;
	}
	case 16:
	{
		if (m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y <= m_fKongRajiPos.y)
		{
			m_pPlayer->Set_NaviIndex(13);
		}
		break;
	}
	default:
		break;
	}

	return S_OK;
}

HRESULT CKongRajiTrigger::Once_Trigger()
{
	if (m_bOnce_Switch == false)
	{
		m_pPlayer = static_cast<CMiniGame_KongRaji*>(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player)));
		m_pPlayerTransform = (CTransform*)m_pPlayer->Get_Component(TAG_COM(Com_Transform));

		m_bOnce_Switch = true;
	}

	return S_OK;

}

HRESULT CKongRajiTrigger::Update_Trigger(_double dDeltaTime)
{
	m_fKongRajiPos = m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS);


	if (m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT].x - 0.3f <= m_fKongRajiPos.x &&
		m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT].x + 0.3f >= m_fKongRajiPos.x)
	{
		m_pPlayer->Set_MoveToHeightOn(true);
		//if (m_fColliderPos[ONE_POINT].y + 0.1f <= KongRajiPos.y &&
		//	m_fColliderPos[TWO_POINT].y - 0.1f >= KongRajiPos.y)
		//{
		//	//m_pPlayer->Set_MoveToHeightOn(true);
		//}
	}
	else {
		m_pPlayer->Set_MoveToHeightOn(false);
	}

	//if (m_fColliderPos[CKongRajiTrigger::COLLIDER_ONE_POINT].y < m_fKongRajiPos.y &&
	//	m_fColliderPos[CKongRajiTrigger::COLLIDER_TWO_POINT].y > m_fKongRajiPos.y)
	//{
	//	m_pPlayer->Set_MoveToWidthOn(false);
	//}

	return S_OK;
}


CKongRajiTrigger * CKongRajiTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CKongRajiTrigger*	pInstance = NEW CKongRajiTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CKongRajiTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CKongRajiTrigger::Clone(void * pArg)
{
	CKongRajiTrigger*	pInstance = NEW CKongRajiTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CKongRajiTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CKongRajiTrigger::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);

}