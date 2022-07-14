#include "stdafx.h"
#include "..\public\ShellingSkillAttackPoint.h"
#include "Camera_Main.h"

CShellingSkillAttackPoint::CShellingSkillAttackPoint(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CShellingSkillAttackPoint::CShellingSkillAttackPoint(const CShellingSkillAttackPoint & rhs)
	: CGameObject(rhs)
{
}

HRESULT CShellingSkillAttackPoint::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CShellingSkillAttackPoint::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
	{
		m_pTransformCom_Parabola->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
		m_pTransformCom_Point->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
	}

	m_pTransformCom_Point->Rotation_CW(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	m_pTransformCom_Point->Scaled_All(_float3(1.5f, 1.5f, 1.5f));

	return S_OK;
}

_int CShellingSkillAttackPoint::Update(_double fDeltaTime)
{
	if (false == m_bActive)
	{
		return 0;
	}

	if (__super::Update(fDeltaTime) < 0)	return -1;
	
	Update_Transform();
	Check_MaxRange();
	//m_pTransformCom_Parabola->LookAt(m_pOwnerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));
	
	return _int();
}

_int CShellingSkillAttackPoint::LateUpdate(_double fDeltaTime)
{
	if (false == m_bActive)
	{
		return 0;
	}


	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this));
	return _int();
}

_int CShellingSkillAttackPoint::Render()
{
	if (__super::Render() < 0)		return -1;

	// Render Parabola
	NULL_CHECK_RETURN(m_pModel_Parabola, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom_Parabola->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom_Parabola->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom_Parabola->Bind_OnShader(m_pShaderCom_Parabola, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel_Parabola->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			FAILED_CHECK(m_pModel_Parabola->Bind_OnShader(m_pShaderCom_Parabola, i, j, MODLETEXTYPE(j)));
		}
		FAILED_CHECK(m_pModel_Parabola->Render(m_pShaderCom_Parabola, 3, i));
	}
	//


	// Render Attack Point
	NULL_CHECK_RETURN(m_pVIBufferCom_Point, E_FAIL);

	FAILED_CHECK(m_pTransformCom_Point->Bind_OnShader(m_pShaderCom_Point, "g_WorldMatrix"));

	FAILED_CHECK(m_pShaderCom_Point->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom_Point->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	if (FAILED(m_pTextureCom_Point->Bind_OnShader(m_pShaderCom_Point, "g_DiffuseTexture", 0)))
		return E_FAIL;

	m_pVIBufferCom_Point->Render(m_pShaderCom_Point, 3);
	//

	return _int();
}

_int CShellingSkillAttackPoint::LateRender()
{
	return _int();
}

void CShellingSkillAttackPoint::Set_Active(_bool bActive)
{
	if (true == bActive && false == m_bActive)
	{
		Update_Transform();
	}

	m_bActive = bActive;
}

_bool CShellingSkillAttackPoint::Get_IsActive()
{
	return m_bActive;
}

void CShellingSkillAttackPoint::Set_OwnerTransform(CTransform * pOwnerTransform)
{
	m_pOwnerTransform = pOwnerTransform;
}

void CShellingSkillAttackPoint::Set_MaxRange(_float fMaxRange)
{
	m_fMaxRange = fMaxRange;
}

_fVector CShellingSkillAttackPoint::Get_AttackPointPos()
{
	_Vector vPos = m_pTransformCom_Point->Get_MatrixState(CTransform::TransformState::STATE_POS);
	return vPos;
}

void CShellingSkillAttackPoint::Update_Transform()
{
	_float3 fPickingPos = Check_MousePicking();

	if (true == Check_MaxRange())
	{
		m_pTransformCom_Point->Set_MatrixState(CTransform::TransformState::STATE_POS, fPickingPos);
	}
}

_float3 CShellingSkillAttackPoint::Check_MousePicking()
{
	_float3 fPickingPos = _float3(0.f, 0.f, 0.f);

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_Vector vCursorPos = XMVectorSet(
		(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
		(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
		0, 1.f);

	_Matrix InvProjMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_PROJ));

	_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

	_Matrix InvViewMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
	vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


	CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	_Vector vCamPos = pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

	if (XMVectorGetY(vCamPos) * XMVectorGetY(vRayDir) < 0)
	{
		_float fPos_Y = XMVectorGetY(m_pOwnerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));
		_float Scale = (XMVectorGetY(vCamPos) - fPos_Y) / -(XMVectorGetY(vRayDir));

		_float3 vTargetPos = vCamPos + (Scale)* vRayDir;

		fPickingPos.x = vTargetPos.x; 
		fPickingPos.y = fPos_Y + 0.001f;
		fPickingPos.z = vTargetPos.z;
	}

	return fPickingPos;
}

_bool CShellingSkillAttackPoint::Check_MaxRange()
{
	_Vector vLookDir = m_pOwnerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) - m_pTransformCom_Point->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vDist = XMVector3Length(vLookDir);
	_float fMaxRange = m_fMaxRange - 9;

	if (fMaxRange < XMVectorGetX(vDist))
	{
		vLookDir = XMVector3Normalize(vLookDir) * fMaxRange;
		m_pTransformCom_Point->Set_MatrixState(CTransform::TransformState::STATE_POS, m_pOwnerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) - (vLookDir * 0.99f));
		return false;
	}
	return true;
}

HRESULT CShellingSkillAttackPoint::SetUp_Components()
{
	// Parabola
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom_Parabola));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerEffect_ShellingParabola), TAG_COM(Com_Model), (CComponent**)&m_pModel_Parabola));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom_Parabola, &tDesc));
	//
	

	//// Point
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_ShaderSub), (CComponent**)&m_pShaderCom_Point));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom_Point));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_ShellingPoint), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom_Point));
	
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_SubTransform), (CComponent**)&m_pTransformCom_Point, &tDesc));
	//

	return S_OK;
}

CShellingSkillAttackPoint * CShellingSkillAttackPoint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CShellingSkillAttackPoint*	pInstance = NEW CShellingSkillAttackPoint(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CShellingSkillAttackPoint");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CShellingSkillAttackPoint::Clone(void * pArg)
{
	CShellingSkillAttackPoint*	pInstance = NEW CShellingSkillAttackPoint(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CShellingSkillAttackPoint");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShellingSkillAttackPoint::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom_Parabola);
	Safe_Release(m_pModel_Parabola);
	Safe_Release(m_pTransformCom_Parabola);

	Safe_Release(m_pShaderCom_Point);
	Safe_Release(m_pTextureCom_Point);
	Safe_Release(m_pTransformCom_Point);
	Safe_Release(m_pVIBufferCom_Point);

}
