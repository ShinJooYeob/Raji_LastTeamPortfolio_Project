#include "stdafx.h"
#include "..\public\PlayerWeapon_Spear.h"

CPlayerWeapon_Spear::CPlayerWeapon_Spear(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CPlayerWeapon(pDevice, pDeviceContext)
{
}

CPlayerWeapon_Spear::CPlayerWeapon_Spear(const CPlayerWeapon_Spear & rhs)
	: CPlayerWeapon(rhs)
{
}

HRESULT CPlayerWeapon_Spear::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon_Spear::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());
	Set_IsOcllusion(true);


	//소드 트레일 온오프 함수 1인자로 온 2인자가 소드 트레일 시작 위치 3인자가 소드 트레일 종착 위치.
	_Matrix mat = m_fAttachedMatrix.XMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	m_pSwordTrail->Set_TrailTurnOn(true, mat.r[3] - mat.r[2], mat.r[3] + mat.r[2]);

	// Rim Light //
	//Set_LimLight_N_Emissive(_float4(1.f, 0, 0, 1.f), 0.f);
	//
	
	return S_OK;
}

_int CPlayerWeapon_Spear::Update(_double fDeltaTime)
{
	if (true == m_bBlockUpdate)
		return 0;

	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_tPlayerWeaponDesc.eWeaponState)
	{
	case EWeaponState::STATE_STRUCTURE:
		Update_Structure(fDeltaTime);
		break;
	case EWeaponState::STATE_EQUIP:
		Update_Equip(fDeltaTime);
		break;
	case EWeaponState::STATE_NON_EQUIP:
		Update_NoEquip(fDeltaTime);
		break;
	}

	m_pModel->Change_AnimIndex(m_iCurAnim, 0.f);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime, true));

	


	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate(_double fDeltaTimer)
{
	if (true == m_bBlockUpdate)
		return 0;

	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	if (true == m_bThrowing)
	{
		Throw(fDeltaTimer);
	}


	switch (m_tPlayerWeaponDesc.eWeaponState)
	{
	case EWeaponState::STATE_STRUCTURE:
		LateUpdate_Structure(fDeltaTimer);
		break;
	case EWeaponState::STATE_EQUIP:
		LateUpdate_Equip(fDeltaTimer);
		break;
	case EWeaponState::STATE_NON_EQUIP:
		LateUpdate_NoEquip(fDeltaTimer);
		break;
	}


	_Matrix mat = m_fAttachedMatrix.XMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	m_pSwordTrail->Update_SwordTrail(mat.r[3] - mat.r[2],	mat.r[3] + mat.r[2], fDeltaTimer);
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &m_fAttachedMatrix));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_SWORD, m_pSwordTrail));

	return _int();
}

_int CPlayerWeapon_Spear::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix",  &m_fAttachedMatrix, sizeof(_float4x4)));
	
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		}
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_iPassNum, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CPlayerWeapon_Spear::LateRender()
{
	return _int();
}

_fVector CPlayerWeapon_Spear::Get_BonePos(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();
	_Vector vPos, vRot, vScale;
	XMMatrixDecompose(&vScale, &vRot, &vPos, TransformMatrix);

	return vPos;
}

void CPlayerWeapon_Spear::Update_AttachCamPos()
{
}

_int CPlayerWeapon_Spear::Update_Structure(_double fDeltaTime)
{
	m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Spear::Update_Equip(_double fDeltaTime)
{

	return _int();
}

_int CPlayerWeapon_Spear::Update_NoEquip(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate_Structure(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate_Equip(_double fDeltaTime)
{
	Update_AttachMatrix();

	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate_NoEquip(_double fDeltaTime)
{
	return _int();
}

void CPlayerWeapon_Spear::Update_AttachMatrix()
{
	m_fAttachedMatrix  =  m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
}

void CPlayerWeapon_Spear::Change_Pivot(ESpearPivot ePitvot)
{
	switch (ePitvot)
	{
	case ESpearPivot::SPEAR_PIVOT_NORMAL:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-97, -120, -60), _float3(-0.661f, -0.04f, -1.133f));
		break;
	case ESpearPivot::SPEAR_PIVOT_THROW:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.f, 1.f, 1.f), _float3(90, 0, 0), _float3(-0.661f, -0.04f, -1.133f));
		break;
	case ESpearPivot::SPEAR_PIVOT_TAKEDOWN:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.f, 1.f, 1.f), _float3(80, 130, 0), _float3(-0.661f, -0.04f, -1.133f));
		break;
	}
}

void CPlayerWeapon_Spear::Throw_Start(_fVector vThrowDir)
{
	m_pTransformCom->Set_Matrix(m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix());
	m_pTransformCom->LookDir(vThrowDir);
	m_bThrowing = true;
	m_bThrowDir = vThrowDir;
	m_iPassNum = 3;
	m_iCurAnim = 8;
}

void CPlayerWeapon_Spear::Throw_End()
{
	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	m_bThrowing = false;
	m_bThrowDir = { 0.f, 0.f, 0.f };
	m_iPassNum = 4;
	m_iCurAnim = 0;
}

void CPlayerWeapon_Spear::Throw(_double fDeltaTimer)
{
	m_pTransformCom->MovetoDir(m_bThrowDir.XMVector(), fDeltaTimer);
}

HRESULT CPlayerWeapon_Spear::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 30;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CSwordTrail::TRAILDESC tSwordDesc;
	//2번인가가 노이즈 사용일거임 0번이 기본 1번은 나도 모름
	tSwordDesc.iPassIndex = 0;
	tSwordDesc.vColor = _float4(0, 0.84705882f, 1, 1.f);
	tSwordDesc.iTextureIndex = 1;
	//노이즈 사용하는 트레일 일때만 사용
	tSwordDesc.NoiseSpeed = 0;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_SwordTrail), TAG_COM(Com_SwordTrail), (CComponent**)&m_pSwordTrail, &tSwordDesc));
	//이후에 추가적으로 값 바꿔주고싶을 때
	//m_pSwordTrailCom->Set_Color(_float4(0, 0.84705882f, 1, 1.f));
	//m_pSwordTrailCom->Set_PassIndex(0);
	//m_pSwordTrailCom->Set_TextureIndex(1);


	return S_OK;
}

HRESULT CPlayerWeapon_Spear::SetUp_EtcInfo()
{
	m_iPassNum = 4;
	m_iCurAnim = 0;
	return S_OK;
}

CPlayerWeapon_Spear * CPlayerWeapon_Spear::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Spear*	pInstance = new CPlayerWeapon_Spear(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Spear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Spear::Clone(void * pArg)
{
	CPlayerWeapon_Spear*	pInstance = new CPlayerWeapon_Spear(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Spear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerWeapon_Spear::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pSwordTrail);
}