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

	_float3 Test = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	if (g_pGameInstance->Get_DIKeyState(DIK_N)& DIS_Down)
	{
		m_bTest = !m_bTest;

		if (m_bTest)
		{
			m_iCompleteCount = 0;
			for (_int i = 0; i < m_pTreeMeshs.size(); ++i)
			{
				if (m_pTreeMeshs[i]->Get_AngleZero())
					m_iCompleteCount++;
				else
					break;
			}
		}
	}

	if (m_bIsInterpolation)
	{
		m_fInterpolation += (_float)fDeltaTime;
	}

	if (m_fInterpolation > 1.f)
	{
		m_fInterpolation = 0.f;
		m_bIsInterpolation = false;

		for (_int i = 0; i < m_pTreeMeshs.size(); ++i)
		{
			m_pTreeMeshs[i]->ResetTotalHeight();
		}
		_float3 Pos = m_pTreeMeshs[m_iCompleteCount - 1]->Get_ObjPos();
		Pos.y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		m_pTreeMeshs[m_iCompleteCount]->Set_ObjPos(Pos);

		if (m_iCompleteCount == m_pTreeMeshs.size()-1)
			m_bIsClear = true;
		else 
			m_iCompleteCount++;
	}

	if (m_bTest && !m_bIsClear)
	{
		if (m_iCompleteCount < m_pTreeMeshs.size() && !m_bIsInterpolation)
		{
			if (g_pGameInstance->Get_DIKeyState(DIK_LEFT)& DIS_Press)
			{
				m_pTreeMeshs[m_iCompleteCount]->Turn_CCW();
			}

			if (g_pGameInstance->Get_DIKeyState(DIK_RIGHT)& DIS_Press)
			{
				m_pTreeMeshs[m_iCompleteCount]->Turn_CW();
			}

			if (g_pGameInstance->Get_DIKeyState(DIK_RIGHT)& DIS_Up || g_pGameInstance->Get_DIKeyState(DIK_LEFT)& DIS_Up)
			{
				if (m_pTreeMeshs[m_iCompleteCount]->CheckAngle())
				{
					m_bIsInterpolation = true;
					//m_iCompleteCount++;
					//if (m_iCompleteCount > m_pTreeMeshs.size())
					//	m_iCompleteCount = m_pTreeMeshs.size();
				}
			}
		}

		if (m_bIsInterpolation)
		{
			for (_int i = 0; i < m_pTreeMeshs.size(); ++i)
			{
				if (m_pTreeMeshs[i]->Get_ObjPos().y == XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)))
					continue;

				_float fLength = g_pGameInstance->Easing(TYPE_CircularIn,
					m_pTreeMeshs[i]->Get_TotalHeight(),
					m_pTreeMeshs[i]->Get_TotalHeight() - m_pTreeMeshs[i]->Get_Height(),
					m_fInterpolation, 1.f);

				if (m_fInterpolation >= 1.f)
					fLength = m_pTreeMeshs[i]->Get_TotalHeight() - m_pTreeMeshs[i]->Get_Height();

				_float3 Pos = m_pTreeMeshs[i]->Get_ObjPos();
				Pos = XMVectorSetY(Pos.XMVector(), fLength);

				m_pTreeMeshs[i]->Set_ObjPos(Pos);
			}
		}
	}

	for (auto& Tree : m_pTreeMeshs)
		Tree->Update(fDeltaTime);

	return _int();
}

_int CDemon_Tree::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsClear)
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_M)& DIS_Down)
		{
			m_bIsClear = false;
			m_iCompleteCount = 0;
			m_bTest = false;
			//test Reset
			for (auto& Tree : m_pTreeMeshs)
				Safe_Release(Tree);

			m_pTreeMeshs.clear();

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
		}
		
		//없어지는걸 넣을 예정
	}

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

_float CDemon_Tree::Get_NowTreeMesh_Angle()
{
	return m_pTreeMeshs[m_iCompleteCount]->Get_RotationAngle();
}

_float CDemon_Tree::Get_NowTreeMesh_BeginningAngle()
{
	return m_pTreeMeshs[m_iCompleteCount]->Get_BeginningAngle();
}

vector<class CTreeMesh*>* CDemon_Tree::Get_VecMeshs()
{
	return &m_pTreeMeshs;
}

HRESULT CDemon_Tree::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	return S_OK;
}

CDemon_Tree * CDemon_Tree::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CDemon_Tree*	pInstance = NEW CDemon_Tree(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CDemon_Tree");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CDemon_Tree::Clone(void * pArg)
{
	CDemon_Tree*	pInstance = NEW CDemon_Tree(*this);

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
