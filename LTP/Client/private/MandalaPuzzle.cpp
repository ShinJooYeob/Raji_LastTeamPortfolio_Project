#include "stdafx.h"
#include "..\public\MandalaPuzzle.h"
#include "MandalaMesh.h"

CMandalaPuzzle::CMandalaPuzzle(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CMandalaPuzzle::CMandalaPuzzle(const CMandalaPuzzle & rhs)
	: CMapObject(rhs)
{
}

HRESULT CMandalaPuzzle::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMandalaPuzzle::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	SetUp_Components();

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	CMandalaMesh* pMandalaMesh = nullptr;

	CMandalaMesh::MANDALADESC MandalaMeshDesc;

	MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_00;
	MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
	m_MandalaMeshs.push_back(pMandalaMesh);

	MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_01;
	MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
	m_MandalaMeshs.push_back(pMandalaMesh);

	MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_02;
	MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
	m_MandalaMeshs.push_back(pMandalaMesh);
	 
	MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_03;
	MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
	m_MandalaMeshs.push_back(pMandalaMesh);

	MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_04;
	MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
	m_MandalaMeshs.push_back(pMandalaMesh);

	return S_OK;
}

_int CMandalaPuzzle::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (g_pGameInstance->Get_DIKeyState(DIK_N)& DIS_Down)
	{
		m_bTest = !m_bTest;

		//if (m_bTest)
		//{
		//	m_iCompleteCount = 0;
		//	for (_int i = 0; i < m_MandalaMeshs.size(); ++i)
		//	{
		//		if (m_MandalaMeshs[i]->Get_AngleZero())
		//			m_iCompleteCount++;
		//		else
		//			break;
		//	}
		//}
	}

	if (m_bTest && !m_bIsClear)
	{
		if (m_iMandalaMeshCount == 0)
		{
			m_bIsAngleClear = false;

			if (m_MandalaMeshs[m_iMandalaMeshCount]->Get_AngleZero())
				m_bIsAngleClear = true;
		}

		if (g_pGameInstance->Get_DIKeyState(DIK_UP)& DIS_Down)
		{
			m_bIsAngleClear = false;
			++m_iMandalaMeshCount;
			if (m_iMandalaMeshCount > 4)
				m_iMandalaMeshCount = 3;

			if (m_MandalaMeshs[m_iMandalaMeshCount]->Get_AngleZero())
				m_bIsAngleClear = true;

		}
		if (g_pGameInstance->Get_DIKeyState(DIK_DOWN)& DIS_Down)
		{
			m_bIsAngleClear = false;
			--m_iMandalaMeshCount;
			if (m_iMandalaMeshCount < 0)
				m_iMandalaMeshCount = 0;

			if (m_MandalaMeshs[m_iMandalaMeshCount]->Get_AngleZero())
				m_bIsAngleClear = true;
		}


		if (m_iMandalaMeshCount < m_MandalaMeshs.size() && !m_bIsAngleClear)
		{
			if (g_pGameInstance->Get_DIKeyState(DIK_LEFT)& DIS_Press)
			{
				if (!m_MandalaMeshs[m_iMandalaMeshCount]->Get_AngleZero())
					m_MandalaMeshs[m_iMandalaMeshCount]->Turn_CCW();
			}

			if (g_pGameInstance->Get_DIKeyState(DIK_RIGHT)& DIS_Press)
			{
				if (!m_MandalaMeshs[m_iMandalaMeshCount]->Get_AngleZero())
					m_MandalaMeshs[m_iMandalaMeshCount]->Turn_CW();
			}

			if (g_pGameInstance->Get_DIKeyState(DIK_RIGHT)& DIS_Up || g_pGameInstance->Get_DIKeyState(DIK_LEFT)& DIS_Up)
			{
				if (m_MandalaMeshs[m_iMandalaMeshCount]->CheckAngle())
				{
					_int iClearCount = 0;

					for (auto& MandalaMesh : m_MandalaMeshs)
					{
						if (MandalaMesh->Get_AngleZero())
							++iClearCount;
					}

					if (iClearCount == m_MandalaMeshs.size())
						m_bIsClear = true;
				}
			}
		}
	}

	for (auto& MandalaMesh : m_MandalaMeshs)
	{
		MandalaMesh->Update(fDeltaTime);
	}

	return _int();
}

_int CMandalaPuzzle::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsClear)
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_M)& DIS_Down)
		{
			m_bIsClear = false;
			m_iMandalaMeshCount = 0;
			m_bTest = false;
			//test Reset
			for (auto& MandalaMesh : m_MandalaMeshs)
				Safe_Release(MandalaMesh);

			m_MandalaMeshs.clear();

			CMandalaMesh* pMandalaMesh = nullptr;

			CMandalaMesh::MANDALADESC MandalaMeshDesc;

			MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_00;
			MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
			m_MandalaMeshs.push_back(pMandalaMesh);

			MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_01;
			MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
			m_MandalaMeshs.push_back(pMandalaMesh);

			MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_02;
			MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
			m_MandalaMeshs.push_back(pMandalaMesh);

			MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_03;
			MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
			m_MandalaMeshs.push_back(pMandalaMesh);

			MandalaMeshDesc.m_eMandalaNum = CMandalaMesh::MANDALA_04;
			MandalaMeshDesc.m_Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pMandalaMesh), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_MandalaMesh), &MandalaMeshDesc);
			m_MandalaMeshs.push_back(pMandalaMesh);
		}

		//없어지는걸 넣을 예정
	}

	for (auto& MandalaMesh : m_MandalaMeshs)
	{
		MandalaMesh->LateUpdate(fDeltaTime);
	}

	return _int();
}

_int CMandalaPuzzle::Render()
{
	return _int();
}

_int CMandalaPuzzle::LateRender()
{
	return _int();
}

HRESULT CMandalaPuzzle::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	return S_OK;
}

CMandalaPuzzle * CMandalaPuzzle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMandalaPuzzle*	pInstance = new CMandalaPuzzle(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMandalaPuzzle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMandalaPuzzle::Clone(void * pArg)
{
	CMandalaPuzzle*	pInstance = new CMandalaPuzzle(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CMandalaPuzzle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMandalaPuzzle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

	for (auto& MandalaMesh : m_MandalaMeshs)
		Safe_Release(MandalaMesh);

	m_MandalaMeshs.clear();
}
