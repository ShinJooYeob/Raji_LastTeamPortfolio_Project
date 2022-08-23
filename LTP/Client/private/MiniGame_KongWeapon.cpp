#include "stdafx.h"
#include "..\public\MiniGame_KongWeapon.h"
#include "MiniGame_KongRaji.h"


CMiniGame_KongWeapon::CMiniGame_KongWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMiniGame_KongWeapon::CMiniGame_KongWeapon(const CMiniGame_KongWeapon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMiniGame_KongWeapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMiniGame_KongWeapon::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_KongRaji_Weapon_UniversalDesc, pArg, sizeof(m_KongRaji_Weapon_UniversalDesc));
	}
	else {
		MSGBOX("CMiniGame_KongWeapon의 pArg가 Nullptr입니다.");
	}


	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());


	SetUp_Info();


	SetUp_BoneMatrix();
	m_fAttachedMatrix = XMMatrixIdentity();

	return S_OK;
}

_int CMiniGame_KongWeapon::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	if (m_iOnceIndex == 0)
	{
		m_pPlayer = static_cast<CMiniGame_KongRaji*>(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player)));
		m_pPlayerTransform = (CTransform*)m_pPlayer->Get_Component(TAG_COM(Com_Transform));
		m_iOnceIndex++;
	}

	if (m_bMagnet == true)
	{
		Update_AttachMatrix();
		Update_Collider(dDeltaTime);
	}
	else {
		m_pTransformCom->Scaled_All(_float3(1.f,1.f,1.f));
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(28.7f, 43.3f, 40.f));

		MagnetOn(dDeltaTime);
	}

	return _int();
}

_int CMiniGame_KongWeapon::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	/*FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));*/

	if (m_bMagnet == true)
	{
		_Matrix mat = m_fAttachedMatrix.XMatrix();

		mat.r[0] = XMVector3Normalize(mat.r[0]);
		mat.r[1] = XMVector3Normalize(mat.r[1]);
		mat.r[2] = XMVector3Normalize(mat.r[2]);


		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &_float4x4(mat), m_pDissolve));
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
		m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

	}
	else {
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	}



	if (m_pColliderCom != nullptr)
		FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
	//Caculate_AttachedBone //디폴트피벗*  뼈의 디폴트 뼈 *  뼈의 오픈셋 * 컴바인드 * 부모의 트랜스폼,

	return _int();
}

_int CMiniGame_KongWeapon::Render()
{
	if (__super::Render() < 0)
		return -1;
	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	if (m_bMagnet)
	{
		FAILED_CHECK(m_pDissolve->Render(8));
	}
	else {
		FAILED_CHECK(m_pDissolve->Render(3));
	} 
	//디졸브 내부에서 밑의 머테리얼을 찾아주고 있음
								
	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//	{
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
	//	}
	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 8, i));
	//}

	return _int();
}

_int CMiniGame_KongWeapon::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

void CMiniGame_KongWeapon::Set_IsDead()
{


	m_bIsDead = true;
}

void CMiniGame_KongWeapon::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMiniGame_KongWeapon::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

HRESULT CMiniGame_KongWeapon::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 100;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM_ATTACHED;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));


	SetUp_Collider();

	return S_OK;
}
 
HRESULT CMiniGame_KongWeapon::SetUp_Info()
{
	return S_OK;
}

HRESULT CMiniGame_KongWeapon::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(1.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(2.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(3.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(4.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();


	return S_OK;
}

HRESULT CMiniGame_KongWeapon::SetUp_BoneMatrix()
{
	return S_OK;
}

HRESULT CMiniGame_KongWeapon::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_KongRaji_Weapon_UniversalDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();



	//	_Matrix tt = m_pTransformCom->Get_WorldMatrix()  * m_Monster_Weapon_UniversalDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	//	_Matrix t2 = m_fAttachedMatrix.TransposeXMatrix();
	//
	//#define Rate 0.8f
	//
	//
	//	tt.r[0] = tt.r[0] * (1.f - Rate) + t2.r[0] * Rate;
	//	tt.r[1] = tt.r[1] * (1.f - Rate) + t2.r[1] * Rate;
	//	tt.r[2] = tt.r[2] * (1.f - Rate) + t2.r[2] * Rate;
	//	tt.r[3] = tt.r[3];
	//
	//	m_fAttachedMatrix = tt;

	return S_OK;
}

HRESULT CMiniGame_KongWeapon::Update_Collider(_double dDeltaTime)
{

	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//현재 우리 코드에서 사용중인 어태치는 월드매트리스가 없음 그렇기 때문에 뼈 정보를 가져오는데
	//콜라이더는 뼈정보를 사용하지 않기 때문에 피벗을 지정해줘야함
	_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_KongRaji_Weapon_UniversalDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	_Vector vPos = mat.r[3];

	m_pColliderCom->Update_Transform(0, mat);

	//						2 룩    0라  1업
	mat.r[3] = vPos - (mat.r[2] * -0.03f + mat.r[0] * 2.f + mat.r[1] * -0.7299f);
	m_pColliderCom->Update_Transform(1, mat);

	mat.r[3] = vPos - (mat.r[2] * 0.f + mat.r[0] * 3.01999f + mat.r[1] * 0.f);
	m_pColliderCom->Update_Transform(2, mat);

	mat.r[3] = vPos - (mat.r[2] *0.02f + mat.r[0] * 3.9899f + mat.r[1] *0.7699f);
	m_pColliderCom->Update_Transform(3, mat);



	//test!!!!
	//mat.r[3] = vPos - (mat.r[2] * -110.03f + mat.r[0] * 112.f +mat.r[1] * -110.7299f);
	//m_pColliderCom->Update_Transform(1, mat);

	//mat.r[3] = vPos - (mat.r[2] * 110.f + mat.r[0] * 113.01999f + mat.r[1] * 110.f);
	//m_pColliderCom->Update_Transform(2, mat);

	//mat.r[3] = vPos - (mat.r[2] * TestPos.z + mat.r[0] * TestPos.x + mat.r[1] * TestPos.y);
	//m_pColliderCom->Update_Transform(3, mat);

	//if (g_pGameInstance->Get_DIKeyState(DIK_T) & DIS_Press )
	//{
	//	TestPos.x += 0.01f;
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_G) & DIS_Press)
	//{
	//	TestPos.x -= 0.01f;
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_Y) & DIS_Press)
	//{
	//	TestPos.y += 0.01f;
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_H) & DIS_Press)
	//{
	//	TestPos.y -= 0.01f;
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_U) & DIS_Press)
	//{
	//	TestPos.z += 0.01f;
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_J) & DIS_Press)
	//{
	//	TestPos.z -= 0.01f;
	//}
	return S_OK;
}

HRESULT CMiniGame_KongWeapon::Update_Weapon(_double dDeltaTime)
{
	

	return S_OK;
}

HRESULT CMiniGame_KongWeapon::MagnetOn(_double dDeltaTime)
{
	_float fDistance;
	
	fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

	if (fDistance < 0.1f)
	{
		m_pTransformCom->Set_Matrix(XMMatrixIdentity());
		m_bMagnet = true;
	}


	return S_OK;
}

CMiniGame_KongWeapon * CMiniGame_KongWeapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMiniGame_KongWeapon*	pInstance = NEW CMiniGame_KongWeapon(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_KongWeapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMiniGame_KongWeapon::Clone(void * pArg)
{
	CMiniGame_KongWeapon*	pInstance = NEW CMiniGame_KongWeapon(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMiniGame_KongWeapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMiniGame_KongWeapon::Free()
{

	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);

}