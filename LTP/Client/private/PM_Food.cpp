#include "stdafx.h"
#include "..\public\PM_Food.h"
#include "PM_Monster.h"
#include "PackMen.h"
#include "Scene.h"
#include "MiniGameBuilding.h"
#include "Scene_Loading.h"

CPM_Food::CPM_Food(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPM_Food::CPM_Food(const CPM_Food & rhs)
	: CGameObject(rhs)
{
}


HRESULT CPM_Food::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CPM_Food::Initialize_Clone(void * pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);

	FAILED_CHECK(__super::Initialize_Clone(pArg));

	memcpy(&m_tDesc, pArg, sizeof(PMFOODDESC));

	FAILED_CHECK(SetUp_Components());



	return S_OK;
}

_int CPM_Food::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);


	FAILED_CHECK(Update_Food(dDeltaTime));


	if (m_tDesc.iKinds != FOOD_BIGBURGER)
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CPM_Food::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	if (m_vecForRenderTransform.size() > 0)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_NONANIMINSTANCE,this, &m_vecForRenderTransform,m_pModelInstance,m_pShaderCom,m_pModel));
	}



	return _int();
}

_int CPM_Food::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 14, &m_vecForRenderTransform));



	return _int();
}

_int CPM_Food::LateRender()
{
	return _int();
}

void CPM_Food::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (iMyColliderIndex > 0 && !m_vecInstancedTransform[iMyColliderIndex - 1].bIsDead)
	{
		m_vecInstancedTransform[iMyColliderIndex - 1].bIsDead = true;
		m_pColliderCom->Delete_ChildeBuffer(0, iMyColliderIndex);

		if (m_tDesc.iKinds == FOOD_BURGER)
		{
			g_pGameInstance->Play3D_Sound(TEXT("PM_PowerUP.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_PLAYER, 0.7f);
			((CPackMen*)pConflictedObj)->Set_IsBustTimer(3.f);
		}
	}

}

#define CoilliderSize 0.7f
#define BurgurSize 0.5f
#define ColaSize 0.075f

HRESULT CPM_Food::Ready_AtIndexFood(const _tchar * pLayerTag, _uint iIndex)
{

	INSTANCELAYER tLayerDesc;

	tLayerDesc.bIsDead = false;

	Safe_Release(tLayerDesc.pTransform);
	tLayerDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(tLayerDesc.pTransform, E_FAIL);
	tLayerDesc.pTransform->Set_TurnSpeed(XMConvertToRadians(60));

	tLayerDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, CPM_Monster::m_vecTurningPoints[iIndex].vPosition);

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(CoilliderSize);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	tLayerDesc.pTransform->Scaled_All(_float3(1.f));
	m_pColliderCom->Update_Transform(m_pColliderCom->Get_NumColliderBuffer() - 1, tLayerDesc.pTransform->Get_WorldMatrix());
	tLayerDesc.pTransform->Scaled_All(_float3(BurgurSize));

	m_vecInstancedTransform.push_back(tLayerDesc);

	return S_OK;
}

HRESULT CPM_Food::Ready_AtPosFood(const _tchar * pLayerTag, _float3 vPosition)
{

	INSTANCELAYER tLayerDesc;

	tLayerDesc.bIsDead = false;

	Safe_Release(tLayerDesc.pTransform);
	tLayerDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(tLayerDesc.pTransform, E_FAIL);
	tLayerDesc.pTransform->Set_TurnSpeed(XMConvertToRadians(60));

	tLayerDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, vPosition);
	tLayerDesc.pTransform->Scaled_All(_float3(10.f));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(CoilliderSize);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	tLayerDesc.pTransform->Scaled_All(_float3(1.f));
	m_pColliderCom->Update_Transform(m_pColliderCom->Get_NumColliderBuffer() - 1, tLayerDesc.pTransform->Get_WorldMatrix());
	tLayerDesc.pTransform->Scaled_All(_float3(BurgurSize * 25));

	m_vecInstancedTransform.push_back(tLayerDesc);



	return S_OK;
}

HRESULT CPM_Food::Ready_HorizFood(const _tchar * pLayerTag, _uint iStartIndex, _uint iEndIndex, _uint iMaxmumCount)
{
	_float3 vStartPos = CPM_Monster::m_vecTurningPoints[iStartIndex].vPosition;
	_float3 vEndPos = CPM_Monster::m_vecTurningPoints[iEndIndex].vPosition;

	_float fLength = (vEndPos.x - vStartPos.x) / iMaxmumCount;

	_uint iCount = 0;

	_float3 vPosition = vStartPos;
	vPosition.x += fLength;


	CGameInstance* pInstance = g_pGameInstance;

	while (vPosition.x + fLength*0.5f < vEndPos.x && iCount < iMaxmumCount)
	{
		
		INSTANCELAYER tLayerDesc;

		tLayerDesc.bIsDead = false;

		Safe_Release(tLayerDesc.pTransform);
		tLayerDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(tLayerDesc.pTransform, E_FAIL);
		tLayerDesc.pTransform->Set_TurnSpeed(XMConvertToRadians(60));
		
		tLayerDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, vPosition);

		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(CoilliderSize);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pColliderCom->Set_ParantBuffer();

		tLayerDesc.pTransform->Scaled_All(_float3(1.f));
		m_pColliderCom->Update_Transform(m_pColliderCom->Get_NumColliderBuffer() - 1, tLayerDesc.pTransform->Get_WorldMatrix());
		tLayerDesc.pTransform->Scaled_All(_float3(ColaSize));

		m_vecInstancedTransform.push_back(tLayerDesc);


		iCount++;
		vPosition.x += fLength;
	}


	return S_OK;
}

