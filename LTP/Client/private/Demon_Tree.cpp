#include "stdafx.h"
#include "..\public\Demon_Tree.h"
#include "TreeMesh.h"

CDemon_Tree::CDemon_Tree(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CDemon_Tree::CDemon_Tree(const CDemon_Tree & rhs)
	: CMapObject(rhs)
{
}

HRESULT CDemon_Tree::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CDemon_Tree::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	SetUp_Components();

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	CTreeMesh* TreeMesh = nullptr;

	CTreeMesh::DemonTreeDesc DemonTreeDesc;

	DemonTreeDesc.m_eDemonTreeNum = CTreeMesh::DEMONTREE_05;
	DemonTreeDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&TreeMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_TreeMesh), &DemonTreeDesc);
	m_pTreeMeshs.push_back(TreeMesh);

	DemonTreeDesc.m_eDemonTreeNum = CTreeMesh::DEMONTREE_04;
	DemonTreeDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&TreeMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_TreeMesh), &DemonTreeDesc);
	m_pTreeMeshs.push_back(TreeMesh);

	DemonTreeDesc.m_eDemonTreeNum = CTreeMesh::DEMONTREE_03;
	DemonTreeDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&TreeMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_TreeMesh), &DemonTreeDesc);
	m_pTreeMeshs.push_back(TreeMesh);

	DemonTreeDesc.m_eDemonTreeNum = CTreeMesh::DEMONTREE_02;
	DemonTreeDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&TreeMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_TreeMesh), &DemonTreeDesc);
	m_pTreeMeshs.push_back(TreeMesh);

	DemonTreeDesc.m_eDemonTreeNum = CTreeMesh::DEMONTREE_01;
	DemonTreeDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&TreeMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_TreeMesh), &DemonTreeDesc);
	m_pTreeMeshs.push_back(TreeMesh);

	return S_OK;
}

_int CDemon_Tree::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;
	m_iCompleteCount = 0;

	for (auto& Tree : m_pTreeMeshs)
	{
		if (Tree->Get_AngleZero())
			m_iCompleteCount++;
	}

	for (auto& Tree : m_pTreeMeshs)
		Tree->Update(fDeltaTime);

	return _int();
}

_int CDemon_Tree::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	for (auto& Tree : m_pTreeMeshs)
		Tree->LateUpdate(fDeltaTime);

	return _int();
}

_int CDemon_Tree::Render()
{
	if (__super::Render() < 0)
		return -1;


	return 0;
}

_int CDemon_Tree::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

HRESULT CDemon_Tree::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	return S_OK;
}

CDemon_Tree * CDemon_Tree::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CDemon_Tree*	pInstance = new CDemon_Tree(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CDemon_Tree");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CDemon_Tree::Clone(void * pArg)
{
	CDemon_Tree*	pInstance = new CDemon_Tree(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CDemon_Tree");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDemon_Tree::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

	for (auto& Tree : m_pTreeMeshs)
		Safe_Release(Tree);

	m_pTreeMeshs.clear();
}
