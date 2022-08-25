#include "stdafx.h"
#include "..\public\Taiko_Monster.h"
#include "Scene_MiniGame_JJB.h"

CTaiko_Monster::CTaiko_Monster(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTaiko_Monster::CTaiko_Monster(const CTaiko_Monster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTaiko_Monster::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CTaiko_Monster::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
	{
		memcpy(&NoteDesc, pArg, sizeof(NOTEDESC));
	}

	FAILED_CHECK(SetUp_Components());

	m_pTransformCom->Rotation_CCW(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	
	m_pTransformCom->Scaled_All(NoteDesc.vScaled);
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, NoteDesc.vNotePosition);

	InitializeMonster();

	m_PosX = XMVectorGetX(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	
	return S_OK;
}

_int CTaiko_Monster::Update(_double dDeltaTime)
{
	if (NoteDesc.bIsUse == false)
		return _int();

	if (__super::Update(dDeltaTime) < 0)return -1;

	m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), dDeltaTime);

	m_PosX -= (_float)dDeltaTime * 19.f;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(m_PosX, m_vPos.y, m_vPos.z));\

	//NoteDesc.bIsUse = true;

	if(NoteDesc.NotePosType == CTaiko_Monster::NOTEPOS_IN)
		Set_LimLight_N_Emissive(_float4(1.f,0.f,0.f,1.f), _float4(1.f, 1.f, 1.f, 0).XMVector());
	else if(NoteDesc.NotePosType == CTaiko_Monster::NOTEPOS_OUT)
		Set_LimLight_N_Emissive(_float4(0.f, 0.f, 1.f, 1.f), _float4(1.f, 1.f, 1.f, 0).XMVector());
		//Set_LimLight_N_Emissive(_float4(1.f, 0.f, 0.f, 1.f), _float4(1.f, 1.f, 1.f, 0).XMVector());
		
		
		//m_pTransformCom->Scaled_All(_float3(2.2f));






	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);

	//FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime, m_bIsOnScreen));
	//FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	Update_Collider(dDeltaTime);

	return _int();
}

_int CTaiko_Monster::LateUpdate(_double dDeltaTime)
{
	if (NoteDesc.bIsUse == false)
		return _int();

	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	m_fRenderSortValue = -2.f;
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this));

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));

	return _int();
}

_int CTaiko_Monster::Render()
{
	if (NoteDesc.bIsUse == false)
		return _int();

	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	_float Color = 0.3f;
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDeltaTime", &Color, sizeof(_float)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	if (NoteDesc.NotePosType == CTaiko_Monster::NOTEPOS_IN)
	{
		FAILED_CHECK(m_pDissolve->Render(18));
	}
	else
		FAILED_CHECK(m_pDissolve->Render(3)); //디졸브 내부에서 밑의 머테리얼을 찾아주고 있음


	return _int();
}

_int CTaiko_Monster::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CTaiko_Monster::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

void CTaiko_Monster::InitializeMonster()
{
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(28.f, 0.5f, 0.f));
	m_PosX = XMVectorGetX(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
}

HRESULT CTaiko_Monster::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PMMonster), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));
	FAILED_CHECK(m_pModel->Update_AnimationClip(g_fDeltaTime, true));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = GetSingle(CUtilityMgr)->RandomFloat(2.5f, 5.f);
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 4;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));

	FAILED_CHECK(SetUp_Collider());

	return S_OK;
}

HRESULT CTaiko_Monster::Adjust_AnimMovedTransform(_double dDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 1://애니메이션 인덱스마다 잡아주면 됨
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴
			{

				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate > 0.7666666666666666) //특정 프레임 플레이 레이트이후에 들어오면실행
			{


				m_iAdjMovedIndex++;
			}

			break;
		case 2:

			break;
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

HRESULT CTaiko_Monster::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

	if (NoteDesc.NoteType == CTaiko_Monster::NOTE_BIG)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(3.5f, 3.5f, 3.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	}
	else if (NoteDesc.NoteType == CTaiko_Monster::NOTE_SMALL)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	}

	return S_OK;
}

HRESULT CTaiko_Monster::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
	{
		_Matrix mat = XMMatrixIdentity();
		mat.r[3] = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

		m_pColliderCom->Update_Transform(i, mat);

	}

	if(m_bIsCollisionOn)
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pColliderCom));
	
	return S_OK;
}

void CTaiko_Monster::UseON()
{
	NoteDesc.bIsUse = true;
	m_bIsCollisionOn = true;
	InitializeMonster();
}

void CTaiko_Monster::UseOFF()
{
	NoteDesc.bIsUse = false;
	m_bIsCollisionOn = false;
	/*InitializeMonster();*/

	if (NoteDesc.NoteType == CTaiko_Monster::NOTE_SMALL && NoteDesc.NotePosType == CTaiko_Monster::NOTEPOS_IN)
	{
		static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Push_NoteSmallRedMonsterLsit(this);
	}
	else if (NoteDesc.NoteType == CTaiko_Monster::NOTE_BIG && NoteDesc.NotePosType == CTaiko_Monster::NOTEPOS_IN)
	{
		static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Push_NoteBigRedMonsterLsit(this);
	}
	else if (NoteDesc.NoteType == CTaiko_Monster::NOTE_SMALL && NoteDesc.NotePosType == CTaiko_Monster::NOTEPOS_OUT)
	{
		static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Push_NoteSmallBlueMonsterLsit(this);
	}
	else if (NoteDesc.NoteType == CTaiko_Monster::NOTE_BIG && NoteDesc.NotePosType == CTaiko_Monster::NOTEPOS_OUT)
	{
		static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Push_NoteBigBlueMonsterLsit(this);
	}

}

CTaiko_Monster * CTaiko_Monster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTaiko_Monster*	pInstance = NEW CTaiko_Monster(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTaiko_Monster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTaiko_Monster::Clone(void * pArg)
{
	CTaiko_Monster*	pInstance = NEW CTaiko_Monster(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CTaiko_Monster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTaiko_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pColliderCom);

}
