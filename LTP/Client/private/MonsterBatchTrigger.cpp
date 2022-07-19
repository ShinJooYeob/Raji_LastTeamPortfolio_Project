#include "stdafx.h"
#include "..\public\MonsterBatchTrigger.h"
#include "Monster.h"

CMonsterBatchTrigger::CMonsterBatchTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMonsterBatchTrigger::CMonsterBatchTrigger(const CMonsterBatchTrigger & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonsterBatchTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMonsterBatchTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));


	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.914213562373095f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();


	if (m_eNowSceneNum != SCENE_EDIT)
	{
		CNavigation::NAVIDESC NaviDesc;
		NaviDesc.iCurrentIndex = 0;
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc));

	}


	return S_OK;
}

HRESULT CMonsterBatchTrigger::Add_MonsterBatch(_float4x4 & WorldMatrix, OBJECTPROTOTYPEID eID)
{
	MONSTERBATCH tMonsterBatchTag;

	memcpy(&tMonsterBatchTag.vWorldMat, &WorldMatrix,sizeof(_float4x4));
	tMonsterBatchTag.eMonsterID = eID;

	m_vecBatchedValue.push_back(tMonsterBatchTag);

	FAILED_CHECK(Ready_NaviIndex_By_Position(_uint(m_vecBatchedValue.size() - 1)))

	return S_OK;
}

HRESULT CMonsterBatchTrigger::Ready_NaviIndex_By_Position(_uint iIndex)
{
	if (m_eNowSceneNum == SCENE_EDIT) return E_FAIL;

	_float4 vPosition = *((_float4*)(&m_vecBatchedValue[iIndex].vWorldMat._41));

	FAILED_CHECK(m_pNavigationCom->FindCellIndex(vPosition.XMVector()));

	m_vecBatchedValue[iIndex].iNaviIndex = m_pNavigationCom->Get_CurNavCellIndex();

	return S_OK;
}

HRESULT CMonsterBatchTrigger::Spwan_Monster(_uint iIndex, const _tchar * pLayerTag)
{
	if (m_eNowSceneNum == SCENE_EDIT) return E_FAIL;

	CGameInstance* pInstance =g_pGameInstance;

	FAILED_CHECK(pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, pLayerTag, TAG_OP(m_vecBatchedValue[iIndex].eMonsterID)));

	CMonster* pMonster = (CMonster*)(pInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, pLayerTag));
	NULL_CHECK_RETURN(pMonster, E_FAIL);


	CTransform* pMonsterTransform = (CTransform*)(pMonster->Get_Component(TAG_COM(Com_Transform)));
	NULL_CHECK_RETURN(pMonsterTransform, E_FAIL);

	CNavigation* pMonsterNavi = (CNavigation*)(pMonster->Get_Component(TAG_COM(Com_Navaigation)));
	NULL_CHECK_RETURN(pMonsterNavi, E_FAIL);


	pMonsterTransform->Set_Matrix(m_vecBatchedValue[iIndex].vWorldMat);

	pMonsterNavi->Set_CurNavCellIndex(m_vecBatchedValue[iIndex].iNaviIndex);

	pMonsterTransform->Set_MatrixState(CTransform::STATE_POS, pMonsterNavi->Get_NaviPosition(pMonsterTransform->Get_MatrixState(CTransform::STATE_POS)));


	return S_OK;
}

void CMonsterBatchTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	for (_uint i = 0; i < m_vecBatchedValue.size(); i++)
		Spwan_Monster(i, TAG_LAY(Layer_Monster));
	Set_IsDead();
}

_int CMonsterBatchTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	//if (g_pGameInstance->Get_DIKeyState(DIK_X)&DIS_Down)
	//{
	//	for (_uint i = 0 ; i < m_vecBatchedValue.size();i++)
	//		Spwan_Monster(i, TAG_LAY(Layer_Monster));

	//	
	//}



	for (_uint i = 0 ; i < m_pColliderCom->Get_NumColliderBuffer(); i++)
		m_pColliderCom->Update_Transform(i,m_pTransformCom->Get_WorldMatrix());
	
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));

	return _int();
}

_int CMonsterBatchTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;



#ifdef _DEBUG
	//if (g_pGameInstance->Get_NowSceneNum() == SCENE_EDIT)
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CMonsterBatchTrigger::Render()
{
	if (__super::Render() < 0)      return -1;

	return _int();
}

_int CMonsterBatchTrigger::LateRender()
{
	return _int();
}


CMonsterBatchTrigger * CMonsterBatchTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonsterBatchTrigger*	pInstance = NEW CMonsterBatchTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTrigger_ChangeCameraView");
		Safe_Release(pInstance);
	}
	return pInstance;

}

CGameObject * CMonsterBatchTrigger::Clone(void * pArg)
{
	CMonsterBatchTrigger*	pInstance = NEW CMonsterBatchTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonsterBatchTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonsterBatchTrigger::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
}

