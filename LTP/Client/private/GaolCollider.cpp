#include "stdafx.h"
#include "..\public\GaolCollider.h"
#include "Taiko_Monster.h"
#include "Scene_MiniGame_JJB.h"
#include "PartilceCreateMgr.h"

CGaolCollider::CGaolCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CGaolCollider::CGaolCollider(const CGaolCollider & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGaolCollider::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CGaolCollider::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
	{
		memcpy(&m_GoalColliderDesc, pArg, sizeof(GOALCOLLIDERDESC));
	}

	FAILED_CHECK(SetUp_Components());

	m_pTransformCom->Scaled_All(_float3(1.f));
	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(-13.2f, 0.5f, -1.f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_GoalColliderDesc.vNotePosition);


	return S_OK;
}

_int CGaolCollider::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	//Collider
	_uint	iNumCollider = m_pCollider->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
	{
		_Matrix mat = XMMatrixIdentity();
		mat.r[3] = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		m_pCollider->Update_Transform(i, mat);

	}



	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider));

	return _int();
}

_int CGaolCollider::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CGaolCollider::Render()
{
	if (__super::Render() < 0)
		return -1;

	return _int();
}

_int CGaolCollider::LateRender()
{
	return _int();
}

void CGaolCollider::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (eConflictedObjCollisionType == CollisionType_Monster)
	{
		CTaiko_Monster* Monster = static_cast<CTaiko_Monster*>(pConflictedObj);
		if (!Monster->Get_IsUse()) return;

		if (m_GoalColliderDesc.ColliderType == CGaolCollider::COLLIDER_GOOD)
		{

			if (g_pGameInstance->Get_DIKeyState(DIK_F)&DIS_Down || g_pGameInstance->Get_DIKeyState(DIK_J)&DIS_Down)
			{

				if (Monster->Get_NoteDesc().NoteType == CTaiko_Monster::NOTE_SMALL && Monster->Get_NoteDesc().NotePosType == CTaiko_Monster::NOTEPOS_IN)
				{
					Monster->UseOFF();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Set_ComboPlus();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->ShowComboUI();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->ShowGoodEffectUI();

					_int MaxCombo = static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Get_MaxCombo();

					if(MaxCombo == 50)
						g_pGameInstance->PlaySoundW(L"Taiko_50combo_1P.wav", CHANNEL_EFFECT);

					g_pGameInstance->PlaySoundW(L"Taiko_Don.wav", CHANNEL_EFFECT);
				}
				/*else
				{
					Monster->UseOFF();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Set_ComboZero();
				}*/
			}

			if (g_pGameInstance->Get_DIKeyState(DIK_D)&DIS_Down || g_pGameInstance->Get_DIKeyState(DIK_K)&DIS_Down)
			{
				if (Monster->Get_NoteDesc().NoteType == CTaiko_Monster::NOTE_SMALL && Monster->Get_NoteDesc().NotePosType == CTaiko_Monster::NOTEPOS_OUT)
				{
					Monster->UseOFF();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Set_ComboPlus();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->ShowComboUI();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->ShowGoodEffectUI();

					_int MaxCombo = static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Get_MaxCombo();

					if (MaxCombo == 50)
						g_pGameInstance->PlaySoundW(L"Taiko_50combo_1P.wav", CHANNEL_EFFECT);

					g_pGameInstance->PlaySoundW(L"Taiko_Katsu.wav", CHANNEL_EFFECT);
				}
				/*else
				{
					Monster->UseOFF();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Set_ComboZero();
				}*/
			}

			if (g_pGameInstance->Get_DIKeyState(DIK_SPACE)&DIS_Down)
			{
				if (Monster->Get_NoteDesc().NoteType == CTaiko_Monster::NOTE_BIG)
				{
					Monster->UseOFF();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Set_ComboPlus();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->ShowComboUI();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->ShowGoodEffectUI();

					_int MaxCombo = static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Get_MaxCombo();

					if (MaxCombo == 50)
						g_pGameInstance->PlaySoundW(L"Taiko_50combo_1P.wav", CHANNEL_EFFECT);

					g_pGameInstance->PlaySoundW(L"Taiko_Big Don.wav", CHANNEL_EFFECT);
				}
				/*else
				{
					Monster->UseOFF();
					static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Set_ComboZero();
				}*/
			}
		}
		else if(m_GoalColliderDesc.ColliderType == CGaolCollider::COLLIDER_DIE)
		{
			Monster->UseOFF();
			static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Set_ComboZero();
		}
	}
}

HRESULT CGaolCollider::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	if (m_GoalColliderDesc.ColliderType == CGaolCollider::COLLIDER_GOOD)
	{
		ColliderDesc.vScale = _float3(0.5f);//2.8f
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	}
	else if (m_GoalColliderDesc.ColliderType == CGaolCollider::COLLIDER_DIE)
	{
		ColliderDesc.vScale = _float3(0.5f, 5.f, 1.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_AABB, &ColliderDesc));
	}

	return S_OK;
}

CGaolCollider* CGaolCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGaolCollider*	pInstance = NEW CGaolCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGaolCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGaolCollider::Clone(void * pArg)
{
	CGaolCollider*	pInstance = NEW CGaolCollider(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGaolCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGaolCollider::Free()
{
	__super::Free();

	Safe_Release(m_pCollider);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