HRESULT CPM_Food::Ready_VerticFood(const _tchar * pLayerTag, _uint iStartIndex, _uint iEndIndex, _uint iMaxmumCount)
{
	_float3 vStartPos = CPM_Monster::m_vecTurningPoints[iStartIndex].vPosition;
	_float3 vEndPos = CPM_Monster::m_vecTurningPoints[iEndIndex].vPosition;

	_uint iCount = 0;
	_float fLength = (vStartPos.z - vEndPos.z) / iMaxmumCount;

	_float3 vPosition = vStartPos;
	vPosition.z -= fLength;


	while (vPosition.z - fLength * 0.5f > vEndPos.z && iCount < iMaxmumCount)
	{
		INSTANCELAYER tLayerDesc;

		tLayerDesc.bIsDead = false;

		Safe_Release(tLayerDesc.pTransform);
		tLayerDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(tLayerDesc.pTransform, E_FAIL);
		tLayerDesc.pTransform->Set_TurnSpeed(XMConvertToRadians(60));

		tLayerDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, vPosition);


		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(CoilliderSize);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pColliderCom->Set_ParantBuffer();


		tLayerDesc.pTransform->Scaled_All(_float3(1.f));
		m_pColliderCom->Update_Transform(m_pColliderCom->Get_NumColliderBuffer() - 1, tLayerDesc.pTransform->Get_WorldMatrix());
		tLayerDesc.pTransform->Scaled_All(_float3(ColaSize));

		m_vecInstancedTransform.push_back(tLayerDesc);

		iCount++;
		vPosition.z -= fLength;
	}


	return S_OK;
}

HRESULT CPM_Food::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	if (m_tDesc.iKinds == FOOD_CANCOLA)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PM_Food), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if(m_tDesc.iKinds == FOOD_BURGER || m_tDesc.iKinds == FOOD_BIGBURGER)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PM_PowerUpFood), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}

	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_256), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance,&tModelIntDsec));



	m_pPlayerTransfom = (CTransform*)g_pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player),TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pPlayerTransfom, E_FAIL);



	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

	COLLIDERDESC			AttackColliderDesc;
	ZeroMemory(&AttackColliderDesc, sizeof(COLLIDERDESC));
	AttackColliderDesc.vScale = _float3(1000.f);
	AttackColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	AttackColliderDesc.vPosition = _float4(0.f, 2.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &AttackColliderDesc));

	m_pColliderCom->Update_Transform(m_pColliderCom->Get_NumColliderBuffer() - 1, XMMatrixIdentity());


	return S_OK;
}

HRESULT CPM_Food::Update_Food(_double dDeltaTime)
{
	m_vecForRenderTransform.clear();
	m_vecForRenderTransform.reserve(m_vecInstancedTransform.size());


	switch (m_tDesc.iKinds)
	{
	case Client::CPM_Food::FOOD_CANCOLA:
	{

		for (auto& InstLayer : m_vecInstancedTransform)
		{
			if (InstLayer.bIsDead)	continue;

			InstLayer.pTransform->Turn_CCW(InstLayer.pTransform->Get_MatrixState(CTransform::STATE_LOOK), dDeltaTime);

			m_vecForRenderTransform.push_back(InstLayer.pTransform);

		}


		if (m_vecForRenderTransform.size() == 0)
		{
			CMiniGameBuilding::Copy_NowScreenToBuliding(CMiniGameBuilding::MINIGAME_PACKMAN);

			 
			FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
			g_pGameInstance->Get_NowScene()->Set_SceneChanging(SCENE_STAGE6);
			return 0;
		}
	}
		break;
	case Client::CPM_Food::FOOD_BURGER:
		for (auto& InstLayer : m_vecInstancedTransform)
		{
			if (InstLayer.bIsDead)	continue;

			InstLayer.pTransform->Turn_CW(InstLayer.pTransform->Get_MatrixState(CTransform::STATE_RIGHT), dDeltaTime);


			m_vecForRenderTransform.push_back(InstLayer.pTransform);

		}
		break;
	case Client::CPM_Food::FOOD_BIGBURGER:
		for (auto& InstLayer : m_vecInstancedTransform)
		{
			if (InstLayer.bIsDead)	continue;

			InstLayer.pTransform->Turn_CCW(InstLayer.pTransform->Get_MatrixState(CTransform::STATE_UP), dDeltaTime * 0.3f);

			m_vecForRenderTransform.push_back(InstLayer.pTransform);

		}
		break;
	default:
		break;
	}






	return S_OK;
}

CPM_Food * CPM_Food::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPM_Food*	pInstance = NEW CPM_Food(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPM_Food");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPM_Food::Clone(void * pArg)
{
	CPM_Food*	pInstance = NEW CPM_Food(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPM_Food");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPM_Food::Free()
{
	__super::Free();


	for (auto& pLayer : m_vecInstancedTransform)
	{
		pLayer.pTransform->Set_IsOwnerDead(true);
		Safe_Release(pLayer.pTransform);
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pColliderCom);
	
	Safe_Release(m_pModelInstance);
	
} 
