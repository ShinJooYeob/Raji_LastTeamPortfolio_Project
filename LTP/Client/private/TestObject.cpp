#include "stdafx.h"
#include "..\public\TestObject.h"
#include "..\public\AssimpCreateMgr.h"


CTestObject::CTestObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTestObject::CTestObject(const CTestObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestObject::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CTestObject::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());



	//Set_LimLight_N_Emissive(_float4(0.2f, 0.5f, 1.f,1.f ));

	return S_OK;
}

_int CTestObject::Update(_double fDeltaTime)
{

	if (__super::Update(fDeltaTime) < 0)return -1;

	//_float ttt = 15.f;
	//wstring tt = L"Jangbin e hyung Dambea go?" + to_wstring(ttt) + L"\n";
	//tt.substr(4, 10).c_str();
	//OutputDebugStringW(tt.c_str());


	if (g_pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		static bool  t = false;	t = !t;
		m_pModel->Change_AnimIndex((t) ? 0: 10);
	}
	//if (g_pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	//{
	//	m_pTransformCom->Move_Backward(fDeltaTime);
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	//{
	//	m_pTransformCom->Move_Right(fDeltaTime);
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	//{
	//	m_pTransformCom->Move_Left(fDeltaTime);
	//}
	//_float3 up = _float3(0, 1, 0);

	//if (g_pGameInstance->Get_DIKeyState(DIK_E) & DIS_Press)
	//{
	//	m_pTransformCom->Turn_CCW(up.XMVector(), fDeltaTime);
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_Q) & DIS_Press)
	//{
	//	m_pTransformCom->Turn_CW(up.XMVector(),fDeltaTime);
	//}

	if (KEYDOWN(DIK_F))
	{
		const wchar_t* name = GetSingle(CAssimpCreateMgr)->GetName_Iter_Plus();
		FAILED_CHECK(Change_Component_by_NewAssign(SCENE_STATIC, name, TAG_COM(Com_Model)));
	}

	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);

	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	return _int();
}

_int CTestObject::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	for (_uint i = 0; i < m_vecInstancedTransform.size(); i++)
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_vecInstancedTransform[i], m_pShaderCom, m_pModel));
	}
	//FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	//m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	return _int();
}

_int CTestObject::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecInstancedTransform));


	return _int();
}

_int CTestObject::LateRender()
{
	return _int();
}



HRESULT CTestObject::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	// MODELCOM_NAME
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Player), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	for (_uint i = 0; i < 512; i++)
	{
		CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(pTransform, E_FAIL);
		pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(-256 + _float(i)*1.f, 0, 2));
		m_vecInstancedTransform.push_back(pTransform);
	}

	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_512), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));


	return S_OK;
}

HRESULT CTestObject::Adjust_AnimMovedTransform(_double fDeltatime)
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

CTestObject * CTestObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestObject*	pInstance = new CTestObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTestObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTestObject::Clone(void * pArg)
{
	CTestObject*	pInstance = new CTestObject(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CTestObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestObject::Free()
{
	__super::Free();

	for (auto& pTransform : m_vecInstancedTransform)
		Safe_Release(pTransform);
	m_vecInstancedTransform.clear();
	Safe_Release(m_pModelInstance);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
