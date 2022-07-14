#include "stdafx.h"
#include "..\public\StaticMapObject.h"



CStaticMapObject::CStaticMapObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CStaticMapObject::CStaticMapObject(const CStaticMapObject & rhs)
	: CMapObject(rhs)
{
}

HRESULT CStaticMapObject::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}

HRESULT CStaticMapObject::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	FAILED_CHECK(SetUp_Components());


	if (pArg != nullptr)
	{
		_float3 vPos = *(_float3*)pArg;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPos);
	}
	m_iPassIndex = 3;

	return S_OK;

}

_int CStaticMapObject::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;



	if (g_pGameInstance->Get_DIKeyState(DIK_C) & DIS_Down)
	{
		m_pDissolve->Set_DissolveOn(false, 5.5f);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_X) & DIS_Down)
	{
		m_pDissolve->Set_DissolveOn(true, 1.5f);
	}






	m_pDissolve->Update_Dissolving(fDeltaTime);
	return _int();
}

_int CStaticMapObject::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;






	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel,nullptr,m_pDissolve));

	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}



#ifdef _DEBUG
	/*for (_uint i = 0; i < m_pColliderCom->Get_NumColliderBuffer(); i++)
	{
		m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());
	}*/
	for (_uint i = 0; i < m_pColliderCom->Get_NumColliderBuffer(); i++)
	{
		_Matrix FrustumSizeCheckMat = m_pTransformCom->Get_WorldMatrix();

		FrustumSizeCheckMat.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * (m_fFrustumRadius * 2.f);
		FrustumSizeCheckMat.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * (m_fFrustumRadius* 2.f);
		FrustumSizeCheckMat.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * (m_fFrustumRadius* 2.f);

		m_pColliderCom->Update_Transform(i, FrustumSizeCheckMat);
	}
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));

#endif // _DEBUG

	return _int();
}

_int CStaticMapObject::Render()
{
	if (__super::Render() < 0)
		return -1;


	NULL_CHECK_RETURN(m_pModel, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	FAILED_CHECK(__super::SetUp_ConstTable(m_pShaderCom));



	FAILED_CHECK(m_pDissolve->Render(m_iPassIndex));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();
	//
	//for (_uint i= 0 ; i < NumMaterial ; i ++)
	//{
	//
	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
	//
	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_iPassIndex,i));
	//}

	return 0;
}

_int CStaticMapObject::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

HRESULT CStaticMapObject::SetUp_Components()
{

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge), TAG_COM(Com_Model), (CComponent**)&m_pModel));
//	FAILED_CHECK(Add_Component(m_eNowSceneNum, TEXT("AlgaeRock_Ledge.fbx"), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));


#ifdef _DEBUG
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

	COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));


	//ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	//ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	//ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	//FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	//m_pColliderCom->Set_ParantBuffer();//기본은 제일 마지막에 넣은 버퍼의 부포를 0번째 인덱스의 버퍼로 설정

	//ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	//ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	//ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	//FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	//m_pColliderCom->Set_ParantBuffer(1);//제일 마지막에 넣은 버퍼의 부포를 1번째 인덱스의 버퍼로 설정



	//ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	//ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	//ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	//FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	//m_pColliderCom->Set_ParantBuffer(1,3);//3번 인덱스의 콜리더 버퍼의 부모를 1번째 인덱스의 버퍼로 설정

	//ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	//ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	//ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	//FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	//ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	//ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	//ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	//FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));



#endif // _DEBUG



	CDissolve::DISSOLVEDESC	tDissolveDesc;

	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &tDissolveDesc));

	





	return S_OK;
}

CStaticMapObject * CStaticMapObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CStaticMapObject*	pInstance = NEW CStaticMapObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CStaticMapObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStaticMapObject::Clone(void * pArg)
{
	CStaticMapObject*	pInstance = NEW CStaticMapObject(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CStaticMapObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStaticMapObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);

#ifdef _DEBUG
	Safe_Release(m_pColliderCom);
#endif // _DEBUG

}
