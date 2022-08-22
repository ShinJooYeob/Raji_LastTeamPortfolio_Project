#include "stdafx.h"
#include "..\public\RajiMask.h"
#include "Player.h"
#include "RepelWall.h"
#include "Scene_Laboratory_Jino.h"

CRajiMask::CRajiMask(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CRajiMask::CRajiMask(const CRajiMask & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRajiMask::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CRajiMask::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Etc());

	return S_OK;
}

_int CRajiMask::Update(_double fDeltaTime)
{	 
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (true == m_bAttached)
	{
		_Matrix mat = m_tAttachDesc.Caculate_AttachedBoneMatrix_BlenderFixed();//m_tAttachDesc.Caculate_AttachedBoneMatrix_BlenderFixed();
		mat.r[0] = XMVector3Normalize(mat.r[0]) * m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT);
		mat.r[1] = XMVector3Normalize(mat.r[1]) * m_pTransformCom->Get_MatrixScale(CTransform::STATE_UP);
		mat.r[2] = XMVector3Normalize(mat.r[2]) * m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK);
		mat.r[3] = mat.r[3] + (mat.r[1] * 0.2f) + (mat.r[2] * 0.25f);
		m_pTransformCom->Set_Matrix(mat);
		m_pTransformCom->Turn_Direct(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), XMConvertToRadians(180.f));
	}
	
	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));

	return _int();
}

_int CRajiMask::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CRajiMask::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(10)); 
	return _int();
}

_int CRajiMask::LateRender()
{
	return _int();
}

void CRajiMask::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{

}

void CRajiMask::Set_Appear(_bool bAppear)
{
	if (true == bAppear)
	{
		m_pDissolveCom->Set_DissolveOn(true, 1.f);		// Appear
	}
	else if(false == bAppear)
	{
		m_pDissolveCom->Set_DissolveOn(false, 1.f);	// Disappear
		g_pGameInstance->PlaySoundW(L"Jino_MrM_Teleport_01.wav", CHANNELID::CHANNEL_MONSTER, 1.f);
	}

}

void CRajiMask::Set_AttachDesc(ATTACHEDESC tAttachDesc)
{
	m_tAttachDesc = tAttachDesc;
	m_bAttached = true;
}

HRESULT CRajiMask::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MahaHead), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CRajiMask::SetUp_Etc()
{
	m_pTransformCom->Scaled_All(_float3(0.5f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(101.019745f, 34.9635887f, 327.654724f));

	CGameObject* pPlayer = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player));
	
	Set_IsOcllusion(false);

	m_pTransformCom->Rotation_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(0.f));

	_Matrix mat;
	mat.r[0] = { 0.317290902f, 0.196666703f, -0.333184212f, 0.f };
	mat.r[1] = { -0.370693237f, 0.276831210f, -0.189607561f, 0.f };
	mat.r[2] = { 0.109812856f, 0.367073119f, 0.321245193f, 0.f };
	mat.r[3] = { 101.019753f, 34.4235878f, 327.654724f, 1.f };
	m_pTransformCom->Set_Matrix(mat);
	return S_OK;
}

CRajiMask * CRajiMask::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRajiMask*	pInstance = NEW CRajiMask(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRajiMask");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRajiMask::Clone(void * pArg)
{
	CRajiMask*	pInstance = NEW CRajiMask(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CRajiMask");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRajiMask::Free()
{
	__super::Free();
	 
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
